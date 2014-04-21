#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <WinSock2.h>

#include "DXWindow.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE , PSTR, int)
{
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
