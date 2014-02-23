#pragma once
const int DIRECTINPUT_VERSION = 0x0800;

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class InputManager
{
public:
	InputManager();
	~InputManager();

    bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);

    bool Update();

    void GetMouseLocation(int& mouseX, int& mouseY) const { mouseX = m_mouseX; mouseY = m_mouseY; }

    bool IsEscapeKeyPressed() const { return m_keyboardState[DIK_ESCAPE] & 0x80; }

    bool IsKeyPressed(unsigned char keyId) const;// { return m_keyboardState[keyId] & 0x80; }

// Private Functions
private:
	InputManager(const InputManager& other)
        : m_directInput(0), m_keyboard(0), m_mouse(0), m_keyboardState(), m_screenWidth(0),
          m_screenHeight(0), m_mouseX(0), m_mouseY(0)
    { m_mouseX = other.m_mouseX; };

    bool ReadKeyboard();
    bool ReadMouse();

    void ProcessInput();

// Private Data Members
private:
    IDirectInput8* m_directInput;
    IDirectInputDevice8* m_keyboard;
    IDirectInputDevice8* m_mouse;

    unsigned char m_keyboardState[256];
    DIMOUSESTATE m_mouseState;

    int m_screenWidth;
    int m_screenHeight;
    int m_mouseX;
    int m_mouseY;
};

InputManager& G_InputManager();
