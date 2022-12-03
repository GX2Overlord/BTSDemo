// includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

// program defines
#define SCREEN_WIDTH  1600
#define SCREEN_HEIGHT 900
#define PADDLE_WIDTH  50
#define PADDLE_HEIGHT 300
#define PADDLE_GAP    100
#define PADDLE_SPEED  25
#define BALL_RADIUS   50
#define BALL_SPEEDH   10
#define BALL_SPEEDV   5
#define BALL_SPEEDMIN 5
#define BALL_SPEEDMAX 20
#define BALL_BOOST    1

// game variables
int scoreLeftPlayer = 0;
int scoreRightPlayer = 0;
const RECT leftPaddleStart = { 
	PADDLE_GAP, 
	SCREEN_HEIGHT / 2 - PADDLE_HEIGHT /2 , 
	PADDLE_GAP + PADDLE_WIDTH,
	SCREEN_HEIGHT / 2 + PADDLE_HEIGHT / 2
};
const RECT rightPaddleStart = {
	SCREEN_WIDTH - PADDLE_GAP - PADDLE_WIDTH,
	SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2 ,
	SCREEN_WIDTH - PADDLE_GAP,
	SCREEN_HEIGHT / 2 + PADDLE_HEIGHT / 2
};
const POINT ballStart = {
	SCREEN_WIDTH / 2,
	SCREEN_HEIGHT / 2,
};
RECT  leftPaddle;
RECT  rightPaddle;
POINT ball;
POINT ballVelocity;

// operating system variables
PAINTSTRUCT ps;
HDC hdc;
HDC hdcMem;
HBITMAP hbmMem;
HDC hOld;

// game function prototypes
void GetUserInput(HWND hWnd);
void RunPhysicsAndGameLogic(HWND hWnd);
void DrawPaddlesAndBall(HWND hWnd, HDC hdc);
void UpdateScoreBoardAndReset(HWND hWnd);
void RunPaddleAI();

// operating system function prototypes 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow);

// game function implementations
void GetUserInput(HWND hWnd) {
	if (GetFocus() == hWnd) {
		if (GetAsyncKeyState(VK_UP) && rightPaddle.top > 0) {
			rightPaddle.top -= PADDLE_SPEED;
			rightPaddle.bottom -= PADDLE_SPEED;
		}
		if (GetAsyncKeyState(VK_DOWN) && rightPaddle.bottom < SCREEN_HEIGHT) {
			rightPaddle.top += PADDLE_SPEED;
			rightPaddle.bottom += PADDLE_SPEED;
		}
		if (GetAsyncKeyState('W') && leftPaddle.top > 0) {
			leftPaddle.top -= PADDLE_SPEED;
			leftPaddle.bottom -= PADDLE_SPEED;
		}
		if (GetAsyncKeyState('S') && leftPaddle.bottom < SCREEN_HEIGHT) {
			leftPaddle.top += PADDLE_SPEED;
			leftPaddle.bottom += PADDLE_SPEED;
		}
		if (GetAsyncKeyState('R')) {
			scoreLeftPlayer = 0;
			scoreRightPlayer = 0;
			UpdateScoreBoardAndReset(hWnd);
		}
	}
}
void RunPhysicsAndGameLogic(HWND hWnd) {
	RECT result;
	RECT ballRectangle = { ball.x - BALL_RADIUS, ball.y - BALL_RADIUS, ball.x + BALL_RADIUS, ball.y + BALL_RADIUS };
	ball.x += ballVelocity.x;
	ball.y += ballVelocity.y;
	if (ballRectangle.top < 0) {
		ballVelocity.y = abs(ballVelocity.y) + BALL_BOOST;
	}
	if (ballRectangle.bottom > SCREEN_HEIGHT) {
		ballVelocity.y = -abs(ballVelocity.y) - BALL_BOOST;
	}
	if (IntersectRect(&result, &ballRectangle, &leftPaddle)) {
		if (ball.y < leftPaddle.top)
			ballVelocity.y = -abs(ballVelocity.y) - BALL_BOOST;
		else if (ball.y > leftPaddle.bottom)
			ballVelocity.y = abs(ballVelocity.y) + BALL_BOOST;
		else
			ballVelocity.x = abs(ballVelocity.x) + BALL_BOOST;
	}
	if (IntersectRect(&result, &ballRectangle, &rightPaddle)) {
		if (ball.y < rightPaddle.top)
			ballVelocity.y = -abs(ballVelocity.y) - BALL_BOOST;
		else if (ball.y > rightPaddle.bottom)
			ballVelocity.y = abs(ballVelocity.y) + BALL_BOOST;
		else
			ballVelocity.x = -abs(ballVelocity.x) - BALL_BOOST;
	}
	if (ballRectangle.left > SCREEN_WIDTH) {
		scoreLeftPlayer += 1;
		UpdateScoreBoardAndReset(hWnd);
	}
	if (ballRectangle.right < 0) {
		scoreRightPlayer += 1;
		UpdateScoreBoardAndReset(hWnd);
	}
}
void DrawPaddlesAndBall(HWND hWnd, HDC hdc) {
	RECT fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HBRUSH blackbrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH greenbrush = CreateSolidBrush(RGB(100, 200, 100));
	HBRUSH bluebrush = CreateSolidBrush(RGB(100, 100, 200));

	FillRect(hdc, &fullscreen, blackbrush);
	FillRect(hdc, &leftPaddle, greenbrush);
	FillRect(hdc, &rightPaddle, bluebrush);
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(200, 100, 0));
	Ellipse(hdc, ball.x - BALL_RADIUS, ball.y - BALL_RADIUS, ball.x + BALL_RADIUS, ball.y + BALL_RADIUS);	

	DeleteObject(greenbrush);
	DeleteObject(bluebrush);
	DeleteObject(blackbrush);
}
void UpdateScoreBoardAndReset(HWND hWnd) {
	char titlebar[512] = { 0, };
	sprintf(titlebar,
		"Let's Play Windows Pong!      "
		"[Controls: use \"W\" \"S\" and"
		"\"Up\" \"Down\" to play]      "
		"Left Player Score: %i         "
		"Right Player Score: %i        "
		"Press the \"R\" key to reset! ",
		scoreLeftPlayer, scoreRightPlayer);
	SetWindowTextA(hWnd, titlebar);
	leftPaddle = leftPaddleStart;
	rightPaddle = rightPaddleStart;
	ball = ballStart;
	srand(time(0));
	ballVelocity.x = rand() % BALL_SPEEDH + BALL_SPEEDMIN;
	ballVelocity.y = rand() % BALL_SPEEDV + BALL_SPEEDMIN;
	if (rand() % 2)
		ballVelocity.x = -ballVelocity.x;
	if (rand() % 2)
		ballVelocity.y = -ballVelocity.y;
}
void RunPaddleAI() {
	// the AI programmer just went on vacation... oh no! what do we do!?

}

// operating system function implementations
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
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
		int xPos = (GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2;
		int yPos = (GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2;
		SetWindowPos(window, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		ShowWindow(window, SW_SHOW);
		hdc = BeginPaint(window, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		hbmMem = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
		hOld = SelectObject(hdcMem, hbmMem);
		UpdateScoreBoardAndReset(window);
		MSG msg; ZeroMemory(&msg, sizeof(msg));
		while (GetMessage(&msg, window, 0, 0) > 0) {
			DispatchMessage(&msg);
		}
		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);
		EndPaint(window, &ps);
	}
	UnregisterClassW(L"Windows Pong in C", GetModuleHandleW(0));
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CREATE: {
			SetTimer(hWnd, 1, 16, NULL); // ~60hz refresh
			break;
		}
		case WM_TIMER: {
			GetUserInput(hWnd);
			RunPaddleAI();
			RunPhysicsAndGameLogic(hWnd);
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_PAINT: {
			DrawPaddlesAndBall(hWnd, hdcMem);
			BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdcMem, 0, 0, SRCCOPY);
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}
// Top secret AI code!
//if (rightPaddle.top < ball.y) {
//	rightPaddle.top += PADDLE_SPEED;
//	rightPaddle.bottom += PADDLE_SPEED;
//}
//if (rightPaddle.bottom > ball.y) {
//	rightPaddle.top -= PADDLE_SPEED;
//	rightPaddle.bottom -= PADDLE_SPEED;
//}