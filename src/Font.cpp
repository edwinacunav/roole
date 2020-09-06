#include "MarkupParser.hpp"
#include "GraphicsImpl.hpp"
#include "Font.hpp"
#include "Graphics.hpp"
#include "Image.hpp"
#include "Sprite.hpp"
#include "Math.hpp"
#include "Text.hpp"
#include "Utility.hpp"
#include "utf8proc.h"
#include <array>
#include <cassert>
#include <map>
#include <stdexcept>
using namespace std;

namespace Roole
{
  static int font_render_scale = 2;
}

struct Roole::Font::Impl
{
  string name;
  string text = "?";
  int height = 4, width = 0;
  double x = 0.0, y = 0.0;
  double scale_x = 1.0, scale_y = 1.0;
  double rel_x = 0.0, rel_y = 0.0;
  ZPos z = 1;
  Color c = Color(Color::WHITE);
  AlphaMode mode = AM_DEFAULT;
  unsigned base_flags;
  bool visible = true;
  // The most common characters are stored directly in an array for maximum performance.
  // (This is the start of the Basic Multilingual Plane, up until the part where right-to-left
  // languages begin, which don't really work with Roole yet.)
  array<array<Image, 0x58f>, FF_COMBINATIONS> fast_glyphs;
  // Everything else is looked up through a map...
  array<map<utf8proc_int32_t, Image>, FF_COMBINATIONS> other_glyphs;
  
  Image& image(char32_t codepoint, unsigned font_flags)
  {
    Image* image;
    if (codepoint < fast_glyphs[font_flags].size()) {
      image = &fast_glyphs[font_flags][codepoint];
    } else {
      image = &other_glyphs[font_flags][codepoint];
    }
    // If this codepoint has not been rendered before, do it now.
    if (image->width() == 0 && image->height() == 0) {
      auto scaled_height = height * (Roole::font_render_scale + Graphics::global_scale);
      u32string string(1, codepoint);
      Bitmap bitmap(scaled_height, scaled_height);
      auto required_width = ceil(Roole::draw_text(bitmap, 0, 0, Color::WHITE, string,
                                                  name, scaled_height, font_flags));
      if (required_width > bitmap.width()) {
        // If the character was wider than high, we need to render it again.
        Bitmap(required_width, scaled_height).swap(bitmap);
        Roole::draw_text(bitmap, 0, 0, Color::WHITE, string,
                        name, scaled_height, font_flags);
      }
      *image = Image(bitmap, 0, 0, required_width, scaled_height);
    }
    return *image;
  }
};

Roole::Font::Font(int font_height, const string& font_name, unsigned font_flags)
: pimpl(new Impl)
{
  pimpl->name = font_name;
  pimpl->height = font_height;
  pimpl->base_flags = font_flags;
}

const string& Roole::Font::name() const
{
  return pimpl->name;
}

void Roole::Font::set_name(const string& name)
{
  pimpl->name = name;
}

const string& Roole::Font::text() const
{
  return pimpl->text;
}

void Roole::Font::set_text(const string& text)
{
  pimpl->text = text;
}

int Roole::Font::height() const
{
  return pimpl->height;
}

void Roole::Font::set_height(int font_height)
{
  pimpl->height = font_height;
}

double Roole::Font::get_x() const
{
  return pimpl->x * font_render_scale;
}

void Roole::Font::set_x(double fx)
{
  pimpl->x = fx;
}

double Roole::Font::get_y() const
{
  return pimpl->y * font_render_scale;
}

void Roole::Font::set_y(double fy)
{
  pimpl->y = fy;
}

void Roole::Font::set_xyz(double fx, double fy, Roole::ZPos fz)
{
  pimpl->x = fx;
  pimpl->y = fy;
  pimpl->z = fz;
}

void Roole::Font::set_scale_x(double sx)
{
  pimpl->scale_x = sx;
}

void Roole::Font::set_scale_y(double sy)
{
  pimpl->scale_y = sy;
}

void Roole::Font::set_rel_x(double rx)
{
  pimpl->rel_x = rx;
}

void Roole::Font::set_rel_y(double ry)
{
  pimpl->rel_y = ry;
}

void Roole::Font::set_z(Roole::ZPos fz)
{
  pimpl->z = fz;
}

unsigned Roole::Font::flags() const
{
  return pimpl->base_flags;
}

Color& Roole::Font::color() const
{
  return pimpl->c;
}

void Roole::Font::set_color(unsigned c)
{
  pimpl->c = c;
}

void Roole::Font::set_visible(bool seen)
{
  pimpl->visible = seen;
}

double Roole::Font::text_width() const
{
  return markup_width(escape_markup(pimpl->text));
}

double Roole::Font::text_width(const string& text) const
{
  return markup_width(escape_markup(text));
}

double Roole::Font::markup_width() const
{
  return markup_width(pimpl->text);
}

double Roole::Font::markup_width(const string& markup) const
{
  double width = 0;
  // Split the text into lines (split_words = false) because Font doesn't implement word-wrapping.
  MarkupParser parser(pimpl->base_flags, false, [&](vector<FormattedString>&& line) {
    double line_width = 0;
    for (auto& part : line) {
      for (auto codepoint : part.text) {
        auto& image = pimpl->image(codepoint, part.flags);
        double image_scale = (1.0 + Graphics::global_scale) * height() / image.height();
        line_width += image_scale * image.width();
      }
    }
    width = max(width, line_width);
  });
  parser.parse(markup);
  return width;
}

void Roole::Font::draw(double fx, double fy) const
{
  if (!pimpl->visible) return;
  double current_y = fy == 0.0 ? get_y() : fy;
  MarkupParser parser(pimpl->base_flags, false, [&](vector<FormattedString>&& line) {
    double current_x = fx == 0.0 ? get_x() : fx;
    for (auto& part : line) {
      for (auto codepoint : part.text) {
        auto& image = pimpl->image(codepoint, part.flags);
        double image_scale = (1.0 + Graphics::global_scale) * height() / image.height();
        image.draw(current_x, current_y, pimpl->z,
                   image_scale * pimpl->scale_x, image_scale * pimpl->scale_y,
                   multiply(pimpl->c, part.color), pimpl->mode);
        current_x += image_scale * pimpl->scale_x * image.width();
      }
    }
    current_y += pimpl->scale_y * height();
  });
  parser.parse(escape_markup(pimpl->text));
}

void Roole::Font::draw_markup(double fx, double fy) const
{
  if (!pimpl->visible) return;
  double current_y = fy == 0.0 ? get_y() : fy;
  // Split the text into lines (split_words = false) because Font doesn't implement word-wrapping.
  MarkupParser parser(pimpl->base_flags, false, [&](vector<FormattedString>&& line) {
    double current_x = fx == 0.0 ? get_x() : fx;
    for (auto& part : line) {
      for (auto codepoint : part.text) {
        auto& image = pimpl->image(codepoint, part.flags);
        double image_scale = (1.0 + Graphics::global_scale) * height() / image.height();
        image.draw(current_x, current_y, pimpl->z,
                   image_scale * pimpl->scale_x, image_scale * pimpl->scale_y,
                   multiply(pimpl->c, part.color), pimpl->mode);
        current_x += image_scale * pimpl->scale_x * image.width();
      }
    }
    current_y += pimpl->scale_y * height();
  });
  parser.parse(pimpl->text);
}

void Roole::Font::draw_rel() const
{
  int x = get_x(), y = get_y();
  if (pimpl->rel_x) x -= text_width(pimpl->text) * pimpl->scale_x * pimpl->rel_x;
  if (pimpl->rel_y) y -= height() * pimpl->scale_y * pimpl->rel_y;
  draw(x, y);
}

void Roole::Font::draw_markup_rel() const
{
  int x = get_x(), y = get_y();
  if (pimpl->rel_x) x -= markup_width(pimpl->text) * pimpl->scale_x * pimpl->rel_x;
  if (pimpl->rel_y) y -= height() * pimpl->scale_y * pimpl->rel_y;
  draw_markup(x, y);
}

void Roole::Font::set_image(std::string codepoint, unsigned font_flags, const Roole::Image& image)
{
  auto utc4 = utf8_to_composed_utc4(codepoint);
  if (utc4.length() != 1)
    throw invalid_argument("Could not compose '" + codepoint + "' into a single codepoint");
  if (utc4[0] < pimpl->fast_glyphs[font_flags].size())
    pimpl->fast_glyphs[font_flags][utc4[0]] = image;
  else
    pimpl->other_glyphs[font_flags][utc4[0]] = image;
}

void Roole::Font::set_image(std::string codepoint, const Roole::Image& image)
{
  for (unsigned font_flags = 0; font_flags < FF_COMBINATIONS; ++font_flags)
    set_image(codepoint, font_flags, image);
}
