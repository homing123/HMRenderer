
#include "Engine.h"

const LinearColor LinearColor::Error(1.f, 0.f, 1.f, 1.f);
const LinearColor LinearColor::White(1.f, 1.f, 1.f, 1.f);
const LinearColor LinearColor::Black(0.f, 0.f, 0.f, 1.f);
const LinearColor LinearColor::Gray(0.5f, 0.5f, 0.5f, 1.f);
const LinearColor LinearColor::Red(1.f, 0.f, 0.f, 1.f);
const LinearColor LinearColor::Green(0.f, 1.f, 0.f, 1.f);
const LinearColor LinearColor::Blue(0.f, 0.f, 1.f, 1.f);
const LinearColor LinearColor::Yellow(1.f, 1.f, 0.f, 1.f);
const LinearColor LinearColor::Magenta(1.f, 0.f, 1.f, 1.f);

constexpr int screenWidth = 800;
constexpr int screenHeight = 600;
ScreenPoint ScreenSize = ScreenPoint(screenWidth, screenHeight);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_DISPLAYCHANGE:
	case WM_SIZE:
		/*int width = lParam & 0x0000FFFF;
		int height = (lParam >> 16) & 0x0000FFFF;*/
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_KEYDOWN:
		Engine::PInstance->GetInput().ChangeState((Key)wParam, true);
		break;
	case WM_KEYUP:
		Engine::PInstance->GetInput().ChangeState((Key)wParam, false);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}


HWND ScreenCreate(HINSTANCE hInstance)
{
	const LPCWSTR lpClassName = TEXT("class");
	const LPCWSTR lpTitleName = TEXT("Title");

	WNDCLASSEX wndc;
	wndc.cbSize = sizeof(wndc);
	wndc.style = CS_HREDRAW | CS_VREDRAW;
	wndc.lpfnWndProc = WndProc;
	wndc.cbClsExtra = NULL;
	wndc.cbWndExtra = NULL;
	wndc.hInstance = hInstance;
	wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndc.lpszMenuName = NULL;
	wndc.lpszClassName = lpClassName;
	wndc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndc);

	HWND hWnd = CreateWindow(
		lpClassName,
		lpTitleName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		ScreenSize.X,
		ScreenSize.Y,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	return hWnd;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	HWND hWnd = ScreenCreate(hInstance);
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	MSG msg;

	Engine engine = Engine(ScreenSize, hWnd);
	Engine::PInstance = &engine;

	Engine::PInstance->LoadResource();
	Engine::PInstance->LoadScene();
	Engine::PInstance->Start();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Engine::PInstance->Cycle();
	}

	return msg.wParam;
}

int main() 
{
	return 0;
}

