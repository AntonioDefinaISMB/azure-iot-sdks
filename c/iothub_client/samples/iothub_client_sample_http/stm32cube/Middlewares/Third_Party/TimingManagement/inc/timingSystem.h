#ifndef __TIMING_AGENT_H
#define __TIMING_AGENT_H

#include <time.h>

void    TimingSystemInitialize(void);
int 	TimingSystemSetSystemTime(time_t epochTimeNow);
time_t 	TimingSystemGetSystemTime(void);

#endif
