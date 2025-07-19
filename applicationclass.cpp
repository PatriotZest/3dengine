#include "applicationclass.h"

ApplicationClass::ApplicationClass() {
	m_Direct3D = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other) {

}

ApplicationClass::~ApplicationClass() {

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;

	// create and intialize the d3d object
	m_Direct3D = new D3DClass;
	result = m_Direct3D->Intialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not intialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void ApplicationClass::Shutdown() {
	//release the Direct3D object
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

bool ApplicationClass::Frame() {
	bool result;

	//render graphics scene
	result = Render();
	if (!result) {
		return false;
	}
	return true;
}

bool ApplicationClass::Render() {
	//clear buffers to begin scene (???)
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	//present rendered scene to the screen (???)
	m_Direct3D->EndScene();
	return true;
}