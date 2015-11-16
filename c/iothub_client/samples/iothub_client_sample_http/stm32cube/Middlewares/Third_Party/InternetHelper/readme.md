# InternetAdapter
> InternetAdapter performs the interaction between the `AzureIOT` SDK `HTTPAPI` implementation and the physical Internet interface. It is divided into 3 folders:
- **[inc][inc_folder]**: The headers;
- **[interface][interface_folder]**: The high level source code which is completely independent from hardware
- **[port][port_folder]**: The real "hardware interaction" (in this case, with wifi_interface). In order to exploit the same source code with the same OS `RTOS` but changing hardware internet network device (e.g. ethernet port), 
it is enough to change the content of this folder. 
[inc_folder]: https://bitbucket.org/MobileSolutions_ISMB/stephanie/src/542c488f592fbbefe02048f0bcae93f4e5e42f08/NucleoF4AzureIOT/Middlewares/Third_Party/InternetAdapter/inc/?at=master
[interface_folder]: https://bitbucket.org/MobileSolutions_ISMB/stephanie/src/542c488f592fbbefe02048f0bcae93f4e5e42f08/NucleoF4AzureIOT/Middlewares/Third_Party/InternetAdapter/interface/?at=master
[port_folder]: https://bitbucket.org/MobileSolutions_ISMB/stephanie/src/542c488f592fbbefe02048f0bcae93f4e5e42f08/NucleoF4AzureIOT/Middlewares/Third_Party/InternetAdapter/port/?at=master