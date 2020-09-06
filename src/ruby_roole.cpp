#include "ruby_window.hpp"
#include "Graphics.hpp"

static VALUE Integer, Float;

static VALUE graphics_global_scale(VALUE self)
{
  return rb_iv_get(self, "@scale");
}

static VALUE graphics_global_scale_set(VALUE self, VALUE new_scale)
{
  VALUE klass = rb_obj_class(new_scale);
  if (klass != Integer || klass != Float)
    return rb_iv_get(self, "@scale");
  Roole::Graphics::global_scale = NUM2DBL(new_scale);
  rb_iv_set(self, "@scale", new_scale);
  return new_scale;
}

static VALUE graphics_dimensions(VALUE self)
{
  return rb_ary_new3(2, rb_iv_get(self, "@width"), rb_iv_get(self, "@height"));
}

void init_roole()
{
  VALUE Roole = rb_define_module("Roole");
  VALUE Graphics = rb_define_module("Graphics");
  Integer = rb_const_get(rb_cObject, rb_intern("Integer"));
  Float = rb_const_get(rb_cObject, rb_intern("Float"));
  double scale = Roole::Graphics::global_scale;
  rb_iv_set(Graphics, "@scale", DBL2NUM(scale));
  rb_define_attr(Graphics, "width", 1, 0);
  rb_define_attr(Graphics, "height", 1, 0);
  VALUE methods[] = { rb_sym("width"), rb_sym("height") };
  VALUE modfunc = rb_obj_method(Graphics, rb_sym("module_function"));
  rb_method_call(2, methods, modfunc);
  rb_define_module_function(Graphics, "dimensions", RMF(graphics_dimensions), 0);
  rb_define_module_function(Graphics, "scale", RMF(graphics_global_scale), 0);
  rb_define_module_function(Graphics, "scale=", RMF(graphics_global_scale_set), 1);
}
