#include "systemclass.h"

SystemClass::SystemClass(){
	m_Input = 0;
	m_Application = 0;
}

SystemClass::SystemClass(const SystemClass& other) {

}

SystemClass::~SystemClass(){}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// width and height of the screen is zeroed before variable is sent to function
	screenWidth = 0;
	screenHeight = 0;

	// winapi
	InitializeWindows(screenWidth, screenHeight);

	//input object for keyboard input
	m_Input = new InputClass;
	m_Input->Initialize();

	//this object will handle rendering of the graphics
	m_Application = new ApplicationClass;

	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) {
		return false;
	}
	return true;

}

void SystemClass::Shutdown() {
	//delete
	if (m_Application) {
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	if (m_Input) {
		delete m_Input;
		m_Input = 0;

	}
	ShutdownWindows();
	return;
}

void SystemClass::Run() {
	MSG msg;
	bool done, result;

	//intialize message struct
	ZeroMemory(&msg, sizeof(MSG));

	//loop until quit
	done = false;
	while (!done) {
		//windows message handler
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//if exit
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			result = Frame();
			if (!result) {
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame() {
	bool result;

	//exit input for app
	if (m_Input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	//do frame process for class object
	result = m_Application->Frame();
	if (!result) {
		return false;
	}
	return true;

}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
		//check if a key is pressed on the keyboard
	case WM_KEYDOWN:
	{
		//pressed key state sent to input object
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:
	{
		//released key state to be sent to input object
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//external pointer to object
	ApplicationHandle = this;

	//Get instance (?) wtf that mean idk gng
	m_hinstance = GetModuleHandle(NULL);


	//Name of application
	m_applicationName = L"Engine";

	//Setup windows class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//reg wind class;
	RegisterClassEx(&wc);

	//determine the resolution of clients dekstop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//screen settings based on full screen or windowed
	if(FULL_SCREEN) {
		//if full screen set to max size of users desktop and 32 bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//change to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//set position
		posX = posY = 0;



	}
	else {
		//resolution
		screenWidth = 800;
		screenHeight = 600;

		//place window in middle
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}
	//create window with screen settings and get handle
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	//main focus the window
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//hide the cursor
	ShowCursor(false);

	return;

}

void SystemClass::ShutdownWindows()
{
	//show mouse cursor
	ShowCursor(true);

	//fix display if full screen
	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);

	}

	//remove window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//remove app instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//release pointer
	ApplicationHandle = NULL;
	return;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) {
		//destroy check
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}

	}
}
