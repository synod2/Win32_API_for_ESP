#include <windows.h>
#include <winuser.h>
#include <string>
#define IDBUTTON 102

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void SetWindow();
HANDLE getPhandle();
void getTimer();
void drawRect();

int gw=0,gh=0;

TCHAR szClassName[] = TEXT("MyFirstProgram");
LPCWSTR TargetName= L"지뢰 찾기";
HINSTANCE g_hInst;
HWND hWnd;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MSG msg;
	WNDCLASSEX wc;
	
	RECT tRect;
	HWND tWnd;

	int wWidth, wHeight;

	//윈도우 구조체
	g_hInst = hInstance;
	wc.hInstance = hInstance;
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

	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	//wc를 윈도우 클래스로 등록, 실패 시 프로그램 종료
	if (!RegisterClassEx(&wc))
		return 0;
	/*HWND hWnd = CreateWindowEx(WS_EX_TOPMOST , szClassName, L"world",
		0, 100, 90, wWidth, wHeight, NULL, NULL, hInstance, NULL);*/
	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT, szClassName, L"world",
		WS_POPUP, 100, 90, wWidth, wHeight, NULL, NULL, hInstance, NULL);

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindow, NULL, 0, 0);


	while (GetMessage(&msg, NULL, 0, 0)) //GetMessage의 반환값이 0이 될떄 까지 실행. 
	{
		TranslateMessage(&msg); //넘겨받은 데이터를 변환, 해석하는 역할 
		DispatchMessage(&msg); //메시지를 WndProc으로 전달, 여기에 대응하는것이 프로시저콜백 함수.
	}

	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWndButton;
	switch (message) {
	case WM_PAINT: {
		drawRect();				
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
	int wWidth, wHeight;

	while(true){
		tWnd = FindWindow(0, TargetName);
		getTimer();
		if (tWnd) {
			GetWindowRect(tWnd, &tRect);
			wWidth = tRect.right - tRect.left;
			wHeight = tRect.bottom - tRect.top;
			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER) {
				tRect.top += 23;
				wHeight -= 23;
			}
			MoveWindow(hWnd, tRect.left, tRect.top, wWidth, wHeight, true);
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

void getTimer() {
	int value = 0;
	DWORD address = 0x0100579C; //timer
	HANDLE phandle = getPhandle();
	ReadProcessMemory(phandle, (void*)address, &value, sizeof(value), 0);
	if (value == 0) {
		printf("reset");
		InvalidateRect(hWnd, NULL, TRUE);
		Sleep(1000);
	}
}

void drawRect() {
	RECT  rect;
	PAINTSTRUCT ps;
	wchar_t tmp[256];
	byte mat[100][100];

	HANDLE phandle = getPhandle();
	HDC hdc = BeginPaint(hWnd, &ps);
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));

	DWORD waddr = 0x01005334, haddr = 0x01005338;
	DWORD maddr = 0x01005361; //mine start addr
	int w = 0, h = 0;

	ReadProcessMemory(phandle, (void*)waddr, &w, sizeof(w), 0);
	ReadProcessMemory(phandle, (void*)haddr, &h, sizeof(h), 0);
		
	gw = w;
	gh = h;

	byte val = 0;
	DWORD taddr = 0;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			taddr = maddr + j + i*32;
			ReadProcessMemory(phandle, (void*)taddr, &val, sizeof(val), 0);
			printf("%x\n", val);
			mat[i][j] = val;
		}
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (mat[i][j] == 0x8f) {
				rect.left = (j * 16) + 15;
				rect.top = (i * 16) + 78;
				rect.right = ((j + 1) * 16) + 15;
				rect.bottom = ((i + 1) * 16) + 78;
				FrameRect(hdc, &rect, redBrush);
			}
		}
	}

	EndPaint(hWnd, &ps);
}