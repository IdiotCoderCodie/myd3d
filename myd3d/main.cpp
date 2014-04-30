#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <WinSock2.h>

#include "DXWindow.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include <time.h>

unsigned int exeCode = 0;
using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE , PSTR, int)
{
    srand(time(NULL));
    // Create console for input.
    AllocConsole();

    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long)handleOut, _O_TEXT);
    FILE* hfOut = _fdopen(hCrt, "w");
    setvbuf(hfOut, NULL, _IONBF, 1);
    *stdout = *hfOut;

    HANDLE handleIn = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long)handleIn, _O_TEXT);
    FILE* hfIn = _fdopen(hCrt, "r");
    setvbuf(hfIn, NULL, _IONBF, 128);
    *stdin = *hfIn;

    cout << "Terrain Destruction Game!" << endl;
    cout << "Enter a selection: " << endl;
    cout << "[1] Player." << endl;
    cout << "[2] OffworldCam Player 1" << endl;
    cout << "[3] OffworldCam Player 2" << endl;
    cout << "[4] Cannonball Cam Player 1" << endl;
    cout << "[5] Cannonball Cam Player 2" << endl;

    cin >> exeCode;

	DXWindow* window;
	window = new DXWindow;
	// Check the window was created successfully.
	if(!window || !window->IsInitialized())
		return 0;

	window->Run();

	delete window;
	window = 0;

	return 0;
}
