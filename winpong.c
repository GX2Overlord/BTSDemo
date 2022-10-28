// includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// program defines
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// game variables


// operating system variables


// game function prototypes
void GetUserInput();
void RunPhysics();
void DrawPaddlesAndBall();
void UpdateScoreBoardAndReset(HWND hWnd);
//void RunRightPaddleAI();


// operating system function prototypes 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

// game function implementations


// operating system function implementations
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = L"Windows Pong in C";
	wndClass.hInstance = GetModuleHandleW(0);
	wndClass.hCursor = LoadCursorW(0, IDC_ARROW);
	wndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wndClass.hIcon = LoadIconW(0, IDI_APPLICATION);
	RegisterClassExW(&wndClass);
	RECT window_size = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, FALSE);
	HWND window = CreateWindowW(L"Windows Pong in C", L"Win Pong",
		WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left,
		window_size.bottom - window_size.top,
		NULL, NULL, GetModuleHandleW(0), 0);
	if (window) {
		ShowWindow(window, SW_SHOW);
		HDC windowDC = GetDC(window);
		SetBkColor(windowDC, 0x0);
		MSG msg; ZeroMemory(&msg, sizeof(msg));
		while (GetMessage(&msg, window, 0, 0) > 0) {
			DispatchMessage(&msg);
		}
	}
	UnregisterClassW(L"Windows Pong in C", GetModuleHandleW(0));
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CREATE: {
			SetTimer(hWnd, 1, 20, NULL);
			break;
		}
		case WM_TIMER: {
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			//render(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}



//int main() 
//{
//    printf("Hello BTS!\n");
//    system("pause");
//    return 0;
//}
