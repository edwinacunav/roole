#include "Math.hpp"
#include <cstdlib>
using namespace std;

double Roole::random(double min, double max)
{
  double rnd = rand();
  return rnd / (static_cast<double>(RAND_MAX) + 1) * (max - min) + min;
}

double Roole::offset_x(double angle, double radius)
{
  return +sin(angle / 180 * M_PI) * radius;
}

double Roole::offset_y(double angle, double radius)
{
  return -cos(angle / 180 * M_PI) * radius;
}

double Roole::angle(double from_x, double from_y, double to_x, double to_y, double def)
{
  double dist_x = to_x - from_x;
  double dist_y = to_y - from_y;
  if (dist_x == 0 && dist_y == 0) return def;
  return normalize_angle(radians_to_gosu(atan2(dist_y, dist_x)));
}

double Roole::angle_diff(double from, double to)
{
  return normalize_angle(to - from + 180) - 180;
}

double Roole::normalize_angle(double angle)
{
  return wrap(angle, 0.0, 360.0);
}

int Roole::wrap(int value, int min, int max)
{
  int result = (value - min) % (max - min);
  return result < 0 ? result + max : result + min;
}

float Roole::wrap(float value, float min, float max)
{
  float result = fmodf(value - min, max - min);
  return result < 0 ? result + max : result + min;
}

double Roole::wrap(double value, double min, double max)
{
  double result = fmod(value - min, max - min);
  return result < 0 ? result + max : result + min;
}

double Roole::distance(double x1, double y1, double x2, double y2)
{
  return sqrt(distance_sqr(x1, y1, x2, y2));
}
