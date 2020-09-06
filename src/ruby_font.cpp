#include "extras.hpp"
#include "Bitmap.hpp"
#include "Font.hpp"

static unsigned counter;
static VALUE RColor, zero, one, onefl, max;

void color_counter_reduce();

extern rb_data_type_t ColorType;

template<class C>
static void free_instance(void *inst)
{
  C *c = static_cast<C*>(inst);
  if (c != 0 && counter > 0) {
    counter -= 1;
    delete c;
  }
}

rb_data_type_t FontType = { "Font",
  { 0, free_instance<Roole::Font>, 0, { 0, 0 } }, 0, 0, 0 };

template<rb_data_type_t *FontType>
static VALUE font_allocate(VALUE klass)
{
  return rb_data_typed_object_wrap(klass, 0, FontType);
}

void font_init_vars(VALUE self)
{
  rb_iv_set(self, "@x", zero);
  rb_iv_set(self, "@y", zero);
  rb_iv_set(self, "@z", one);
  rb_iv_set(self, "@scale_x", onefl);
  rb_iv_set(self, "@scale_y", onefl);
  rb_iv_set(self, "@rel_x", zero);
  rb_iv_set(self, "@rel_y", zero);
  rb_iv_set(self, "@visible", Qtrue);
}

void font_store_color(VALUE self, void *c)
{
  VALUE color = rb_data_typed_object_wrap(RColor, 0, &ColorType);
  RTYPEDDATA(color)->data = c;
  rb_iv_set(self, "@color", color);
}

static VALUE font_init(int argc, VALUE *argv, VALUE self)
{
  if (argc < 2)
    rb_raise(rb_eArgError,
      "Wrong number of arguments (given %d, expected 2..3)");
  counter += 1;
  Check_Type(argv[0], T_FIXNUM);
  Check_Type(argv[1], T_STRING);
  rb_iv_set(self, "@height", argv[0]);
  rb_iv_set(self, "@name", argv[1]);
  int flags = 0;
  if (argc == 3) {
    VALUE opt = argv[2];
    if (strcmp(rb_obj_classname(opt), "Hash"))
      rb_raise(rb_eTypeError,
        "Third argument should be a Hash or keyword argument!");
    VALUE val = rb_hash_delete(opt, rb_sym("bold"));
    rb_iv_set(self, "@bold", val == Qtrue ? Qtrue : Qfalse);
    if (val == Qtrue) flags |= Roole::FF_BOLD;
    val = rb_hash_delete(opt, rb_sym("italic"));
    rb_iv_set(self, "@italic", val == Qtrue ? Qtrue : Qfalse);
    if (val == Qtrue) flags |= Roole::FF_ITALIC;
    val = rb_hash_delete(opt, rb_sym("underline"));
    rb_iv_set(self, "@underline", val == Qtrue ? Qtrue : Qfalse);
    if (val == Qtrue) flags |= Roole::FF_UNDERLINE;
  }
  int height = RB_FIX2INT(argv[0]);
  char *cname = RSTRING_PTR(argv[1]);
  Roole::Font *f = new Roole::Font(height, std::string(cname), flags);
  RTYPEDDATA(self)->data = f;
  font_init_vars(self);
  font_store_color(self, &f->color());
  return self;
}

static VALUE font_height_set(VALUE self, VALUE height)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return zero;
  f->set_height(RB_FIX2INT(height));
  rb_iv_set(self, "@height", height);
  return height;
}

static VALUE font_name_set(VALUE self, VALUE name)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rstr("");
  char *cname = RSTRING_PTR(name);
  f->set_name(std::string(cname));
  rb_iv_set(self, "@name", name);
  return name;
}

static VALUE font_text_set(VALUE self, VALUE text)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rstr("");
  char *ctext = RSTRING_PTR(text);
  f->set_text(std::string(ctext));
  rb_iv_set(self, "@width", RB_INT2FIX( f->text_width() ));
  rb_iv_set(self, "@width_markup", RB_INT2FIX( f->markup_width() ));
  rb_iv_set(self, "@text", text);
  return text;
}

static VALUE font_x_set(VALUE self, VALUE x)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@x", zero);
  f->set_x(NUM2DBL(x));
  return rb_iv_set(self, "@x", x);
}

static VALUE font_y_set(VALUE self, VALUE y)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@y", zero);
  f->set_y(NUM2DBL(y));
  return rb_iv_set(self, "@y", y);
}

static VALUE font_z_set(VALUE self, VALUE z)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@z", zero);
  f->set_z(NUM2DBL(z));
  return rb_iv_set(self, "@z", z);
}

static VALUE font_xy_set(VALUE self, VALUE x, VALUE y)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_ary_new3(2, zero, zero);
  f->set_x(NUM2DBL(x));
  f->set_y(NUM2DBL(y));
  rb_iv_set(self, "@x", x);
  rb_iv_set(self, "@y", y);
  return rb_ary_new3(2, x, y);
}

static VALUE font_xyz_set(VALUE self, VALUE x, VALUE y, VALUE z)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_ary_new3(3, zero, zero, zero);
  f->set_xyz(NUM2DBL(x), NUM2DBL(y), RB_FIX2INT(z));
  rb_iv_set(self, "@x", x);
  rb_iv_set(self, "@y", y);
  rb_iv_set(self, "@z", z);
  return rb_ary_new3(3, x, y, z);
}

static VALUE font_scale_x_set(VALUE self, VALUE x)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@scale_x", zero);
  f->set_scale_x(NUM2DBL(x));
  return rb_iv_set(self, "@scale_x", x);
}

static VALUE font_scale_y_set(VALUE self, VALUE y)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@scale_y", zero);
  f->set_scale_y(NUM2DBL(y));
  return rb_iv_set(self, "@scale_y", y);
}

static VALUE font_scale_xy_set(VALUE self, VALUE xy)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return zero;
  double sy = NUM2DBL(xy);
  f->set_scale_x(sy);
  f->set_scale_y(sy);
  rb_iv_set(self, "@scale_x", xy);
  return rb_iv_set(self, "@scale_y", xy);
}

static VALUE font_rel_x_set(VALUE self, VALUE x)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@rel_x", zero);
  f->set_rel_x(NUM2DBL(x));
  return rb_iv_set(self, "@rel_x", x);
}

static VALUE font_rel_y_set(VALUE self, VALUE y)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@rel_y", zero);
  f->set_rel_y(NUM2DBL(y));
  return rb_iv_set(self, "@rel_y", y);
}

static VALUE font_rel_xy_set(VALUE self, VALUE xy)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return zero;
  double sy = NUM2DBL(xy);
  f->set_rel_x(sy);
  f->set_rel_y(sy);
  rb_iv_set(self, "@rel_x", xy);
  return rb_iv_set(self, "@rel_y", xy);
}

static VALUE font_color_set(VALUE self, VALUE color)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_get(self, "@color");
  Color *clr = (Color*)RTYPEDDATA(color)->data;
  f->set_color(clr->gl());
  font_store_color(self, &f->color());
  color_counter_reduce();
  return color;
}

static VALUE font_alpha_set(VALUE self, VALUE alpha)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_get(self, "@color");
  f->color().set_alpha( RB_FIX2INT(alpha) );
  return RB_INT2FIX(f->color().alpha());
}

static VALUE font_visible_set(VALUE self, VALUE seen)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return rb_iv_set(self, "@visible", Qnil);
  f->set_visible(seen == Qtrue);
  return rb_iv_set(self, "@visible", seen);
}

static VALUE font_draw(VALUE self)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return Qnil;
  f->draw();
  return Qnil;
}

static VALUE font_draw_markup(VALUE self)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return Qnil;
  f->draw();
  return Qnil;
}

static VALUE font_draw_rel(VALUE self)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return Qnil;
  f->draw_rel();
  return Qnil;
}

static VALUE font_draw_markup_rel(VALUE self)
{
  Roole::Font *f = (Roole::Font*)RTYPEDDATA(self)->data;
  if (!f) return Qnil;
  f->draw_markup_rel();
  return Qnil;
}

void init_font()
{
  zero = RB_INT2FIX(0);
  one = RB_INT2FIX(1);
  onefl = DBL2NUM(1.0);
  max = RB_INT2FIX(255);
  RColor = rb_define_class("Color", rb_cObject);
  VALUE RFont = rb_define_class("Font", rb_cObject);
  rb_define_alloc_func(RFont, font_allocate<&FontType>);
  rb_define_attr(RFont, "width", 1, 0);
  rb_define_attr(RFont, "width_markup", 1, 0);
  rb_define_attr(RFont, "height", 1, 0);
  rb_define_attr(RFont, "name", 1, 0);
  rb_define_attr(RFont, "bold", 1, 0);
  rb_define_attr(RFont, "italic", 1, 0);
  rb_define_attr(RFont, "underline", 1, 0);
  rb_define_attr(RFont, "text", 1, 0);
  rb_define_attr(RFont, "x", 1, 0);
  rb_define_attr(RFont, "y", 1, 0);
  rb_define_attr(RFont, "z", 1, 0);
  rb_define_attr(RFont, "scale_x", 1, 0);
  rb_define_attr(RFont, "scale_y", 1, 0);
  rb_define_attr(RFont, "rel_x", 1, 0);
  rb_define_attr(RFont, "rel_y", 1, 0);
  rb_define_attr(RFont, "color", 1, 0);
  rb_define_attr(RFont, "visible", 1, 0);
  rb_define_method(RFont, "initialize", RMF(font_init), -1);
  rb_define_method(RFont, "height=", RMF(font_height_set), 1);
  rb_define_method(RFont, "name=", RMF(font_name_set), 1);
  rb_define_method(RFont, "text=", RMF(font_text_set), 1);
  rb_define_method(RFont, "x=", RMF(font_x_set), 1);
  rb_define_method(RFont, "y=", RMF(font_y_set), 1);
  rb_define_method(RFont, "z=", RMF(font_z_set), 1);
  rb_define_method(RFont, "set_xy", RMF(font_xy_set), 2);
  rb_define_method(RFont, "set_xyz", RMF(font_xyz_set), 3);
  rb_define_method(RFont, "scale_x=", RMF(font_scale_x_set), 1);
  rb_define_method(RFont, "scale_y=", RMF(font_scale_y_set), 1);
  rb_define_method(RFont, "scale_xy=", RMF(font_scale_xy_set), 1);
  rb_define_method(RFont, "rel_x=", RMF(font_rel_x_set), 1);
  rb_define_method(RFont, "rel_y=", RMF(font_rel_y_set), 1);
  rb_define_method(RFont, "rel_xy=", RMF(font_rel_xy_set), 1);
  rb_define_method(RFont, "color=", RMF(font_color_set), 1);
  rb_define_method(RFont, "alpha=", RMF(font_alpha_set), 1);
  rb_define_method(RFont, "visible=", RMF(font_visible_set), 1);
  rb_define_method(RFont, "draw", RMF(font_draw), 0);
  rb_define_method(RFont, "draw_markup", RMF(font_draw_markup), 0);
  rb_define_method(RFont, "draw_rel", RMF(font_draw_rel), 0);
  rb_define_method(RFont, "draw_markup_rel", RMF(font_draw_markup_rel), 0);
}