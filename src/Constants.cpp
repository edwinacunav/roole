#include "Version.hpp"
#include "Buttons.hpp"
#include "GraphicsBase.hpp"

#ifdef __cplusplus
extern "C" {
#endif

const char *Roole_version()
{
  return Roole::VERSION.c_str();
}

const char *Roole_licenses()
{
  return Roole::LICENSES.c_str();
}

unsigned Roole_MAJOR_VERSION = ROOLE_MAJOR_VERSION;
unsigned Roole_MINOR_VERSION = ROOLE_MINOR_VERSION;
unsigned Roole_POINT_VERSION = ROOLE_POINT_VERSION;

// Alpha/Blend Modes
unsigned Roole_AM_DEFAULT = Roole::AM_DEFAULT;
unsigned Roole_AM_INTERPOLATE = Roole::AM_INTERPOLATE;
unsigned Roole_AM_ADD = Roole::AM_ADD;
unsigned Roole_AM_MULTIPLY = Roole::AM_MULTIPLY;

// Font Flags
unsigned Roole_FF_BOLD = Roole::FF_BOLD;
unsigned Roole_FF_ITALIC = Roole::FF_ITALIC;
unsigned Roole_FF_UNDERLINE = Roole::FF_UNDERLINE;
unsigned Roole_FF_COMBINATIONS = Roole::FF_COMBINATIONS;

// Alignment
unsigned Roole_AL_LEFT = Roole::AL_LEFT;
unsigned Roole_AL_RIGHT = Roole::AL_RIGHT;
unsigned Roole_AL_CENTER = Roole::AL_CENTER;
unsigned Roole_AL_JUSTIFY = Roole::AL_JUSTIFY;

// Image Flags
unsigned Roole_IF_SMOOTH = Roole::IF_SMOOTH;
unsigned Roole_IF_TILEABLE_LEFT = Roole::IF_TILEABLE_LEFT;
unsigned Roole_IF_TILEABLE_TOP = Roole::IF_TILEABLE_TOP;
unsigned Roole_IF_TILEABLE_RIGHT = Roole::IF_TILEABLE_RIGHT;
unsigned Roole_IF_TILEABLE_BOTTOM = Roole::IF_TILEABLE_BOTTOM;
unsigned Roole_IF_TILEABLE = Roole::IF_TILEABLE;
unsigned Roole_IF_RETRO = Roole::IF_RETRO;

unsigned Roole_KB_RANGE_BEGIN = Roole::KB_RANGE_BEGIN;
unsigned Roole_KB_ESCAPE = Roole::KB_ESCAPE;
unsigned Roole_KB_F1 = Roole::KB_F1;
unsigned Roole_KB_F2 = Roole::KB_F2;
unsigned Roole_KB_F3 = Roole::KB_F3;
unsigned Roole_KB_F4 = Roole::KB_F4;
unsigned Roole_KB_F5 = Roole::KB_F5;
unsigned Roole_KB_F6 = Roole::KB_F6;
unsigned Roole_KB_F7 = Roole::KB_F7;
unsigned Roole_KB_F8 = Roole::KB_F8;
unsigned Roole_KB_F9 = Roole::KB_F9;
unsigned Roole_KB_F10 = Roole::KB_F10;
unsigned Roole_KB_F11 = Roole::KB_F11;
unsigned Roole_KB_F12 = Roole::KB_F12;
unsigned Roole_KB_0 = Roole::KB_0;
unsigned Roole_KB_1 = Roole::KB_1;
unsigned Roole_KB_2 = Roole::KB_2;
unsigned Roole_KB_3 = Roole::KB_3;
unsigned Roole_KB_4 = Roole::KB_4;
unsigned Roole_KB_5 = Roole::KB_5;
unsigned Roole_KB_6 = Roole::KB_6;
unsigned Roole_KB_7 = Roole::KB_7;
unsigned Roole_KB_8 = Roole::KB_8;
unsigned Roole_KB_9 = Roole::KB_9;
unsigned Roole_KB_TAB = Roole::KB_TAB;
unsigned Roole_KB_RETURN = Roole::KB_RETURN;
unsigned Roole_KB_SPACE = Roole::KB_SPACE;
unsigned Roole_KB_LEFT_SHIFT = Roole::KB_LEFT_SHIFT;
unsigned Roole_KB_RIGHT_SHIFT = Roole::KB_RIGHT_SHIFT;
unsigned Roole_KB_LEFT_CONTROL = Roole::KB_LEFT_CONTROL;
unsigned Roole_KB_RIGHT_CONTROL = Roole::KB_RIGHT_CONTROL;
unsigned Roole_KB_LEFT_ALT = Roole::KB_LEFT_ALT;
unsigned Roole_KB_RIGHT_ALT = Roole::KB_RIGHT_ALT;
unsigned Roole_KB_LEFT_META = Roole::KB_LEFT_META;
unsigned Roole_KB_RIGHT_META = Roole::KB_RIGHT_META;
unsigned Roole_KB_BACKSPACE = Roole::KB_BACKSPACE;
unsigned Roole_KB_LEFT = Roole::KB_LEFT;
unsigned Roole_KB_RIGHT = Roole::KB_RIGHT;
unsigned Roole_KB_UP = Roole::KB_UP;
unsigned Roole_KB_DOWN = Roole::KB_DOWN;
unsigned Roole_KB_HOME = Roole::KB_HOME;
unsigned Roole_KB_END = Roole::KB_END;
unsigned Roole_KB_INSERT = Roole::KB_INSERT;
unsigned Roole_KB_DELETE = Roole::KB_DELETE;
unsigned Roole_KB_PAGE_UP = Roole::KB_PAGE_UP;
unsigned Roole_KB_PAGE_DOWN = Roole::KB_PAGE_DOWN;
unsigned Roole_KB_ENTER = Roole::KB_ENTER;
unsigned Roole_KB_BACKTICK = Roole::KB_BACKTICK;
unsigned Roole_KB_MINUS = Roole::KB_MINUS;
unsigned Roole_KB_EQUALS = Roole::KB_EQUALS;
unsigned Roole_KB_LEFT_BRACKET = Roole::KB_LEFT_BRACKET;
unsigned Roole_KB_RIGHT_BRACKET = Roole::KB_RIGHT_BRACKET;
unsigned Roole_KB_BACKSLASH = Roole::KB_BACKSLASH;
unsigned Roole_KB_SEMICOLON = Roole::KB_SEMICOLON;
unsigned Roole_KB_APOSTROPHE = Roole::KB_APOSTROPHE;
unsigned Roole_KB_COMMA = Roole::KB_COMMA;
unsigned Roole_KB_PERIOD = Roole::KB_PERIOD;
unsigned Roole_KB_SLASH = Roole::KB_SLASH;
unsigned Roole_KB_A = Roole::KB_A;
unsigned Roole_KB_B = Roole::KB_B;
unsigned Roole_KB_C = Roole::KB_C;
unsigned Roole_KB_D = Roole::KB_D;
unsigned Roole_KB_E = Roole::KB_E;
unsigned Roole_KB_F = Roole::KB_F;
unsigned Roole_KB_G = Roole::KB_G;
unsigned Roole_KB_H = Roole::KB_H;
unsigned Roole_KB_I = Roole::KB_I;
unsigned Roole_KB_J = Roole::KB_J;
unsigned Roole_KB_K = Roole::KB_K;
unsigned Roole_KB_L = Roole::KB_L;
unsigned Roole_KB_M = Roole::KB_M;
unsigned Roole_KB_N = Roole::KB_N;
unsigned Roole_KB_O = Roole::KB_O;
unsigned Roole_KB_P = Roole::KB_P;
unsigned Roole_KB_Q = Roole::KB_Q;
unsigned Roole_KB_R = Roole::KB_R;
unsigned Roole_KB_S = Roole::KB_S;
unsigned Roole_KB_T = Roole::KB_T;
unsigned Roole_KB_U = Roole::KB_U;
unsigned Roole_KB_V = Roole::KB_V;
unsigned Roole_KB_W = Roole::KB_W;
unsigned Roole_KB_X = Roole::KB_X;
unsigned Roole_KB_Y = Roole::KB_Y;
unsigned Roole_KB_Z = Roole::KB_Z;
unsigned Roole_KB_ISO = Roole::KB_ISO;
unsigned Roole_KB_NUMPAD_0 = Roole::KB_NUMPAD_0;
unsigned Roole_KB_NUMPAD_1 = Roole::KB_NUMPAD_1;
unsigned Roole_KB_NUMPAD_2 = Roole::KB_NUMPAD_2;
unsigned Roole_KB_NUMPAD_3 = Roole::KB_NUMPAD_3;
unsigned Roole_KB_NUMPAD_4 = Roole::KB_NUMPAD_4;
unsigned Roole_KB_NUMPAD_5 = Roole::KB_NUMPAD_5;
unsigned Roole_KB_NUMPAD_6 = Roole::KB_NUMPAD_6;
unsigned Roole_KB_NUMPAD_7 = Roole::KB_NUMPAD_7;
unsigned Roole_KB_NUMPAD_8 = Roole::KB_NUMPAD_8;
unsigned Roole_KB_NUMPAD_9 = Roole::KB_NUMPAD_9;
unsigned Roole_KB_NUMPAD_DELETE = Roole::KB_NUMPAD_DELETE;
unsigned Roole_KB_NUMPAD_PLUS = Roole::KB_NUMPAD_PLUS;
unsigned Roole_KB_NUMPAD_MINUS = Roole::KB_NUMPAD_MINUS;
unsigned Roole_KB_NUMPAD_MULTIPLY = Roole::KB_NUMPAD_MULTIPLY;
unsigned Roole_KB_NUMPAD_DIVIDE = Roole::KB_NUMPAD_DIVIDE;
unsigned Roole_KB_RANGE_END = Roole::KB_RANGE_END;

unsigned Roole_MS_RANGE_BEGIN = Roole::MS_RANGE_BEGIN;
unsigned Roole_MS_LEFT = Roole::MS_LEFT;
unsigned Roole_MS_MIDDLE = Roole::MS_MIDDLE;
unsigned Roole_MS_RIGHT = Roole::MS_RIGHT;
unsigned Roole_MS_WHEEL_UP = Roole::MS_WHEEL_UP;
unsigned Roole_MS_WHEEL_DOWN = Roole::MS_WHEEL_DOWN;
unsigned Roole_MS_OTHER_0 = Roole::MS_OTHER_0;
unsigned Roole_MS_OTHER_1 = Roole::MS_OTHER_1;
unsigned Roole_MS_OTHER_2 = Roole::MS_OTHER_2;
unsigned Roole_MS_OTHER_3 = Roole::MS_OTHER_3;
unsigned Roole_MS_OTHER_4 = Roole::MS_OTHER_4;
unsigned Roole_MS_OTHER_5 = Roole::MS_OTHER_5;
unsigned Roole_MS_OTHER_6 = Roole::MS_OTHER_6;
unsigned Roole_MS_OTHER_7 = Roole::MS_OTHER_7;
unsigned Roole_MS_RANGE_END = Roole::MS_RANGE_END;

unsigned Roole_GP_RANGE_BEGIN = Roole::GP_RANGE_BEGIN;
unsigned Roole_GP_LEFT = Roole::GP_LEFT;
unsigned Roole_GP_RIGHT = Roole::GP_RIGHT;
unsigned Roole_GP_UP = Roole::GP_UP;
unsigned Roole_GP_DOWN = Roole::GP_DOWN;
unsigned Roole_GP_BUTTON_0 = Roole::GP_BUTTON_0;
unsigned Roole_GP_BUTTON_1 = Roole::GP_BUTTON_1;
unsigned Roole_GP_BUTTON_2 = Roole::GP_BUTTON_2;
unsigned Roole_GP_BUTTON_3 = Roole::GP_BUTTON_3;
unsigned Roole_GP_BUTTON_4 = Roole::GP_BUTTON_4;
unsigned Roole_GP_BUTTON_5 = Roole::GP_BUTTON_5;
unsigned Roole_GP_BUTTON_6 = Roole::GP_BUTTON_6;
unsigned Roole_GP_BUTTON_7 = Roole::GP_BUTTON_7;
unsigned Roole_GP_BUTTON_8 = Roole::GP_BUTTON_8;
unsigned Roole_GP_BUTTON_9 = Roole::GP_BUTTON_9;
unsigned Roole_GP_BUTTON_10 = Roole::GP_BUTTON_10;
unsigned Roole_GP_BUTTON_11 = Roole::GP_BUTTON_11;
unsigned Roole_GP_BUTTON_12 = Roole::GP_BUTTON_12;
unsigned Roole_GP_BUTTON_13 = Roole::GP_BUTTON_13;
unsigned Roole_GP_BUTTON_14 = Roole::GP_BUTTON_14;
unsigned Roole_GP_BUTTON_15 = Roole::GP_BUTTON_15;

unsigned Roole_GP_0_LEFT = Roole::GP_0_LEFT;
unsigned Roole_GP_0_RIGHT = Roole::GP_0_RIGHT;
unsigned Roole_GP_0_UP = Roole::GP_0_UP;
unsigned Roole_GP_0_DOWN = Roole::GP_0_DOWN;
unsigned Roole_GP_0_BUTTON_0 = Roole::GP_0_BUTTON_0;
unsigned Roole_GP_0_BUTTON_1 = Roole::GP_0_BUTTON_1;
unsigned Roole_GP_0_BUTTON_2 = Roole::GP_0_BUTTON_2;
unsigned Roole_GP_0_BUTTON_3 = Roole::GP_0_BUTTON_3;
unsigned Roole_GP_0_BUTTON_4 = Roole::GP_0_BUTTON_4;
unsigned Roole_GP_0_BUTTON_5 = Roole::GP_0_BUTTON_5;
unsigned Roole_GP_0_BUTTON_6 = Roole::GP_0_BUTTON_6;
unsigned Roole_GP_0_BUTTON_7 = Roole::GP_0_BUTTON_7;
unsigned Roole_GP_0_BUTTON_8 = Roole::GP_0_BUTTON_8;
unsigned Roole_GP_0_BUTTON_9 = Roole::GP_0_BUTTON_9;
unsigned Roole_GP_0_BUTTON_10 = Roole::GP_0_BUTTON_10;
unsigned Roole_GP_0_BUTTON_11 = Roole::GP_0_BUTTON_11;
unsigned Roole_GP_0_BUTTON_12 = Roole::GP_0_BUTTON_12;
unsigned Roole_GP_0_BUTTON_13 = Roole::GP_0_BUTTON_13;
unsigned Roole_GP_0_BUTTON_14 = Roole::GP_0_BUTTON_14;
unsigned Roole_GP_0_BUTTON_15 = Roole::GP_0_BUTTON_15;

unsigned Roole_GP_1_LEFT = Roole::GP_1_LEFT;
unsigned Roole_GP_1_RIGHT = Roole::GP_1_RIGHT;
unsigned Roole_GP_1_UP = Roole::GP_1_UP;
unsigned Roole_GP_1_DOWN = Roole::GP_1_DOWN;
unsigned Roole_GP_1_BUTTON_0 = Roole::GP_1_BUTTON_0;
unsigned Roole_GP_1_BUTTON_1 = Roole::GP_1_BUTTON_1;
unsigned Roole_GP_1_BUTTON_2 = Roole::GP_1_BUTTON_2;
unsigned Roole_GP_1_BUTTON_3 = Roole::GP_1_BUTTON_3;
unsigned Roole_GP_1_BUTTON_4 = Roole::GP_1_BUTTON_4;
unsigned Roole_GP_1_BUTTON_5 = Roole::GP_1_BUTTON_5;
unsigned Roole_GP_1_BUTTON_6 = Roole::GP_1_BUTTON_6;
unsigned Roole_GP_1_BUTTON_7 = Roole::GP_1_BUTTON_7;
unsigned Roole_GP_1_BUTTON_8 = Roole::GP_1_BUTTON_8;
unsigned Roole_GP_1_BUTTON_9 = Roole::GP_1_BUTTON_9;
unsigned Roole_GP_1_BUTTON_10 = Roole::GP_1_BUTTON_10;
unsigned Roole_GP_1_BUTTON_11 = Roole::GP_1_BUTTON_11;
unsigned Roole_GP_1_BUTTON_12 = Roole::GP_1_BUTTON_12;
unsigned Roole_GP_1_BUTTON_13 = Roole::GP_1_BUTTON_13;
unsigned Roole_GP_1_BUTTON_14 = Roole::GP_1_BUTTON_14;
unsigned Roole_GP_1_BUTTON_15 = Roole::GP_1_BUTTON_15;

unsigned Roole_GP_2_LEFT = Roole::GP_2_LEFT;
unsigned Roole_GP_2_RIGHT = Roole::GP_2_RIGHT;
unsigned Roole_GP_2_UP = Roole::GP_2_UP;
unsigned Roole_GP_2_DOWN = Roole::GP_2_DOWN;
unsigned Roole_GP_2_BUTTON_0 = Roole::GP_2_BUTTON_0;
unsigned Roole_GP_2_BUTTON_1 = Roole::GP_2_BUTTON_1;
unsigned Roole_GP_2_BUTTON_2 = Roole::GP_2_BUTTON_2;
unsigned Roole_GP_2_BUTTON_3 = Roole::GP_2_BUTTON_3;
unsigned Roole_GP_2_BUTTON_4 = Roole::GP_2_BUTTON_4;
unsigned Roole_GP_2_BUTTON_5 = Roole::GP_2_BUTTON_5;
unsigned Roole_GP_2_BUTTON_6 = Roole::GP_2_BUTTON_6;
unsigned Roole_GP_2_BUTTON_7 = Roole::GP_2_BUTTON_7;
unsigned Roole_GP_2_BUTTON_8 = Roole::GP_2_BUTTON_8;
unsigned Roole_GP_2_BUTTON_9 = Roole::GP_2_BUTTON_9;
unsigned Roole_GP_2_BUTTON_10 = Roole::GP_2_BUTTON_10;
unsigned Roole_GP_2_BUTTON_11 = Roole::GP_2_BUTTON_11;
unsigned Roole_GP_2_BUTTON_12 = Roole::GP_2_BUTTON_12;
unsigned Roole_GP_2_BUTTON_13 = Roole::GP_2_BUTTON_13;
unsigned Roole_GP_2_BUTTON_14 = Roole::GP_2_BUTTON_14;
unsigned Roole_GP_2_BUTTON_15 = Roole::GP_2_BUTTON_15;

unsigned Roole_GP_3_LEFT = Roole::GP_3_LEFT;
unsigned Roole_GP_3_RIGHT = Roole::GP_3_RIGHT;
unsigned Roole_GP_3_UP = Roole::GP_3_UP;
unsigned Roole_GP_3_DOWN = Roole::GP_3_DOWN;
unsigned Roole_GP_3_BUTTON_0 = Roole::GP_3_BUTTON_0;
unsigned Roole_GP_3_BUTTON_1 = Roole::GP_3_BUTTON_1;
unsigned Roole_GP_3_BUTTON_2 = Roole::GP_3_BUTTON_2;
unsigned Roole_GP_3_BUTTON_3 = Roole::GP_3_BUTTON_3;
unsigned Roole_GP_3_BUTTON_4 = Roole::GP_3_BUTTON_4;
unsigned Roole_GP_3_BUTTON_5 = Roole::GP_3_BUTTON_5;
unsigned Roole_GP_3_BUTTON_6 = Roole::GP_3_BUTTON_6;
unsigned Roole_GP_3_BUTTON_7 = Roole::GP_3_BUTTON_7;
unsigned Roole_GP_3_BUTTON_8 = Roole::GP_3_BUTTON_8;
unsigned Roole_GP_3_BUTTON_9 = Roole::GP_3_BUTTON_9;
unsigned Roole_GP_3_BUTTON_10 = Roole::GP_3_BUTTON_10;
unsigned Roole_GP_3_BUTTON_11 = Roole::GP_3_BUTTON_11;
unsigned Roole_GP_3_BUTTON_12 = Roole::GP_3_BUTTON_12;
unsigned Roole_GP_3_BUTTON_13 = Roole::GP_3_BUTTON_13;
unsigned Roole_GP_3_BUTTON_14 = Roole::GP_3_BUTTON_14;
unsigned Roole_GP_3_BUTTON_15 = Roole::GP_3_BUTTON_15;

unsigned Roole_GP_RANGE_END = Roole::GP_RANGE_END;

unsigned Roole_NUM_BUTTONS = Roole::NUM_BUTTONS;
unsigned Roole_NUM_GAMEPADS = Roole::NUM_GAMEPADS;
unsigned Roole_NO_BUTTON = Roole::NO_BUTTON;

unsigned Roole_KB_NUM = Roole::KB_NUM;
unsigned Roole_MS_NUM = Roole::MS_NUM;
unsigned Roole_GP_NUM = Roole::GP_NUM;
unsigned Roole_GP_NUM_PER_GAMEPAD = Roole::GP_NUM_PER_GAMEPAD;

#ifdef __cplusplus
}
#endif
