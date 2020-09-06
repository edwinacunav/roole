#include "Audio.hpp"
#include "AudioImpl.hpp"
using namespace std;

// Returns the current state of a source
static ALint state(int& channel)
{
  ALint state;
  alGetSourcei(Roole::al_source_for_channel(channel), AL_SOURCE_STATE, &state);
  if (state != AL_PLAYING && state != AL_PAUSED) {
    channel = Roole::NO_CHANNEL;
  }
  return state;
}

Roole::Channel::Channel()
: channel(NO_CHANNEL), token(0)
{}

Roole::Channel::Channel(int channel, int token)
: channel(channel), token(token)
{}

int Roole::Channel::current_channel() const
{
  if (channel != NO_CHANNEL && channel_expired(channel, token)) {
    channel = NO_CHANNEL;
  }
  return channel;
}

bool Roole::Channel::playing() const
{
  if (current_channel() == NO_CHANNEL) return false;
  return state(channel) == AL_PLAYING;
}

bool Roole::Channel::paused() const
{
  if (current_channel() == NO_CHANNEL) return false;
  return state(channel) == AL_PAUSED;
}

void Roole::Channel::pause()
{
  ALuint source = al_source_for_channel(channel);
  if (playing())
    alSourcePause(source);
  else if (paused())
    alSourcePlay(source);
}

void Roole::Channel::resume()
{
  if (paused()) {
    ALuint source = al_source_for_channel(channel);
    alSourcePlay(source);
  }
}

void Roole::Channel::stop()
{
  if (current_channel() == NO_CHANNEL) return;
  ALuint source = al_source_for_channel(channel);
  alSourceStop(source);
  channel = NO_CHANNEL;
}

void Roole::Channel::set_volume(double volume)
{
  if (current_channel() == NO_CHANNEL) return;
  ALuint source = al_source_for_channel(channel);
  alSourcef(source, AL_GAIN, max(volume, 0.0));
}

void Roole::Channel::set_pan(double pan)
{
  if (current_channel() == NO_CHANNEL) return;
  ALuint source = al_source_for_channel(channel);
  alSource3f(source, AL_POSITION, pan * 10, 0, 0);
}

void Roole::Channel::set_speed(double speed)
{
  if (current_channel() == NO_CHANNEL) return;
  ALuint source = al_source_for_channel(channel);
  alSourcef(source, AL_PITCH, speed);
}
