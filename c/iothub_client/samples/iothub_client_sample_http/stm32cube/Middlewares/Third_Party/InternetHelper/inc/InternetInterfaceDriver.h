#ifndef __INTERNET_INTERFACE_DRIVER_H
#define __INTERNET_INTERFACE_DRIVER_H

#include "InternetInterfaceTypes.h"
#include "InternetAdapterTypes.h"
#include <stdint.h>

/** This method must be called in main
*/
TInternetInterfaceResult        InternetInterfaceInitialize(const TInternetAdapterSettings* pSettings);

#endif
