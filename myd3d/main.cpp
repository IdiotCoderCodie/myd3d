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