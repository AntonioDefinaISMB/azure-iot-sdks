# How to compile and deploy STM32 Cube porting for Azure IOT SDK #

## Pre-requirements ##

The porting is available for STM32 Nucleo F4 board equipped with Wi-Fi STM32_SPWF01SA. Moreover, it is necessary to have access to Azure subscription and create an Azure IOT Hub (see the instruction in [Azure IOT Hub web portal](https://azure.microsoft.com/services/iot-hub/ "Azure IOT Hub web portal"). Alternatively, it is possible to access to a demo IOT Hub with connection string included in file `connectionStringSTM32AzureIOTHubDemo.txt` 
[//]: # ([N.B.: To be agreed between ST and Microsoft])
In any case, it is necessary to get the connection string to the global IOT Hub and follow the instruction in order to create a dedicated device connection or accessing to a pre-existing one and extracting the dedicated connection string to set inside the source code (see [Device Explorer Instructions](https://github.com/Azure/azure-iot-sdks/blob/master/tools/DeviceExplorer/doc/how_to_use_device_explorer.md "Device Explorer Instructions")). Moreover, considering the size of the project, an IAR full license is required, see [IAR WebSite](https://www.iar.com/iar-embedded-workbench/ "IAR website") for further information.

## Instructions ##

The steps to follow in order to compile and deploy the Azure IOT SDK porting for STM32 Cube are listed in the following: 
[//]: # (ST is in charge to decide if use zip file or GIT)
1. Install IAR Embedded Workbench;
2. Download the missing source code, including the IAR project. There are two possible solutions:
	1. Clone STM32 Git launching script from GIT bash;
	2. Download and unzip the archive containing the source code.
3. Go to `STM32CubeGeneratedFolder`\Projects\EWARM\STM32F401RE-Nucleo\ and open Project.eww. NOTE: Pay attention to the global length of the path. Suggestion: it is better to avoid to clone Azure IOT SDK GIT in nested subfolder!
4. Open AzureIOTSDKConfigUser.h and set:
	1. `DEFAULT_SSID` with Service Set Identifier (the name of Wi-Fi network);
	2. `DEFAULT_SECUREKEY`with Secure Key to access to Wi-Fi network;
5. Go to `AzureIOTSDKLocalGitPath`\c\iothub_client\samples\iothub_client_sample_http\iothub_client_sample_http.c and set the variable `static const char* connectionString` with valid connection string (the one produced\extracted with Device Explorer, see pre-requirements section above)
6. Launch application