
class Roole::Window
  def button_down(bid)
    super(bid)
    if Input::L == bid
      puts "I'm L!"
      return
    end
    press_button(bid)
  end
end