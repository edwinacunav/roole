#include <SDL.h>
#include "extras.hpp"
#include <ruby/io.h>
#include <ruby/encoding.h>
#include "Scene.hpp"
#include "xxd/icon.xxd"
#include <exception>
//extern const char script_game[];
void init_module();
void init_roole();
void init_load();
void init_color();
void init_input();
void init_sprite();
void init_image();
void init_font();
void init_audio();
void init_scene();
void init_window();
void load_clear_all_sprites();

static VALUE scene, window, result;

static VALUE get_scene()
{
  return rb_iv_get(scene, "@scene");
}

static VALUE main_scene_init(int *state)
{
  rb_funcall(scene, rb_intern("init"), 0);
  if (get_scene() == Qnil || Scene::is_zombie()) {
    printf("No Scene was ever found!\n");
    rb_funcall(window, rb_intern("close"), 0);
    *state = 1;
    ruby_cleanup(0);
    return Qnil;
  }
  return Qtrue;
}

void main_scene_rescue(int *state)
{
  VALUE exc = rb_gv_get("$!");
  VALUE klass = rb_obj_as_string(rb_obj_class(exc));
  if (exc == Qnil) return;
  VALUE msg = rb_funcall(exc, rb_intern("message"), 0);
  VALUE bct = rb_funcall(exc, rb_intern("backtrace"), 0);
  printf("%s!\n%s\n", RSTRING_PTR(klass), RSTRING_PTR(msg));
  int len = RARRAY_LEN(bct);
  if (len > 0) {
    printf("Backtrace:\n");
    for (int n = 0; n < len; n++)
      printf("%s\n", RSTRING_PTR(rb_ary_entry(bct, n)));
    *state = 3;
  } else {
    printf("No backtrace was found!\n");
    *state = 4;
  }
  printf("Game is closing now.\n");
}

void main_loop(int *state)
{
  if (get_scene() == Qnil) {
    printf("No scene was found!\nGame is closing now.\n");
    Scene::set_zombie();
    *state = 2;
    return;
  }
  printf("Entering the Main Loop Now...\n");
  while (get_scene() != Qnil) {
    rb_funcall(window, rb_intern("show"), 0);
    if (Scene::hit_close_button())
      rb_funcall(scene, rb_intern("close"), 0);
    if (Scene::is_zombie())
      rb_funcall(window, rb_intern("close"), 0);
    if (Scene::has_crashed())
      main_scene_rescue(state);
    if (*state > 0 || get_scene() == Qnil) break;
  }
}

void print_help_ful_info()
{
  printf("\nMain Purpose:\n");
  printf("===========================\n");
  printf("This engine is focused on providing you with a simple binary\n");
  printf("executable capable of running on any Debian based Linux OS\n");
  printf("without forcing the player to previously install Ruby on it.\n");
  printf("Even so you can compile it on any other Linux OS thanks to tools\n");
  printf("like CMake that let me build it on Debian.\n\n");
  printf("Instructions:\n");
  printf("===========================\n");
  printf("This engine requires you to create a load.txt file where you\n");
  printf("can enter as many Ruby script filenames as needed.\n");
  printf("Examples:\n");
  printf("main\n");
  printf("lib/scene\n");
  printf("The file extension is optional.\n");
  printf("Leave a blank line at the end of the file!\n\n");
  printf("List of Available Commands:\n");
  printf("===========================\n");
  printf("No Command   : Run the game!\n");
  printf("-h --help    : Show this help-ful information\n");
  printf("-v --version : Show the current version of this engine\n\n");
  printf("Nope, this is not an engine with some sort of super powers!\n\n");
  printf("You may praise the Lord for His everlasting Grace!\n");
}

static VALUE file_readlines(VALUE fn)
{
  return rb_funcall(rb_cFile, rb_intern("readlines"), 1, fn);
}

static VALUE ruby_require(VALUE fn)
{
  return rb_funcall(rb_mKernel, rb_intern("require"), 1, fn);
}

void load_scripts(int *state)
{
  VALUE filename, filenames, local = rstr("./"), file = rstr("load.txt");
  filenames = rb_protect(RPF(file_readlines), file, state);
  if (*state) return;
  long total = RARRAY_LEN(filenames);
  for (int n = 0; n < total; n++) {
    filename = rb_ary_entry(filenames, n);
    rb_funcall(filename, rb_intern("chomp!"), 0);
    filename = rb_str_plus(local, filename);
    rb_protect(RPF(ruby_require), filename, state);
    if (*state) break;
  }
}

void call_scene_init(int *state) {
  printf("Loading Scene\n");
  scene = rb_const_get(rb_cObject, rb_intern("Scene"));
  VALUE roole = rb_define_module("Roole");
  window = rb_const_get(roole, rb_intern("Window"));
  window = rb_funcall(window, rb_intern("new"), 0);
  result = rb_protect(RPF(main_scene_init), Qnil, state);
}  

int main(int argc, char *argv[])
{
  int rargc = 0;
  char **rargv = 0;
  ruby_sysinit(&rargc, &rargv);
  ruby_setup();
  rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));
  VALUE rversion = rb_const_get(rb_cObject, rb_intern("RUBY_VERSION"));
  const char* version = RSTRING_PTR(rversion);
  if (argc == 2) {
    printf("Roole Engine by Kyonides Arkanthes\n");
    printf("Based on the Gosu for Ruby Engine by Julian Raschke et al.\n");
    if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
      printf("Roole version 0.11.0\n");
      printf("Ruby version %s\n", version);
    } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
      print_help_ful_info();
    }
    ruby_cleanup(0);
    return 0;
  }
  printf("Loading Roole Scripts...\n");
  init_module();
  init_roole();
  init_color();
  init_sprite();
  init_load();
  init_image();
  init_font();
  init_audio();
  init_input();
  init_scene();
  init_window();
  printf("Loading Ruby Scripts...\n");
  int state;
  try {
    load_scripts(&state);
  } catch(std::exception &e) {
    printf("An error was found, reason: %s\n", e.what());
  }
  if (!state)
    call_scene_init(&state);
  if (!state)
    main_loop(&state);
  else
    main_scene_rescue(&state);
  load_clear_all_sprites();
  if (window != 0) rb_funcall(window, rb_intern("close"), 0);
  ruby_cleanup(0);
  return state;
}
