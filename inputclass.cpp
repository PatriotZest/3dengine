#include "inputclass.h"

InputClass::InputClass() {

}

InputClass::~InputClass() {

}

void InputClass::Initialize() {
	int i;

	//intialize all keys to being released and not pressed
	for (i = 0; i < 256; i++){
		m_keys[i] = false;
	}
	return;
}

void InputClass::KeyDown(unsigned int input) {
	//if a key is pressed, save state in key array;
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input) {
	//key released then clear state in key array
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key) {
	//return what state the key is in
	return m_keys[key];
}