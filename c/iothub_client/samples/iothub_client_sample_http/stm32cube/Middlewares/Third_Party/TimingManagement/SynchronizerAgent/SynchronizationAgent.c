#include "InternetInterface.h"
#include "SynchronizationAgent.h"
#include "ThreadAPI.h"
#include "TimingSystem.h"
#include "agenttime.h"
#include "AzureIOTSDKConfig.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SIZE_BUFFER_ANSWERTIME  4
#define CONVERSION_EPOCHFACTOR  2208988800ul

typedef struct _TTCPConnectionCredential_t
{
  char* pIPAddress;
  short tcpPort;
}TTCPConnectionCredential;

static THREAD_HANDLE                    threadHandle;
static TTCPConnectionCredential         tcpConnectionCredential;

static int                              ThreadSynchronize(void * pointerArg);
static TTCPConnectionCredential*        CreateTCPConnectionCredential(const char* ipAddress,short tcpPort);
static void                             ReleaseTCPConnectionCredential(TTCPConnectionCredential* pTCPConnectionCredential);

time_t SynchronizationAgentConvertNTPTime2EpochTime(uint8_t* pBufferTimingAnswer,size_t sizeBuffer)
{
  uint32_t      valueNumericExtracted;
  time_t        epochTime;

  epochTime = (time_t)-1;
  
  if(sizeBuffer>=SIZE_BUFFER_ANSWERTIME)
  {
    valueNumericExtracted       = ((pBufferTimingAnswer[0]<<24 )|(pBufferTimingAnswer[1]<<16)|(pBufferTimingAnswer[2]<<8)| pBufferTimingAnswer[3]);
    epochTime                   = (time_t)(valueNumericExtracted-CONVERSION_EPOCHFACTOR);
  }
  
  return epochTime;
}

void SynchronizationAgentStartDefaultConfig(void)
{
  SynchronizationAgentStart(SYNCHRONIZATIONAGENT_DEFAULT_ENDPOINT_IPADDRESS,SYNCHRONIZATIONAGENT_DEFAULT_ENDPOINT_TCPPORT);
}

void SynchronizationAgentStart(const char* ipAddress,short tcpPort)
{
  TTCPConnectionCredential* pTCPConnectionCredential;
  
  pTCPConnectionCredential = CreateTCPConnectionCredential(ipAddress,tcpPort);
  
  ThreadAPI_Create(&threadHandle,ThreadSynchronize,pTCPConnectionCredential);
}

int ThreadSynchronize(void * pointerArg)
{
  size_t        sizeEffectiveRead;
  time_t        epochTimeToSetForSystem;
  uint8_t       bufferTimingAnswer[SIZE_BUFFER_ANSWERTIME];
  uint8_t       flagSuccessOperation;

  TTCPConnectionCredential* pTCPConnectionCredential;
  
  pTCPConnectionCredential = (TTCPConnectionCredential*)pointerArg;
 
  if(pointerArg==0 || pTCPConnectionCredential->pIPAddress==0)
    return 0;
  
  memset(bufferTimingAnswer,0,sizeof(bufferTimingAnswer));
  epochTimeToSetForSystem = 0;
  
  flagSuccessOperation = 0;
  
  #if ENABLE_SYNCAGENT_DEBUG>=0
    printf("Synchronization Agent Launched\r\n");
  #endif
  
  if(InternetInterfaceStart()==INTERNETINTERFACERESULT_SUCCESS)
  {
    if(InternetInterfaceSocketOpen(NULL,pTCPConnectionCredential->pIPAddress, pTCPConnectionCredential->tcpPort,SOCKETCONNECTIONTYPE_TCPSTANDARD)==SOCKETSTATUS_CONNECTED)
    {
        #if ENABLE_SYNCAGENT_DEBUG>=0
          printf("Synchronization Agent is connected with NTP Server: %s\r\n",pTCPConnectionCredential->pIPAddress);
        #endif
        if(InternetInterfaceSocketReceive(NULL,bufferTimingAnswer,SIZE_BUFFER_ANSWERTIME,&sizeEffectiveRead)==SOCKETOPERATIONRESULT_SUCCESS && sizeEffectiveRead>=SIZE_BUFFER_ANSWERTIME)
        {
          flagSuccessOperation = 1;
        }
        
        InternetInterfaceSocketClose(NULL);
    }
  }
  
  if(flagSuccessOperation==1)
  {
      epochTimeToSetForSystem = SynchronizationAgentConvertNTPTime2EpochTime(bufferTimingAnswer,SIZE_BUFFER_ANSWERTIME);
      TimingSystemSetSystemTime(epochTimeToSetForSystem);//FIXME: RESTORE
      #if ENABLE_SYNCAGENT_DEBUG>=0
        printf("Synchronization Agent Set UTC Time: %s\r\n",(get_ctime(&epochTimeToSetForSystem)));
      #endif
  }
  
  ReleaseTCPConnectionCredential(pTCPConnectionCredential);  
  
  return 1;
}

TTCPConnectionCredential* CreateTCPConnectionCredential(const char* ipAddress,short tcpPort)
{
  memset(&tcpConnectionCredential,0,sizeof(TTCPConnectionCredential));
  
  tcpConnectionCredential.pIPAddress = (char*)malloc((size_t)(strlen(ipAddress)+1));
  memset(tcpConnectionCredential.pIPAddress,0,strlen(ipAddress)+1);
  
  strcpy(tcpConnectionCredential.pIPAddress,ipAddress);
  tcpConnectionCredential.tcpPort = tcpPort;
  
  return &tcpConnectionCredential;
}

void ReleaseTCPConnectionCredential(TTCPConnectionCredential* pTCPConnectionCredential)
{
  free(tcpConnectionCredential.pIPAddress);
}