class MapScene < BaseScene
  def initialize
    super()
    Scene.timer = 120
    @images << Load.picture('mapsign.png')
    @images[0].y = Graphics.height - @images[0].height
    @labels << label = Font.new(26, "Blackwood Castle", bold: true)
    label.text = "Welcome to"
    label.set_xyz(110, @images[0].y + 28, 1)
    @labels << label = Font.new(38, "Blackwood Castle", bold: true)
    label.text = "Ghost Town"
    label.set_xyz(110, @images[0].y + 58, 1)
    @labels.each do |lbl|
      lbl.rel_xy = 0.5
      lbl.color = Color.new(255, 0, 0, 0)
    end
    @max = @texts.size
  end

  def update
    super
    @hide_image = Scene.timer == 0
    if Input.press?(Input::Escape)
      Scene.call(TitleScene.new(0))
      return
    end
  end

  def draw
    unless @hide_image
      @images[0].draw
      @labels.each{|l| l.draw_rel }
    end
  end
end