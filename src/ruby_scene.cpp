#include "extras.hpp"
#include "Scene.hpp"

static VALUE scene_frames(VALUE self)
{
  return rb_iv_get(self, "@transition_frames");
}

static VALUE scene_frames_set(VALUE self, VALUE frames)
{
  return rb_iv_set(self, "@transition_frames", frames);
}

static VALUE scene_scenes(VALUE self)
{
  return rb_cv_get(self, "@@scenes");
}

static VALUE scene_scene_set(VALUE self, VALUE scene)
{
  rb_iv_set(self, "@close_timer", scene_frames(self));
  VALUE scenes = rb_cv_get(self, "@@scenes");
  rb_ary_push(scenes, scene);
  rb_iv_set(self, "@scene", scene);
  return scene;
}

static VALUE scene_call(VALUE self, VALUE scene)
{
  rb_iv_set(self, "@close_timer", scene_frames(self));
  rb_cv_set(self, "@@scenes", rb_ary_new3(1, scene));
  rb_iv_set(self, "@scene", scene);
  return Qnil;
}

static VALUE scene_return(VALUE self)
{
  rb_iv_set(self, "@close_timer", scene_frames(self));
  VALUE scenes = scene_scenes(self);
  rb_ary_pop(scenes);
  rb_iv_set(self, "@scene", rb_ary_entry(scenes, -1));
  return rb_iv_get(self, "@scene");
}

static VALUE scene_close(VALUE self)
{
  Scene::closing();
  rb_ary_pop(scene_scenes(self));
  rb_iv_set(self, "@scene", Qnil);
  return Qnil;
}

static VALUE this_scene(VALUE self)
{
  return rb_iv_get(self, "@scene");
}

static VALUE scene_update(VALUE self)
{
  return rb_funcall(this_scene(self), rb_intern("update"), 0);
}

static VALUE scene_draw(VALUE self)
{
  return rb_funcall(this_scene(self), rb_intern("draw"), 0);
}

void init_scene()
{
  VALUE scene = rb_define_module("Scene");
  rb_define_attr(scene, "timer", 1, 1);
  rb_define_attr(scene, "close_timer", 1, 1);
  rb_define_attr(scene, "window", 1, 0);
  rb_define_attr(scene, "width", 1, 0);
  rb_define_attr(scene, "height", 1, 0);
  rb_define_attr(scene, "title", 1, 0);
  rb_define_attr(scene, "scene", 1, 0);
  rb_cv_set(scene, "@@scenes", rb_ary_new());
  rb_iv_set(scene, "@scene", Qnil);
  rb_iv_set(scene, "@timer", RB_INT2FIX(0));
  rb_iv_set(scene, "@close_timer", RB_INT2FIX(0));
  rb_iv_set(scene, "@window", Qnil);
  rb_iv_set(scene, "@transition_frames", RB_INT2FIX(20));
  rb_iv_set(scene, "@title", rstr("Roole Game Window"));
  rb_iv_set(scene, "@width", RB_INT2FIX(640));
  rb_iv_set(scene, "@height", RB_INT2FIX(480));
  rb_iv_set(scene, "@framerate", DBL2NUM(16.6666666666));
  rb_iv_set(scene, "@fullscreen", Qfalse);
  rb_iv_set(scene, "@resize", Qfalse);
  VALUE modfunc = rb_obj_method(scene, rb_sym("module_function"));
  VALUE methods[] = {
    rb_sym("timer"), rb_sym("timer="), rb_sym("window"),
    rb_sym("close_timer"), rb_sym("close_timer="), rb_sym("title"),
    rb_sym("width"), rb_sym("height")
  };
  rb_method_call(8, methods, modfunc);
  rb_define_module_function(scene, "transition_frames", RMF(scene_frames), 0);
  rb_define_module_function(scene, "transition_frames=", RMF(scene_frames_set), 0);
  rb_define_module_function(scene, "scenes", RMF(scene_scenes), 0);
  rb_define_module_function(scene, "scene=", RMF(scene_scene_set), 1);
  rb_define_module_function(scene, "call", RMF(scene_call), 1);
  rb_define_module_function(scene, "return", RMF(scene_return), 0);
  rb_define_module_function(scene, "close", RMF(scene_close), 0);
  rb_define_module_function(scene, "update", RMF(scene_update), 0);
  rb_define_module_function(scene, "draw", RMF(scene_draw), 0);
}
