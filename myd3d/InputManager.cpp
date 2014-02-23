#include "InputManager.h"
#include "d3d_safe_release.h"
#include <cassert>

// Global input manager.
//InputManager G_InputManager;

InputManager::InputManager(void)
    : m_directInput(0),
      m_keyboard(0),
      m_mouse(0),
      m_keyboardState(),
      m_mouseState(),
      m_screenWidth(),
      m_screenHeight(),
      m_mouseX(0),
      m_mouseY(0)
{
}


InputManager::~InputManager(void)
{
    try
    {
        d3d_safe_unacquire_release(m_mouse);
        d3d_safe_unacquire_release(m_keyboard);
        d3d_safe_release(m_directInput);
    }
    catch(int& e)
    {
    }
}


bool InputManager::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Initialize direct input object.
    HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
                                         (void**)&m_directInput, NULL);
    if(FAILED(result))
    {
        return false;
        assert(false);
    }

    //----------------------------------------------------------------------------------------------
    // Init the interface for the keyboard.
    //----------------------------------------------------------------------------------------------
    result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
    if(FAILED(result))
    {
        return false;
        assert(false);
    }

    result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    if(FAILED(result))
    {
        return false;
        assert(false);
    }


    // Set cooperative level of keyboard. NOTE:set to exclusive, Prt Scr won't work. No other 
    // applications will have access.
    result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if(FAILED(result))
    {
        return false;
    }

    // Keyboard interface setup... acquire to get access.
    result = m_keyboard->Acquire();
    if(FAILED(result))
    {
        return false;
        assert(false);
    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Setup mouse.
    //----------------------------------------------------------------------------------------------
    result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    if(FAILED(result))
    {
        return false;
        assert(false);
    }

    result = m_mouse->SetDataFormat(&c_dfDIMouse);
    if(FAILED(result))
    {
        return false;
        assert(false);
    }

    // Set cooperation level of the mouse.
    result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if(FAILED(result))
    {
        return false;
        assert(false);
    }

    // Acquire mouse.
    result = m_mouse->Acquire();
    if(FAILED(result))
    {
        return false;
        assert(false);
    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    
    return true;
}


bool InputManager::Update()
{
    bool result = ReadKeyboard();
    if(!result)
    {
        return false;
    }

    result = ReadMouse();
    if(!result)
    {
        return false;
    }

    ProcessInput();

    return true;
}


bool InputManager::ReadKeyboard()
{
    // TEST CODE. //////////////
    static unsigned char prevKeyState[256];
    for(int i = 0; i < 256; i++)
    {
        prevKeyState[i] = m_keyboardState[i];
    }
    ////////////////////////////

    // Read state of the keyboard.
    HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if(FAILED(result))
    {
        // If we lost focus or not acquired the keyboard... attempt to acquire it.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }

    // TEST CODE. //////////////
    for(int i = 0; i < 256; i++)
    {
        if(prevKeyState[i] != m_keyboardState[i])
        {
            return true;
        }
    }
    ////////////////////////////

    return true;
}


bool InputManager::ReadMouse()
{
    HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if(FAILED(result))
    {
        // If we lost focus or not acquired the keyboard... attempt to acquire it.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_mouse->Acquire();
        }
        else
        {
            return false;
        }
    }

    

    return true;
}


void InputManager::ProcessInput()
{
    m_mouseX += m_mouseState.lX;
    m_mouseY += m_mouseState.lY;

    // Ensure the mouse location does not exceed screen dimensions.
    m_mouseX < 0 ? m_mouseX = 0 : m_mouseX;
    m_mouseY < 0 ? m_mouseY = 0 : m_mouseY;

    m_mouseX > m_screenWidth  ? m_mouseX = m_screenWidth  : m_mouseX;
    m_mouseY > m_screenHeight ? m_mouseY = m_screenHeight : m_mouseY;
}


bool InputManager::IsKeyPressed(unsigned char key) const
{
	return m_keyboardState[key] & 0x80;
}

InputManager& G_InputManager()
{
    static InputManager inputManager;
    return inputManager;
}