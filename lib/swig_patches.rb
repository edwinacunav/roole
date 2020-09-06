# Exceptions in Window callbacks often get lost, this is especially annoying in draw/update.
# It is not clear whether this is a SWIG issue or if some stack frame is not exception
# compatible, but I just call protected_update etc. in the Ruby wrapper so I can add this
# custom debugging help:
=begin
  alias_method :initialize_without_hash, :initialize
  def initialize width, height, *args
    if args.empty? or args.first.is_a? Hash
      options = args.first || {}
      fullscreen = options[:fullscreen]
      update_interval = options[:update_interval]
      resizable = options[:resizable]
    else
      fullscreen, update_interval = *args
    end
    $gosu_gl_blocks = nil
    initialize_without_hash width, height, !!fullscreen, update_interval || 16.666666, !!resizable
  end
=end
class Hidden::Window
  %w(update draw needs_redraw? needs_cursor?
     lose_focus button_down button_up).each do |callback|
    define_method "protected_#{callback}" do |*args|
      begin
        # If there has been an exception, don't do anything as to not make matters worse.
        # Conveniently turn the return value into a boolean result (for needs_cursor? etc).
        defined?(@_exception) ? false : !!send(callback, *args)
      rescue Exception => e
        # Exit the message loop naturally, then re-throw during the next tick.
        @_exception = e
        close!
        false
      end
    end
  end
  
  def protected_draw_2
    protected_draw
    $gl_blocks_2 = $gl_blocks
    $gl_blocks = nil
  end
  
  alias_method :show_internal, :show
  def show
    show_internal
  rescue => e
    e.backtrace.reject!{|line| line.include?('lib/gosu/swig_patches.rb') }
    puts e.message, 'Backtrace:', e.backtrace
    close!
  end
end
__END__
module Gosu
  # Keep a reference to these blocks that is only cleared after Window#draw.
  # Otherwise, the GC might free these blocks while Gosu is still rendering.
  def self.gl(z = nil, &block)
    $gl_blocks ||= []
    $gl_blocks << block
    if z.nil?
      unsafe_gl(&block)
    else
      unsafe_gl(z, &block)
    end
  end
end

# SWIG somehow maps the instance method "argb" as an overload of the class
# method of the same name.
class Hidden::Color
  alias_method :argb, :to_i
end

# SWIG will not let me rename my method to '[]=', so use alias_method here.
class Hidden::Font
  alias_method :[]=, :set_image
end
