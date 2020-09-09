module Input
  extend self
  def ok?
    trigger?(Return) or trigger?(Enter) or trigger?(KbSpace)
  end
end