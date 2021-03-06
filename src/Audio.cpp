#include "AudioImpl.hpp"
#include "OggFile.hpp"
#include "Audio.hpp"
#include "Math.hpp"
#include "IO.hpp"
#include "Platform.hpp"
#include "Utility.hpp"
#include <cassert>
#include <cstdlib>
#include <algorithm>

#ifdef ROOLE_IS_MAC
#import <Foundation/Foundation.h>
#include "AudioToolboxFile.hpp"
#else
#include "MPEGFile.hpp"
#include "SndFile.hpp"
#endif

#ifdef ROOLE_IS_IPHONE
#import <AVFoundation/AVFoundation.h>
#endif

using namespace std;

static bool is_ogg_file(Roole::Reader reader)
{
  if (reader.resource().size() < 4) return false;
  char bytes[4];
  reader.read(bytes, 4);
  return bytes[0] == 'O' && bytes[1] == 'g' && bytes[2] == 'g' && bytes[3] == 'S';
}

static bool is_ogg_file(const char* filename)
{
  Roole::File file(filename);
  return is_ogg_file(file.front_reader());
}

static Roole::Song* cur_song = nullptr;
static bool cur_song_looping;

struct Roole::Sample::SampleData
{
  ALuint buffer;
  double this_volume, this_speed, this_pan;

  SampleData(AudioFile&& audio_file)
  {
    al_initialize();
    alGenBuffers(1, &buffer);
    alBufferData(buffer, audio_file.format(), &audio_file.decoded_data().front(),
                 static_cast<ALsizei>(audio_file.decoded_data().size()),
                 audio_file.sample_rate());
  }
  
  ~SampleData()
  { // It's hard to free things in the right order in Ruby/Roole.
    // Make sure buffer isn't deleted after the context/device are shut down.
    if (!al_initialized()) return;
    alDeleteBuffers(1, &buffer);
  }
  
  double volume() const
  {
    return this_volume;
  }
  
  void set_volume(double volume)
  {
    this_volume = clamp(volume, 0.0, 1.2);
  }

  double speed() const
  {
    return this_speed;
  }

  void set_speed(double speed)
  {
    this_speed = speed;
  }
  
  double pan() const
  {
    return this_pan;
  }
  
  void set_pan(double pan)
  {
    this_pan = pan;
  }
};

Roole::Sample::Sample()
{}

Roole::Sample::Sample(const char* filename)
{
  if (is_ogg_file(filename)) {
    File file(filename);
    data.reset(new SampleData(OggFile(file.front_reader())));
    return;
  }
#ifdef ROOLE_IS_MAC
  File file(filename);
  data.reset(new SampleData(AudioToolboxFile(file.front_reader())));
#else
  try {
    data.reset(new SampleData(SndFile(filename)));
  } catch (const runtime_error& ex) {
    File file(filename);
    data.reset(new SampleData(MPEGFile(file.front_reader())));
  }
#endif
}

Roole::Sample::Sample(Roole::Reader reader)
{
  if (is_ogg_file(reader)) {
    data.reset(new SampleData(OggFile(reader)));
    return;
  }
#ifdef ROOLE_IS_MAC
  data.reset(new SampleData(AudioToolboxFile(reader)));
#else
  try {
    data.reset(new SampleData(SndFile(reader)));
  } catch (const runtime_error& ex) {
    data.reset(new SampleData(MPEGFile(reader)));
  }
#endif
}

Roole::Channel Roole::Sample::play(bool looping) const
{
  return play_pan(looping);
}

Roole::Channel Roole::Sample::play_pan(bool looping) const
{
  if (!data) return Channel();
  Channel channel = allocate_channel();
  // Couldn't allocate a free channel.
  if (channel.current_channel() == NO_CHANNEL) return channel;
  ALuint source = al_source_for_channel(channel.current_channel());
  alSourcei(source, AL_BUFFER, data->buffer);
  alSource3f(source, AL_POSITION, data->pan() * 10, 0, 0);
  alSourcef(source, AL_GAIN, max(data->volume(), 0.0));
  alSourcef(source, AL_PITCH, data->speed());
  alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
  alSourcePlay(source);
  return channel;
}

double Roole::Sample::volume() const
{
  return data->volume();
}

void Roole::Sample::set_volume(double volume)
{
  data->set_volume(volume / 100.0);
}

void Roole::Sample::set_speed(double speed)
{
  data->set_speed(speed / 100.0);
}

void Roole::Sample::set_pan(double pan)
{
  data->set_pan(pan / 100.0);
}

class Roole::Song::BaseData
{
  BaseData(const BaseData&) = delete;
  BaseData& operator=(const BaseData&) = delete;
  double this_volume;

protected:
  BaseData() : this_volume(1) {}
  virtual void apply_volume() = 0;

public:
  virtual ~BaseData() {}
  virtual void play(bool looping) = 0;
  virtual void pause() = 0;
  virtual void resume() = 0;
  virtual bool paused() const = 0;
  virtual void stop() = 0;
  virtual void update() = 0;
  
  double volume() const
  {
    return this_volume;
  }
  
  void set_volume(double volume)
  {
    this_volume = clamp(volume, 0.0, 1.2);
    apply_volume();
  }
};

// AudioFile impl
class Roole::Song::StreamData : public BaseData
{
  unique_ptr<AudioFile> file;
  ALuint buffers[2];
  
  void apply_volume() override
  {
    alSourcef(al_source_for_songs(), AL_GAIN, max(volume(), 0.0));
  }
  
  bool stream_to_buffer(ALuint buffer)
  {
    #ifdef ROOLE_IS_IPHONE
    static const unsigned BUFFER_SIZE = 4096 * 4;
    #else
    static const unsigned BUFFER_SIZE = 4096 * 8;
    #endif
    char audio_data[BUFFER_SIZE];
    size_t read_bytes = file->read_data(audio_data, BUFFER_SIZE);
    if (read_bytes > 0) {
      alBufferData(buffer, file->format(), audio_data,
                   static_cast<ALsizei>(read_bytes), file->sample_rate());
    }
    return read_bytes > 0;
  }
  
public:
  StreamData(const char* filename)
  {
    if (is_ogg_file(filename)) {
      File source_file(filename);
      file.reset(new OggFile(source_file.front_reader()));
    } else {
    #ifdef ROOLE_IS_MAC
      file.reset(new AudioToolboxFile(filename));
    #else
      try {
        file.reset(new SndFile(filename));
      } catch (const runtime_error& ex) {
        File source_file(filename);
        file.reset(new MPEGFile(source_file.front_reader()));
      }
    #endif
    }
    al_initialize();
    alGenBuffers(2, buffers);
  }

  StreamData(Reader reader)
  {
    if (is_ogg_file(reader)) {
      file.reset(new OggFile(reader));
    } else {
    #ifdef ROOLE_IS_MAC
      file.reset(new AudioToolboxFile(reader));
    #else
      try {
        file.reset(new SndFile(reader));
      } catch (const runtime_error& ex) {
        file.reset(new MPEGFile(reader));
      }
    #endif
    }
    al_initialize();
    alGenBuffers(2, buffers);
  }
  
  ~StreamData()
  { // It's hard to free things in the right order in Ruby/Roole.
    // Make sure buffers aren't deleted after the context/device are shut down.
    if (!al_initialized()) return;
    alDeleteBuffers(2, buffers);
  }
  
  void play(bool looping) override
  {
    ALuint source = al_source_for_songs();
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSourcef(source, AL_GAIN, max(volume(), 0.0));
    alSourcef(source, AL_PITCH, 1);
    alSourcei(source, AL_LOOPING, AL_FALSE); // need to implement this manually...
    stream_to_buffer(buffers[0]);
    stream_to_buffer(buffers[1]);
    // TODO: Not good for songs with less than two buffers full of data.
    alSourceQueueBuffers(source, 2, buffers);
    alSourcePlay(source);
  }

  void stop() override
  {
    ALuint source = al_source_for_songs();
    alSourceStop(source);
    // Unqueue all buffers for this source.
    // The number of QUEUED buffers apparently includes the number of PROCESSED ones,
    // so getting rid of the QUEUED ones is enough.
    ALuint buffer;
    int queued;
    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
    while (queued--) {
      alSourceUnqueueBuffers(source, 1, &buffer);
    }
    file->rewind();
  }
  
  void pause() override
  {
    alSourcePause(al_source_for_songs());
  }
  
  void resume() override
  {
    alSourcePlay(al_source_for_songs());
  }
  
  bool paused() const override
  {
    ALint state;
    alGetSourcei(al_source_for_songs(), AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
  }
  
  void update() override
  {
    ALuint source = al_source_for_songs();
    ALuint buffer;
    int processed;
    bool active = true;
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
    for (int i = 0; i < processed; ++i) {
      alSourceUnqueueBuffers(source, 1, &buffer);
      active = stream_to_buffer(buffer);
      if (active) alSourceQueueBuffers(source, 1, &buffer);
    }
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (active && state != AL_PLAYING && state != AL_PAUSED) {
      // We seemingly got starved.
      alSourcePlay(source);
    } else if (!active) {
      // We got starved and there is nothing left to play.
      stop();
      if (cur_song_looping) {
        // Start anew.
        play(true);
      } else {
        // Let the world know we're finished.
        cur_song = nullptr;
      }
    }
  }
};

Roole::Song::Song(const char* filename)
{
#ifdef ROOLE_IS_IPHONE
  if (has_extension(filename, ".mp3") ||
      has_extension(filename, ".aac") ||
      has_extension(filename, ".m4a")) {
    data.reset(new ModuleData(filename));
    return;
  }
#endif
  data.reset(new StreamData(filename));
}

Roole::Song::Song(Reader reader)
{
  data.reset(new StreamData(reader));
}

Roole::Song::~Song()
{
  stop();
}

Roole::Song* Roole::Song::current_song()
{
  return cur_song;
}

void Roole::Song::play(bool looping)
{
  if (paused()) data->resume();
  if (cur_song && cur_song != this) {
    cur_song->stop();
    assert(cur_song == nullptr);
  }
  if (cur_song == nullptr) data->play(looping);
  cur_song = this;
  cur_song_looping = looping;
}

void Roole::Song::resume()
{
  if (cur_song == this && paused()) data->resume();
}

void Roole::Song::pause()
{
  if (cur_song == this) data->pause();
}

bool Roole::Song::paused() const
{
  return cur_song == this && data->paused();
}

void Roole::Song::stop()
{
  if (cur_song != this) return;
  data->stop();
  cur_song = nullptr;
}

bool Roole::Song::playing() const
{
  return cur_song == this && !data->paused();
}

double Roole::Song::volume() const
{
  return data->volume();
}

void Roole::Song::set_volume(double volume)
{
  data->set_volume(volume / 100.0);
}

void Roole::Song::update()
{
  if (current_song())
    current_song()->data->update();
}

#ifdef ROOLE_IS_IPHONE
// AVAudioPlayer impl
class Roole::Song::ModuleData : public BaseData
{
  AVAudioPlayer* player;
  
  void apply_volume() override
  {
    player.volume = volume();
  }
    
public:
  ModuleData(const string& filename)
  {
    NSURL* URL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:filename.c_str()]];
    player = [[AVAudioPlayer alloc] initWithContentsOfURL:URL error:nil];
  }
    
  void play(bool looping) override
  {
    if (paused()) stop();
    player.numberOfLoops = looping ? -1 : 0;
    [player play];
  }
    
  void pause() override
  {
    [player pause];
  }
  
  void resume() override
  {
    [player play];
  }
  
  bool paused() const override
  {
    return !player.playing;
  }
  
  void stop() override
  {
    [player stop];
    player.currentTime = 0;
  }
  
  void update() override
  {}
};
#endif