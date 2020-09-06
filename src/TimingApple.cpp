#include "Platform.hpp"
#if defined(ROOLE_IS_MAC)

#include "Timing.hpp"
#include <unistd.h>

void Roole::sleep(unsigned milliseconds)
{
  usleep(milliseconds * 1000);
}
// Thanks to this blog for the unconvoluted code example:
// http://shiftedbits.org/2008/10/01/mach_absolute_time-on-the-iphone/
#include <mach/mach_time.h>

unsigned long Roole::milliseconds()
{
  static mach_timebase_info_data_t info;
  static uint64_t first_tick = [] {
    mach_timebase_info(&info);
    return mach_absolute_time();
  }();
  uint64_t runtime = mach_absolute_time() - first_tick;
  return runtime * info.numer / info.denom / 1000000;
}

#endif
