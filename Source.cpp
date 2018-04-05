#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

WCHAR szCurrentDirectory[MAX_PATH];
WCHAR szExtendedLengthPath[1024 * 1024];

#define LONG_NAME L"\\1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton1;
	static HWND hButton2;
	switch (msg)
	{
	case WM_CREATE:
		hButton1 = CreateWindow(TEXT("BUTTON"), TEXT("長いパスのファイルを作成"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton2 = CreateWindow(TEXT("BUTTON"), TEXT("長いパスのファイルの削除"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDCANCEL, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hButton1, 10, 10, 256, 32, TRUE);
		MoveWindow(hButton2, 10, 50, 256, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			GetCurrentDirectoryW(_countof(szCurrentDirectory), szCurrentDirectory);
			lstrcpyW(szExtendedLengthPath, L"\\\\?\\");
			lstrcatW(szExtendedLengthPath, szCurrentDirectory);
			for (int i = 0; i < 80; ++i)
			{
				lstrcatW(szExtendedLengthPath, LONG_NAME);
				CreateDirectory(szExtendedLengthPath, NULL);
			}
			lstrcatW(szExtendedLengthPath, LONG_NAME);
			const HANDLE hFile = CreateFileW(szExtendedLengthPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				DWORD dwWritten;
				WriteFile(hFile, "Test", 4, &dwWritten, NULL);
				CloseHandle(hFile);
			}
			else
			{
				TCHAR szErrorMessage[1024] = { 0 };
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szErrorMessage, _countof(szErrorMessage), NULL);
				TCHAR szText[1024];
				wsprintf(szText, TEXT("ファイルの作成に失敗しました。%s"), szErrorMessage);
				MessageBox(hWnd, szText, 0, 0);
			}
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			GetCurrentDirectoryW(_countof(szCurrentDirectory), szCurrentDirectory);
			lstrcpyW(szExtendedLengthPath, L"\\\\?\\");
			lstrcatW(szExtendedLengthPath, szCurrentDirectory);
			for (int i = 0; i < 80; ++i)
			{
				lstrcatW(szExtendedLengthPath, LONG_NAME);
			}
			lstrcatW(szExtendedLengthPath, LONG_NAME);
			DeleteFile(szExtendedLengthPath);
			szExtendedLengthPath[lstrlenW(szExtendedLengthPath) - lstrlenW(LONG_NAME)] = 0;
			for (int i = 0; i < 80; ++i)
			{
				RemoveDirectory(szExtendedLengthPath);
				szExtendedLengthPath[lstrlenW(szExtendedLengthPath) - lstrlenW(LONG_NAME)] = 0;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("長いパスのファイルを作成"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
