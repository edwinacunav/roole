#include "OffScreenTarget.hpp"
#include "Texture.hpp"
#include "Image.hpp"
#include "Sprite.hpp"
#include "Platform.hpp"
#ifndef ROOLE_IS_IPHONE
#include <SDL.h>
#endif
using namespace std;

#ifdef ROOLE_IS_OPENGLES
  #define ROOLE_LOAD_GL_EXT(fn, type) \
      static auto fn = fn ## OES;

  #define ROOLE_GL_CONST(name) \
      name ## _OES

  #define ROOLE_GL_DEPTH_COMPONENT \
      GL_DEPTH_COMPONENT16_OES
#else
  #define ROOLE_LOAD_GL_EXT(fn, type) \
      static auto fn = (type) SDL_GL_GetProcAddress(#fn); \
      if (!fn) throw runtime_error("Unable to load " #fn);

  #define ROOLE_GL_CONST(name) \
      name

  #define ROOLE_GL_DEPTH_COMPONENT \
      GL_DEPTH_COMPONENT
#endif

Roole::OffScreenTarget::OffScreenTarget(int width, int height, unsigned image_flags)
{
#ifndef ROOLE_IS_IPHONE
  if (!SDL_GL_ExtensionSupported("GL_EXT_framebuffer_object"))
    throw runtime_error("Missing GL_EXT_framebuffer_object extension");
#endif
    // Create a new texture that will be our rendering target.
    texture = make_shared<Texture>(width, height, image_flags & IF_RETRO);
    // Mark the full texture as blocked for our TexChunk.
    texture->block(0, 0, width, height);
    // Besides the texture, also create a renderbuffer for depth information.
    // Roole doesn't use this, but custom OpenGL code could might.
    ROOLE_LOAD_GL_EXT(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC);
    glGenRenderbuffers(1, &renderbuffer);
    ROOLE_LOAD_GL_EXT(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC);
    glBindRenderbuffer(ROOLE_GL_CONST(GL_RENDERBUFFER), renderbuffer);
    ROOLE_LOAD_GL_EXT(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC);
    glRenderbufferStorage(ROOLE_GL_CONST(GL_RENDERBUFFER), ROOLE_GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(ROOLE_GL_CONST(GL_RENDERBUFFER), 0);
    // Now tie everything together.
    ROOLE_LOAD_GL_EXT(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC);
    glGenFramebuffers(1, &framebuffer);
    ROOLE_LOAD_GL_EXT(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC);
    glBindFramebuffer(ROOLE_GL_CONST(GL_FRAMEBUFFER), framebuffer);
    ROOLE_LOAD_GL_EXT(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC);
    glFramebufferTexture2D(ROOLE_GL_CONST(GL_FRAMEBUFFER), ROOLE_GL_CONST(GL_COLOR_ATTACHMENT0),
                           GL_TEXTURE_2D, texture->tex_name(), 0);
    ROOLE_LOAD_GL_EXT(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC);
    glFramebufferRenderbuffer(ROOLE_GL_CONST(GL_FRAMEBUFFER), ROOLE_GL_CONST(GL_DEPTH_ATTACHMENT),
                              ROOLE_GL_CONST(GL_RENDERBUFFER), renderbuffer);
}

Roole::OffScreenTarget::~OffScreenTarget()
{
  try {
    ROOLE_LOAD_GL_EXT(glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC);
    glDeleteRenderbuffers(1, &renderbuffer);
    ROOLE_LOAD_GL_EXT(glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC);
    glDeleteFramebuffers(1, &framebuffer);
  } catch (...) {
    throw runtime_error("Closing the window to prevent memory leaks.");
    // If we can't load these functions, just accept the resource leak.
  }
}

Roole::Image Roole::OffScreenTarget::render(const std::function<void ()>& f)
{
  ROOLE_LOAD_GL_EXT(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC);
  glBindFramebuffer(ROOLE_GL_CONST(GL_FRAMEBUFFER), framebuffer);
  ROOLE_LOAD_GL_EXT(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC);
  GLenum status = glCheckFramebufferStatus(ROOLE_GL_CONST(GL_FRAMEBUFFER));
  if (status != ROOLE_GL_CONST(GL_FRAMEBUFFER_COMPLETE))
    throw runtime_error("Incomplete framebuffer");
  f();
  glBindFramebuffer(ROOLE_GL_CONST(GL_FRAMEBUFFER), 0);
  unique_ptr<ImageData> tex_chunk(new TexChunk(texture, 0, 0, texture->width(), texture->height(), 0));
  return Image(move(tex_chunk));
}
