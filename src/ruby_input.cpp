#include "extras.hpp"
#include "Input.hpp"
#include "Window.hpp"

static VALUE RInput, RScene;

void input_define_constants(VALUE self)
{
  rb_define_const(self, "Escape", RB_INT2FIX(Roole::KB_ESCAPE));
  rb_define_const(self, "F1", RB_INT2FIX(Roole::KB_F1));
  rb_define_const(self, "F2", RB_INT2FIX(Roole::KB_F2));
  rb_define_const(self, "F3", RB_INT2FIX(Roole::KB_F3));
  rb_define_const(self, "F4", RB_INT2FIX(Roole::KB_F4));
  rb_define_const(self, "F5", RB_INT2FIX(Roole::KB_F5));
  rb_define_const(self, "F6", RB_INT2FIX(Roole::KB_F6));
  rb_define_const(self, "F7", RB_INT2FIX(Roole::KB_F7));
  rb_define_const(self, "F8", RB_INT2FIX(Roole::KB_F8));
  rb_define_const(self, "F9", RB_INT2FIX(Roole::KB_F9));
  rb_define_const(self, "F10", RB_INT2FIX(Roole::KB_F10));
  rb_define_const(self, "F11", RB_INT2FIX(Roole::KB_F11));
  rb_define_const(self, "F12", RB_INT2FIX(Roole::KB_F12));
  rb_define_const(self, "KB0", RB_INT2FIX(Roole::KB_0));
  rb_define_const(self, "KB1", RB_INT2FIX(Roole::KB_1));
  rb_define_const(self, "KB2", RB_INT2FIX(Roole::KB_2));
  rb_define_const(self, "KB3", RB_INT2FIX(Roole::KB_3));
  rb_define_const(self, "KB4", RB_INT2FIX(Roole::KB_4));
  rb_define_const(self, "KB5", RB_INT2FIX(Roole::KB_5));
  rb_define_const(self, "KB6", RB_INT2FIX(Roole::KB_6));
  rb_define_const(self, "KB7", RB_INT2FIX(Roole::KB_7));
  rb_define_const(self, "KB8", RB_INT2FIX(Roole::KB_8));
  rb_define_const(self, "KB9", RB_INT2FIX(Roole::KB_9));
  rb_define_const(self, "Tab", RB_INT2FIX(Roole::KB_TAB));
  rb_define_const(self, "Return", RB_INT2FIX(Roole::KB_RETURN));
  rb_define_const(self, "Space", RB_INT2FIX(Roole::KB_SPACE));
  rb_define_const(self, "LeftShift", RB_INT2FIX(Roole::KB_LEFT_SHIFT));
  rb_define_const(self, "RightShift", RB_INT2FIX(Roole::KB_RIGHT_SHIFT));
  rb_define_const(self, "LeftControl", RB_INT2FIX(Roole::KB_LEFT_CONTROL));
  rb_define_const(self, "RightControl", RB_INT2FIX(Roole::KB_RIGHT_CONTROL));
  rb_define_const(self, "LeftAlt", RB_INT2FIX(Roole::KB_LEFT_ALT));
  rb_define_const(self, "RightAlt", RB_INT2FIX(Roole::KB_RIGHT_ALT));
  rb_define_const(self, "LeftMeta", RB_INT2FIX(Roole::KB_LEFT_META));
  rb_define_const(self, "RightMeta", RB_INT2FIX(Roole::KB_RIGHT_META));
  rb_define_const(self, "Backspace", RB_INT2FIX(Roole::KB_BACKSPACE));
  rb_define_const(self, "Left", RB_INT2FIX(Roole::KB_LEFT));
  rb_define_const(self, "Right", RB_INT2FIX(Roole::KB_RIGHT));
  rb_define_const(self, "Up", RB_INT2FIX(Roole::KB_UP));
  rb_define_const(self, "Down", RB_INT2FIX(Roole::KB_DOWN));
  rb_define_const(self, "Home", RB_INT2FIX(Roole::KB_HOME));
  rb_define_const(self, "End", RB_INT2FIX(Roole::KB_END));
  rb_define_const(self, "PrintScreen", RB_INT2FIX(Roole::KB_PRINTSCREEN));
  rb_define_const(self, "ScrollLock", RB_INT2FIX(Roole::KB_SCROLLLOCK));
  rb_define_const(self, "Pause", RB_INT2FIX(Roole::KB_PAUSE));
  rb_define_const(self, "Insert", RB_INT2FIX(Roole::KB_INSERT));
  rb_define_const(self, "Delete", RB_INT2FIX(Roole::KB_DELETE));
  rb_define_const(self, "PageUp", RB_INT2FIX(Roole::KB_PAGE_UP));
  rb_define_const(self, "PageDown", RB_INT2FIX(Roole::KB_PAGE_DOWN));
  rb_define_const(self, "Enter", RB_INT2FIX(Roole::KB_ENTER));
  rb_define_const(self, "Backtick", RB_INT2FIX(Roole::KB_BACKTICK));
  rb_define_const(self, "Minus", RB_INT2FIX(Roole::KB_MINUS));
  rb_define_const(self, "Equals", RB_INT2FIX(Roole::KB_EQUALS));
  rb_define_const(self, "LeftBracket", RB_INT2FIX(Roole::KB_LEFT_BRACKET));
  rb_define_const(self, "RightBracket", RB_INT2FIX(Roole::KB_RIGHT_BRACKET));
  rb_define_const(self, "Backslash", RB_INT2FIX(Roole::KB_BACKSLASH));
  rb_define_const(self, "Semicolon", RB_INT2FIX(Roole::KB_SEMICOLON));
  rb_define_const(self, "Apostrophe", RB_INT2FIX(Roole::KB_APOSTROPHE));
  rb_define_const(self, "Comma", RB_INT2FIX(Roole::KB_COMMA));
  rb_define_const(self, "Period", RB_INT2FIX(Roole::KB_PERIOD));
  rb_define_const(self, "Slash", RB_INT2FIX(Roole::KB_SLASH));
  rb_define_const(self, "Capslock", RB_INT2FIX(Roole::KB_CAPSLOCK));
  rb_define_const(self, "A", RB_INT2FIX(Roole::KB_A));
  rb_define_const(self, "B", RB_INT2FIX(Roole::KB_B));
  rb_define_const(self, "C", RB_INT2FIX(Roole::KB_C));
  rb_define_const(self, "D", RB_INT2FIX(Roole::KB_D));
  rb_define_const(self, "E", RB_INT2FIX(Roole::KB_E));
  rb_define_const(self, "F", RB_INT2FIX(Roole::KB_F));
  rb_define_const(self, "G", RB_INT2FIX(Roole::KB_G));
  rb_define_const(self, "H", RB_INT2FIX(Roole::KB_H));
  rb_define_const(self, "I", RB_INT2FIX(Roole::KB_I));
  rb_define_const(self, "J", RB_INT2FIX(Roole::KB_J));
  rb_define_const(self, "K", RB_INT2FIX(Roole::KB_K));
  rb_define_const(self, "L", RB_INT2FIX(Roole::KB_L));
  rb_define_const(self, "M", RB_INT2FIX(Roole::KB_M));
  rb_define_const(self, "N", RB_INT2FIX(Roole::KB_N));
  rb_define_const(self, "O", RB_INT2FIX(Roole::KB_O));
  rb_define_const(self, "P", RB_INT2FIX(Roole::KB_P));
  rb_define_const(self, "Q", RB_INT2FIX(Roole::KB_Q));
  rb_define_const(self, "R", RB_INT2FIX(Roole::KB_R));
  rb_define_const(self, "S", RB_INT2FIX(Roole::KB_S));
  rb_define_const(self, "T", RB_INT2FIX(Roole::KB_T));
  rb_define_const(self, "U", RB_INT2FIX(Roole::KB_U));
  rb_define_const(self, "V", RB_INT2FIX(Roole::KB_V));
  rb_define_const(self, "W", RB_INT2FIX(Roole::KB_W));
  rb_define_const(self, "X", RB_INT2FIX(Roole::KB_X));
  rb_define_const(self, "Y", RB_INT2FIX(Roole::KB_Y));
  rb_define_const(self, "Z", RB_INT2FIX(Roole::KB_Z));
  rb_define_const(self, "Iso", RB_INT2FIX(Roole::KB_ISO));
  rb_define_const(self, "NumPad0", RB_INT2FIX(Roole::KB_NUMPAD_0));
  rb_define_const(self, "NumPad1", RB_INT2FIX(Roole::KB_NUMPAD_1));
  rb_define_const(self, "NumPad2", RB_INT2FIX(Roole::KB_NUMPAD_2));
  rb_define_const(self, "NumPad3", RB_INT2FIX(Roole::KB_NUMPAD_3));
  rb_define_const(self, "NumPad4", RB_INT2FIX(Roole::KB_NUMPAD_4));
  rb_define_const(self, "NumPad5", RB_INT2FIX(Roole::KB_NUMPAD_5));
  rb_define_const(self, "NumPad6", RB_INT2FIX(Roole::KB_NUMPAD_6));
  rb_define_const(self, "NumPad7", RB_INT2FIX(Roole::KB_NUMPAD_7));
  rb_define_const(self, "NumPad8", RB_INT2FIX(Roole::KB_NUMPAD_8));
  rb_define_const(self, "NumPad9", RB_INT2FIX(Roole::KB_NUMPAD_9));
  rb_define_const(self, "NumPadDelete", RB_INT2FIX(Roole::KB_NUMPAD_DELETE));
  rb_define_const(self, "NumPadPlus", RB_INT2FIX(Roole::KB_NUMPAD_PLUS));
  rb_define_const(self, "NumPadAdd", RB_INT2FIX(Roole::KB_NUMPAD_PLUS));
  rb_define_const(self, "NumPadMinus", RB_INT2FIX(Roole::KB_NUMPAD_MINUS));
  rb_define_const(self, "NumPadSubtract", RB_INT2FIX(Roole::KB_NUMPAD_MINUS));
  rb_define_const(self, "NumPadMultiply", RB_INT2FIX(Roole::KB_NUMPAD_MULTIPLY));
  rb_define_const(self, "NumPadDivide", RB_INT2FIX(Roole::KB_NUMPAD_DIVIDE));
  rb_define_const(self, "MsLeft", RB_INT2FIX(Roole::MS_LEFT));
  rb_define_const(self, "MsMiddle", RB_INT2FIX(Roole::MS_MIDDLE));
  rb_define_const(self, "MsRight", RB_INT2FIX(Roole::MS_RIGHT));
  rb_define_const(self, "MsWheelUp", RB_INT2FIX(Roole::MS_WHEEL_UP));
  rb_define_const(self, "MsWheelDown", RB_INT2FIX(Roole::MS_WHEEL_DOWN));
  rb_define_const(self, "MsOther0", RB_INT2FIX(Roole::MS_OTHER_0));
  rb_define_const(self, "MsOther1", RB_INT2FIX(Roole::MS_OTHER_1));
  rb_define_const(self, "MsOther2", RB_INT2FIX(Roole::MS_OTHER_2));
  rb_define_const(self, "MsOther3", RB_INT2FIX(Roole::MS_OTHER_3));
  rb_define_const(self, "MsOther4", RB_INT2FIX(Roole::MS_OTHER_4));
  rb_define_const(self, "MsOther5", RB_INT2FIX(Roole::MS_OTHER_5));
  rb_define_const(self, "MsOther6", RB_INT2FIX(Roole::MS_OTHER_6));
  rb_define_const(self, "MsOther7", RB_INT2FIX(Roole::MS_OTHER_7));
  rb_define_const(self, "GpLeft", RB_INT2FIX(Roole::GP_LEFT));
  rb_define_const(self, "GpRight", RB_INT2FIX(Roole::GP_RIGHT));
  rb_define_const(self, "GpUp", RB_INT2FIX(Roole::GP_UP));
  rb_define_const(self, "GpDown", RB_INT2FIX(Roole::GP_DOWN));
  rb_define_const(self, "GpButton0", RB_INT2FIX(Roole::GP_BUTTON_0));
  rb_define_const(self, "GpButton1", RB_INT2FIX(Roole::GP_BUTTON_1));
  rb_define_const(self, "GpButton2", RB_INT2FIX(Roole::GP_BUTTON_2));
  rb_define_const(self, "GpButton3", RB_INT2FIX(Roole::GP_BUTTON_3));
  rb_define_const(self, "GpButton4", RB_INT2FIX(Roole::GP_BUTTON_4));
  rb_define_const(self, "GpButton5", RB_INT2FIX(Roole::GP_BUTTON_5));
  rb_define_const(self, "GpButton6", RB_INT2FIX(Roole::GP_BUTTON_6));
  rb_define_const(self, "GpButton7", RB_INT2FIX(Roole::GP_BUTTON_7));
  rb_define_const(self, "GpButton8", RB_INT2FIX(Roole::GP_BUTTON_8));
  rb_define_const(self, "GpButton9", RB_INT2FIX(Roole::GP_BUTTON_9));
  rb_define_const(self, "GpButton10", RB_INT2FIX(Roole::GP_BUTTON_10));
  rb_define_const(self, "GpButton11", RB_INT2FIX(Roole::GP_BUTTON_11));
  rb_define_const(self, "GpButton12", RB_INT2FIX(Roole::GP_BUTTON_12));
  rb_define_const(self, "GpButton13", RB_INT2FIX(Roole::GP_BUTTON_13));
  rb_define_const(self, "GpButton14", RB_INT2FIX(Roole::GP_BUTTON_14));
  rb_define_const(self, "GpButton15", RB_INT2FIX(Roole::GP_BUTTON_15));
  rb_define_const(self, "Gp0Left", RB_INT2FIX(Roole::GP_0_LEFT));
  rb_define_const(self, "Gp0Right", RB_INT2FIX(Roole::GP_0_RIGHT));
  rb_define_const(self, "Gp0Up", RB_INT2FIX(Roole::GP_0_UP));
  rb_define_const(self, "Gp0Down", RB_INT2FIX(Roole::GP_0_DOWN));
  rb_define_const(self, "Gp0Button0", RB_INT2FIX(Roole::GP_0_BUTTON_0));
  rb_define_const(self, "Gp0Button1", RB_INT2FIX(Roole::GP_0_BUTTON_1));
  rb_define_const(self, "Gp0Button2", RB_INT2FIX(Roole::GP_0_BUTTON_2));
  rb_define_const(self, "Gp0Button3", RB_INT2FIX(Roole::GP_0_BUTTON_3));
  rb_define_const(self, "Gp0Button4", RB_INT2FIX(Roole::GP_0_BUTTON_4));
  rb_define_const(self, "Gp0Button5", RB_INT2FIX(Roole::GP_0_BUTTON_5));
  rb_define_const(self, "Gp0Button6", RB_INT2FIX(Roole::GP_0_BUTTON_6));
  rb_define_const(self, "Gp0Button7", RB_INT2FIX(Roole::GP_0_BUTTON_7));
  rb_define_const(self, "Gp0Button8", RB_INT2FIX(Roole::GP_0_BUTTON_8));
  rb_define_const(self, "Gp0Button9", RB_INT2FIX(Roole::GP_0_BUTTON_9));
  rb_define_const(self, "Gp0Button10", RB_INT2FIX(Roole::GP_0_BUTTON_10));
  rb_define_const(self, "Gp0Button11", RB_INT2FIX(Roole::GP_0_BUTTON_11));
  rb_define_const(self, "Gp0Button12", RB_INT2FIX(Roole::GP_0_BUTTON_12));
  rb_define_const(self, "Gp0Button13", RB_INT2FIX(Roole::GP_0_BUTTON_13));
  rb_define_const(self, "Gp0Button14", RB_INT2FIX(Roole::GP_0_BUTTON_14));
  rb_define_const(self, "Gp0Button15", RB_INT2FIX(Roole::GP_0_BUTTON_15));
  rb_define_const(self, "Gp1Left", RB_INT2FIX(Roole::GP_1_LEFT));
  rb_define_const(self, "Gp1Right", RB_INT2FIX(Roole::GP_1_RIGHT));
  rb_define_const(self, "Gp1Up", RB_INT2FIX(Roole::GP_1_UP));
  rb_define_const(self, "Gp1Down", RB_INT2FIX(Roole::GP_1_DOWN));
  rb_define_const(self, "Gp1Button0", RB_INT2FIX(Roole::GP_1_BUTTON_0));
  rb_define_const(self, "Gp1Button1", RB_INT2FIX(Roole::GP_1_BUTTON_1));
  rb_define_const(self, "Gp1Button2", RB_INT2FIX(Roole::GP_1_BUTTON_2));
  rb_define_const(self, "Gp1Button3", RB_INT2FIX(Roole::GP_1_BUTTON_3));
  rb_define_const(self, "Gp1Button4", RB_INT2FIX(Roole::GP_1_BUTTON_4));
  rb_define_const(self, "Gp1Button5", RB_INT2FIX(Roole::GP_1_BUTTON_5));
  rb_define_const(self, "Gp1Button6", RB_INT2FIX(Roole::GP_1_BUTTON_6));
  rb_define_const(self, "Gp1Button7", RB_INT2FIX(Roole::GP_1_BUTTON_7));
  rb_define_const(self, "Gp1Button8", RB_INT2FIX(Roole::GP_1_BUTTON_8));
  rb_define_const(self, "Gp1Button9", RB_INT2FIX(Roole::GP_1_BUTTON_9));
  rb_define_const(self, "Gp1Button10", RB_INT2FIX(Roole::GP_1_BUTTON_10));
  rb_define_const(self, "Gp1Button11", RB_INT2FIX(Roole::GP_1_BUTTON_11));
  rb_define_const(self, "Gp1Button12", RB_INT2FIX(Roole::GP_1_BUTTON_12));
  rb_define_const(self, "Gp1Button13", RB_INT2FIX(Roole::GP_1_BUTTON_13));
  rb_define_const(self, "Gp1Button14", RB_INT2FIX(Roole::GP_1_BUTTON_14));
  rb_define_const(self, "Gp1Button15", RB_INT2FIX(Roole::GP_1_BUTTON_15));
  rb_define_const(self, "Gp2Left", RB_INT2FIX(Roole::GP_2_LEFT));
  rb_define_const(self, "Gp2Right", RB_INT2FIX(Roole::GP_2_RIGHT));
  rb_define_const(self, "Gp2Up", RB_INT2FIX(Roole::GP_2_UP));
  rb_define_const(self, "Gp2Down", RB_INT2FIX(Roole::GP_2_DOWN));
  rb_define_const(self, "Gp2Button0", RB_INT2FIX(Roole::GP_2_BUTTON_0));
  rb_define_const(self, "Gp2Button1", RB_INT2FIX(Roole::GP_2_BUTTON_1));
  rb_define_const(self, "Gp2Button2", RB_INT2FIX(Roole::GP_2_BUTTON_2));
  rb_define_const(self, "Gp2Button3", RB_INT2FIX(Roole::GP_2_BUTTON_3));
  rb_define_const(self, "Gp2Button4", RB_INT2FIX(Roole::GP_2_BUTTON_4));
  rb_define_const(self, "Gp2Button5", RB_INT2FIX(Roole::GP_2_BUTTON_5));
  rb_define_const(self, "Gp2Button6", RB_INT2FIX(Roole::GP_2_BUTTON_6));
  rb_define_const(self, "Gp2Button7", RB_INT2FIX(Roole::GP_2_BUTTON_7));
  rb_define_const(self, "Gp2Button8", RB_INT2FIX(Roole::GP_2_BUTTON_8));
  rb_define_const(self, "Gp2Button9", RB_INT2FIX(Roole::GP_2_BUTTON_9));
  rb_define_const(self, "Gp2Button10", RB_INT2FIX(Roole::GP_2_BUTTON_10));
  rb_define_const(self, "Gp2Button11", RB_INT2FIX(Roole::GP_2_BUTTON_11));
  rb_define_const(self, "Gp2Button12", RB_INT2FIX(Roole::GP_2_BUTTON_12));
  rb_define_const(self, "Gp2Button13", RB_INT2FIX(Roole::GP_2_BUTTON_13));
  rb_define_const(self, "Gp2Button14", RB_INT2FIX(Roole::GP_2_BUTTON_14));
  rb_define_const(self, "Gp2Button15", RB_INT2FIX(Roole::GP_2_BUTTON_15));
  rb_define_const(self, "Gp3Left", RB_INT2FIX(Roole::GP_3_LEFT));
  rb_define_const(self, "Gp3Right", RB_INT2FIX(Roole::GP_3_RIGHT));
  rb_define_const(self, "Gp3Up", RB_INT2FIX(Roole::GP_3_UP));
  rb_define_const(self, "Gp3Down", RB_INT2FIX(Roole::GP_3_DOWN));
  rb_define_const(self, "Gp3Button0", RB_INT2FIX(Roole::GP_3_BUTTON_0));
  rb_define_const(self, "Gp3Button1", RB_INT2FIX(Roole::GP_3_BUTTON_1));
  rb_define_const(self, "Gp3Button2", RB_INT2FIX(Roole::GP_3_BUTTON_2));
  rb_define_const(self, "Gp3Button3", RB_INT2FIX(Roole::GP_3_BUTTON_3));
  rb_define_const(self, "Gp3Button4", RB_INT2FIX(Roole::GP_3_BUTTON_4));
  rb_define_const(self, "Gp3Button5", RB_INT2FIX(Roole::GP_3_BUTTON_5));
  rb_define_const(self, "Gp3Button6", RB_INT2FIX(Roole::GP_3_BUTTON_6));
  rb_define_const(self, "Gp3Button7", RB_INT2FIX(Roole::GP_3_BUTTON_7));
  rb_define_const(self, "Gp3Button8", RB_INT2FIX(Roole::GP_3_BUTTON_8));
  rb_define_const(self, "Gp3Button9", RB_INT2FIX(Roole::GP_3_BUTTON_9));
  rb_define_const(self, "Gp3Button10", RB_INT2FIX(Roole::GP_3_BUTTON_10));
  rb_define_const(self, "Gp3Button11", RB_INT2FIX(Roole::GP_3_BUTTON_11));
  rb_define_const(self, "Gp3Button12", RB_INT2FIX(Roole::GP_3_BUTTON_12));
  rb_define_const(self, "Gp3Button13", RB_INT2FIX(Roole::GP_3_BUTTON_13));
  rb_define_const(self, "Gp3Button14", RB_INT2FIX(Roole::GP_3_BUTTON_14));
  rb_define_const(self, "Gp3Button15", RB_INT2FIX(Roole::GP_3_BUTTON_15));
}

void input_reset_buttons()
{
  //VALUE input = rb_define_module("Input");
  //input_clear_buttons(input);
}

static VALUE input_pressed(VALUE self, VALUE btn)
{
  Roole::Button b = btn == Qnil ? Roole::NO_BUTTON : Roole::Button(RB_FIX2INT(btn));
  return Roole::Input::down(b) ? Qtrue : Qfalse;
}

static VALUE input_released(VALUE self, VALUE btn)
{
  Roole::Button b = btn == Qnil ? Roole::NO_BUTTON : Roole::Button(RB_FIX2INT(btn));
  return Roole::Input::up(b) ? Qtrue : Qfalse;
}

static VALUE input_triggered(VALUE self, VALUE btn)
{
  Roole::Button b = btn == Qnil ? Roole::NO_BUTTON : Roole::Button(RB_FIX2INT(btn));
  return Roole::Input::was_down(b) && Roole::Input::up(b) ? Qtrue : Qfalse;
}

static VALUE input_is_chain(VALUE self, VALUE ary)
{
  return rb_iv_get(self, "@chain") == ary ? Qtrue : Qfalse;
}

static VALUE input_mouse_x(VALUE self)
{
  VALUE tmp = rb_iv_get(RScene, "@window");
  Roole::Window *w = (Roole::Window*)RTYPEDDATA(tmp)->data;
  if (!w) return DBL2NUM(0.0);
  tmp = DBL2NUM(w->input().mouse_x());
  return tmp;
}

static VALUE input_mouse_y(VALUE self)
{
  VALUE tmp = rb_iv_get(RScene, "@window");
  Roole::Window *w = (Roole::Window*)RTYPEDDATA(tmp)->data;
  if (!w) return DBL2NUM(0.0);
  tmp = DBL2NUM(w->input().mouse_y());
  return tmp;
}

void init_input()
{
  RScene = rb_define_module("Scene");
  RInput = rb_define_module("Input");
  input_define_constants(RInput);
  rb_define_attr(RInput, "repeat_button", 1, 1);
  rb_define_attr(RInput, "timer", 1, 1);
  VALUE modfunc = rb_obj_method(RInput, rb_sym("module_function"));
  VALUE methods[] = {
    rb_sym("repeat_button"), rb_sym("repeat_button="),
    rb_sym("timer"), rb_sym("timer=")
  };
  rb_method_call(4, methods, modfunc);
  rb_iv_set(RInput, "@repeat_button", Qnil);
  rb_iv_set(RInput, "@button", Qnil);
  rb_iv_set(RInput, "@timer", RB_INT2FIX(0));
  rb_iv_set(RInput, "@chain", rb_ary_new());
  rb_define_module_function(RInput, "press?", RMF(input_pressed), 1);
  rb_define_module_function(RInput, "release?", RMF(input_released), 1);
  rb_define_module_function(RInput, "trigger?", RMF(input_triggered), 1);
  rb_define_module_function(RInput, "chain?", RMF(input_is_chain), 1);
  rb_define_module_function(RInput, "mouse_x", RMF(input_mouse_x), 0);
  rb_define_module_function(RInput, "mouse_y", RMF(input_mouse_y), 0);
}
