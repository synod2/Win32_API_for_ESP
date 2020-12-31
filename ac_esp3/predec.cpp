#pragma once
#include <windows.h>
#include <winuser.h>
#include <string>
#include "preDec.h"



class Player {

	//private:
	//	WinFunc winFunc;

public:

	char name[20];
	int base;
	int health;
	HANDLE handle;
	//float matrix[16];
	//vec2d_f screen;
	//vec3d_f position_head;
	//vec3d_f position_feet;
	vec3d_f position;
	//int team;

	Player(HANDLE phandle)
	{
		handle = phandle;
		ReadProcessMemory(phandle, (LPCVOID)player_base, &base, sizeof(base), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posx), &position.x, sizeof(position.x), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posy), &position.y, sizeof(position.y), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posz), &position.z, sizeof(position.z), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_health), &health, sizeof(health), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_name), &name, sizeof(name), 0);
	}

	//void GetInfo();
	//void Print();
};

HANDLE getPhandle() {
	DWORD pid;
	HWND twnd = FindWindow(0, TargetName);
	int value = 0;
	HANDLE phandle = 0;
	std::wstring boxMsg = L"can not find ";
	boxMsg += TargetName;
	if (!twnd) {
		MessageBox(hWnd, boxMsg.c_str(), TEXT("my program"), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	else {
		GetWindowThreadProcessId(twnd, &pid);
		phandle = OpenProcess(PROCESS_VM_READ, 0, pid);
		if (!phandle) {
			boxMsg = L"Can Not Open Process " + std::wstring(TargetName);
			MessageBox(hWnd, boxMsg.c_str(), TEXT("my program"), MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
		else {
			return phandle;
		}
	}
	return phandle;
}