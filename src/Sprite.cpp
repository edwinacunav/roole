#include "Sprite.hpp"
#include "Bitmap.hpp"
#include "Graphics.hpp"
#include "IO.hpp"
#include "ImageData.hpp"
#include "Math.hpp"
#include <stdexcept>
#include "EmptyImageData.hpp"
using namespace std;

struct Sprite::Data
{
  double x = 0;
  double y = 0;
  Roole::ZPos z = 0;
  double scale_x = 1.0;
  double scale_y = 1.0;
  double angle = 0.0;
  double center_x = 0.5;
  double center_y = 0.5;
  Roole::AlphaMode mode = Roole::AM_DEFAULT;
  bool visible = false;
  Color c = Color(Color::WHITE);
  Color c1 = Color::WHITE;
  Color c2 = Color::WHITE;
  Color c3 = Color::WHITE;
  Color c4 = Color::WHITE;
};

Sprite::Sprite()
: p(new Data), data_(Roole::EmptyImageData::instance_ptr())
{}

Sprite::Sprite(const char* filename, unsigned flags)
: p(new Data)
{
  Roole::Bitmap bmp;
  Roole::load_image_file(bmp, filename);
  Sprite(bmp, flags).data_.swap(data_);
}

Sprite::Sprite(const Roole::Bitmap& source, unsigned flags)
: p(new Data)
{
  Sprite(source, 0, 0, source.width(), source.height(), flags).data_.swap(data_);
}

Sprite::Sprite(const Roole::Bitmap& source, unsigned src_x, unsigned src_y,
               unsigned src_width, unsigned src_height, unsigned flags)
: p(new Data)
{
  data_ = Roole::Graphics::create_image(source, src_x, src_y, src_width, src_height, flags);
}

Sprite::Sprite(unique_ptr<Roole::ImageData>&& data)
: p(new Data), data_(data.release())
{
  if (!data_) throw invalid_argument("Sprite cannot be initialized with nullptr");
}

double Sprite::x_scale() const
{
  return Roole::Graphics::global_scale + p->scale_x;
}

double Sprite::y_scale() const
{
  return Roole::Graphics::global_scale + p->scale_y;
}

double Sprite::x() const
{
  return p->x * x_scale();
}

double Sprite::y() const
{
  return p->y * y_scale();
}

double Sprite::x2() const
{
  return x() + width() * x_scale();
}

double Sprite::y2() const
{
  return y() + height() * y_scale();
}

double Sprite::scale_x() const
{
  return p->scale_x;
}

double Sprite::scale_y() const
{
  return p->scale_y;
}

double Sprite::center_x() const
{
  return p->center_x;
}

double Sprite::center_y() const
{
  return p->center_y;
}

Roole::ZPos Sprite::z() const
{
  return p->z;
}

double Sprite::angle() const
{
  return p->angle;
}

Color& Sprite::color() const
{
  return p->c;
}

Color& Sprite::color1() const
{
  return p->c1;
}

Color& Sprite::color2() const
{
  return p->c2;
}

Color& Sprite::color3() const
{
  return p->c3;
}

Color& Sprite::color4() const
{
  return p->c4;
}

unsigned Sprite::width() const
{
  return data_->width();
}

unsigned Sprite::height() const
{
  return data_->height();
}

bool Sprite::visible() const
{
  return p->visible;
}

void Sprite::set_x(double x)
{
  p->x = x;
}

void Sprite::set_y(double y)
{
  p->y = y;
}

void Sprite::set_scale_x(double x)
{
  p->scale_x = x;
}

void Sprite::set_scale_y(double y)
{
  p->scale_y = y;
}

void Sprite::set_center_x(double x)
{
  p->center_x = x;
}

void Sprite::set_center_y(double y)
{
  p->center_y = y;
}

void Sprite::set_z(Roole::ZPos z)
{
  p->z = z;
}

void Sprite::set_xyz(double x, double y, Roole::ZPos z)
{
  p->x = x;
  p->y = y;
  p->z = z;
}

void Sprite::set_angle(double a)
{
  p->angle = a;
}

void Sprite::set_color(unsigned c)
{
  p->c = c;
  printf("Getting a new Color!\nRed: %d Green: %d\n", p->c.red(), p->c.green());
}

void Sprite::set_color1(unsigned c)
{
  p->c1 = c;
}

void Sprite::set_color2(unsigned c)
{
  p->c2 = c;
}

void Sprite::set_color3(unsigned c)
{
  p->c3 = c;
}

void Sprite::set_color4(unsigned c)
{
  p->c4 = c;
}

void Sprite::set_visible(bool seen)
{
  p->visible = seen;
}

void Sprite::draw() const
{
  if (!p->visible) return;
  data_->draw(x(), y(), p->c, x2(), y(), p->c,
              x(), y2(), p->c, x2(), y2(), p->c, p->z, p->mode);
}

void Sprite::draw_mod() const
{
  if (!p->visible) return;
  data_->draw(x(), y(), p->c1, x2(), y(), p->c2,
              x(), y2(), p->c3, x2(), y2(), p->c4, p->z, p->mode);
}

void Sprite::draw_rot() const
{
  if (!p->visible) return;
  double size_x = width() * p->scale_x;
  double size_y = height() * p->scale_y;
  double offs_x = Roole::offset_x(p->angle, 1);
  double offs_y = Roole::offset_y(p->angle, 1);
  double dist_to_left_x   = +offs_y * size_x * p->center_x;
  double dist_to_left_y   = -offs_x * size_x * p->center_x;
  double dist_to_right_x  = -offs_y * size_x * (1 - p->center_x);
  double dist_to_right_y  = +offs_x * size_x * (1 - p->center_x);
  double dist_to_top_x    = +offs_x * size_y * p->center_y;
  double dist_to_top_y    = +offs_y * size_y * p->center_y;
  double dist_to_bottom_x = -offs_x * size_y * (1 - p->center_y);
  double dist_to_bottom_y = -offs_y * size_y * (1 - p->center_y);
  data_->draw(x() + dist_to_left_x  + dist_to_top_x,
              y() + dist_to_left_y  + dist_to_top_y, p->c,
              x() + dist_to_right_x + dist_to_top_x,
              y() + dist_to_right_y + dist_to_top_y, p->c,
              x() + dist_to_left_x  + dist_to_bottom_x,
              y() + dist_to_left_y  + dist_to_bottom_y, p->c,
              x() + dist_to_right_x + dist_to_bottom_x,
              y() + dist_to_right_y + dist_to_bottom_y, p->c,
              p->z, p->mode);
}

void Sprite::dispose()
{
  disposed = true;
}

bool Sprite::is_disposed() const
{
  return disposed;
}

vector<Sprite> Roole::get_tiles(const Bitmap& bmp, int tile_width, int tile_height, unsigned flags)
{
  int tiles_x, tiles_y;
  vector<Sprite> images;
  if (tile_width > 0) {
    tiles_x = bmp.width() / tile_width;
  }
  else {
    tiles_x = -tile_width;
    tile_width = bmp.width() / tiles_x;
  }
  if (tile_height > 0) {
    tiles_y = bmp.height() / tile_height;
  }
  else {
    tiles_y = -tile_height;
    tile_height = bmp.height() / tiles_y;
  }
  for (int y = 0; y < tiles_y; ++y) {
    for (int x = 0; x < tiles_x; ++x) {
      images.emplace_back(bmp, x * tile_width, y * tile_height, tile_width, tile_height, flags);
    }
  }
  return images;
}

vector<Sprite> Roole::get_tiles(const char* filename, int tile_width, int tile_height, unsigned flags)
{
  Bitmap bmp;
  load_image_file(bmp, filename);
  return get_tiles(bmp, tile_width, tile_height, flags);
}
