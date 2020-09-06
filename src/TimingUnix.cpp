#include "Platform.hpp"
#if defined(ROOLE_IS_X)

#include "Timing.hpp"
#include <sys/time.h>
#include <unistd.h>

void Roole::sleep(unsigned milliseconds)
{
  usleep(milliseconds * 1000);
}

unsigned long Roole::milliseconds()
{
  static unsigned long start = 0;
  timeval tp;
  gettimeofday(&tp, nullptr);
  if (start == 0)
    start = tp.tv_usec / 1000UL + tp.tv_sec * 1000UL;
  return tp.tv_usec / 1000UL + tp.tv_sec * 1000UL - start;
}

#endif
