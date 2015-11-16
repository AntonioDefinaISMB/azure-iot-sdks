#ifndef __TYPES_SOCKET_MAPPING_H
#define __TYPES_SOCKET_MAPPING_H

#include "httpapi_types.h"
#include "httpapi.h"

//#define MINIMUM_SOCKETHANDLE_VALUE      1
#define SOCKETSYSTEMHANDLE              HTTP_HANDLE

typedef enum __TSocketFlag_t
{
  SOCKETFLAG_NONE,
  SOCKETFLAG_SET,
}TSocketFlag;

typedef enum __TSocketMappingResult_t
{
  SOCKETMAPPINGRESULT_NONE,
  SOCKETMAPPINGRESULT_OK,
  SOCKETMAPPINGRESULT_FAILED
}TSocketMappingResult;

typedef enum __TSocketStatus_t
{
  SOCKETSTATUS_NONE,
  SOCKETSTATUS_CONNECTED,
  SOCKETSTATUS_DISCONNECTED
}TSocketStatus;

#endif
