#include <windows.h>
#define IDBUTTON 102
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/* Ŭ���� �̸��� ���� ������ ���� */
TCHAR szClassName[] = TEXT("MyFirstProgram");
HINSTANCE g_hInst;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;

	//������ ����ü
	g_hInst = hInstance;
	wc.hInstance = hInstance;
	wc.lpszClassName = szClassName; //Ŭ���� �̸� 
	wc.lpfnWndProc = WndProc; //ȣ���ϴ� ��ü�� ������ 
	wc.style = CS_DBLCLKS; //������ Ư�� ����. ����Ŭ�� ��Ƴ�
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ �ڵ�. ������ ���� 
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //���������� �ڵ�. �۰� ���� ������ ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� �ڵ�. �⺻ ���콺 Ŀ�� ��� 
	wc.lpszMenuName = NULL; //���α׷��� �޴� ��� ����
	wc.cbClsExtra = 0; //������ Ŭ���� �ڿ� ���� ����Ʈ ���� 
	wc.cbWndExtra = 0; //������ �ν��Ͻ� �Ǵ� ����ü 
	//â ������� ������ �⺻ ���� ��� 
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	//wc�� ������ Ŭ������ ���, ���� �� ���α׷� ����
	if (!RegisterClassEx(&wc))
		return 0;

	hWnd = CreateWindow(szClassName, L"world",
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) //GetMessage�� ��ȯ���� 0�� �ɋ� ���� ����. 
	{
		TranslateMessage(&msg); //�Ѱܹ��� �����͸� ��ȯ, �ؼ��ϴ� ���� 
		DispatchMessage(&msg); //�޽����� WndProc���� ����, ���⿡ �����ϴ°��� ���ν����ݹ� �Լ�.
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
		hWndButton = CreateWindowEx(0,  /* �� �Ǵ�  'Ȯ���' ��� */
			TEXT("BUTTON"),             /* ������� GUI 'Ŭ����' */
			TEXT("�� ������!"),          /* GUI �ڸ�(Caption) */
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, /* ��Ʈ�� ����� |�� ���� */
			10,                         /* ���� ��ġ (�������κ����� ��ġ) */
			10,                         /* ��� ��ġ (������κ����� ��ġ) */
			200,                        /* ��Ʈ���� �ʺ� */
			30,                         /* ��Ʈ���� ���� */
			hWnd,                       /* �θ� â ó�� */
			(HMENU)IDBUTTON,            /* WM_COMMAND�� ���� ��Ʈ���� ID */
			g_hInst,                    /* �������α׷� �ν��Ͻ� */
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