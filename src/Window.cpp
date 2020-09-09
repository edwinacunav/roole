#include "Platform.hpp"
#if !defined(ROOLE_IS_IPHONE)

#include "Scene.hpp"
#include "Roole.hpp"
#include "GraphicsImpl.hpp"
#include <SDL.h>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace Roole
{
  namespace FPS
  {
    void register_frame();
  }

  static void throw_sdl_error(const string& operation)
  {
    const char* error = SDL_GetError();
    throw runtime_error(operation + ": " + (error ? error : "(unknown error)"));
  }

  static void cleanup();

  SDL_Window* shared_window()
  {
    static SDL_Window* window = nullptr;
    if (window == nullptr) {
      if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw_sdl_error("Could not initialize SDL Video");
      atexit(cleanup);
      uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
    #if SDL_VERSION_ATLEAST(2, 0, 1)
      //flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    #endif
      window =
        SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64, 64, flags);
      if (window == nullptr)
        throw_sdl_error("Could not create window");
      SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    }
    return window;
  }

  static SDL_GLContext shared_gl_context()
  {
    static SDL_GLContext context = nullptr;
    if (context == nullptr) {
    #ifdef ROOLE_IS_OPENGLES
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #endif
      context = SDL_GL_CreateContext(shared_window());
      if (context == nullptr)
        throw_sdl_error("Could not create OpenGL context");
    }
    return context;
  }
    
  void ensure_current_context()
  {
    SDL_GL_MakeCurrent(shared_window(), shared_gl_context());
  }

  static void cleanup()
  {
    SDL_GL_DeleteContext(shared_gl_context());
    SDL_DestroyWindow(shared_window());
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }
}

struct Roole::Window::Impl
{
  bool fullscreen;
  double update_interval;
  bool resizable;
  bool resizing = false;
  bool is_scene_closed = false;
  bool need_cursor = true;
  // A single `bool open` is not good enough to support the tick() method: When close() is called
  // from outside the window's call graph, the next call to tick() must return false (transition
  // from CLOSING to CLOSED), but the call after that must return show the window again
  // (transition from CLOSED to OPEN).
  enum { INIT, CLOSED, OPEN, CLOSING } state = INIT;
  unique_ptr<Graphics> graphics;
  unique_ptr<Input> input;
};

Roole::Window::Window(unsigned width, unsigned height, bool fullscreen, double update_interval,
                     bool resizable)
: pimpl(new Impl)
{
#if SDL_VERSION_ATLEAST(2, 0, 5)
  SDL_SetWindowResizable(shared_window(), (SDL_bool)resizable);
#endif
  // Even in fullscreen mode, temporarily show the window in windowed mode to centre it.
  // This ensures that the window will be centered correctly when exiting fullscreen mode.
  // Fixes https://github.com/gosu/gosu/issues/369
  // (This will implicitly create graphics() and input(), and make the OpenGL context current.)
  //resize(width, height, false);
  //SDL_SetWindowPosition(shared_window(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  // Really enable fullscreen if desired.
  resize(width, height, fullscreen);
  SDL_GL_SetSwapInterval(1);
  pimpl->update_interval = update_interval;
  pimpl->resizable = resizable;
  input().on_button_down = [this](Button button) { button_down(button); };
  input().on_button_up   = [this](Button button) { button_up(button); };
}

Roole::Window::~Window()
{
  SDL_HideWindow(shared_window());
}

unsigned Roole::Window::width() const
{
  return graphics().width();
}

unsigned Roole::Window::height() const
{
  return graphics().height();
}

bool Roole::Window::fullscreen() const
{
  return pimpl->fullscreen;
}

bool Roole::Window::resizable() const
{
  return pimpl->resizable;
}

double Roole::Window::update_interval() const
{
  return pimpl->update_interval;
}

string Roole::Window::caption() const
{
  const char* title = SDL_GetWindowTitle(shared_window());
  return title ? title : "Game Window";
}

void Roole::Window::resize(unsigned width, unsigned height, bool fullscreen)
{
  pimpl->fullscreen = fullscreen;
  int actual_width = width;
  int actual_height = height;
  double scale_factor = 1.0;
  double black_bar_width = 0;
  double black_bar_height = 0;
  if (fullscreen) {
    SDL_SetWindowPosition(shared_window(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    actual_width  = Roole::screen_width(this);
    actual_height = Roole::screen_height(this);
    if (resizable()) {
      // Resizable fullscreen windows stubbornly follow the desktop resolution.
      width  = actual_width;
      height = actual_height;
    } else {
      // Scale the window to fill the desktop resolution.
      double scale_x = 1.0 * actual_width / width;
      double scale_y = 1.0 * actual_height / height;
      scale_factor = min(scale_x, scale_y);
      // Add black bars to preserve the aspect ratio, if necessary.
      if (scale_x < scale_y)
        black_bar_height = (actual_height / scale_x - height) / 2;
      else if (scale_y < scale_x)
        black_bar_width = (actual_width / scale_y - width) / 2;
    }
  }
  else {
    unsigned max_width  = Roole::available_width(this);
    unsigned max_height = Roole::available_height(this);
    if (resizable()) {
      // If the window is resizable, limit its size, without preserving the aspect ratio.
      width  = actual_width  = min(width,  max_width);
      height = actual_height = min(height, max_height);
    } else if (width > max_width || height > max_height) {
      // If the window cannot fit on the screen, shrink its contents.
      scale_factor = min(1.0 * max_width / width, 1.0 * max_height / height);
      actual_width  = width  * scale_factor;
      actual_height = height * scale_factor;
    }
  }
  SDL_SetWindowFullscreen(shared_window(), fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
  if (!pimpl->resizing)
    SDL_SetWindowSize(shared_window(), actual_width, actual_height);
#if SDL_VERSION_ATLEAST(2, 0, 1)
  SDL_GL_GetDrawableSize(shared_window(), &actual_width, &actual_height);
#endif
  ensure_current_context();
  if (!pimpl->graphics)
    pimpl->graphics.reset(new Graphics(actual_width, actual_height));
  else
    pimpl->graphics->set_physical_resolution(actual_width, actual_height);
  pimpl->graphics->set_resolution(width, height, black_bar_width, black_bar_height);
  if (!pimpl->input)
    pimpl->input.reset(new Input(shared_window()));
  pimpl->input->set_mouse_factors(1 / scale_factor, 1 / scale_factor,
                                  black_bar_width, black_bar_height);
  //if (!fullscreen)
    SDL_SetWindowPosition(shared_window(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Roole::Window::set_width(unsigned w)
{
  pimpl->resizing = true;
  resize(w, height(), pimpl->fullscreen);
  pimpl->resizing = false;
}

void Roole::Window::set_height(unsigned h)
{
  pimpl->resizing = true;
  resize(width(), h, pimpl->fullscreen);
  pimpl->resizing = false;
}

void Roole::Window::set_fullscreen(bool full)
{
  pimpl->fullscreen = full;
  resize(width(), height(), fullscreen());
}

void Roole::Window::set_update_interval(double update_interval)
{
  pimpl->update_interval = update_interval;
}

void Roole::Window::set_caption(const string& caption)
{
  SDL_SetWindowTitle(shared_window(), caption.c_str());
}

void Roole::Window::set_resizable(bool allow)
{
  pimpl->resizable = allow;
}

void Roole::Window::set_need_cursor(bool need)
{
  pimpl->need_cursor = need;
  SDL_ShowCursor(need);
}

void Roole::Window::show()
{
  unsigned long time_before_tick = milliseconds();
  if (pimpl->state == Impl::CLOSING) {
    pimpl->state = Impl::CLOSED;
    return;
  }
  if (pimpl->state == Impl::CLOSED) {
    if (pimpl->is_scene_closed) {
      Roole::cleanup();
      if (time_before_tick - Scene::timer() > 200 && Scene::timer() > 0)
        Scene::set_zombie();
      return;
    }
  }
  if (pimpl->state == Impl::INIT) {
    SDL_ShowWindow(shared_window());
    pimpl->state = Impl::OPEN;
    // SDL_GL_GetDrawableSize returns different values before and after showing the window.
    // -> When first showing the window, update the physical size of Graphics (=glViewport).
    // Fixes https://github.com/gosu/gosu/issues/318
    int width, height;
    SDL_GL_GetDrawableSize(shared_window(), &width, &height);
    graphics().set_physical_resolution(width, height);
  }
  events();
  if (pimpl->is_scene_closed) return;
  if (pimpl->state == Impl::CLOSING) pimpl->state = Impl::CLOSED;
  // Sleep to keep this loop from eating 100% CPU.
  unsigned long tick_time = milliseconds() - time_before_tick;
  if (tick_time < update_interval()) sleep(update_interval() - tick_time);
  time_before_tick = milliseconds();
}

void Roole::Window::events()
{
  if (pimpl->is_scene_closed) return;
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_WINDOWEVENT: {
        switch (e.window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED: {
            if (pimpl->resizable && (width() != e.window.data1 || height() != e.window.data2)) {
              pimpl->resizing = true;
              resize(e.window.data1, e.window.data2, fullscreen());
              pimpl->resizing = false;
            }
            break;
          }
          default: {
            break;
          }
        }
        break;
      }
      case SDL_QUIT: {
        Scene::set_close_button();
        close();
        break;
      }
      case SDL_DROPFILE: {
        char* dropped_filedir = e.drop.file;
        if (dropped_filedir == nullptr) break;
        drop(string(dropped_filedir));
        SDL_free(dropped_filedir);
        break;
      }
      default: {
        input().feed_sdl_event(&e);
        break;
      }
    }
  }
  Song::update();
  input().update();
  update();
  if (Scene::is_closed() || !need_redraw()) return;
  ensure_current_context();
  graphics().frame([&] {
    draw();
    FPS::register_frame();
  });
  SDL_GL_SwapWindow(shared_window());
}

void Roole::Window::close()
{
  pimpl->state = Impl::CLOSING;
  pimpl->is_scene_closed = true;
  Scene::set_timer(milliseconds());
  if (Scene::hit_close_button())
    Roole::cleanup();
  else
    SDL_HideWindow(shared_window());
}

void Roole::Window::button_down(Button button)
{
  bool toggle_fullscreen;
  if (button == KB_F12) {
    Scene::reset_game();
    return;
  }
  // Default shortcuts for toggling fullscreen mode, see: https://github.com/gosu/gosu/issues/361
#ifdef ROOLE_IS_MAC
  // cmd+F and cmd+ctrl+F are both common shortcuts for toggling fullscreen mode on macOS.
  toggle_fullscreen = button == KB_F &&
      (Input::down(KB_LEFT_META) || Input::down(KB_RIGHT_META)) &&
      !Input::down(KB_LEFT_SHIFT) && !Input::down(KB_RIGHT_SHIFT) &&
      !Input::down(KB_LEFT_ALT) && !Input::down(KB_RIGHT_ALT);
#else
  // Alt+Enter and Alt+Return toggle fullscreen mode on all other platforms.
  toggle_fullscreen = (button == KB_RETURN || button == KB_ENTER) &&
      (Input::down(KB_LEFT_ALT)     || Input::down(KB_RIGHT_ALT)) &&
      !Input::down(KB_LEFT_CONTROL) && !Input::down(KB_RIGHT_CONTROL) &&
      !Input::down(KB_LEFT_META)    && !Input::down(KB_RIGHT_META) &&
      !Input::down(KB_LEFT_SHIFT)   && !Input::down(KB_RIGHT_SHIFT);
#endif
  // F11 is supported as a shortcut for fullscreen mode on all platforms.
  if (!toggle_fullscreen && button == KB_F11 &&
      !Input::down(KB_LEFT_ALT)     && !Input::down(KB_RIGHT_ALT) &&
      !Input::down(KB_LEFT_CONTROL) && !Input::down(KB_RIGHT_CONTROL) &&
      !Input::down(KB_LEFT_META)    && !Input::down(KB_RIGHT_META) &&
      !Input::down(KB_LEFT_SHIFT)   && !Input::down(KB_RIGHT_SHIFT)) {
      toggle_fullscreen = !toggle_fullscreen;
  }
  if (toggle_fullscreen)
    resize(width(), height(), !fullscreen());
  press_button(button);
}

bool Roole::Window::need_redraw() const
{
  return true;
}

bool Roole::Window::need_cursor() const
{
  return pimpl->need_cursor;
}

bool Roole::Window::is_disposed()
{
  return pimpl->state == Impl::CLOSED && pimpl->is_scene_closed;
}

const Roole::Graphics& Roole::Window::graphics() const
{
  return *pimpl->graphics;
}

Roole::Graphics& Roole::Window::graphics()
{
  return *pimpl->graphics;
}

const Roole::Input& Roole::Window::input() const
{
  return *pimpl->input;
}

Roole::Input& Roole::Window::input()
{
  return *pimpl->input;
}

#endif
