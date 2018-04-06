#include <Windows.h>
#include <wchar.h>
#include "Resource.h"
#include "App.h"

static KeyboardServer kServer;

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_UP:
			kServer.UpPress();
			break;
		case VK_DOWN:
			kServer.DownPress();
			break;
		case VK_LEFT:
			kServer.LeftPress();
			break;
		case VK_RIGHT:
			kServer.RightPress();
			break;
		case VK_SPACE:
			kServer.SpacePress();
			break;
		case VK_RETURN:
			kServer.EnterPress();
			break;
		}
		break;
	case WM_KEYUP:
		switch( wParam )
		{
		case VK_UP:
			kServer.UpRelease();
			break;
		case VK_DOWN:
			kServer.DownRelease();
			break;
		case VK_LEFT:
			kServer.LeftRelease();
			break;
		case VK_RIGHT:
			kServer.RightRelease();
			break;
		case VK_SPACE:
			kServer.SpaceRelease();
			break;
		case VK_RETURN:
			kServer.EnterRelease();
			break;
		}
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPWSTR, INT )
{
	WNDCLASSEX wc = { sizeof( WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
					  GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
					  "Space Invaders Window", NULL };
	wc.style = CS_HREDRAW;
	wc.hIconSm = LoadIcon( hInstance, ( LPCSTR )IDI_ICON );
	wc.hIcon = LoadIcon( hInstance, ( LPCSTR )IDI_ICON );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	RegisterClassEx( &wc );

	RECT wr;
	wr.left = 10;
	wr.right = 1010;
	wr.top = 30;
	wr.bottom = 730;
	AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, FALSE );
	HWND hWnd = CreateWindowW(  L"Space Invaders Window", L"Space Invaders",
								WS_OVERLAPPEDWINDOW, 0, 0, wr.right-wr.left, wr.bottom-wr.top,
								NULL, NULL, wc.hInstance, NULL );
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	App App( hWnd, kServer );

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	while ( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			App.UpdateFrame();
		}
	}

	UnregisterClass( "Space Invaders Window", wc.hInstance );
	return 0;
}