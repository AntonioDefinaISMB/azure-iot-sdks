#ifndef __AZURE_IOT_SDK_CONFIGUSER_H
#define __AZURE_IOT_SDK_CONFIGUSER_H

#include "AzureIOTSDKConfigDefault.h"

#define AZUREIOTHUBCONNECTIONSTRING_SPECIFICDEVICE      "HostName=sensortocloud.azure-devices.net;DeviceId=D1;SharedAccessKey=njF98qTTX80hRoUyOFP4nGNOnOFIUhAbOuCMjeIRlVk="             //Connection String to set after device registratio with the Azure IOT Hub (USE_GENERICACCESSKEY_IOTHUB<=0 or undefined, checks in the project preprocessor configurations)
#define AZUREIOTHUBCONNECTIONSTRING_GENERIC             AZUREIOTHUBCONNECTIONSTRING_GENERICINVALID      //Connection String to set after device registratio with the Azure IOT Hub (USE_GENERICACCESSKEY_IOTHUB>0, checks in the project preprocessor configurations)

#define DEFAULT_SSID                                    "BronzeNet"                            //Wi-Fi SSID parameter to set before starting
#define DEFAULT_SECUREKEY                               "MicEvent2013"                       //Wi-Fi Secure Key parameter to set before starting


#endif
