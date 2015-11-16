#ifndef __SOCKET_MAPPING_H
#define __SOCKET_MAPPING_H

#include "FreeRTOS.h"
#include "queue.h"
#include <stdint.h>

#include "SocketMappingTypes.h"
#include "InternetInterfaceTypes.h"

void                    SocketMappingInitialize(void);
TSocketMappingResult    SocketMappingCheckAvailability(void);
TSocketStatus           SocketMappingOpenConnection(SOCKETSYSTEMHANDLE pHandle,const char* ipAddress,short tcpPort,TSocketConnectionType socketConnectionType);
TSocketOperationResult  SocketMappingReadData(SOCKETSYSTEMHANDLE pHandle,uint8_t* pBuffer,size_t maxSizeData,size_t* pSizeDataRead);
TSocketOperationResult  SocketMappingWriteData(SOCKETSYSTEMHANDLE pHandle,uint8_t* pBuffer,size_t sizeData);
TSocketStatus           SocketMappingCloseConnection(SOCKETSYSTEMHANDLE pHandle);
TSocketStatus           SocketMappingGetStatus(SOCKETSYSTEMHANDLE pHandle);
TSocketStatus           SocketMappingSetConnectionStatusRealHandle(SOCKETSYSTEMHANDLE pHandle,TSocketStatus socketStatus);

#endif
