#include "extras.hpp"

void module_accessors(VALUE module, int size, const char **names);

static VALUE Load, Sprite, Song, Sample;

VALUE song_new(VALUE name, VALUE loop);

VALUE set_hash_sprite(VALUE hash, VALUE name)
{
  VALUE image = rb_hash_aref(hash, name);
  if (image != Qnil)
    return rb_funcall(image, rb_intern("dup"), 0);
  image = rb_funcall(Sprite, rb_intern("new"), 1, name);
  rb_hash_aset(hash, name, image);
  return rb_funcall(image, rb_intern("dup"), 0);
}

static VALUE load_backdrop(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@backdrops");
  name = rb_str_plus(rb_iv_get(self, "@backdrops_dir"), name);
  return set_hash_sprite(hash, name);
}

static VALUE load_picture(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@pictures");
  name = rb_str_plus(rb_iv_get(self, "@pictures_dir"), name);
  return set_hash_sprite(hash, name);
}

static VALUE load_character(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@characters");
  name = rb_str_plus(rb_iv_get(self, "@characters_dir"), name);
  return set_hash_sprite(hash, name);
}

static VALUE load_battler(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@battlers");
  name = rb_str_plus(rb_iv_get(self, "@battlers_dir"), name);
  return set_hash_sprite(hash, name);
}

static VALUE load_face(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@faces");
  name = rb_str_plus(rb_iv_get(self, "@faces_dir"), name);
  return set_hash_sprite(hash, name);
}

static VALUE load_cursor(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@cursors");
  name = rb_str_plus(rb_iv_get(self, "@cursors_dir"), name);
  return set_hash_sprite(hash, name);
}

static VALUE load_icon(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@icons");
  name = rb_str_plus(rb_iv_get(self, "@icons_dir"), name);
  return set_hash_sprite(hash, name);
}

static VALUE load_window(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@windows");
  name = rb_str_plus(rb_iv_get(self, "@windows_dir"), name);
  return set_hash_sprite(hash, name);
}

VALUE set_hash_audio(VALUE hash, VALUE name, VALUE loop)
{
  VALUE audio = rb_hash_aref(hash, name);
  if (audio != Qnil) return audio;
  if (loop == Qnil)
    audio = rb_funcall(Sample, rb_intern("new"), 1, name);
  else
    audio = song_new(name, loop);
  rb_hash_aset(hash, name, audio);
  return audio;
}

static VALUE load_bgm(int argc, VALUE *argv, VALUE self)
{
  if (argc == 0 || argc > 2)
    rb_raise(rb_eArgError,
      "Wrong number of arguments for Load.bgm (given %d, expected 1..2)", argc);
  VALUE hash = rb_iv_get(self, "@bgm");
  VALUE name = argv[0];
  name = rb_str_plus(rb_iv_get(self, "@bgm_dir"), name);
  VALUE loop = (argc == 2 && argv[1] == Qtrue)? Qtrue : Qfalse;
  return set_hash_audio(hash, name, loop);
}

static VALUE load_se(VALUE self, VALUE name)
{
  VALUE hash = rb_iv_get(self, "@se");
  name = rb_str_plus(rb_iv_get(self, "@se_dir"), name);
  return set_hash_audio(hash, name, Qnil);
}

static VALUE load_clear_all(VALUE self)
{
  rb_iv_set(self, "@backdrops", rb_hash_new());
  rb_iv_set(self, "@pictures", rb_hash_new());
  rb_iv_set(self, "@characters", rb_hash_new());
  rb_iv_set(self, "@battlers", rb_hash_new());
  rb_iv_set(self, "@faces", rb_hash_new());
  rb_iv_set(self, "@cursors", rb_hash_new());
  rb_iv_set(self, "@icons", rb_hash_new());
  rb_iv_set(self, "@windows", rb_hash_new());
  rb_iv_set(self, "@bgm", rb_hash_new());
  rb_iv_set(self, "@se", rb_hash_new());
  return Qtrue;
}

void load_clear_all_sprites()
{
  load_clear_all(Load);
}

void init_load()
{
  Load = rb_define_module("Load");
  VALUE Audio = rb_define_module("Audio");
  Song = rb_define_class_under(Audio, "Song", rb_cObject);
  Sample = rb_define_class_under(Audio, "Sample", rb_cObject);
  Sprite = rb_define_class("Sprite", rb_cObject);
  load_clear_all(Load);
  rb_iv_set(Load, "@backdrops_dir", rstr("images/backdrops/"));
  rb_iv_set(Load, "@pictures_dir", rstr("images/pictures/"));
  rb_iv_set(Load, "@characters_dir", rstr("images/characters/"));
  rb_iv_set(Load, "@battlers_dir", rstr("images/battlers/"));
  rb_iv_set(Load, "@faces_dir", rstr("images/faces/"));
  rb_iv_set(Load, "@cursors_dir", rstr("images/cursors/"));
  rb_iv_set(Load, "@icons_dir", rstr("images/icons/"));
  rb_iv_set(Load, "@windows_dir", rstr("images/windows/"));
  rb_iv_set(Load, "@bgm_dir", rstr("audio/bgm/"));
  rb_iv_set(Load, "@se_dir", rstr("audio/se/"));
  const char *dirs[10] = {
    "backdrops_dir", "pictures_dir", "characters_dir",
    "battlers_dir", "faces_dir", "cursors_dir",
    "icons_dir", "windows_dir", "bgm_dir", "se_dir"
  };
  module_accessors(Load, 10, dirs);
  rb_define_module_function(Load, "clear_all", RMF(load_clear_all), 0);
  rb_define_module_function(Load, "backdrop", RMF(load_backdrop), 1);
  rb_define_module_function(Load, "title", RMF(load_backdrop), 1);
  rb_define_module_function(Load, "picture", RMF(load_picture), 1);
  rb_define_module_function(Load, "character", RMF(load_character), 1);
  rb_define_module_function(Load, "battler", RMF(load_battler), 1);
  rb_define_module_function(Load, "face", RMF(load_face), 1);
  rb_define_module_function(Load, "cursor", RMF(load_cursor), 1);
  rb_define_module_function(Load, "icon", RMF(load_icon), 1);
  rb_define_module_function(Load, "window", RMF(load_window), 1);
  rb_define_module_function(Load, "bgm", RMF(load_bgm), -1);
  rb_define_module_function(Load, "se", RMF(load_se), 1);
}
