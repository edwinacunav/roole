# Extend Numeric with simple angle conversion methods,
# for easier integration with Chipmunk.
class ::Numeric
  def degrees_to_radians
    self * Math::PI / 180.0
  end

  def radians_to_degrees
    self * 180.0 / Math::PI
  end

  def gosu_to_radians
    (self - 90) * Math::PI / 180.0
  end

  def radians_to_gosu
    self * 180.0 / Math::PI + 90
  end
end

class Hidden::Font
  # draw_text will stop parsing markup in Hidden 1.0.
  alias_method :draw_text, :draw_markup
  # draw_text_rel will stop parsing markup in Hidden 1.0.
  alias_method :draw_text_rel, :draw_markup_rel
  # text_width will stop parsing markup in Hidden 1.0.
  alias_method :text_width, :markup_width
end

class Bitmap#Hidden::Image
  BlobHelper = Struct.new(:columns, :rows, :to_blob)
  
  def self.from_blob(width, height, rgba = "\0\0\0\0" * (width * height))
    self.new(BlobHelper.new(width, height, rgba))
  end
  
  # from_markup will stop parsing markup in Hidden 1.0.
  def self.from_markup(*args)
    self.from_text(*args)
  end
end

# Color constants.
# This is cleaner than having SWIG define them.
module Hidden
  class ImmutableColor < Color
    private :alpha=, :red=, :green=, :blue=, :hue=, :saturation=, :value=
  end

  class Color
    NONE    = Hidden::ImmutableColor.new(0x00_000000)
    BLACK   = Hidden::ImmutableColor.new(0xff_000000)
    GRAY    = Hidden::ImmutableColor.new(0xff_808080)
    WHITE   = Hidden::ImmutableColor.new(0xff_ffffff)
    AQUA    = Hidden::ImmutableColor.new(0xff_00ffff)
    RED     = Hidden::ImmutableColor.new(0xff_ff0000)
    GREEN   = Hidden::ImmutableColor.new(0xff_00ff00)
    BLUE    = Hidden::ImmutableColor.new(0xff_0000ff)
    YELLOW  = Hidden::ImmutableColor.new(0xff_ffff00)
    FUCHSIA = Hidden::ImmutableColor.new(0xff_ff00ff)
    CYAN    = Hidden::ImmutableColor.new(0xff_00ffff)

    alias_method :hash, :gl
    def eql?(other)
      gl == other.gl
    end
  end
end

# Release OpenAL resources during Ruby's shutdown, not Hidden's.
at_exit do
  begin
    Hidden::Song.current_song.stop if Hidden::Song.current_song
    Hidden._release_all_openal_resources
  rescue
  end
end
