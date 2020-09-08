#ifndef __VGDE_INPUT_H__
#define __VGDE_INPUT_H__

#include "vec.h"
#include "vgde.h"

#include <Windows.h>
#include <Xinput.h>

static const int vk_unknown        = 0;
static const int vk_a              = 1;
static const int vk_b              = 2;
static const int vk_c              = 3;
static const int vk_d              = 4;
static const int vk_e              = 5;
static const int vk_f              = 6;
static const int vk_g              = 7;
static const int vk_h              = 8;
static const int vk_i              = 9;
static const int vk_j              = 10;
static const int vk_k              = 11;
static const int vk_l              = 12;
static const int vk_m              = 13;
static const int vk_n              = 14;
static const int vk_o              = 15;
static const int vk_p              = 16;
static const int vk_q              = 17;
static const int vk_r              = 18;
static const int vk_s              = 19;
static const int vk_t              = 20;
static const int vk_u              = 21;
static const int vk_v              = 22;
static const int vk_w              = 23;
static const int vk_x              = 24;
static const int vk_y              = 25;
static const int vk_z              = 26;
static const int vk_num0           = 27;
static const int vk_num1           = 28;
static const int vk_num2           = 29;
static const int vk_num3           = 30;
static const int vk_num4           = 31;
static const int vk_num5           = 32;
static const int vk_num6           = 33;
static const int vk_num7           = 34;
static const int vk_num8           = 35;
static const int vk_num9           = 36;
static const int vk_escape         = 37;
static const int vk_lControl       = 38;
static const int vk_lShift         = 39;
static const int vk_lAlt           = 40;
static const int vk_lSystem        = 41; ///< The left os specific key, eg the windows key;
static const int vk_rControl       = 42;
static const int vk_rShift         = 43;
static const int vk_rAlt           = 44;
static const int vk_rSystem        = 45;
static const int vk_menu           = 46;
static const int vk_lBracket       = 47;
static const int vk_rBracket       = 48;
static const int vk_semicolon      = 49;
static const int vk_comma          = 50;
static const int vk_period         = 51;
static const int vk_quote          = 52;
static const int vk_slash          = 53;
static const int vk_backslash      = 54;
static const int vk_tilde          = 55;
static const int vk_equal          = 56;
static const int vk_dash           = 57;
static const int vk_space          = 58;
static const int vk_return         = 59; ///< Enter;
static const int vk_backspace      = 60;
static const int vk_tab            = 61;
static const int vk_pageUp         = 62;
static const int vk_pageDown       = 63;
static const int vk_end            = 64;
static const int vk_home           = 65;
static const int vk_insert         = 66;
static const int vk_delete         = 67;
static const int vk_add            = 68;
static const int vk_subtract       = 69;
static const int vk_multiply       = 70;
static const int vk_divide         = 71;
static const int vk_left           = 72;
static const int vk_right          = 73;
static const int vk_up             = 74;
static const int vk_down           = 75;
static const int vk_numpad0        = 76;
static const int vk_numpad1        = 77;
static const int vk_numpad2        = 78;
static const int vk_numpad3        = 79;
static const int vk_numpad4        = 80;
static const int vk_numpad5        = 81;
static const int vk_numpad6        = 82;
static const int vk_numpad7        = 83;
static const int vk_numpad8        = 84;
static const int vk_numpad9        = 85;
static const int vk_f1             = 86;
static const int vk_f2             = 87;
static const int vk_f3             = 88;
static const int vk_f4             = 89;
static const int vk_f5             = 90;
static const int vk_f6             = 91;
static const int vk_f7             = 92;
static const int vk_f8             = 93;
static const int vk_f9             = 94;
static const int vk_f10            = 95;
static const int vk_f11            = 96;
static const int vk_f12            = 97;
static const int vk_f13            = 98;
static const int vk_f14            = 99;
static const int vk_f15            = 100;
static const int vk_pause          = 101;
static const int vk_keyCount       = 102;

static const int vk_control        = 1000;
static const int vk_shift          = 1001;
static const int vk_alt            = 1002;

static const int mb_left           = 201;
static const int mb_right          = 202;
static const int mb_middle         = 203;
static const int mb_x1             = 204;
static const int mb_x2             = 205;
static const int mb_buttonCount    = 6;

static const int XBoxA			   = XINPUT_GAMEPAD_A;
static const int XBoxB			   = XINPUT_GAMEPAD_B;
static const int XBoxX			   = XINPUT_GAMEPAD_X;
static const int XBoxY	           = XINPUT_GAMEPAD_Y;
static const int XBoxLeft		   = XINPUT_GAMEPAD_DPAD_LEFT;
static const int XBoxRight         = XINPUT_GAMEPAD_DPAD_RIGHT;
static const int XBoxUp            = XINPUT_GAMEPAD_DPAD_UP;
static const int XBoxDown          = XINPUT_GAMEPAD_DPAD_DOWN;
static const int XBoxStart         = XINPUT_GAMEPAD_START;
static const int XBoxBack          = XINPUT_GAMEPAD_BACK;
static const int XBoxLShoulder     = XINPUT_GAMEPAD_LEFT_SHOULDER;
static const int XBoxRShoulder     = XINPUT_GAMEPAD_RIGHT_SHOULDER;
static const int XBoxLTrigger      = 300;
static const int XBoxRTrigger      = 301;
static const int XBoxButtonCount   = 14;
static const int XBoxLStick        = 0;
static const int XBoxRStick        = 1;
static const int XBoxVibrationMax  = 65535;
static const int XBoxVibrationHigh = 49151;
static const int XBoxVibrationMed  = 32768;
static const int XBoxVibrationLow  = 16383;
static const int XBoxVibrationNone = 0;

bool inputInit(GLFWwindow *window);
void inputUpdate();

int keyToVKey(int key);
int buttonToVKey(int btn);

bool *currentKeyState();
bool *previousKeyState();

bool *currentButtonState();
bool *previousButtonState();

bool isKeyDown(int key);
bool isKeyDown(int key, int key1, int key2 = 0);

bool isKeyPressed(int key);
bool isKeyPressed(int key, int key1, int key2 = 0);

bool isKeyReleased(int key);
bool isKeyReleased(int key, int key1, int key2 = 0);

//bool isKeyTapped(int key, int time = 100);

bool isButtonDown(int btn);
bool isButtonDown(int btn, int btn1);

bool isButtonPressed(int btn);
bool isButtonPressed(int btn, int btn1);

bool isButtonReleased(int btn);
bool isButtonReleased(int btn, int btn1);

bool isKeyDownGlobal(int key);
bool isButtonDownGlobal(int btn);

vec2i mousePositionGlobal();
vec2i mousePosition();
int mouseX();
int mouseY();

void setMousePositionGlobal(const vec2i &pos);
void setMousePosition(const vec2i &pos);
void setMousePosition(int x, int y);
void moveMouse(const vec2i &offset);
void moveMouse(int xoffset, int yoffset);

#endif
