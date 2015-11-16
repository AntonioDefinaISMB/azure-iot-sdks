#include "wifi_interface.h"
#include "wifi_module.h"
#include "ThreadAPI.h"
#include "InternetAdapter.h"
#include "SocketMapping.h"
#include "SocketOS.h"
#include "AzureIOTSDKConfig.h"

#include <string.h>
#include <stdlib.h>

static  wifi_config             config;
static  uint32_t                macaddstart[64];


static uint8_t*  ssid          = 0;
static uint8_t*  seckey        = 0;

static volatile short           isSSIDConnected = 0;
static volatile short           isWiFiStarted   = 0;
static uint8_t  tlsSocketID;
static WiFi_Priv_Mode mode = WPA_Personal;

#if ENABLE_INTERNETADAPTER_PATCH>0
#define TIMEOUT_WAIT_SOCKETCLOSE        5000
#define TIMEOUT_SINGLESLEEP_SOCKETCLOSE 100

typedef struct __TInternetAdapterSocketInfo_t
{
  SOCKETHANDLE  socketHandle;
  TSocketStatus socketStatus;
}TInternetAdapterSocketInfo;
#endif

#if ENABLE_INTERNETADAPTER_PATCH>0
static volatile TInternetAdapterSocketInfo listSocketInfo[AZUREHTTP_MAX_NUMBER_SOCKET_DEFAULT];
#endif

static TInternetInterfaceResult InternetAdapterRealConnectionProcedure(void);

TInternetInterfaceResult   InternetAdapterInitialize(const TInternetAdapterSettings* pSettings)
{
  ssid          = (uint8_t*)calloc(strlen(pSettings->pSSID)+1,sizeof(uint8_t));
  seckey        = (uint8_t*)calloc(strlen(pSettings->pSecureKey)+1,sizeof(uint8_t));
  
  tlsSocketID   = INVALID_SOCKET_HANDLE;
  isWiFiStarted = 0;
  
  if(ssid==0 || seckey==0)
  {
    return INTERNETINTERFACERESULT_FAILED;
  }

  memcpy(ssid,pSettings->pSSID,strlen(pSettings->pSSID));
  memcpy(seckey,pSettings->pSecureKey,strlen(pSettings->pSecureKey));
  
  #if ENABLE_INTERNETADAPTER_PATCH>0
  memset((void*)listSocketInfo,0,sizeof(listSocketInfo));
  #endif
  
  return INTERNETINTERFACERESULT_SUCCESS;
}

TInternetInterfaceResult InternetAdapterRealConnectionProcedure(void)
{
    config.power          = active;
    config.power_level    = high;
    config.dhcp           = on;               //use DHCP IP address
    config.web_server     = WIFI_TRUE;  
     
    /* Init the wi-fi module */  
    
    printf("WiFi: WAIT FOR Module Activation Procedure...\r\n");
    
    if(wifi_init(&config)!=WiFi_MODULE_SUCCESS)
    {
      return INTERNETINTERFACERESULT_FAILED;
    }
    
    printf("Wi-Fi Module Initialized\r\n");
    GET_Configuration_Value("nv_wifi_macaddr",macaddstart);
    
    printf("MAC Address is: %s\r\n",(char*)macaddstart);
    
    printf("WiFi: Try to Connect to Network: %s...\r\n",ssid);
    
    if(wifi_connect(ssid, seckey, mode)==WiFi_MODULE_SUCCESS)
    {
      printf("WiFi: Operation Connection Set Correctly\r\n");
      
      return INTERNETINTERFACERESULT_SUCCESS;
    }
    
    printf("WiFi: Connection Module Failed\r\n");
    
    return INTERNETINTERFACERESULT_FAILED;
}

TInternetInterfaceResult InternetAdapterStop(void)
{
  isWiFiStarted         = 0;
  isSSIDConnected       = 0;
  
  return INTERNETINTERFACERESULT_SUCCESS;
}

TInternetInterfaceResult        InternetAdapterStart(TCallingProcedureType callingProcedureType)
{
  if(isWiFiStarted==0)
  {
    if(InternetAdapterRealConnectionProcedure()==INTERNETINTERFACERESULT_SUCCESS)
    {
      while(isSSIDConnected==0)
      {
        switch(callingProcedureType)
        {
          case CALLINGPROCEDURETYPE_MAIN:
            HAL_Delay(10);
          break;
          
          case CALLINGPROCEDURETYPE_OSTHREAD:
            ThreadAPI_Sleep(10);
          break;
        }
      }
      
      printf("WiFi: Connection Confirmed to Network\r\n");
      
      isWiFiStarted = 1;
    }
  }
  
  return isWiFiStarted==1?INTERNETINTERFACERESULT_SUCCESS:INTERNETINTERFACERESULT_FAILED;
}

TSocketStatus   InternetAdapterSocketOpenConnection(SOCKETHANDLE* pSocketHandle,const char* ipAddress,short tcpPort,TSocketConnectionType socketConnectionType)
{
  uint8_t indexSocket;
  if(wifi_socket_client_open((uint8_t*)ipAddress, tcpPort, socketConnectionType==SOCKETCONNECTIONTYPE_TLS? (uint8_t*)"s":(uint8_t*)"t", pSocketHandle)==WiFi_MODULE_SUCCESS)
  {
    if(socketConnectionType==SOCKETCONNECTIONTYPE_TLS)
    {
      tlsSocketID = *pSocketHandle;
    }
    
    #if ENABLE_INTERNETADAPTER_PATCH>0
    for(indexSocket=0;indexSocket<AZUREHTTP_MAX_NUMBER_SOCKET_DEFAULT;indexSocket++)
    {
      if(listSocketInfo[indexSocket].socketHandle==0)
      {
        listSocketInfo[indexSocket].socketHandle = (*pSocketHandle);
        listSocketInfo[indexSocket].socketStatus = SOCKETSTATUS_CONNECTED;
        
        break;
      }
    }
    #endif
    
    return SOCKETSTATUS_CONNECTED;
  }
  
  return SOCKETSTATUS_DISCONNECTED;
}

TSocketOperationResult  InternetAdapterSocketReadData(SOCKETHANDLE socketHandle,uint8_t* pBuffer,size_t maxSizeData,size_t* pSizeDataRead)
{
  return SocketOSReadData(socketHandle,pBuffer,maxSizeData,pSizeDataRead);
}

TSocketOperationResult  InternetAdapterSocketWriteData(SOCKETHANDLE socketHandle,uint8_t* pBuffer,size_t sizeData)
{
  if(wifi_socket_client_write(socketHandle,(uint16_t)sizeData,(char*)pBuffer)==WiFi_MODULE_SUCCESS)
  {
    return SOCKETOPERATIONRESULT_SUCCESS;
  }
  
  return SOCKETOPERATIONRESULT_FAILED;
}

TSocketStatus   InternetAdapterSocketCloseConnection(SOCKETHANDLE socketHandle,TSocketConnectionType socketConnectionType)
{
  uint8_t indexSocket;
  uint8_t timerWaitClose;
    
  #if ENABLE_INTERNETADAPTER_PATCH>0
  if(socketConnectionType==SOCKETCONNECTIONTYPE_TLS)
  {
    if(wifi_socket_client_close(socketHandle)==WiFi_MODULE_SUCCESS)
    {
      return SOCKETSTATUS_DISCONNECTED;
    }
  }
  else
  {     
    timerWaitClose = 0;
    
    for(indexSocket=0;indexSocket<AZUREHTTP_MAX_NUMBER_SOCKET_DEFAULT;indexSocket++)
    {
        if(listSocketInfo[indexSocket].socketHandle ==socketHandle)
        {
          while(listSocketInfo[indexSocket].socketStatus==SOCKETSTATUS_DISCONNECTED && timerWaitClose<(TIMEOUT_WAIT_SOCKETCLOSE/TIMEOUT_SINGLESLEEP_SOCKETCLOSE))
          {
            ThreadAPI_Sleep(TIMEOUT_SINGLESLEEP_SOCKETCLOSE);
          }
        }
    }   
    return SOCKETSTATUS_DISCONNECTED;
  }
  
  listSocketInfo[indexSocket].socketStatus = SOCKETSTATUS_DISCONNECTED;
  listSocketInfo[indexSocket].socketHandle = 0;

  #else
    if(wifi_socket_client_close(socketHandle)==WiFi_MODULE_SUCCESS)
    {
      return SOCKETSTATUS_DISCONNECTED;
    }  
  #endif
  
  return SOCKETSTATUS_DISCONNECTED;
}

void ind_wifi_on()
{

}

void ind_wifi_connected()
{
  isSSIDConnected = 1;
}

void ind_wifi_resuming()
{

}

void ind_wifi_socket_data_received(uint8_t * data_ptr, uint32_t message_size, uint32_t chunck_size)
{
  InternetAdapterCallbackSocketReceive(tlsSocketID,data_ptr, message_size, chunck_size);
}

void ind_wifi_socket_client_remote_server_closed(uint8_t * socket_closed_id)
{
  #if ENABLE_INTERNETADAPTER_PATCH>0
  uint8_t indexSocket;
  #endif
  
  SocketMappingCallbackSetConnectionStatus((SOCKETHANDLE)(*socket_closed_id),SOCKETSTATUS_DISCONNECTED);
  
  #if ENABLE_INTERNETADAPTER_PATCH>0
  for(indexSocket=0;indexSocket<AZUREHTTP_MAX_NUMBER_SOCKET_DEFAULT;indexSocket++)
  {
    if(listSocketInfo[indexSocket].socketHandle==(*socket_closed_id))
    {
      listSocketInfo[indexSocket].socketStatus = SOCKETSTATUS_DISCONNECTED;
      
      break;
    }
  }
  #endif
}
