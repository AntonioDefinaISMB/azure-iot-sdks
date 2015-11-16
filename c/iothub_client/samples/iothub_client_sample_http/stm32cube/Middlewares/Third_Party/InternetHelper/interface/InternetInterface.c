#include "InternetInterface.h"
#include "InternetInterfaceDriver.h"
#include "SocketMapping.h"
#include "InternetAdapter.h"

TInternetInterfaceResult   InternetInterfaceInitialize(const TInternetAdapterSettings* pSettings)
{
  if(InternetAdapterInitialize(pSettings)==INTERNETINTERFACERESULT_SUCCESS)
  {
    SocketMappingInitialize();

    return INTERNETINTERFACERESULT_SUCCESS;
  }
  
  return INTERNETINTERFACERESULT_FAILED;
}

TSocketStatus InternetInterfaceSocketGetConnectionStatus(SOCKETSYSTEMHANDLE pHandle)
{
  return SocketMappingGetStatus(pHandle);
}


TInternetInterfaceResult        InternetInterfaceStart(void)
{
  return InternetAdapterStart(CALLINGPROCEDURETYPE_OSTHREAD);
}

TInternetInterfaceResult        InternetInterfaceStop(void)
{
  return InternetAdapterStop();
}

TSocketStatus  InternetInterfaceSocketOpen(SOCKETSYSTEMHANDLE pHandle,const char* ipAddress,short tcpPort,TSocketConnectionType socketConnectionType)
{
  if(SocketMappingCheckAvailability()==SOCKETMAPPINGRESULT_OK)
  {
    if(SocketMappingOpenConnection(pHandle,ipAddress, tcpPort,socketConnectionType)==SOCKETSTATUS_CONNECTED)
    {
      return SOCKETSTATUS_CONNECTED;
    }
  }
  return SOCKETSTATUS_DISCONNECTED;
}

TSocketOperationResult InternetInterfaceSocketSend(SOCKETSYSTEMHANDLE pHandle,void* pData,size_t sizeData)
{
  if(SocketMappingGetStatus(pHandle)==SOCKETSTATUS_CONNECTED)
  {
    if(SocketMappingWriteData(pHandle,pData,sizeData)==SOCKETOPERATIONRESULT_SUCCESS)
    {
      return SOCKETOPERATIONRESULT_SUCCESS;
    }
  }
  
  return SOCKETOPERATIONRESULT_FAILED;
}

TSocketStatus  InternetInterfaceSocketClose(SOCKETSYSTEMHANDLE pHandle)
{
  return SocketMappingCloseConnection(pHandle);
}


TSocketOperationResult  InternetInterfaceSocketReceive(SOCKETSYSTEMHANDLE pHandle,void* pData,size_t maxSizeData,size_t *pSizeRead)
{
  return SocketMappingReadData(pHandle,(uint8_t*)pData,maxSizeData,pSizeRead);
}

TInternetInterfaceResult   InternetInterfaceLoadCertificate(const unsigned char* certificate, size_t size)
{
  //N.B.: IT IS POSSIBLE TO MANAGE CERTIFICATE FOR ONE-WAY MUTUAL CERTIFICATION ON TLS
  return INTERNETINTERFACERESULT_SUCCESS;
}
