module Input
  #self.constants.each do |long_name|
    #constant = self.const_get(long_name)
    #@states[constant] = State.new
  #end
  module_function
  def press_button(button)
    #@timer = @button == @old_button ? @timer + 1 : 0
  end

  #def release_button(button)
    #@button = nil
    #@states[button].press = false
    #@states[button].trigger = true
    #@timer = 0
  #end

  def ok?
    return true if state(KbReturn).trigger or state(KbEnter).trigger
    return true if state(KbSpace).trigger
    state(KbC).trigger
  end
end