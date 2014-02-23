#include "DXWindow.h"
#include <map>
#include <fstream>

#include <AntTweakBar.h>

#define WINCONFIG_FILENAME "window_config.txt"

DXWindow::DXWindow(void) :	
    m_applicationName(),
    m_hinstance(),
    m_hwnd(0),
    m_Graphics(0)
{
    int screenWidth, screenHeight;
    bool fullscreen;

    if(InitializeWindow(screenWidth, screenHeight, fullscreen))
    {
        // Initialize input manager.
        bool result = G_InputManager().Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
        if(!result)
        {
            MessageBox(m_hwnd, L"Could not initialize input manager.", L"Error", MB_OK);
            //assert(false);
        }

        // Initialize graphics.
        m_Graphics = new Graphics(screenWidth, screenHeight, m_hwnd, fullscreen);
    }	
}


DXWindow::~DXWindow(void)
{
    if(m_Graphics)
    {
        delete m_Graphics;
        m_Graphics = 0;
    }

    try
    {
        ShutdownWindow();
    }
    catch(int& e)
    {
    }
}


bool DXWindow::IsInitialized() const
{
    if(!m_Graphics || !m_Graphics->IsInitialized())
        return false;
    else
        return true;
}


void DXWindow::Run()
{
    MSG msg;
    // Init message structure.
    ZeroMemory(&msg, sizeof(MSG));

    bool done = false;
    while(!done)
    {
        // Handle mwindows messages.
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
        {
            done = true;
        }
        else
        { 
            // If frame returns false, user wanted to quit.
            done = !(Frame());
        }

        if(G_InputManager().IsEscapeKeyPressed())
        {
            done = true;
        }
    }
}


bool DXWindow::Frame()
{
    bool result = G_InputManager().Update();
    if(!result)
    {
        return false;
    }

    return m_Graphics->Frame();
    //return m_Graphics->Frame();
    return true;
}


LRESULT CALLBACK DXWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) const
{
    /*switch(umsg)
    {
    case WM_KEYDOWN:
        m_InputMgr->KeyDown((unsigned int)wparam);
        break;

    case WM_KEYUP:
        m_InputMgr->KeyUp((unsigned int)wparam);
        break;

    default:
        break;
    }*/
    if(TwEventWin(hwnd, umsg, wparam, lparam))
        return 0;

    // Send all messages to default message handler.
    return DefWindowProc(hwnd, umsg, wparam, lparam);
}


bool DXWindow::InitializeWindow(int& screenWidth, int& screenHeight, bool& fullscreen)
{
    // Set external pointer to this object.
    ApplicationHandle = this;

    // Get the instance of the application
    m_hinstance = GetModuleHandle(NULL);

    //----------------------------------------------------------------------------------------------
    // Load window config settings.
    std::ifstream fin;
    fin.open(WINCONFIG_FILENAME);

    if(fin.fail())
    {// Failed to open config file.
        return false;
        assert(false);
    }
    
    std::string line;
    while(std::getline(fin, line))
    {
        int eq = line.find('=');
        std::string key     = line.substr(0, eq);
        std::string value   = line.substr(eq+1);
        if(key.compare("AppName") == 0)
        {
            // FAILED ATTEMPT AT CONVERTING TO WIDE STRING. URGH. IT WORKED BUT WINDOWS DOESN'T LIKE
            // Convert to wide string. First get the length needed, then create the wstring, then 
            // convert the data form value to the wstring appWString.
            /*size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, value.c_str(), (int)value.length(), 0, 
                                                     0 );
            std::wstring appWString(reqLength, L'\0');
            ::MultiByteToWideChar(CP_UTF8, 0, value.c_str(), (int)value.length(), &appWString[0], 
                                  (int)value.length() );

            m_applicationName = appWString.c_str();*/
            m_applicationName = L"Paper Planes";
        }
        else if(key.compare("Fullscreen") == 0)
        {
            fullscreen = (value.compare("TRUE") == 0 ? true : false);
        }
        else if(key.compare("ScreenHeight") == 0)
        {
            screenHeight = atoi(value.c_str());
        }
        else if(key.compare("ScreenWidth") == 0)
        {
            screenWidth = atoi(value.c_str());
        }
        
    }
    //----------------------------------------------------------------------------------------------

    // Setup windows class with default settings.
    WNDCLASSEX wc;
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc		= WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= m_hinstance;
    wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm			= wc.hIcon;
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName		= NULL;
    wc.lpszClassName	= m_applicationName;
    wc.cbSize			= sizeof(WNDCLASSEX);

    // Register window class.
    RegisterClassEx(&wc);

    int posX, posY;

    if(fullscreen)
    {
        // Get the display resolution.
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // Set to fullscreen.
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth	= (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight	= (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel	= 32;
        dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        posX = posY = 0;
    }
    else
    {
        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }

    // Create the window with the settings generated.
    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

    // Bring the window up on screen and set to main focus.
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    // Hide cursor.
    ShowCursor(true);

    return true;
}


void DXWindow::ShutdownWindow()
{
    ShowCursor(true);

    ChangeDisplaySettings(NULL, 0);

    // Remove Window.
    DestroyWindow(m_hwnd);
    m_hwnd = nullptr;

    // Remove Application instance.
    UnregisterClass(m_applicationName, m_hinstance);
    m_hinstance = nullptr;

    ApplicationHandle = nullptr;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    default: // Pass all other messages to the message handler in DXWindow.
        return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
    }
}
