#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <WinSock2.h>
#include <Windows.h>

#include "InputManager.h"
#include "Graphics.h"


class DXWindow
{
public:
    DXWindow(void);
    ~DXWindow(void);

    bool IsInitialized() const;
    void Run();

    LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) const;

    // Private Functions
private:
    // Private Copy and Assignment Constructors.
    DXWindow(const DXWindow&);
    DXWindow& operator=(const DXWindow&);

    bool Initialize();
    bool InitializeWindow(int& screenWidth, int& screenHeight, bool& fullscreen);
    void ShutdownWindow();
    bool Frame();

    // Private Data Members
private:
    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;
    Graphics* m_Graphics;
};

// Used to redirect windows system messaging.
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

// Global pointer to window to redirect windows messaging.
static DXWindow* ApplicationHandle = 0;
