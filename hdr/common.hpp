// Roole Game Engine
// Kyonides Arkanthes 2020

#include "extras.hpp"

template<class C>
static void free_instance(void *inst)
{
  C *c = static_cast<C*>(inst);
  if (c != 0) delete c;
}

template<class C>
static VALUE klass_dispose(VALUE self)
{
  C *d = static_cast<C*>(RTYPEDDATA_DATA(self));
  if (!d || d->is_disposed()) return Qnil;
  d->dispose();
  return Qtrue;
}

template<class C>
static VALUE klass_is_disposed(VALUE self)
{
  C *d = static_cast<C*>(RTYPEDDATA(self)->data);
  if (!d) return Qtrue;
  return d->is_disposed() ? Qtrue : Qfalse;
}

template<class C>
static void define_dispose(VALUE klass)
{
  rb_define_method(klass, "dispose", RMF(klass_dispose<C>), 0);
  rb_define_method(klass, "destroy", RMF(klass_dispose<C>), 0);
  rb_define_method(klass, "disposed?", RMF(klass_is_disposed<C>), 0);
}
