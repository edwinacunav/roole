#include "ruby_window.hpp"
#include "common.hpp"
#include "Window.hpp"
#include "Scene.hpp"

static VALUE window, scene, graphics, input, zero;

rb_data_type_t WindowType = { "Window",
  { 0, free_instance<Roole::Window>, 0, { 0, 0 } }, 0, 0, 0 };

void Roole::Window::update()
{
  window_protect_update(window);
}

void Roole::Window::draw()
{
  window_protect_draw(window);
}

void Roole::Window::drop(const std::string &filename)
{
  window_drop(window, rstr(filename.c_str()));
}

static VALUE window_init(VALUE self)
{
  VALUE window = rb_iv_set(scene, "@window", self);
  VALUE w = rb_iv_get(scene, "@width");
  VALUE h = rb_iv_get(scene, "@height");
  VALUE fs = rb_iv_get(scene, "@fullscreen");
  VALUE rate = rb_iv_get(scene, "@framerate");
  VALUE res = rb_iv_get(scene, "@resize");
  VALUE title = rb_iv_get(scene, "@title");
  rb_iv_set(self, "@width", w);
  rb_iv_set(self, "@height", h);
  rb_iv_set(self, "@fullscreen", fs);
  rb_iv_set(self, "@update_interval", rate);
  rb_iv_set(self, "@resize", res);
  rb_iv_set(self, "@caption", title);
  rb_iv_set(graphics, "@width", w);
  rb_iv_set(graphics, "@height", h);
  int wi = RB_FIX2INT(w), he = RB_FIX2INT(h);
  double fr = NUM2DBL(rate);
  Roole::Window *win = new Roole::Window(w, h, fs == Qtrue, fr, res == Qtrue);
  RTYPEDDATA(self)->data = win;
  win->set_caption(RSTRING_PTR(title));
  rb_gv_set("$gl_blocks", Qnil);
  return self;
}

static VALUE window_width_set(VALUE self, VALUE width)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return zero;
  window->set_width(RB_FIX2INT(width));
  return rb_iv_set(self, "@width", width);
}

static VALUE window_height_set(VALUE self, VALUE height)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return zero;
  window->set_height(RB_FIX2INT(height));
  return rb_iv_set(self, "@height", height);
}

static VALUE window_fullscreen_set(VALUE self, VALUE boolean)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  window->set_fullscreen(boolean == Qtrue);
  return boolean;
}

static VALUE window_resizable_set(VALUE self, VALUE boolean)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  window->set_resizable(boolean == Qtrue);
  return boolean;
}

static VALUE window_update_interval_set(VALUE self, VALUE time)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return zero;
  window->set_update_interval(DBL2NUM(time));
  return time;
}

static VALUE window_caption_set(VALUE self, VALUE title)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return rstr("");
  window->set_caption(RSTRING_PTR(title));
  return title;
}

static VALUE window_need_cursor_set(VALUE self, VALUE boolean)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  window->set_need_cursor(boolean == Qtrue);
  return boolean;
}

static VALUE window_update(VALUE self)
{
  rb_funcall(scene, rb_intern("update"), 0);
  return Qnil;
}

void window_protect_update(VALUE window)
{
  int state;
  rb_protect(RPF(window_update), window, &state);
  if (state) Scene::set_crash();
}

static VALUE window_draw(VALUE self)
{
  rb_funcall(scene, rb_intern("draw"), 0);
  return Qnil;
}

void window_protect_draw(VALUE window)
{
  int state;
  rb_protect(RPF(window_draw), window, &state);
  if (state) Scene::set_crash();
}

static VALUE window_show(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (window != 0) window->show();
  return Qnil;
}

static VALUE window_close(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (window != 0) window->close();
  return Qnil;
}

static VALUE window_need_redraw(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  return window->need_redraw() ? Qtrue : Qfalse;
}

static VALUE window_need_cursor(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  return window->need_cursor() ? Qtrue : Qfalse;
}

static VALUE window_update_interval(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  return window != 0 ? rb_iv_get(self, "@update_interval") : zero;
}

static VALUE window_caption(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  return window != 0 ? rb_iv_get(self, "@caption") : rstr("Game Window");
}

static VALUE window_button_down(VALUE self, VALUE button)
{
  //rb_iv_set(input, "@button", button);
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  Roole::Button b;
  b = button == Qnil ? Roole::NO_BUTTON : Roole::Button(RB_FIX2INT(button));
  window->button_down(b);
  return button;
}

static VALUE window_press_button(VALUE self, VALUE button)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  Roole::Button b;
  b = button == Qnil ? Roole::NO_BUTTON : Roole::Button(RB_FIX2INT(button));
  window->press_button(b);
  return button;
}

static VALUE window_button_up(VALUE self, VALUE button)
{
  rb_iv_set(input, "@released", button);
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return Qnil;
  Roole::Button b;
  b = button == Qnil ? Roole::NO_BUTTON : Roole::Button(RB_FIX2INT(button));
  return button;
}

static VALUE window_drop(VALUE self, VALUE filename)
{
  return Qnil;
}

static VALUE window_screen_width(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return zero;
  unsigned w = Roole::screen_width(window);
  return RB_INT2FIX(w);
}

static VALUE window_screen_height(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return zero;
  unsigned h = Roole::screen_height(window);
  return RB_INT2FIX(h);
}

static VALUE window_available_width(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return zero;
  unsigned w = Roole::available_width(window);
  return RB_INT2FIX(w);
}

static VALUE window_available_height(VALUE self)
{
  Roole::Window* window = (Roole::Window*)RTYPEDDATA(self)->data;
  if (!window) return zero;
  unsigned h = Roole::available_height(window);
  return RB_INT2FIX(h);
}

template<rb_data_type_t *WindowType>
static VALUE window_allocate(VALUE klass)
{
  return rb_data_typed_object_wrap(klass, 0, WindowType);
}

void init_window()
{
  zero = RB_INT2FIX(0);
  VALUE hidden = rb_const_get(rb_cObject, rb_intern("Roole"));
  VALUE Window = rb_define_class_under(hidden, "Window", rb_cObject);
  scene = rb_define_module("Scene");
  graphics = rb_define_module("Graphics");
  input = rb_define_module("Input");
  rb_define_alloc_func(Window, window_allocate<&WindowType>);
  define_dispose<Roole::Window>(Window);
  rb_define_attr(Window, "width", 1, 0);
  rb_define_attr(Window, "height", 1, 0);
  rb_define_attr(Window, "fullscreen", 1, 0);
  rb_define_attr(Window, "resizable", 1, 0);
  rb_define_attr(Window, "caption", 1, 0);
  rb_define_method(Window, "initialize", RMF(window_init), 0);
  rb_define_method(Window, "update", RMF(window_update), 0);
  rb_define_method(Window, "draw", RMF(window_draw), 0);
  rb_define_method(Window, "show", RMF(window_show), 0);
  rb_define_method(Window, "close", RMF(window_close), 0);
  rb_define_method(Window, "need_redraw?", RMF(window_need_redraw), 0);
  rb_define_method(Window, "need_cursor?", RMF(window_need_cursor), 0);
  rb_define_method(Window, "update_interval", RMF(window_update_interval), 0);
  rb_define_method(Window, "framerate", RMF(window_update_interval), 0);
  rb_define_method(Window, "title", RMF(window_caption), 0);
  rb_define_method(Window, "width=", RMF(window_width_set), 1);
  rb_define_method(Window, "height=", RMF(window_height_set), 1);
  rb_define_method(Window, "fullscreen=", RMF(window_fullscreen_set), 1);
  rb_define_method(Window, "resizable=", RMF(window_resizable_set), 1);
  rb_define_method(Window, "need_cursor=", RMF(window_need_cursor_set), 1);
  rb_define_method(Window, "update_interval=", RMF(window_update_interval_set), 1);
  rb_define_method(Window, "framerate=", RMF(window_update_interval_set), 1);
  rb_define_method(Window, "caption=", RMF(window_caption_set), 1);
  rb_define_method(Window, "title=", RMF(window_caption_set), 1);
  //rb_define_method(Window, "button_down", RMF(window_button_down), 1);
  //rb_define_method(Window, "press_button", RMF(window_press_button), 1);
  //rb_define_method(Window, "button_up", RMF(window_button_up), 1);
  rb_define_method(Window, "drop", RMF(window_drop), 1);
  rb_define_module_function(Window, "screen_width", RMF(window_screen_width), 0);
  rb_define_module_function(Window, "screen_height", RMF(window_screen_height), 0);
  rb_define_module_function(Window, "available_width", RMF(window_available_width), 0);
  rb_define_module_function(Window, "available_height", RMF(window_available_height), 0);
}
