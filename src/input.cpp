#include "input.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace {
bool _currentKeyState     [vk_keyCount    - 1];
bool _previousKeyState    [vk_keyCount    - 1];
bool _currentButtonState  [mb_buttonCount - 1];
bool _previousButtonState [mb_buttonCount - 1];

GLFWwindow *_window;
HWND _hwnd;
}

bool inputInit(GLFWwindow *window) {
	_window = window;
	_hwnd = glfwGetWin32Window(_window);

	for (int i = 0; i < vk_keyCount - 1; ++i) {
		_currentKeyState  [i] = false;
		_previousKeyState [i] = false;
	}

	for (int j = 0; j < mb_buttonCount - 1; ++j) {
		_currentButtonState  [j] = false;
		_previousButtonState [j] = false;
	}
}

void inputUpdate() {
	memcpy(_previousKeyState,    _currentKeyState,    sizeof(_currentKeyState));
	memcpy(_previousButtonState, _currentButtonState, sizeof(_currentButtonState));

	for (int i = 0; i < vk_keyCount; ++i) {
		_currentKeyState[i - 1] = isKeyDown(i);
	}

	for (int j = 0; j < mb_buttonCount; ++j) {
		_currentButtonState[j - 1] = isButtonDown(j + 200);
	}
}

int keyToVKey(int key) {
	int vkey;
	
	switch (key) {
		default:            vkey = 0;             break;
		case vk_a:          vkey = 'A';           break;
		case vk_b:          vkey = 'B';           break;
		case vk_c:          vkey = 'C';           break;
		case vk_d:          vkey = 'D';           break;
		case vk_e:          vkey = 'E';           break;
		case vk_f:          vkey = 'F';           break;
		case vk_g:          vkey = 'G';           break;
		case vk_h:          vkey = 'H';           break;
		case vk_i:          vkey = 'I';           break;
		case vk_j:          vkey = 'J';           break;
		case vk_k:          vkey = 'K';           break;
		case vk_l:          vkey = 'L';           break;
		case vk_m:          vkey = 'M';           break;
		case vk_n:          vkey = 'N';           break;
		case vk_o:          vkey = 'O';           break;
		case vk_p:          vkey = 'P';           break;
		case vk_q:          vkey = 'Q';           break;
		case vk_r:          vkey = 'R';           break;
		case vk_s:          vkey = 'S';           break;
		case vk_t:          vkey = 'T';           break;
		case vk_u:          vkey = 'U';           break;
		case vk_v:          vkey = 'V';           break;
		case vk_w:          vkey = 'W';           break;
		case vk_x:          vkey = 'X';           break;
		case vk_y:          vkey = 'Y';           break;
		case vk_z:          vkey = 'Z';           break;
		case vk_num0:       vkey = '0';           break;
		case vk_num1:       vkey = '1';           break;
		case vk_num2:       vkey = '2';           break;
		case vk_num3:       vkey = '3';           break;
		case vk_num4:       vkey = '4';           break;
		case vk_num5:       vkey = '5';           break;
		case vk_num6:       vkey = '6';           break;
		case vk_num7:       vkey = '7';           break;
		case vk_num8:       vkey = '8';           break;
		case vk_num9:       vkey = '9';           break;
		case vk_escape:     vkey = VK_ESCAPE;     break;
		case vk_lControl:   vkey = VK_LCONTROL;   break;
		case vk_lShift:     vkey = VK_LSHIFT;     break;
		case vk_lAlt:       vkey = VK_LMENU;      break;
		case vk_lSystem:    vkey = VK_LWIN;       break;
		case vk_rControl:   vkey = VK_RCONTROL;   break;
		case vk_rShift:     vkey = VK_RSHIFT;     break;
		case vk_rAlt:       vkey = VK_RMENU;      break;
		case vk_rSystem:    vkey = VK_RWIN;       break;
		case vk_menu:       vkey = VK_APPS;       break;
		case vk_lBracket:   vkey = VK_OEM_4;      break;
		case vk_rBracket:   vkey = VK_OEM_6;      break;
		case vk_semicolon:  vkey = VK_OEM_1;      break;
		case vk_comma:      vkey = VK_OEM_COMMA;  break;
		case vk_period:     vkey = VK_OEM_PERIOD; break;
		case vk_quote:      vkey = VK_OEM_7;      break;
		case vk_slash:      vkey = VK_OEM_2;      break;
		case vk_backslash:  vkey = VK_OEM_5;      break;
		case vk_tilde:      vkey = VK_OEM_3;      break;
		case vk_equal:      vkey = VK_OEM_PLUS;   break;
		case vk_dash:       vkey = VK_OEM_MINUS;  break;
		case vk_space:      vkey = VK_SPACE;      break;
		case vk_return:     vkey = VK_RETURN;     break;
		case vk_backspace:  vkey = VK_BACK;       break;
		case vk_tab:        vkey = VK_TAB;        break;
		case vk_pageUp:     vkey = VK_PRIOR;      break;
		case vk_pageDown:   vkey = VK_NEXT;       break;
		case vk_end:        vkey = VK_END;        break;
		case vk_home:       vkey = VK_HOME;       break;
		case vk_insert:     vkey = VK_INSERT;     break;
		case vk_delete:     vkey = VK_DELETE;     break;
		case vk_add:        vkey = VK_ADD;        break;
		case vk_subtract:   vkey = VK_SUBTRACT;   break;
		case vk_multiply:   vkey = VK_MULTIPLY;   break;
		case vk_divide:     vkey = VK_DIVIDE;     break;
		case vk_left:       vkey = VK_LEFT;       break;
		case vk_right:      vkey = VK_RIGHT;      break;
		case vk_up:         vkey = VK_UP;         break;
		case vk_down:       vkey = VK_DOWN;       break;
		case vk_numpad0:    vkey = VK_NUMPAD0;    break;
		case vk_numpad1:    vkey = VK_NUMPAD1;    break;
		case vk_numpad2:    vkey = VK_NUMPAD2;    break;
		case vk_numpad3:    vkey = VK_NUMPAD3;    break;
		case vk_numpad4:    vkey = VK_NUMPAD4;    break;
		case vk_numpad5:    vkey = VK_NUMPAD5;    break;
		case vk_numpad6:    vkey = VK_NUMPAD6;    break;
		case vk_numpad7:    vkey = VK_NUMPAD7;    break;
		case vk_numpad8:    vkey = VK_NUMPAD8;    break;
		case vk_numpad9:    vkey = VK_NUMPAD9;    break;
		case vk_f1:         vkey = VK_F1;         break;
		case vk_f2:         vkey = VK_F2;         break;
		case vk_f3:         vkey = VK_F3;         break;
		case vk_f4:         vkey = VK_F4;         break;
		case vk_f5:         vkey = VK_F5;         break;
		case vk_f6:         vkey = VK_F6;         break;
		case vk_f7:         vkey = VK_F7;         break;
		case vk_f8:         vkey = VK_F8;         break;
		case vk_f9:         vkey = VK_F9;         break;
		case vk_f10:        vkey = VK_F10;        break;
		case vk_f11:        vkey = VK_F11;        break;
		case vk_f12:        vkey = VK_F12;        break;
		case vk_f13:        vkey = VK_F13;        break;
		case vk_f14:        vkey = VK_F14;        break;
		case vk_f15:        vkey = VK_F15;        break;
		case vk_pause:      vkey = VK_PAUSE;      break;
	}
	
	return vkey;
}

int buttonToVKey(int btn) {
	switch (btn) {
		default:        return 0;
		case mb_left:	return GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
		case mb_right:	return GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON;
		case mb_middle: return VK_MBUTTON;
		case mb_x1:	    return VK_XBUTTON1;
		case mb_x2:	    return VK_XBUTTON2;
	}
}

bool *currentKeyState() {
	return _currentKeyState;
}

bool *previousKeyState() {
	return _previousKeyState;
}

bool *currentButtonState() {
	return _currentButtonState;
}

bool *previousButtonState() {
	return _previousButtonState;
}

bool isKeyDown(int key) {
	if (!glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
		return false;
	}

	if (key == vk_control) {
		return isKeyDown(vk_lControl, vk_rControl);
	} else if (key == vk_shift) {
		return isKeyDown(vk_lShift, vk_rShift);
	} else if (key == vk_alt) {
		return isKeyDown(vk_lAlt, vk_rAlt);
	}

	return (GetAsyncKeyState(keyToVKey(key)) & 0x8000) != 0;
}

bool isKeyDown(int key, int key1, int key2) {
	return (isKeyDown(key) || isKeyDown(key1) || isKeyDown(key2));
}

bool isKeyPressed(int key) {
	if (!glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
		return false;
	}

	if (key == vk_control) {
		return isKeyPressed(vk_lControl, vk_rControl);
	} else if (key == vk_shift) {
		return isKeyPressed(vk_lShift, vk_rShift);
	} else if (key == vk_alt) {
		return isKeyPressed(vk_lAlt, vk_rAlt);
	}

	return (!_previousKeyState[key - 1] && _currentKeyState[key - 1]);
}

bool isKeyPressed(int key, int key1, int key2) {
	return (isKeyPressed(key) || isKeyPressed(key1) || isKeyPressed(key2));
}

bool isKeyReleased(int key) {
	if (!glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
		return false;
	}

	if (key == vk_control) {
		return isKeyReleased(vk_lControl, vk_rControl);
	} else if (key == vk_shift) {
		return isKeyReleased(vk_lShift, vk_rShift);
	} else if (key == vk_alt) {
		return isKeyReleased(vk_lAlt, vk_rAlt);
	}

	return (_previousKeyState[key - 1] && !_currentKeyState[key - 1]);
}

bool isKeyReleased(int key, int key1, int key2) {
	return (isKeyReleased(key) || isKeyReleased(key1) || isKeyReleased(key2));
}

bool isButtonDown(int btn) {
	if (!glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
		return false;
	}

	return (GetAsyncKeyState(buttonToVKey(btn)) & 0x8000) != 0;
}

bool isButtonDown(int btn, int btn1) {
	return (isButtonDown(btn) || isButtonDown(btn1));
}

bool isButtonPressed(int btn) {
	if (!glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
		return false;
	}

	return (!_previousButtonState[btn - 201] && _currentButtonState[btn - 201]);
}

bool isButtonPressed(int btn, int btn1) {
	return (isButtonPressed(btn) || isButtonPressed(btn1));
}

bool isButtonReleased(int btn) {
	if (!glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
		return false;
	}

	return (_previousButtonState[btn - 201] && !_currentButtonState[btn - 201]);
}

bool isButtonReleased(int btn, int btn1) {
	return (isButtonReleased(btn) || isButtonReleased(btn1));
}

bool isKeyDownGlobal(int key) {
	if (key == vk_control) {
		return isKeyDownGlobal(vk_lControl) || isKeyDownGlobal(vk_rControl);
	} else if (key == vk_shift) {
		return isKeyDownGlobal(vk_lShift) || isKeyDownGlobal(vk_rShift);
	} else if (key == vk_alt) {
		return isKeyDownGlobal(vk_lAlt) || isKeyDownGlobal(vk_rAlt);
	}

	return (GetAsyncKeyState(keyToVKey(key)) & 0x8000) != 0;
}

bool isButtonDownGlobal(int btn) {
	return (GetAsyncKeyState(buttonToVKey(btn)) & 0x8000) != 0;
}

vec2i getMousePositionGlobal() {
	POINT p;
	GetCursorPos(&p);

	return vec2i(p.x, p.y);
}

vec2i getMousePosition() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(_hwnd, &p);

	return vec2i(p.x, p.y);
}

int getMouseX() {
	return getMousePosition().x;
}

int getMouseY() {
	return getMousePosition().y;
}

void setMousePositionGlobal(const vec2i &pos) {
	SetCursorPos(pos.x, pos.y);
}

void setMousePosition(const vec2i &pos) {
	POINT p = {pos.x, pos.y};
	RECT r;//, c;
	GetWindowRect(_hwnd, &r);
	//GetClientRect(_hwnd, &c);
	LONG titleBarHeight = GetSystemMetrics(SM_CYCAPTION) + 4;
	int borderWidth = GetSystemMetrics(SM_CXBORDER);


	//Check if the window is fullscreen and if so ignore border.
	RECT b;
	GetWindowRect(GetDesktopWindow(), &b);
	if (r.left   == b.left  &&
		r.top    == b.top   &&
		r.right  == b.right &&
		r.bottom == b.bottom) 
	{
		titleBarHeight = 0;
		borderWidth = 0;
	}

	//TODO(Skyler): Fix mouse position when not in fullscreen.
 	SetCursorPos(r.left + p.x + borderWidth, r.top + p.y + titleBarHeight);
	//SetCursorPos(r.left, r.top);
}

void setMousePosition(int x, int y) {
	setMousePosition(vec2i(x, y));
}

void moveMouse(const vec2i &offset) {
	vec2i pos = getMousePosition();
	setMousePosition(pos + offset);
}

void moveMouse(int xoffset, int yoffset) {
	moveMouse(vec2i(xoffset, yoffset));
}
