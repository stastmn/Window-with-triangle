#include "Task.h"


static const WCHAR* g_WindowClass = L"wndClass";
static const char* g_Title = "Test";
static Graphics* g_Graphics;
static const int g_Width = 800;
static const int g_Height = 600;
static const COLORREF g_backGroundCol = 0x004b8525;
static const int FAILED_REGISTER_WINDOW_ERROR = 2;
static const int FAILED_CREATING_WINDOW_ERROR = 1;


int RunApplication(_In_ HINSTANCE handleInstance,
	_In_opt_ HINSTANCE handlePreviousInstance,
	_In_ LPSTR     args,
	_In_ int       numberOfArgs)
{
	auto registerWindowErr = TryRegisterWindowFrom(handleInstance);
	if (!registerWindowErr)
	{
		ShowMessageBox("Failed to register window.", "Error");
		return FAILED_REGISTER_WINDOW_ERROR;
	}

	auto createWindowErr = TryCreateWindowFrom(handleInstance, numberOfArgs);
	if (!createWindowErr)
	{
		ShowMessageBox("Failed to create window.", "Error");
		return FAILED_CREATING_WINDOW_ERROR;
	}

	MSG message;
	RunWindowLoop(&message);

	delete g_Graphics;
	return (int)message.wParam;
}

ATOM TryRegisterWindowFrom(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcex.hbrBackground = CreateSolidBrush(g_backGroundCol);

	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_WindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	return RegisterClassExW(&wcex);
}


HWND CreateWindowFrom(HINSTANCE hInstance) {
	return CreateWindowW(g_WindowClass, (WCHAR*)g_Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, g_Width, g_Height, nullptr, nullptr, hInstance, nullptr);
}

void HandleWindowResize(int x, int y, HWND hwnd, RECT rect)
{
	if (x == -1) return;
	GetClientRect(hwnd, &rect);
	g_Graphics->Resize(rect);

	g_Graphics->BeginDraw();
	g_Graphics->ClearScreen(g_backGroundCol);
	g_Graphics->DrawTriangle(static_cast<float>(x), static_cast<float>(y));

	g_Graphics->EndDraw();
	return;
}

void HandleMouseLeftClick(int * x, int * y, LPARAM lparam)
{
	*x = LOWORD(lparam);
	*y = HIWORD(lparam);

	g_Graphics->BeginDraw();
	g_Graphics->ClearScreen(g_backGroundCol);
	g_Graphics->DrawTriangle(static_cast<float>(*x), static_cast<float>(*y));
	g_Graphics->EndDraw();
}

void HandleMouseIsClickedAndMoving(int * x, int * y, LPARAM lparam, WPARAM wparam)
{
	if (!(wparam && MK_LBUTTON)) return;

	*x = LOWORD(lparam);
	*y = HIWORD(lparam);

	g_Graphics->BeginDraw();
	g_Graphics->ClearScreen(g_backGroundCol);
	g_Graphics->DrawTriangle(static_cast<float>(*x), static_cast<float>(*y));
	g_Graphics->EndDraw();

}


void ShowMessageBox(const char* message, const char* header)
{
	MessageBox(NULL, _T(message), _T(header), NULL);
}

void RunWindowLoop(MSG* message)
{
	while (GetMessage(message, nullptr, 0, 0))
	{
		TranslateMessage(message);
		DispatchMessage(message);
	}
}

BOOL TryCreateWindowFrom(HINSTANCE hInstance, int numberOfArgs)
{
	HWND hWnd = CreateWindowFrom(hInstance);
	if (!hWnd)	return false;

	g_Graphics = new Graphics();
	if (!g_Graphics->Init(hWnd))
	{
		delete g_Graphics;
		return false;
	}

	ShowWindow(hWnd, numberOfArgs);
	UpdateWindow(hWnd);

	return true;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int x(-1), y(-1);
	RECT rect = { 0, 0, 0, 0 };
	switch (message)
	{
	case WM_SIZE:
		HandleWindowResize(x, y, hWnd, rect);
		break;
	case WM_LBUTTONDOWN:
		HandleMouseLeftClick(&x, &y, lParam);
		break;
	case WM_MOUSEMOVE:
		HandleMouseIsClickedAndMoving(&x, &y, lParam, wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}



