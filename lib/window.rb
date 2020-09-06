
class Roole::Window
  def update#super
    Scene.scene.update
  end

  def draw#super
    Scene.scene.draw
  end

  def press_button(bid)
    Input.press_button(bid)
  end

  #def button_up(bid)#puts bid
  #  Input.release_button(bid)
  #end
end