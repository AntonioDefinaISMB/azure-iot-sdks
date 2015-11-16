#include <time.h>

#include "timingSystem.h"

#if _DLIB_TIME_USES_64
__time64_t __time64(__time64_t * pointer)
{
  return (__time64_t)TimingSystemGetSystemTime();
}
#else
__time32_t __time32(__time32_t * pointer)
{
  return (__time32_t)TimingSystemGetSystemTime();
}
#endif
