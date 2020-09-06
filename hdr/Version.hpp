#pragma once

#include <string>

#define ROOLE_MAJOR_VERSION 0
#define ROOLE_MINOR_VERSION 1
#define ROOLE_POINT_VERSION 0

namespace Roole
{
  //! A string that contains the full version of the Roole library, like "0.7.50" or "1.0.0.pre5".
  extern const std::string VERSION;
  //! A block of legal copy that your game is obliged to display somewhere.
  extern const std::string LICENSES;
}
