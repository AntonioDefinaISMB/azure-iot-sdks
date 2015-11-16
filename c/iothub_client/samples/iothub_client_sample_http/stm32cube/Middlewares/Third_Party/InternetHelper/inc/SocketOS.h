#ifndef __SOCKET_OS_FUNCTIONS_H
#define __SOCKET_OS_FUNCTIONS_H

#include "SocketMappingTypes.h"

#include <stdint.h>
#include "SocketMappingTypes.h"
#include "SocketMappingInternalTypes.h"
#include "InternetInterfaceTypes.h"

TSocketStatus           SocketMappingCallbackSetConnectionStatus(SOCKETHANDLE socketHandle,TSocketStatus socketStatus);
TSocketOperationResult  SocketOSReadData(SOCKETHANDLE socketHandle,uint8_t* pBuffer,size_t maxSizeData,size_t* pSizeDataRead);
void                    SocketOSRelease(SOCKETHANDLE socketHandle);
void                    SocketOSAssign(SOCKETHANDLE socketHandle);
void                    SocketOSInitialize(void);

#endif
