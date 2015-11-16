#ifndef __INTERNET_INTERFACE_H
#define __INTERNET_INTERFACE_H

#include <stdint.h>
#include "httpapi_types.h"
#include "SocketMappingTypes.h"
#include "InternetInterfaceDriver.h"

TInternetInterfaceResult        InternetInterfaceStart(void);
TInternetInterfaceResult        InternetInterfaceStop(void);
TInternetInterfaceResult        InternetInterfaceLoadCertificate(const unsigned char* certificate, size_t size);
TSocketStatus                   InternetInterfaceSocketOpen(SOCKETSYSTEMHANDLE pHandle,const char* ipAddress,short tcpPort,TSocketConnectionType socketConnectionType);
TSocketStatus                   InternetInterfaceSocketClose(SOCKETSYSTEMHANDLE pHandle);
TSocketStatus                   InternetInterfaceSocketGetConnectionStatus(SOCKETSYSTEMHANDLE pHandle);
TSocketOperationResult          InternetInterfaceSocketSend(SOCKETSYSTEMHANDLE pHandle,void* pData,size_t sizeData);
TSocketOperationResult          InternetInterfaceSocketReceive(SOCKETSYSTEMHANDLE pHandle,void* pData,size_t maxSizeData,size_t *pSizeRead);

#endif
