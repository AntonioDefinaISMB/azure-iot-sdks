#ifndef __AZURE_IOT_SDK_CONFIGDEFAULT_H
#define __AZURE_IOT_SDK_CONFIGDEFAULT_H

#define AZURERUNEXAMPLE_STACKSIZE_DEFAULT                       1024

#define AZUREHTTP_MAX_NUMBER_SOCKET_DEFAULT                     1               //N.B. FOR TLS IT IS POSSIBLE TO MANAGE JUST 1 SOCKET
#define AZUREHTTP_MAXNUMBEREVENTSOCKETREAD_DEFAULT              2               
#define AZURESOCKET_NUMBERBYTES_SINGLESOCKETRECEIVE_DEFAULT     2048
#define AZURESOCKET_MAXTIMEOUTWAITBYTE_MS_DEFAULT               5000

#define AZURETHREADAPI_MAX_TASK_NUMBER_DEFAULT                  4
#define AZURETHREADAPI_TIMEOUT_WAIT_SINGLEEVENT_DEFAULT	        5
#define AZURETHREADAPI_STACKSINGLETASK_DEFAULT                  256

#define RTOS_SIZE_STACKMINIMUM_DEFAULT       	4
#define RTOS_MINIMUM_MARGIN_DEFAULT          	1024

#define TCP_HTTPSDEFAULTPORT                    443
#define ENABLE_INTERNETADAPTERPATCH_DEFAULT     1
#define ENABLE_SYNCAGENT_DEBUGDEFAULT           0

#define AZUREIOTHUBCONNECTIONSTRING_INVALID             "<Insert Connection String like: HostName=<hostname>;DeviceId=<deviceID>;SharedAccessKey=<sharedAccessKey>"
#define AZUREIOTHUBCONNECTIONSTRING_GENERICINVALID      "<Insert Connection String like: HostName=<hostname>;DeviceId=<deviceID>;SharedAccessKey=<sharedAccessKey>;SharedAccessKeyName=<sharedAccessKeyName>"

#define DEFAULT_SSID_INVALID                            "<Insert SSID>"
#define DEFAULT_SECUREKEY_INVALID                       "<Insert WiFi Key>"


#endif

