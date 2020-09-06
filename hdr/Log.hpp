#pragma once

#include <cstdlib>
#include <utility>

namespace Roole
{
  template<class... Args>
  void log(const char* format, Args&&... args)
  {
    using namespace std;
    if (getenv("ROOLE_DEBUG")) {
      fprintf(stderr, format, std::forward<Args>(args)...);
      fprintf(stderr, "\n");
    }
  }
}

