// ShowDib8.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ShowDib8.h"
#include "PackedDib.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SHOWDIB8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHOWDIB8));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHOWDIB8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SHOWDIB8);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBitmap;
	static HPALETTE hPalette;
	static int cxClient, cyClient;
	static OPENFILENAME ofn;
	static PBYTE pBits;
	static TCHAR szFileName[MAX_PATH], szTitleName[MAX_PATH];
	static TCHAR szFilter[] = TEXT("Bitmap Files (*.BMP)\0*.bmp\0")
		TEXT("All Files (*.*)\0*.*\0\0");
	BITMAP bitmap;
	BITMAPINFO * pPackedDib;
	HDC hdc, hdcMem;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hwnd;
		ofn.hInstance = NULL;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFileTitle = szTitleName;
		ofn.nMaxFileTitle = MAX_PATH;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = NULL;
		ofn.Flags = 0;
		ofn.nFileOffset = 0;
		ofn.nFileExtension = 0;
		ofn.lpstrDefExt = TEXT("bmp");
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_OPEN:
			// Show the File Open dialog box
			if (!GetOpenFileName(&ofn))
				return 0;

			// If there's an existing packed DIB, free the memory
			if (hBitmap)
			{
				DeleteObject(hBitmap);
				hBitmap = NULL;
			}
			// If there's an existing logical palette, delete it
			if (hPalette)
			{
				DeleteObject(hPalette);
				hPalette = NULL;
			}
			// Load the packed DIB into memory
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			ShowCursor(TRUE);
			pPackedDib = PackedDibLoad(szFileName);
			ShowCursor(FALSE);
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			if (pPackedDib)
			{
				// Create the DIB section from the DIB
				hBitmap = CreateDIBSection(NULL, pPackedDib, DIB_RGB_COLORS, (void **)&pBits, NULL, 0);
				// Copy the bits
				CopyMemory(pBits, PackedDibGetBitsPtr(pPackedDib),
					PackedDibGetBitsSize(pPackedDib));
				// Create palette from the DIB
				hPalette = PackedDibCreatePalette(pPackedDib);
				// Free the packed-DIB memory
				free(pPackedDib);
			}
			else
			{
				MessageBox(hwnd, TEXT("Cannot load DIB file"), szWindowClass, 0);
			}
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (hPalette)
		{
			SelectPalette(hdc, hPalette, FALSE);
			RealizePalette(hdc);
		}
		if (hBitmap)
		{
			GetObject(hBitmap, sizeof(BITMAP),
				&bitmap);
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
				hdcMem, 0, 0, SRCCOPY);
			DeleteDC(hdcMem);
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_QUERYNEWPALETTE:
		if (!hPalette)
			return FALSE;
		hdc = GetDC(hwnd);
		SelectPalette(hdc, hPalette, FALSE);
		RealizePalette(hdc);
		InvalidateRect(hwnd, NULL, TRUE);
		ReleaseDC(hwnd, hdc);
		return TRUE;
	case WM_PALETTECHANGED:
		if (!hPalette || (HWND)wParam == hwnd)
			break;
		hdc = GetDC(hwnd);
		SelectPalette(hdc, hPalette, FALSE);
		RealizePalette(hdc);
		UpdateColors(hdc);
		ReleaseDC(hwnd, hdc);
		break;

	case WM_DESTROY:
		if (hBitmap)
			DeleteObject(hBitmap);
		if (hPalette)
			DeleteObject(hPalette);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
