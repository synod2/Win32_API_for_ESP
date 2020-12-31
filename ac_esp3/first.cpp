#include <windows.h>
#include <winuser.h>
#include <string>
#include <iostream>
#include <sstream>
#include "preDec.h"
#include <ctime>
#include <math.h>

#pragma comment(lib,"Msimg32.lib")
#define PI 3.1415926535

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

/*
----------------------------------main에서의 함수들---------------------------------															
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void SetWindow();
bool initWindow(WNDPROC wndproc);
void exitWindow();
bool initGL();
void updateGL();
void paintGL();
void showInfo(HDC hDC);
void getPlayerInfo(int idx);

/*
----------------------------------WndProc에서의 함수들---------------------------------
*/
void wmClose();
void wmSize(int x, int y);
void wmKeyDown(WPARAM key);

/*
---------------------------------- 전역변수 ---------------------------------
*/

LPCWSTR TargetName = L"AssaultCube";
HWND hWnd;
HDC bhDC;
HGLRC hRC;
HANDLE getPhandle();
TCHAR szClassName[] = TEXT("MyFirstProgram");
Player userPlayer = Player(getPhandle());
Player otherPlayer[32];
HINSTANCE g_hInst;
SIZE g_size;
int gWidth, gHeight;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MSG msg;
	
	bool isRun = initWindow(WndProc);
	//getPlayerInfo(0);
	while (isRun) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				isRun = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			time_t now = time(nullptr);

			updateGL();
			paintGL();
			SwapBuffers(bhDC);

			Sleep(DWORD(1000 / 60 - time(nullptr) + now)); //초당 60 프레임이 나오게 한다.
		}
	
	}
	
	exitWindow();
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		static HDC hdc, MemDC;
		static HBITMAP BackBit, oldBackBit;
		static RECT bufferRT;
		MemDC = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);

		GetClientRect(hWnd, &bufferRT);
		hdc = CreateCompatibleDC(MemDC);
		BackBit = CreateCompatibleBitmap(MemDC, bufferRT.right, bufferRT.bottom);
		oldBackBit = (HBITMAP)SelectObject(hdc, BackBit);

		showInfo(hdc);

		GetClientRect(hWnd, &bufferRT);
		/*BitBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, hdc, 0, 0, SRCCOPY);*/
		TransparentBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom,
			hdc, 0, 0, bufferRT.right, bufferRT.bottom, RGB(255, 255, 255));
		SelectObject(hdc, oldBackBit);
		DeleteObject(BackBit);
		DeleteDC(hdc);
		EndPaint(hWnd, &ps);
		break;

	}
	case WM_CLOSE: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void SetWindow() {
	HWND tWnd;
	RECT tRect;
	RECT wRect;
	int wWidth, wHeight;
	int margin = 4;

	while (true) {
		tWnd = FindWindow(0, TargetName);
		if (tWnd) {
			GetWindowRect(tWnd, &tRect);
			wRect.left = tRect.left - margin;
			wRect.top = tRect.top - margin;
			wWidth = tRect.right - wRect.left + margin;
			wHeight = tRect.bottom - wRect.top + margin;
			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			MoveWindow(hWnd, wRect.left, wRect.top, wWidth, wHeight, true);
		}
		else {
			exit(1);
		}
	}
}


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

void showInfo(HDC hDC) {
	std::stringstream buffer;
	int x = 10, y = 50;
	int i = 10, j = 10;
	int pNumber = 0;
	RECT rect;
	ReadProcessMemory(getPhandle(), (LPCVOID)players_in_map, &pNumber, sizeof(pNumber), 0);

	

	SetTextColor(hDC, RGB(0xFF, 0x00, 0x00));
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));

	buffer << "player shield : " << userPlayer.armor;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};
	/*

	buffer << "player posy : " << userPlayer.position.y;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};

	buffer << "player posz : " << userPlayer.position.z;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};
	
	buffer << "player angle lr : " << userPlayer.screen.x;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};

	buffer << "player angle ud : " << userPlayer.screen.y;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};*/

	/*buffer << "ud angle  : " << udA;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};


	buffer << "player name : " << otherPlayer[idx].name;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};

	buffer << "player health : " << otherPlayer[idx].health;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};

	buffer << "player posx : " << otherPlayer[idx].position.x;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};

	buffer << "player posy : " << otherPlayer[idx].position.y;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};

	buffer << "player posz : " << otherPlayer[idx].position.z;
	TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
	y += 15;
	buffer = {};*/
	
	double dist,udA,lrA,lrabs,udabs;
	for (int idx = 0; idx != pNumber-1; idx++) {
		getPlayerInfo(idx);
		lrA = atan2(userPlayer.position.y - otherPlayer[idx].position.y,
			userPlayer.position.x - otherPlayer[idx].position.x) * 180 / PI;

		lrA += 270;
		if (lrA > 360)
			lrA -= 360;

		/*buffer << "lr angle  : " << lrA;
		TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
		y += 15;
		buffer = {};*/

		dist = sqrt(powf(userPlayer.position.x - otherPlayer[idx].position.x, 2) +
			powf(userPlayer.position.y - otherPlayer[idx].position.y, 2));

		udA = atan2(userPlayer.position.z - otherPlayer[idx].position.z,
			dist) * 180 / PI;
		udA = udA * -1;

		/*buffer << "player number : " << pNumber;
		TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
		y += 15;
		buffer = {};*/

		lrabs = fabs(lrA - userPlayer.screen.x);
		udabs = fabs(udA - userPlayer.screen.y);

		/*buffer << "lrabs : " << lrabs;
		TextOutA(hDC, x, y, buffer.str().c_str(), strlen(buffer.str().c_str()));
		y += 15;
		buffer = {};*/

		if (lrabs <= 45.0) {
			rect.left = ((lrA - userPlayer.screen.x) * gWidth / 90) + gWidth / 2;
			rect.top = ((userPlayer.screen.y - udA) * gHeight / 90) + gHeight / 2;
			rect.right = rect.left + 15;
			rect.bottom = rect.top + 15;
			FrameRect(hDC, &rect, redBrush);

		}
		otherPlayer[idx].update();
	}
	userPlayer.update();
	InvalidateRect(hWnd, NULL, TRUE);
}

void getPlayerInfo(int idx) {
	otherPlayer[idx] = Player(getPhandle(),idx);
}

bool initWindow(WNDPROC wndproc)
{
	WNDCLASSEX wc;

	RECT tRect;
	HWND tWnd;
	int wWidth, wHeight;

	//윈도우 인스턴스
	g_hInst = GetModuleHandle(nullptr);

	//윈도우 구조체
	wc.hInstance = g_hInst;
	wc.lpszClassName = szClassName; //클래스 이름 
	wc.lpszMenuName = NULL; //프로그램의 메뉴 사용 안함
	wc.lpfnWndProc = WndProc; //호출하는 주체가 윈도우 
	wc.style = CS_DBLCLKS; //동작의 특성 지정. 더블클릭 잡아냄
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 핸들. 아이콘 지정 
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //작은아이콘 핸들. 작게 보일 아이콘 지정
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 핸들. 기본 마우스 커서 사용 
	wc.cbClsExtra = NULL; //윈도우 클래스 뒤에 여유 바이트 없음 
	wc.cbWndExtra = NULL; //윈도우 인스턴스 또는 구조체 

	//창 배경으로 윈도우 기본 색상 


	tWnd = FindWindow(0, TargetName);
	GetWindowRect(tWnd, &tRect);
	wWidth = tRect.right - tRect.left;
	wHeight = tRect.bottom - tRect.top;
	if (!tWnd) {
		wWidth = 400;
		wHeight = 350;
	}
	gWidth = wWidth;
	gHeight = wHeight;

	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	//wc를 윈도우 클래스로 등록, 실패 시 프로그램 종료
	if (!RegisterClassEx(&wc))
		return 0;
	/*hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT, szClassName, L"myesp",
		NULL, 100, 90, wWidth, wHeight, NULL, NULL, g_hInst, NULL);*/

	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT, szClassName, L"world",
		WS_POPUP, 100, 90, wWidth, wHeight, NULL, NULL, g_hInst, NULL);

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindow, NULL, 0, 0);

	//device context
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	bhDC = GetDC(hWnd);
	GLuint pfm = ChoosePixelFormat(bhDC, &pfd);
	SetPixelFormat(bhDC, pfm, &pfd);
	hRC = wglCreateContext(bhDC);
	wglMakeCurrent(bhDC, hRC);

	if (!initGL())
		return false;

	return true;
}

void exitWindow()
{
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, bhDC);
	DestroyWindow(hWnd);
	UnregisterClass(szClassName, g_hInst);
}

bool initGL()
{
	//초기화
	glewInit();
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	wmSize(g_size.cx, g_size.cy);

	//성공
	return true;
}

void updateGL()
{

}
void paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void wmClose()
{
	PostQuitMessage(0);
}
void wmSize(int width, int height)
{
	g_size.cx = width;
	g_size.cy = height;

	glViewport(0, 0, g_size.cx, g_size.cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, double(g_size.cx) / double(g_size.cy), 0.001, 1000.0);
}
void wmKeyDown(WPARAM key)
{
	if (key == VK_ESCAPE)
		PostQuitMessage(0);
}