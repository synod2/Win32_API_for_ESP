#include <windows.h>
#define IDBUTTON 102
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/* 클래스 이름을 전역 변수로 생성 */
TCHAR szClassName[] = TEXT("MyFirstProgram");
HINSTANCE g_hInst;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;

	//윈도우 구조체
	g_hInst = hInstance;
	wc.hInstance = hInstance;
	wc.lpszClassName = szClassName; //클래스 이름 
	wc.lpfnWndProc = WndProc; //호출하는 주체가 윈도우 
	wc.style = CS_DBLCLKS; //동작의 특성 지정. 더블클릭 잡아냄
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 핸들. 아이콘 지정 
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //작은아이콘 핸들. 작게 보일 아이콘 지정
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 핸들. 기본 마우스 커서 사용 
	wc.lpszMenuName = NULL; //프로그램의 메뉴 사용 안함
	wc.cbClsExtra = 0; //윈도우 클래스 뒤에 여유 바이트 없음 
	wc.cbWndExtra = 0; //윈도우 인스턴스 또는 구조체 
	//창 배경으로 윈도우 기본 색상 사용 
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	//wc를 윈도우 클래스로 등록, 실패 시 프로그램 종료
	if (!RegisterClassEx(&wc))
		return 0;

	hWnd = CreateWindow(szClassName, L"world",
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) //GetMessage의 반환값이 0이 될떄 까지 실행. 
	{
		TranslateMessage(&msg); //넘겨받은 데이터를 변환, 해석하는 역할 
		DispatchMessage(&msg); //메시지를 WndProc으로 전달, 여기에 대응하는것이 프로시저콜백 함수.
	}
	
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWndButton;
	switch (message) {
	case WM_COMMAND: {
		if ((HWND)lParam && ((HIWORD(wParam) == BN_CLICKED))) {
			int iMid;
			iMid = LOWORD(wParam);
			switch (iMid) {
			case IDBUTTON: {
				MessageBox(hWnd, TEXT("CLICK !"), TEXT("my program"), MB_OK | MB_ICONEXCLAMATION);
				break;
			}
			default:
				break;
			}

		}
		break;
	}
	case WM_CREATE: {
		hWndButton = CreateWindowEx(0,  /* 더 또는  '확장된' 모양 */
			TEXT("BUTTON"),             /* 만들고픈 GUI '클래스' */
			TEXT("날 눌러줘!"),          /* GUI 자막(Caption) */
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, /* 컨트롤 모양은 |로 분할 */
			10,                         /* 좌측 위치 (좌측으로부터의 위치) */
			10,                         /* 상단 위치 (상단으로부터의 위치) */
			200,                        /* 컨트롤의 너비 */
			30,                         /* 컨트롤의 높이 */
			hWnd,                       /* 부모 창 처리 */
			(HMENU)IDBUTTON,            /* WM_COMMAND를 위한 컨트롤의 ID */
			g_hInst,                    /* 응용프로그램 인스턴스 */
			NULL);
		break;
	}
	case WM_DESTROY: {
		MessageBox(hWnd, TEXT("destroy! "), TEXT("my program"), MB_OK | MB_ICONEXCLAMATION);
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE: {
		MessageBox(hWnd, TEXT("close!"), TEXT("my program"), MB_OK | MB_ICONEXCLAMATION);
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}