#pragma once

#include <d3d9.h>
#include <stdio.h>
#include <time.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d9.lib")

struct Sprite
{
	unsigned int width;
	unsigned int height;
	D3DCOLOR key;
	D3DCOLOR* surface;
};

struct Font
{
	int charw;
	int charh;
	int charperrow;
	D3DCOLOR* Surface;
};

void LoadBmp( const char* filename, D3DCOLOR* surface );
void LoadBmp( const char* filename, Sprite* sprite );
void LoadSprite( const char* filename, Sprite* sprite, D3DCOLOR* Surface, D3DCOLOR key );
void LoadFont( const char* filename, Font* font, D3DCOLOR* Surface, int CharWidth, int CharHeight, int CharsPerRow );


class Graphics
{
public:
	Graphics( HWND hWnd );
	~Graphics();
	void StartFrame();
	void FinishFrame();

	void DrawPixel( int x, int y, D3DCOLOR c );
	void DrawLine( int x1, int y1, int x2, int y2, D3DCOLOR c );
	void DrawSprite( float xoffset, float yoffset, Sprite* sprite, D3DCOLOR c );
	void DrawSprite( int xoffset, int yoffset, Sprite* sprite, D3DCOLOR c );
	void DrawChar( char Chars, float x, float y, Font* font, D3DCOLOR c );
	void DrawString( const char* string, float center, float y, Font* font, D3DCOLOR c );
	void DrawInteger( int integer, int x, int y, Font* font, int type );
	void DrawStringTimed( const char* string, float center, float y, Font* font, double timer, int pace, D3DCOLOR c );
private:
	IDirect3D9*				pDirect;
	IDirect3DDevice9*		pDev;
	IDirect3DSurface9*		pBuffer;
	D3DLOCKED_RECT			Rect;
};