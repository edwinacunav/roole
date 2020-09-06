#include "IO.hpp"
#include <cassert>
#include <cstring>
using namespace std;

void Roole::Reader::read(void* dest, size_t length)
{
  res->read(pos, length, dest);
  seek(length);
}

void Roole::Writer::write(const void* source, size_t length)
{ // Try to resize the source if necessary.
  if (pos + length > res->size()) res->resize(pos + length);
  res->write(pos, length, source);
  seek(length);
}

size_t Roole::Buffer::size() const
{
  return buf.size();
}

void Roole::Buffer::resize(size_t new_size)
{
  buf.resize(new_size);
}

void Roole::Buffer::read(size_t offset, size_t length, void* dest_buffer) const
{
  assert(offset + length <= size());
  if (length) memcpy(dest_buffer, &buf[offset], length);
}

void Roole::Buffer::write(size_t offset, size_t length, const void* source_buffer)
{
  assert(offset + length <= size());
  if (length) memcpy(&buf[offset], source_buffer, length);
}

void Roole::load_file(Buffer& buffer, const char* filename)
{
  File file(filename);
  buffer.resize(file.size());
  file.read(0, buffer.size(), buffer.data());
}

void Roole::load_file(Buffer& buffer, const string& filename)
{
  File file(filename);
  buffer.resize(file.size());
  file.read(0, buffer.size(), buffer.data());
}

void Roole::save_file(const Buffer& buffer, const string& filename)
{
  File file(filename, FM_REPLACE);
  file.write(0, buffer.size(), buffer.data());
}
