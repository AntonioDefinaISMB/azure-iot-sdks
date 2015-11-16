#ifndef __INTERNET_ADAPTER_H
#define __INTERNET_ADAPTER_H

#include "InternetAdapterTypes.h"
#include "SocketMappingTypes.h"
#include "SocketMappingInternalTypes.h"
#include <stdint.h>

TInternetInterfaceResult        InternetAdapterInitialize(const TInternetAdapterSettings* pSettings);
TInternetInterfaceResult        InternetAdapterStart(TCallingProcedureType callingProcedureType);
TInternetInterfaceResult        InternetAdapterStop(void);
TSocketStatus                   InternetAdapterSocketOpenConnection(SOCKETHANDLE* pSocketHandle,const char* ipAddress,short tcpPort,TSocketConnectionType socketConnectionType);
TSocketOperationResult          InternetAdapterSocketReadData(SOCKETHANDLE pHandle,uint8_t* pBuffer,size_t maxSizeData,size_t* pSizeDataRead);
TSocketOperationResult          InternetAdapterSocketWriteData(SOCKETHANDLE pHandle,uint8_t* pBuffer,size_t sizeData);
TSocketStatus                   InternetAdapterSocketCloseConnection(SOCKETHANDLE pHandle,TSocketConnectionType socketConnectionType);

void                            InternetAdapterCallbackSocketReceive(SOCKETHANDLE socketHandle,uint8_t * data_ptr, uint32_t message_size, uint32_t chunck_size);

#endif
