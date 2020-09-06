class BaseScene
  def initialize
    @texts = []
    @labels = []
    @images = []
  end

  def update
    Scene.update_timer
  end
end