#include "Platform.hpp"
#if defined(ROOLE_IS_WIN)

#include "Utility.hpp"
#include "WinUtility.hpp"
#include <windows.h>
using namespace std;

string Roole::language()
{
  LCID lcid = GetUserDefaultLCID();
  char buffer[9];
  winapi_check(GetLocaleInfoA(lcid, LOCALE_SISO639LANGNAME, buffer, sizeof buffer));
  return buffer;
}

#endif
