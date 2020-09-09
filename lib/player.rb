module Roole
  module_accessor :player
end

class Player
  def initialize
    @hp = 0
    @mp = 0
    @sp = 0
    @ap = 0
  end

  def update
    @dash = false
    if Input.press?(Input::LeftShift)
      @dash = true
    end
  end
  attr_reader :dash
end