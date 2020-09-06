#include "extras.hpp"
#include "Color.hpp"

static VALUE RColor;
static unsigned counter;

template<class C>
static void free_instance(void *inst)
{
  C *c = static_cast<C*>(inst);
  if (c != 0 && counter > 0) {
    counter -= 1;
    delete c;
  }
}

rb_data_type_t ColorType = { "Color",
  { 0, free_instance<Color>, 0, { 0, 0 } }, 0, 0, 0 };

template<rb_data_type_t *ColorType>
static VALUE color_allocate(VALUE klass)
{
  return rb_data_typed_object_wrap(klass, 0, ColorType);
}

void color_counter_reduce()
{
  counter -= 1;
}

static VALUE color_init(int argc, VALUE *argv, VALUE self)
{
  if (argc == 0 || argc == 2 || argc > 4)
    rb_raise(rb_eArgError, 
      "Wrong number of arguments (given %d, expected 1 or 3..4)", argc);
  counter += 1;
  Color *c;
  if (argc == 1) {
    c = new Color(RB_FIX2INT(argv[0]));
  } if (argc == 3) {
    int red   = RB_FIX2INT(argv[0]);
    int green = RB_FIX2INT(argv[1]);
    int blue  = RB_FIX2INT(argv[2]);
    c = new Color(red, green, blue);
  } else {
    int alpha = RB_FIX2INT(argv[0]);
    int red   = RB_FIX2INT(argv[1]);
    int green = RB_FIX2INT(argv[2]);
    int blue  = RB_FIX2INT(argv[3]);
    c = new Color(alpha, red, green, blue);
  }
  RTYPEDDATA(self)->data = c;
  return self;
}

static VALUE color_alpha(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  return RB_INT2FIX(c->alpha());
}

static VALUE color_red(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  return RB_INT2FIX(c->red());
}

static VALUE color_green(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  return RB_INT2FIX(c->green());
}

static VALUE color_blue(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  return RB_INT2FIX(c->blue());
}

static VALUE color_alpha_set(VALUE self, VALUE alpha)
{
  Color *c = (Color*)RTYPEDDATA(self)->data;
  if (!c || alpha == Qnil) return Qnil;
  c->set_alpha(RB_FIX2INT(alpha));
  return RB_INT2FIX(c->alpha());
}

static VALUE color_red_set(VALUE self, VALUE red)
{
  Color *c = (Color*)RTYPEDDATA(self)->data;
  if (!c || red == Qnil) return Qnil;
  c->set_red(RB_FIX2INT(red));
  return RB_INT2FIX(c->red());
}

static VALUE color_green_set(VALUE self, VALUE green)
{
  Color *c = (Color*)RTYPEDDATA(self)->data;
  if (!c || green == Qnil) return Qnil;
  c->set_green(RB_FIX2INT(green));
  return RB_INT2FIX(c->green());
}

static VALUE color_blue_set(VALUE self, VALUE blue)
{
  Color *c = (Color*)RTYPEDDATA(self)->data;
  if (!c || blue == Qnil) return Qnil;
  c->set_blue(RB_FIX2INT(blue));
  return RB_INT2FIX(c->blue());
}

static VALUE color_argb(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  return RB_INT2FIX(c->argb());
}

static VALUE color_gl(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  return !c ? Qnil : RB_INT2FIX(c->gl());
}

static VALUE color_hue(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  return (!c) ? Qnil : RB_INT2FIX(c->hue());
}

static VALUE color_saturation(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  return (!c) ? Qnil : RB_INT2FIX(c->saturation());
}

static VALUE color_value(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  return (!c) ? Qnil : RB_INT2FIX(c->value());
}

static VALUE color_hue_set(VALUE self, VALUE hue)
{
  Color *c = (Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  c->set_hue(NUM2DBL(hue));
  return DBL2NUM(c->hue());
}

static VALUE color_saturation_set(VALUE self, VALUE sat)
{
  Color *c = (Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  c->set_saturation(NUM2DBL(sat));
  return DBL2NUM(c->saturation());
}

static VALUE color_value_set(VALUE self, VALUE value)
{
  Color *c = (Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  c->set_value(NUM2DBL(value));
  return DBL2NUM(c->value());
}

static VALUE color_dup(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return Qnil;
  counter += 1;
  Color *clr = new Color(c->gl());
  VALUE color = rb_data_typed_object_wrap(RColor, 0, &ColorType);
  RTYPEDDATA(color)->data = clr;
  return color;
}

static VALUE color_inspect(VALUE self)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return rstr("No Color was found!");
  char buffer[48];
  snprintf(buffer, sizeof buffer, "#<Color:ARGB=0x%02x_%02x_%02x_%02x>",
    c->alpha(), c->red(), c->green(), c->blue());
  return rstr(buffer);
}

static VALUE color_is_equal(VALUE self, VALUE other)
{
  const Color *c = (const Color*)RTYPEDDATA(self)->data;
  if (!c) return Qfalse;
  const Color *o = (const Color*)RTYPEDDATA(other)->data;
  if (!o) return Qfalse;
  return c->gl() == o->gl() ? Qtrue : Qfalse;
}

void init_color()
{
  RColor = rb_define_class("Color", rb_cObject);
  rb_define_alloc_func(RColor, color_allocate<&ColorType>);
  rb_define_method(RColor, "initialize", RMF(color_init), -1);
  rb_define_const(RColor, "GL_FORMAT", RB_INT2FIX(static_cast<unsigned int>(Color::GL_FORMAT)));
  /*rb_define_singleton_method(RColor, "from_hsv", RMF(color_from_hsv), -1);
  rb_define_singleton_method(RColor, "from_ahsv", RMF(color_from_ahsv), -1);*/
  rb_define_method(RColor, "red", RMF(color_red), 0);
  rb_define_method(RColor, "green", RMF(color_green), 0);
  rb_define_method(RColor, "blue", RMF(color_blue), 0);
  rb_define_method(RColor, "alpha", RMF(color_alpha), 0);
  rb_define_method(RColor, "red=", RMF(color_red_set), 1);
  rb_define_method(RColor, "green=", RMF(color_green_set), 1);
  rb_define_method(RColor, "blue=", RMF(color_blue_set), 1);
  rb_define_method(RColor, "alpha=", RMF(color_alpha_set), 1);
  rb_define_method(RColor, "hue", RMF(color_hue), 0);
  rb_define_method(RColor, "saturation", RMF(color_saturation), 0);
  rb_define_method(RColor, "value", RMF(color_value), 0);
  rb_define_method(RColor, "hue=", RMF(color_hue_set), 1);
  rb_define_method(RColor, "saturation=", RMF(color_saturation_set), 1);
  rb_define_method(RColor, "value=", RMF(color_value_set), 1);
  /*rb_define_method(RColor, "bgr", RMF(color_bgr), 0);
  rb_define_method(RColor, "abgr", RMF(color_abgr), 0);*/
  //rb_define_singleton_method(Color, "rgb", RMF(color_rgb), 0);
  //rb_define_singleton_method(Color, "rgba", RMF(color_rgba), 0);
  rb_define_method(RColor, "gl", RMF(color_gl), 0);
  rb_define_method(RColor, "argb", RMF(color_argb), 0);
  rb_define_method(RColor, "to_int", RMF(color_argb), 0);
  rb_define_method(RColor, "to_i", RMF(color_argb), 0);
  rb_define_method(RColor, "dup", RMF(color_dup), 0);
  rb_define_method(RColor, "inspect", RMF(color_inspect), 0);
  rb_define_method(RColor, "to_str", RMF(color_inspect), 0);
  rb_define_method(RColor, "to_s", RMF(color_inspect), 0);
  rb_define_method(RColor, "===", RMF(color_is_equal), 1);
  rb_define_method(RColor, "==", RMF(color_is_equal), 1);
}
