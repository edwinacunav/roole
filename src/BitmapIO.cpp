#include "Bitmap.hpp"
#include "IO.hpp"
#include "Platform.hpp"
#include "Utility.hpp"
#include <cstring>
#include <stdexcept>

#define OPTION 0

#if OPTION == 0

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_NO_PSD
#define STBI_NO_PIC
// Disable comma warnings in stb headers.
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomma"
#endif
#include "stb_image.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#elif OPTION == 1

#include <SDL_image.h>
#include <SDL_surface.h>

#endif
using namespace std;

#if OPTION == 0
namespace
{
  int read_callback(void* user, char* data, int size)
  {
    Roole::Reader* reader = static_cast<Roole::Reader*>(user);
    size_t remaining = reader->resource().size() - reader->position();
    size_t actual_size = (size < remaining ? size : remaining);
    reader->read(data, actual_size);
    return static_cast<int>(actual_size);
  }

  void skip_callback(void* user, int n)
  {
    Roole::Reader* reader = static_cast<Roole::Reader*>(user);
    reader->set_position(reader->position() + n);
  }

  int eof_callback(void* user)
  {
    Roole::Reader* reader = static_cast<Roole::Reader*>(user);
    return reader->position() == reader->resource().size();
  }
}
#endif

void Roole::load_image_file(Roole::Bitmap& bitmap, const char* filename)
{
  #if OPTION == 0
  Buffer buffer;
  load_file(buffer, filename);
  stbi_io_callbacks cb;
  cb.read = read_callback;
  cb.skip = skip_callback;
  cb.eof = eof_callback;
  int x, y, n;
  Reader reader = buffer.front_reader();
  stbi_uc* bytes = stbi_load_from_callbacks(&cb, &reader, &x, &y, &n, STBI_rgb_alpha);
  if (bytes != 0) {
    bitmap.resize(x, y);
    memcpy(bitmap.data(), bytes, x * y * sizeof(Color));
  }
  free(bytes);
  #elif OPTION == 1
  //if (bytes == nullptr) {
    //throw runtime_error("Cannot load image: " + string(stbi_failure_reason()) + "\nZero bytes?");
  //SDL_Surface *temp = IMG_Load(filename);
  //SDL_Surface *surf = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_ARGB8888, 0);
  //SDL_Surface *surf = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_BGRA8888, 0);
  //SDL_FreeSurface(temp);
  /*if (IMG_isPNG(SDL_RWops *src) != 0) {surf = IMG_LoadPNG_RW(SDL_RWops *src);
  } else if (IMG_isJPG(SDL_RWops *src) != 0) {surf = IMG_LoadJPG_RW(SDL_RWops *src); }*/
  //if (!surf) {
  //  SDL_FreeSurface(surf);
  ///  throw runtime_error("SDL_image cannot load image");
  //}
  //SDL_LockSurface(surf);
  //unsigned char *bytes = (unsigned char*)(&surf->pixels);
  //SDL_UnlockSurface(surf);
  //IMG_SavePNG(surf, "test.png");
  //bitmap.resize(surf->w, surf->h);//, Color::WHITE);
  //SDL_FreeSurface(surf);//printf("No bytes were loaded by STBI\n");
  #endif
}

// Disable comma warnings in stb headers.
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomma"
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

void Roole::save_image_file(const Roole::Bitmap& bitmap, const string& filename)
{
  int ok;
  if (has_extension(filename, "bmp")) {
    ok = stbi_write_bmp(filename.c_str(), bitmap.width(), bitmap.height(), 4, bitmap.data());
  } else if (has_extension(filename, "tga")) {
    ok = stbi_write_tga(filename.c_str(), bitmap.width(), bitmap.height(), 4, bitmap.data());
  } else {
    ok = stbi_write_png(filename.c_str(), bitmap.width(), bitmap.height(), 4, bitmap.data(), 0);
  }
  if (ok == 0) throw runtime_error("Could not save image data to file: " + filename);
}

static void stbi_write_to_writer(void* context, void* data, int size)
{
  reinterpret_cast<Roole::Writer*>(context)->write(data, size);
}

void Roole::save_image_file(const Roole::Bitmap& bitmap, Roole::Writer writer,
                           const string& format_hint)
{
  int ok;
  
  if (has_extension(format_hint, "bmp")) {
    ok = stbi_write_bmp_to_func(stbi_write_to_writer, &writer, bitmap.width(), bitmap.height(),
                                4, bitmap.data());
  }
  else if (has_extension(format_hint, "tga")) {
    stbi_write_tga_with_rle = 0;
    ok = stbi_write_tga_to_func(stbi_write_to_writer, &writer, bitmap.width(), bitmap.height(),
                                4, bitmap.data());
  }
  else {
    ok = stbi_write_png_to_func(stbi_write_to_writer, &writer, bitmap.width(), bitmap.height(),
                                4, bitmap.data(), 0);
  }
  if (ok == 0)
    throw runtime_error("Could not save image data to memory (format hint = '" +
                          format_hint + "'");
}
