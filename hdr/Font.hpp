//! \file Font.hpp
//! Interface of the Font class.

#pragma once

#include "Fwd.hpp"
#include "Color.hpp"
#include "GraphicsBase.hpp"
#include "Platform.hpp"
#include "Text.hpp"
#include <memory>
#include <string>

namespace Roole
{
  //! Fonts are ideal for drawing short, dynamic strings.
  //! For large, static texts you should use Roole::layout_text and turn the result into an image.
  class Font
  {
    struct Impl;
    std::shared_ptr<Impl> pimpl;

  public:
    //! Constructs a font that can be drawn onto the graphics object.
    //! \param font_name Name of a system font, or a filename to a TTF
    //!        file (must contain '/', does not work on Linux).
    //! \param font_height Height of the font, in pixels.
    //! \param font_flags Flags used to render individual characters of
    //!        the font.
    Font(int height, const std::string& name = default_font_name(), unsigned flags = 0);
    //! Returns the name of the font that was used to create it.
    const std::string& name() const;
    void set_name(const std::string& name);
    const std::string& text() const;
    void set_text(const std::string& text);
    //! Returns the height of the font, in pixels.
    int height() const;
    void set_height(int font_height);
    double get_x() const;
    void set_x(double fx);
    double get_y() const;
    void set_y(double fy);
    void set_z(ZPos fz);
    void set_xyz(double fx, double fy, ZPos fz);
    void set_scale_x(double sx);
    void set_scale_y(double sy);
    void set_rel_x(double rx);
    void set_rel_y(double ry);
    Color& color() const;
    void set_color(unsigned c);
    //! Returns the flags used to create the font characters.
    unsigned flags() const;
    void set_visible(bool seen);
    double text_width() const;
    //! Returns the width, in pixels, the given text would occupy if drawn.
    double text_width(const std::string& text) const;
    double markup_width() const;
    //! Returns the width, in pixels, the given text would occupy if drawn.
    double markup_width(const std::string& markup) const;
    //! Draws text so the top left corner of the text is at (x; y).
    void draw(double x = 0, double y = 0) const;
    //! Draws markup so the top left corner of the text is at (x; y).
    void draw_markup(double x = 0, double y = 0) const;
    //! Draws text at a position relative to (x; y).
    //! \param rel_x Determines where the text is drawn horizontally. If
    //! rel_x is 0.0, the text will be to the right of x, if it is 1.0,
    //! the text will be to the left of x, if it is 0.5, it will be
    //! centered on x. All real numbers are possible values.
    //! \param rel_y See rel_x.
    void draw_rel() const;
    //! Draws text at a position relative to (x; y).
    //! \param rel_x Determines where the text is drawn horizontally. If
    //! rel_x is 0.0, the text will be to the right of x, if it is 1.0,
    //! the text will be to the left of x, if it is 0.5, it will be
    //! centered on x. All real numbers are possible values.
    //! \param rel_y See rel_x.
    void draw_markup_rel() const;
    //! Maps a letter to a specific image, instead of generating one using
    //! Roole's built-in text rendering.
    void set_image(std::string codepoint, unsigned font_flags, const Roole::Image& image);
    //! A shortcut for mapping a character to an image regardless of font_flags.
    void set_image(std::string codepoint, const Roole::Image& image);
  };
}
