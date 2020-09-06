//! \file Sprite.hpp

#include "Fwd.hpp"
#include "Color.hpp"
#include "GraphicsBase.hpp"
#include <memory>
#include <vector>

class Sprite
{
  struct Data;
  std::shared_ptr<Roole::ImageData> data_;

public:
  std::shared_ptr<Data> p;
  Sprite();
  explicit Sprite(const char* filename, unsigned image_flags = Roole::IF_SMOOTH);
  explicit Sprite(const Roole::Bitmap& source, unsigned image_flags = Roole::IF_SMOOTH);
  Sprite(const Roole::Bitmap& source, unsigned src_x, unsigned src_y, unsigned src_width,
      unsigned src_height, unsigned image_flags = Roole::IF_SMOOTH);
  explicit Sprite(std::unique_ptr<Roole::ImageData>&& data);
  double x() const;
  double y() const;
  double x2() const;
  double y2() const;
  double scale_x() const;
  double scale_y() const;
  double center_x() const;
  double center_y() const;
  Roole::ZPos z() const;
  double angle() const;
  Color& color() const;
  Color& color1() const;
  Color& color2() const;
  Color& color3() const;
  Color& color4() const;
  unsigned width() const;
  unsigned height() const;
  bool visible() const;
  void set_x(double x);
  void set_y(double y);
  void set_scale_x(double x);
  void set_scale_y(double y);
  void set_center_x(double x);
  void set_center_y(double y);
  void set_z(Roole::ZPos z);
  void set_xyz(double x, double y, Roole::ZPos z);
  void set_angle(double a);
  void set_color(unsigned c);
  void set_color1(unsigned c);
  void set_color2(unsigned c);
  void set_color3(unsigned c);
  void set_color4(unsigned c);
  void set_visible(bool seen);
  void draw() const;
  void draw_mod() const;
  void draw_rot() const;
  void dispose();
  bool is_disposed() const;
  //! Provides access to the underlying image data object.
  Roole::ImageData& data() const;
private:
  double x_scale() const;
  double y_scale() const;
  bool disposed = false;
};

namespace Roole
{
  std::vector<Sprite> get_tiles(const Bitmap& bmp,
      int tile_width, int tile_height, unsigned image_flags = Roole::IF_SMOOTH);
  std::vector<Sprite> get_tiles(const char* filename,
      int tile_width, int tile_height, unsigned image_flags = Roole::IF_SMOOTH);
}
