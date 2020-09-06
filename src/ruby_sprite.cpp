//#include "common.hpp"
#include "extras.hpp"
#include <cstring>
#include "Bitmap.hpp"
#include "Sprite.hpp"

static VALUE RSprite, RColor, String, Hash, zero, half, one, max;
static unsigned counter;

void color_counter_reduce();

extern rb_data_type_t ColorType;

template<class C>
static void free_instance(void *inst)
{
  C *c = static_cast<C*>(inst);
  if (c != 0 && c->is_disposed()) {
    counter -= 1;
    delete c;
  }
}

rb_data_type_t SpriteType = { "Sprite",
  { 0, free_instance<Sprite>, 0, { 0, 0 } }, 0, 0, 0 };

void sprite_set_attr(VALUE self, bool seen)
{
  rb_iv_set(self, "@x", zero);
  rb_iv_set(self, "@y", zero);
  rb_iv_set(self, "@z", zero);
  rb_iv_set(self, "@scale_x", one);
  rb_iv_set(self, "@scale_y", one);
  rb_iv_set(self, "@center_x", half);
  rb_iv_set(self, "@center_y", half);
  rb_iv_set(self, "@angle", zero);
  rb_iv_set(self, "@mode", rb_sym("default"));
  rb_iv_set(self, "@visible", seen ? Qtrue : Qfalse);
}

void sprite_clear_sprite(VALUE self)
{
  if (RTYPEDDATA(self)->data != 0) {
    Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
    delete s;
  }
  RTYPEDDATA(self)->data = new Sprite();
  rb_iv_set(self, "@visible", Qfalse);
  rb_iv_set(self, "@filename", rstr(""));
  rb_iv_set(self, "@width", zero);
  rb_iv_set(self, "@height", zero);
}

void store_color(VALUE self, void *c, const char *ivar)
{
  VALUE color = rb_data_typed_object_wrap(RColor, 0, &ColorType);
  RTYPEDDATA(color)->data = c;
  rb_iv_set(self, ivar, color);
}

static VALUE sprite_init(int argc, VALUE *argv, VALUE self)
{
  if (argc > 2)
    rb_raise(rb_eArgError, "Wrong number of arguments (given %d, expected 0..2)\n", argc);
  counter += 1;
  Sprite *s;
  if (argc > 0) {
    VALUE kind, second = argv[1];
    if (rb_obj_class(argv[0]) == String) {
      const char *name = RSTRING_PTR(argv[0]);
      int flags;
      if (argc == 2 && rb_obj_class(second) == Hash) {
        kind = rb_obj_class(second);
        flags = rb_sym("TrueClass") == kind ? 1 : 0;
        try {
          s = new Sprite(name, flags);
        } catch (const std::exception& e) {
          rb_raise(rb_eIOError, "File was not found!\n%s", e.what());
          return Qnil;
        }
      } else {
        try {
          s = new Sprite(name);
        } catch (const std::exception& e) {
          rb_raise(rb_eIOError, "File was not found!\n%s", e.what());
          return Qnil;
        }
      }
      RTYPEDDATA(self)->data = s;
      s->set_visible(true);
      sprite_set_attr(self, true);
      store_color(self, &s->color(), "@color");
      store_color(self, &s->color1(), "@color1");
      store_color(self, &s->color2(), "@color2");
      store_color(self, &s->color3(), "@color3");
      store_color(self, &s->color4(), "@color4");
      rb_iv_set(self, "@filename", argv[0]);
      rb_iv_set(self, "@width", RB_INT2FIX(s->width()));
      rb_iv_set(self, "@height", RB_INT2FIX(s->height()));
    }
  } else {
    sprite_set_attr(self, false);
    sprite_clear_sprite(self);
  }
  return self;
}

static VALUE sprite_load(VALUE self, VALUE string)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (s != 0) delete s;
  VALUE klass = rb_obj_class(string);
  if (klass == rb_sym("String")) {
    bool is_name = RSTRING_LEN(string) > 0;
    if (is_name) {
      const char *name = RSTRING_PTR(string);
      try {
        s = new Sprite(name);
      } catch (const std::exception& e) {
        rb_raise(rb_eIOError, "File was not found!\n%s", e.what());
        return Qnil;
      }
      RTYPEDDATA(self)->data = s;
      s->set_visible(true);
      store_color(self, &s->color(), "@color");
      store_color(self, &s->color1(), "@color1");
      store_color(self, &s->color2(), "@color2");
      store_color(self, &s->color3(), "@color3");
      store_color(self, &s->color4(), "@color4");
      rb_iv_set(self, "@visible", Qtrue);
      rb_iv_set(self, "@filename", string);
    } else {
      sprite_clear_sprite(self);
    }
  } else if (klass == Qnil) {
    sprite_clear_sprite(self);
  }
  return self;
}

static VALUE sprite_x_set(VALUE self, VALUE x)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@x", zero);
  s->set_x(NUM2DBL(x));
  return rb_iv_set(self, "@x", x);
}

static VALUE sprite_y_set(VALUE self, VALUE y)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@y", zero);
  s->set_y(NUM2DBL(y));
  return rb_iv_set(self, "@y", y);
}

static VALUE sprite_z_set(VALUE self, VALUE z)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@z", zero);
  s->set_z(NUM2DBL(z));
  return rb_iv_set(self, "@z", z);
}

static VALUE sprite_xy_set(VALUE self, VALUE x, VALUE y)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_ary_new3(2, zero, zero);
  s->set_x(NUM2DBL(x));
  s->set_y(NUM2DBL(y));
  rb_iv_set(self, "@x", x);
  rb_iv_set(self, "@y", y);
  return rb_ary_new3(2, x, y);
}

static VALUE sprite_xyz_set(VALUE self, VALUE x, VALUE y, VALUE z)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_ary_new3(3, zero, zero, zero);
  s->set_xyz(NUM2DBL(x), NUM2DBL(y), RB_FIX2INT(z));
  rb_iv_set(self, "@x", x);
  rb_iv_set(self, "@y", y);
  rb_iv_set(self, "@z", z);
  return rb_ary_new3(3, x, y, z);
}

static VALUE sprite_scale_x_set(VALUE self, VALUE x)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@scale_x", zero);
  s->set_scale_x(NUM2DBL(x));
  return rb_iv_set(self, "@scale_x", x);
}

static VALUE sprite_scale_y_set(VALUE self, VALUE y)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@scale_y", zero);
  s->set_scale_y(NUM2DBL(y));
  return rb_iv_set(self, "@scale_y", y);
}

static VALUE sprite_scale_xy_set(VALUE self, VALUE xy)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return zero;
  double sy = NUM2DBL(xy);
  s->set_scale_x(sy);
  s->set_scale_y(sy);
  rb_iv_set(self, "@scale_x", xy);
  return rb_iv_set(self, "@scale_y", xy);
}

static VALUE sprite_center_x_set(VALUE self, VALUE x)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@center_x", zero);
  s->set_center_x(NUM2DBL(x));
  return rb_iv_set(self, "@center_x", x);
}

static VALUE sprite_center_y_set(VALUE self, VALUE y)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@center_y", zero);
  s->set_center_y(NUM2DBL(y));
  return rb_iv_set(self, "@center_y", y);
}

static VALUE sprite_angle_set(VALUE self, VALUE angle)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@angle", zero);
  s->set_angle(NUM2DBL(angle));
  return rb_iv_set(self, "@angle", angle);
}

static VALUE sprite_color_set(VALUE self, VALUE color)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_get(self, "@color");
  Color *clr = (Color*)RTYPEDDATA(color)->data;
  s->set_color(clr->gl());
  store_color(self, &s->color(), "@color");
  color_counter_reduce();
  return color;
}

static VALUE sprite_alpha_set(VALUE self, VALUE alpha)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_get(self, "@color");
  s->color().set_alpha( RB_FIX2INT(alpha) );
  return RB_INT2FIX(s->color().alpha());
}

static VALUE sprite_visible_set(VALUE self, VALUE seen)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (!s) return rb_iv_set(self, "@visible", Qnil);
  s->set_visible(seen == Qtrue);
  return rb_iv_set(self, "@visible", seen);
}

static VALUE sprite_draw(VALUE self)
{
  Sprite *s = (Sprite*)RTYPEDDATA(self)->data;
  if (s != 0) s->draw();
  return Qnil;
}

static VALUE sprite_dup(VALUE self)
{
  const Sprite *s = (const Sprite*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  counter += 1;
  VALUE name[1] = { rb_iv_get(self, "@filename") };
  VALUE sprite = rb_data_typed_object_wrap(RSprite, 0, &SpriteType);
  sprite = sprite_init(1, name, self);
  return sprite;
}

template<rb_data_type_t *SpriteType>
static VALUE sprite_allocate(VALUE klass)
{
  return rb_data_typed_object_wrap(klass, 0, SpriteType);
}

static VALUE tiles_ary(VALUE source, const Roole::Bitmap& bmp, int width, int height, unsigned flags)
{
  int tiles_x, tiles_y, xw, yh;
  if (width > 0) {
    tiles_x = bmp.width() / width;
  } else {
    tiles_x = -width;
    width = bmp.width() / tiles_x;
  }
  if (height > 0) {
    tiles_y = bmp.height() / height;
  } else {
    tiles_y = -height;
    height = bmp.height() / tiles_y;
  }
  VALUE sprite, tiles = rb_ary_new();
  for (int y = 0; y < tiles_y; y++) {
    yh = y * height;
    for (int x = 0; x < tiles_x; x++) {
      xw = x * width;
      Sprite *s = new Sprite(bmp, xw, yh, width, height, flags);
      s->set_visible(true);
      sprite = rb_data_typed_object_wrap(RSprite, 0, &SpriteType);
      RTYPEDDATA(sprite)->data = s;
      rb_ary_push(tiles, sprite);
      sprite_set_attr(sprite, true);
      rb_iv_set(sprite, "@width", RB_INT2FIX(s->width()));
      rb_iv_set(sprite, "@height", RB_INT2FIX(s->height()));
      rb_iv_set(sprite, "@filename", source);
      store_color(sprite, &s->color(), "@color");
      store_color(sprite, &s->color1(), "@color1");
      store_color(sprite, &s->color2(), "@color2");
      store_color(sprite, &s->color3(), "@color3");
      store_color(sprite, &s->color4(), "@color4");
    }
  }
  return tiles;
}

static VALUE sprite_load_tiles(int argc, VALUE *argv, VALUE self)
{
  if (argc < 3 || argc > 4)
    rb_raise(rb_eArgError, "Wrong number of arguments (given %d, expected 3..4)", argc);
  long width = RB_FIX2INT(argv[1]), height = RB_FIX2INT(argv[2]);
  VALUE options = argc == 4 ? argv[3] : 0;
  unsigned flags = 0;
  Roole::Bitmap bmp;//load_sprite_bitmap(bmp, source);
  const char* filename = RSTRING_PTR(argv[0]);
  Roole::load_image_file(bmp, filename);
  if (options) {
    Check_Type(options, T_HASH);
    VALUE keys = rb_funcall(options, rb_intern("keys"), 0, NULL);
    int keys_size = RARRAY_LEN(keys);
    for (int i = 0; i < keys_size; i++) {
      VALUE key = rb_ary_entry(keys, i);
      VALUE value = rb_hash_aref(options, key);
      if (key == rb_sym("tileable") && value == Qtrue)
        flags |= Roole::IF_TILEABLE;
      else if (key == rb_sym("retro") && value == Qtrue)
        flags |= Roole::IF_RETRO;
    }
  }
  return tiles_ary(argv[0], bmp, width, height, flags);
}

void init_sprite()
{
  zero = RB_INT2FIX(0);
  half = DBL2NUM(0.5);
  one = DBL2NUM(1.0);
  max = RB_INT2FIX(255);
  VALUE hidden = rb_const_get(rb_cObject, rb_intern("Roole"));
  RColor = rb_const_get(hidden, rb_intern("Color"));
  Hash = rb_const_get(rb_cObject, rb_intern("Hash"));
  String = rb_const_get(rb_cObject, rb_intern("String"));
  RSprite = rb_define_class("Sprite", rb_cObject);
  rb_define_alloc_func(RSprite, sprite_allocate<&SpriteType>);
  rb_define_attr(RSprite, "x", 1, 0);
  rb_define_attr(RSprite, "y", 1, 0);
  rb_define_attr(RSprite, "z", 1, 0);
  rb_define_attr(RSprite, "scale_x", 1, 0);
  rb_define_attr(RSprite, "scale_y", 1, 0);
  rb_define_attr(RSprite, "center_x", 1, 0);
  rb_define_attr(RSprite, "center_y", 1, 0);
  rb_define_attr(RSprite, "angle", 1, 0);
  rb_define_attr(RSprite, "color", 1, 0);
  rb_define_attr(RSprite, "color1", 1, 0);
  rb_define_attr(RSprite, "color2", 1, 0);
  rb_define_attr(RSprite, "color3", 1, 0);
  rb_define_attr(RSprite, "color4", 1, 0);
  rb_define_attr(RSprite, "mode", 1, 0);
  rb_define_attr(RSprite, "width", 1, 0);
  rb_define_attr(RSprite, "height", 1, 0);
  rb_define_attr(RSprite, "visible", 1, 0);
  rb_define_attr(RSprite, "filename", 1, 1);
  rb_define_method(RSprite, "initialize", RMF(sprite_init), -1);
  rb_define_method(RSprite, "load", RMF(sprite_load), 1);
  rb_define_method(RSprite, "x=", RMF(sprite_x_set), 1);
  rb_define_method(RSprite, "y=", RMF(sprite_y_set), 1);
  rb_define_method(RSprite, "z=", RMF(sprite_z_set), 1);
  rb_define_method(RSprite, "set_xy", RMF(sprite_xy_set), 2);
  rb_define_method(RSprite, "set_xyz", RMF(sprite_xyz_set), 3);
  rb_define_method(RSprite, "scale_x=", RMF(sprite_scale_x_set), 1);
  rb_define_method(RSprite, "scale_y=", RMF(sprite_scale_y_set), 1);
  rb_define_method(RSprite, "scale_xy=", RMF(sprite_scale_xy_set), 1);
  rb_define_method(RSprite, "center_x=", RMF(sprite_center_x_set), 1);
  rb_define_method(RSprite, "center_y=", RMF(sprite_center_y_set), 1);
  rb_define_method(RSprite, "angle=", RMF(sprite_angle_set), 1);
  rb_define_method(RSprite, "color=", RMF(sprite_color_set), 1);
  rb_define_method(RSprite, "alpha=", RMF(sprite_alpha_set), 1);
  rb_define_method(RSprite, "visible=", RMF(sprite_visible_set), 1);
  rb_define_method(RSprite, "draw", RMF(sprite_draw), 0);
  rb_define_method(RSprite, "dup", RMF(sprite_dup), 0);
  rb_define_module_function(RSprite, "load_tiles", RMF(sprite_load_tiles), -1);
}
