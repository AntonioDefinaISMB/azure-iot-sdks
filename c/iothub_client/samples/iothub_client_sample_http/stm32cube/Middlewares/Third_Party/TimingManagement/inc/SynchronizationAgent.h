#ifndef __SYNCHRONIZATION_AGENT_H
#define __SYNCHRONIZATION_AGENT_H

#include <stdint.h>

#define SYNCHRONIZATIONAGENT_DEFAULT_ENDPOINT_IPADDRESS         "time-d.nist.gov"
#define SYNCHRONIZATIONAGENT_DEFAULT_ENDPOINT_TCPPORT           37

void SynchronizationAgentStartDefaultConfig(void);
void SynchronizationAgentStart(const char* ipAddress,short tcpPort);

#endif
