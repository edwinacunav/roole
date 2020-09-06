#include "Platform.hpp"
#if defined(ROOLE_IS_X)

#include "Directories.hpp"
#include "Utility.hpp"
#include <cassert>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

static string home_dir()
{
  passwd* pwd = getpwuid(geteuid());
  assert(pwd && pwd->pw_dir);
  return pwd->pw_dir;
}

void Roole::use_resource_directory()
{
  // Do nothing, we expect the user to have the correct cwd on Linux.
}

string Roole::resource_prefix()
{
  return string();
}

string Roole::shared_resource_prefix()
{
  return string();
}

string Roole::user_settings_prefix()
{
  return home_dir() + "/.";
}

string Roole::user_documents_prefix()
{
  return home_dir() + "/";
}

#endif
