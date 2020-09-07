#include "extras.hpp"
#include "Audio.hpp"

static VALUE Audio, Song, zero, hundred;

template<class C>
static void free_instance(void *inst)
{
  C *c = static_cast<C*>(inst);
  if (c != 0) delete c;
}

rb_data_type_t SampleType = { "Sample",
  { 0, free_instance<Roole::Sample>, 0, { 0, 0 } }, 0, 0, 0 };

rb_data_type_t SongType = { "Song",
  { 0, free_instance<Roole::Song>, 0, { 0, 0 } }, 0, 0, 0 };

static VALUE sample_init(VALUE self, VALUE name)
{
  rb_iv_set(self, "@filename", name);
  const char *fn = RSTRING_PTR(name);
  Roole::Sample *s = new Roole::Sample(fn);
  RTYPEDDATA(self)->data = s;
  VALUE tmp = rb_iv_get(Audio, "@se_volume");
  s->set_volume(RB_FIX2INT(tmp));
  rb_iv_set(self, "@speed", hundred);
  rb_iv_set(self, "@pan", zero);
  s->set_speed(RB_FIX2INT(hundred));
  s->set_pan(RB_FIX2INT(zero));
  return self;
}

static VALUE sample_play(VALUE self)
{
  Roole::Sample *s = (Roole::Sample*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  s->play();
  return Qtrue;
}

static VALUE sample_speed(VALUE self)
{
  return rb_iv_get(self, "@speed");
}

static VALUE sample_volume_set(VALUE self, VALUE volume)
{
  Roole::Sample *s = (Roole::Sample*)RTYPEDDATA(self)->data;
  if (!s) return zero;
  s->set_volume(RB_FIX2INT(volume));
  return rb_iv_set(self, "@volume", volume);
}

static VALUE sample_speed_set(VALUE self, VALUE speed)
{
  Roole::Sample *s = (Roole::Sample*)RTYPEDDATA(self)->data;
  if (!s) return zero;
  s->set_speed(RB_FIX2INT(speed));
  return rb_iv_set(self, "@speed", speed);
}

static VALUE sample_pan_set(VALUE self, VALUE pan)
{
  Roole::Sample *s = (Roole::Sample*)RTYPEDDATA(self)->data;
  if (!s) return zero;
  s->set_pan(RB_FIX2INT(pan));
  return rb_iv_set(self, "@pan", pan);
}

static VALUE song_init(int argc, VALUE *argv, VALUE self)
{
  if (argc == 0 || argc > 2)
    rb_raise(rb_eArgError,
      "Wrong number of arguments for Audio::Song.new (given %d, expected 1..2)", argc);
  rb_iv_set(self, "@filename", argv[0]);
  VALUE loop = (argc == 2 && argv[1] == Qtrue)? Qtrue : Qfalse;
  rb_iv_set(self, "@loop", loop);
  const char *fn = RSTRING_PTR(argv[0]);
  Roole::Song *s = new Roole::Song(fn);
  RTYPEDDATA(self)->data = s;
  VALUE volume = rb_iv_get(Audio, "@bgm_volume");
  rb_iv_set(self, "@volume", volume);
  s->set_volume(RB_FIX2INT(volume));
  return self;
}

static VALUE song_play(VALUE self)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  rb_iv_set(Audio, "@song", self);
  s->play(rb_iv_get(self, "@loop") == Qtrue);
  return Qtrue;
}

static VALUE song_is_playing(VALUE self)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  return s->playing() ? Qtrue : Qfalse;
}

static VALUE song_resume(VALUE self)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  s->resume();
  return Qtrue;
}

static VALUE song_pause(VALUE self)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  s->pause();
  return Qtrue;
}

static VALUE song_is_paused(int argc, VALUE *argv, VALUE self)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  return s->paused() ? Qtrue : Qfalse;
}

static VALUE song_stop(VALUE self)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  s->stop();
  return Qtrue;
}

static VALUE song_has_stopped(VALUE self)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return Qnil;
  return !s->playing() ? Qtrue : Qfalse;
}

static VALUE song_volume_set(VALUE self, VALUE volume)
{
  Roole::Song *s = (Roole::Song*)RTYPEDDATA(self)->data;
  if (!s) return zero;
  s->set_volume(NUM2DBL(volume));
  volume = DBL2NUM(s->volume());
  return rb_iv_set(self, "@volume", volume);
}

template<rb_data_type_t *RubyType>
static VALUE audio_allocate(VALUE klass)
{
  return rb_data_typed_object_wrap(klass, 0, RubyType);
}

static VALUE audio_current_song(VALUE self)
{
  VALUE song = rb_iv_get(self, "@song");
  if (song != Qnil && song_has_stopped(song))
    song = rb_iv_set(self, "@song", Qnil);
  return song;
}

static VALUE audio_on(VALUE self)
{
  VALUE song = audio_current_song(self);
  if (song != Qnil) song_resume(song);
  return rb_iv_set(self, "@state", Qtrue);;
}

static VALUE audio_off(VALUE self)
{
  VALUE song = audio_current_song(self);
  if (song != Qnil) song_pause(song);
  return rb_iv_set(self, "@state", Qfalse);
}

static VALUE audio_is_on(VALUE self)
{
  return rb_iv_get(self, "@state") == Qtrue;
}

static VALUE audio_is_off(VALUE self)
{
  return rb_iv_get(self, "@state") == Qfalse;
}

static VALUE audio_state(VALUE self)
{
  return rb_iv_get(self, "@state");
}

static VALUE audio_bgm_volume(VALUE self)
{
  return rb_iv_get(self, "@bgm_volume");
}

static VALUE audio_bgm_volume_set(VALUE self, VALUE volume)
{
  VALUE song = audio_current_song(self);
  if (song != Qnil) song_volume_set(song, volume);
  return rb_iv_set(self, "@bgm_volume", volume);
}

static VALUE audio_se_volume(VALUE self)
{
  return rb_iv_get(self, "@se_volume");
}

static VALUE audio_se_volume_set(VALUE self, VALUE volume)
{
  return rb_iv_set(self, "@se_volume", volume);
}

VALUE song_new(VALUE name, VALUE loop)
{
  VALUE song = rb_data_typed_object_wrap(Song, 0, &SongType);
  VALUE args[2] = { name, loop };
  song_init(2, args, song);
  return song;
}

void init_audio()
{
  zero = RB_INT2FIX(0);
  hundred = RB_INT2FIX(100);
  Audio = rb_define_module("Audio");
  rb_iv_set(Audio, "@state", Qtrue);
  rb_iv_set(Audio, "@song", Qnil);
  rb_iv_set(Audio, "@bgm_volume", RB_INT2FIX(50));
  rb_iv_set(Audio, "@se_volume", RB_INT2FIX(50));
  VALUE Sample = rb_define_class_under(Audio, "Sample", rb_cObject);
  Song = rb_define_class_under(Audio, "Song", rb_cObject);
  rb_define_module_function(Audio, "current_song", RMF(audio_current_song), 0);
  rb_define_module_function(Audio, "on!", RMF(audio_on), 0);
  rb_define_module_function(Audio, "off!", RMF(audio_off), 0);
  rb_define_module_function(Audio, "on?", RMF(audio_is_on), 0);
  rb_define_module_function(Audio, "off?", RMF(audio_is_off), 0);
  rb_define_module_function(Audio, "state", RMF(audio_state), 0);
  rb_define_module_function(Audio, "bgm_volume", RMF(audio_bgm_volume), 0);
  rb_define_module_function(Audio, "se_volume", RMF(audio_se_volume), 0);
  rb_define_module_function(Audio, "bgm_volume=", RMF(audio_bgm_volume_set), 1);
  rb_define_module_function(Audio, "se_volume=", RMF(audio_se_volume_set), 1);
  rb_define_alloc_func(Sample, audio_allocate<&SampleType>);
  rb_define_alloc_func(Song, audio_allocate<&SongType>);
  rb_define_attr(Sample, "filename", 1, 1);
  rb_define_attr(Sample, "volume", 1, 0);
  rb_define_attr(Sample, "pan", 1, 0);
  rb_define_method(Sample, "initialize", RMF(sample_init), 1);
  rb_define_method(Sample, "play", RMF(sample_play), 0);
  rb_define_method(Sample, "speed", RMF(sample_speed), 0);
  rb_define_method(Sample, "pitch", RMF(sample_speed), 0);
  rb_define_method(Sample, "volume=", RMF(sample_volume_set), 1);
  rb_define_method(Sample, "speed=", RMF(sample_speed_set), 1);
  rb_define_method(Sample, "pitch=", RMF(sample_speed_set), 1);
  rb_define_method(Sample, "pan=", RMF(sample_pan_set), 1);
  rb_define_attr(Song, "filename", 1, 1);
  rb_define_attr(Song, "volume", 1, 0);
  rb_define_attr(Song, "loop", 1, 1);
  rb_define_method(Song, "initialize", RMF(song_init), -1);
  rb_define_method(Song, "play", RMF(song_play), 0);
  rb_define_method(Song, "playing?", RMF(song_is_playing), 0);
  rb_define_method(Song, "resume", RMF(song_resume), 0);
  rb_define_method(Song, "pause", RMF(song_pause), 0);
  rb_define_method(Song, "paused?", RMF(song_is_paused), 0);
  rb_define_method(Song, "stop", RMF(song_stop), 0);
  rb_define_method(Song, "stop?", RMF(song_has_stopped), 0);
  rb_define_method(Song, "volume=", RMF(song_volume_set), 0);
}