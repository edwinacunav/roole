#include "Platform.hpp"
#if defined(ROOLE_IS_WIN)

#include "WinUtility.hpp"
#include "Directories.hpp"
#include "Utility.hpp"
#include <cwchar>
#include <stdexcept>
#include <shlobj.h>
using namespace std;

static string special_folder_path(int csidl)
{
  WCHAR buf[MAX_PATH + 2];
  if (FAILED(SHGetFolderPathW(nullptr, csidl | CSIDL_FLAG_CREATE, nullptr, 0, buf)))
    throw runtime_error("Error getting special folder path");
  size_t len = wcslen(buf);
  if (buf[len - 1] != L'\\') {
    buf[len] = L'\\';
    buf[len + 1] = 0;
  }
  return Roole::utf16_to_utf8(buf);
}

static string exe_filename()
{
  static string result;
  if (result.empty()) {
    WCHAR buffer[MAX_PATH * 2];
    Roole::winapi_check(GetModuleFileNameW(nullptr, buffer, MAX_PATH * 2),
                       "getting the module filename");
    result = Roole::utf16_to_utf8(buffer);
  }
  return result;
}

void Roole::use_resource_directory()
{
  SetCurrentDirectory(utf8_to_utf16(resource_prefix()).c_str());
}

string Roole::resource_prefix()
{
  static string result;
  if (result.empty()) {
    result = exe_filename();
    auto last_delim = result.find_last_of("\\/");
    result.resize(last_delim == string::npos ? 0 : last_delim + 1);
  }
  return result;
}

string Roole::shared_resource_prefix()
{
  return resource_prefix();
}

string Roole::user_settings_prefix()
{
  return special_folder_path(CSIDL_APPDATA);
}

string Roole::user_documents_prefix()
{
  return special_folder_path(CSIDL_PERSONAL);
}

#endif
