#ifndef __HTTP_API_TYPES_H
#define __HTTP_API_TYPES_H

#define MAX_HOSTNAME     64
#define TEMP_BUFFER_SIZE 4096

typedef struct _HTTP_HANDLE_DATA_t
{
    char                host[MAX_HOSTNAME];
    char*               certificate;
    void*               con;
}HTTP_HANDLE_DATA;

#endif
