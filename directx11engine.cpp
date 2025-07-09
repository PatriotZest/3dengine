#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	SystemClass* System;
	bool result;

	//system object
	System = new SystemClass;

	//initialize and run
	result = System->Initialize();
	if (result) {
		System->Run();
	}
	
	//release memory
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
