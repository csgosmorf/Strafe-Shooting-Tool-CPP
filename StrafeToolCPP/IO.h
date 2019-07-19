#pragma once
#include <Windows.h>
class IO
{
public:
	static bool keyPressed(char key) { return GetKeyState(key) & 0x8000; }
};



