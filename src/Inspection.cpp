#include "Inspection.hpp"
#include "Timing.hpp"

namespace Roole
{
  namespace FPS
  {
    int fps, accum;
    unsigned long sec;

    void register_frame()
    {
      ++accum;
      unsigned long new_sec = Roole::milliseconds() / 1000;
      if (sec != new_sec) {
        sec = new_sec;
        fps = accum;
        accum = 0;
      }
    }
  }
  
  int fps()
  {
    return FPS::fps;
  }
}
