#include "Platform.hpp"
#if defined(ROOLE_IS_WIN)

#include "Timing.hpp"
#include <cstdlib>
#include <windows.h>

void Roole::sleep(unsigned milliseconds)
{
  Sleep(milliseconds);
}

unsigned long Roole::milliseconds()
{
  static unsigned long start = [] {
    timeBeginPeriod(1);
    return timeGetTime();
  }();
  return timeGetTime() - start;
}

#endif
