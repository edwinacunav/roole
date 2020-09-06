#include "Platform.hpp"
#if defined(ROOLE_IS_WIN)

#include "WinUtility.hpp"
#include "IO.hpp"
#include "Utility.hpp"
#include <windows.h>
using namespace std;
// TODO: Error checking
struct Roole::File::Impl
{
  HANDLE handle = INVALID_HANDLE_VALUE;

  ~Impl()
  {
    if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
  }
};

Roole::File::File(const string& filename, FileMode mode)
: pimpl(new Impl)
{
  DWORD access;
  switch (mode) {
  case FM_READ:
    access = GENERIC_READ;
    break;
  case FM_REPLACE:
    access = GENERIC_WRITE;
    break;
  case FM_ALTER:
    access = GENERIC_READ | GENERIC_WRITE;
    break;
  }
  DWORD share_mode = FILE_SHARE_READ;
  DWORD creation_disp = (mode == FM_READ) ? OPEN_EXISTING : OPEN_ALWAYS;
  wstring wfilename = utf8_to_utf16(filename);
  pimpl->handle = CreateFileW(wfilename.c_str(), access, share_mode, 0, creation_disp,
                              FILE_ATTRIBUTE_NORMAL, 0);
  if (pimpl->handle == INVALID_HANDLE_VALUE)
      throw_last_winapi_error("opening " + filename);
  if (mode == FM_REPLACE) resize(0);
}

Roole::File::~File()
{}

size_t Roole::File::size() const
{
  return GetFileSize(pimpl->handle, 0);
}

void Roole::File::resize(size_t new_size)
{
  if (SetFilePointer(pimpl->handle, new_size, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
    throw_last_winapi_error("setting the file pointer");
  }
  winapi_check(SetEndOfFile(pimpl->handle), "resizing a file");
}

void Roole::File::read(size_t offset, size_t length, void* dest_buffer) const
{
  if (SetFilePointer(pimpl->handle, offset, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
    throw_last_winapi_error("setting the file pointer");
  }
  DWORD dummy;
  winapi_check(ReadFile(pimpl->handle, dest_buffer, length, &dummy, 0));
}

void Roole::File::write(size_t offset, size_t length, const void* source_buffer)
{
  if (SetFilePointer(pimpl->handle, offset, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
    throw_last_winapi_error("setting the file pointer");
  }
  DWORD dummy;
  winapi_check(WriteFile(pimpl->handle, source_buffer, length, &dummy, 0));
}

#endif
