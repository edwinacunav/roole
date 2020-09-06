module Scene
  @title = "Elven Adventures"
  @width = 800
  @height = 608
  @framerate = 16.6666666666
  @fullscreen = false
  @resize = false
  module_function
  def init
    Scene.scene = TitleScene.new(0)
  end

  def update_timer
    if @close_timer > 0
      @close_timer -= 1
      Input.clear_buttons
      return
    end
    @timer -= 1 if @timer > 0
  end
end

class Roole::Window
  def press_button(bid)
    puts bid
    Input.press_button(bid)
  end
end