class TitleScene < BaseScene
  def initialize(pos)
    super()
    @song = Load.bgm("Fantasy Night.ogg", true)
    @song.play
    @index = pos
    @frame = 0
    @timer = 10
    @backdrop = Load.title('autumn woods.jpg')
    @title_backdrop = Load.picture('oldscrolltitle.png')
    @title_backdrop.x = (Graphics.width - @title_backdrop.width) / 2
    @title_backdrop.y = 40
    @blood = Sprite.load_tiles('images/characters/bloodelf_female2.png', -4, -4)
    @dark = Sprite.load_tiles('images/characters/DarkElfGirl.png', -4, -4)
    @labels << label = Font.new(88, "Blackwood Castle", bold: true)
    label.text = "Elven"
    label.set_xyz(400, 90, 1)
    @labels << label = Font.new(74, "Blackwood Castle", bold: true)
    label.text = "Adventures"
    label.set_xyz(400, 150, 1)
    @labels.each do |lbl|
      lbl.rel_x = 0.5
      lbl.rel_y = 0.5
      lbl.color = Color.new(255, 0, 0, 0)
    end
    @max = @labels.size
    bx = Graphics.width / 2 - @blood[0].width / 2 - 20
    by = Graphics.height - @blood[0].height - 40
    dx = Graphics.width / 2 - @dark[0].width / 2 + 20
    dy = Graphics.height - @dark[0].height - 40
    @blood.each{|img| img.set_xy(bx, by) }
    @dark.each do |img|
      img.set_xy(dx, dy)
      img.alpha = 120
    end
  end

  def update
    super
    if @shutdown
      Scene.close if Scene.timer == 0
      return
    end
    if @timer == 0
      @timer = 10
      @frame = (@frame + 1) % 4
    else
      @timer -= 1
    end
    if Input.press?(Input::Escape) 
      @shutdown = true
      Scene.timer = 10
      return
    elsif Input.press?(Input::Return) or Input.press?(Input::Enter)
      if @index == 0
        Scene.scene = MapScene.new
      end
    end
  end

  def draw
    return if @shutdown
    @backdrop.draw
    @title_backdrop.draw
    @blood[@frame].draw
    @dark[@frame].draw
    @labels.each{|l| l.draw_rel }
  rescue => e
    puts e.class.to_s + "!", e.message, "Backtrace:", e.backtrace
    Scene.close
  end
end