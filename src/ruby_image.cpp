#include "common.hpp"
#include <cstring>
#include <iostream>
#include "Image.hpp"

static VALUE String, Hash, zero;

rb_data_type_t ImageType = { "Image",
  { 0, free_instance<Roole::Image>, 0, { 0, 0 } }, 0, 0, 0 };

template<rb_data_type_t *ImageType>
static VALUE image_allocate(VALUE klass)
{
  return rb_data_typed_object_wrap(klass, 0, ImageType);
}

void image_clear_sprite(VALUE self)
{
  if (RTYPEDDATA(self)->data != 0) {
    Roole::Image *s = (Roole::Image*)RTYPEDDATA(self)->data;
    delete s;
  }
  RTYPEDDATA(self)->data = new Roole::Image();
  rb_iv_set(self, "@filename", rstr(""));
  rb_iv_set(self, "@width", zero);
  rb_iv_set(self, "@height", zero);
}

void raise_runtime(const std::string e)
{
  rb_raise(rb_eRuntimeError, "%s: File could not be properly allocated!", e.c_str());
}

static VALUE image_init(int argc, VALUE *argv, VALUE self)
{
  if (argc > 2)
    rb_raise(rb_eArgError, "Wrong number of arguments (given %d, expected 0..2)\n", argc);
  Roole::Image *s;
  if (argc > 0) {
    VALUE kind, first = argv[0], second = argv[1];
    if (rb_obj_class(first) == String) {//first = rb_str_to_str(first);
      first = rb_str_to_str(first);
      rb_p(first);
      char* cname = StringValueCStr(first);
      std::string name = cname;
      VALUE tmp = rb_str_new_cstr(name.c_str());
      name = StringValueCStr(tmp);
      printf("Image's filename: %s\n", name.c_str());
      std::cout << name << std::endl;
      int flags;
      if (rb_obj_class(second) == Hash) {
        kind = rb_obj_class(second);
        flags = rb_sym("TrueClass") == kind ? 1 : 0;
        try {
          s = new Roole::Image(name.c_str(), flags);
        } catch (const std::bad_alloc& e) {
          raise_runtime(e.what());
        } catch (const std::exception& e) {
          rb_raise(rb_eIOError, "File was not found!\n%s", e.what());
          return Qnil;
        }
      } else {
        try {
          s = new Roole::Image(name.c_str());
        } catch (const std::bad_alloc& e) {
          raise_runtime(e.what());
        } catch (const std::exception& e) {
          rb_raise(rb_eIOError, "File was not found!\n%s", e.what());
          return Qnil;
        }
      }
      RTYPEDDATA(self)->data = s;
      rb_iv_set(self, "@filename", first);
      rb_iv_set(self, "@width", RB_INT2FIX(s->width()));
      rb_iv_set(self, "@height", RB_INT2FIX(s->height()));
    }
  } else {
    image_clear_sprite(self);
  }
  return self;
}

static VALUE image_draw(VALUE self, VALUE rx, VALUE ry, VALUE rz)
{
  Roole::Image *s = (Roole::Image*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  double x = NUM2DBL(rx), y = NUM2DBL(ry);
  unsigned z = RB_FIX2INT(z);
  s->draw(x, y, z);
  return Qnil;
}

void init_image()
{
  zero = RB_INT2FIX(0);
  String = rb_const_get(rb_cObject, rb_intern("String"));
  Hash = rb_const_get(rb_cObject, rb_intern("Hash"));
  VALUE hidden = rb_define_module("Roole");
  VALUE image = rb_define_class_under(hidden, "Image", rb_cObject);
  rb_define_alloc_func(image, image_allocate<&ImageType>);
  define_dispose<Roole::Image>(image);
  rb_define_method(image, "initialize", RMF(image_init), -1);
  rb_define_method(image, "draw", RMF(image_draw), 3);
}
