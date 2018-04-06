#include "Graphics.h"

void LoadSprite( const char* filename, Sprite* sprite, D3DCOLOR* Surface, D3DCOLOR key )
{
	sprite->surface = Surface;
	sprite->key = key;
	LoadBmp( filename, sprite );
}

void LoadFont( const char* filename, Font* font, D3DCOLOR* Surface, int CharWidth, int CharHeight, int CharsPerRow )
{
	LoadBmp( filename, Surface );
	font->charw = CharWidth;
	font->charh = CharHeight;
	font->charperrow = CharsPerRow;
	font->Surface = Surface;
}

Graphics::Graphics( HWND hWnd )
{
	Rect.pBits = NULL;

	pDirect = Direct3DCreate9( D3D_SDK_VERSION );

	D3DPRESENT_PARAMETERS d3d;
	ZeroMemory( &d3d, sizeof( d3d ) );
	d3d.Windowed = TRUE;
	d3d.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3d.BackBufferFormat = D3DFMT_UNKNOWN;
	d3d.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3d.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	
	pDirect->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | 
		D3DCREATE_PUREDEVICE, &d3d, &pDev );
	pDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBuffer );
}

Graphics::~Graphics()
{
	if( pDev )
	{
		pDev->Release();
		pDev = NULL;
	}
	if( pDirect )
	{
		pDirect->Release();
		pDirect = NULL;
	}
	if( pBuffer )
	{
		pBuffer->Release();
		pBuffer = NULL;
	}
}

void Graphics::StartFrame()
{
	pDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0,0,0 ), 0.0, 0 );
	pBuffer->LockRect( &Rect, NULL, NULL );
}

void Graphics::FinishFrame()
{
	pBuffer->UnlockRect();
	pDev->Present( NULL, NULL, NULL, NULL );
}

void Graphics::DrawPixel( int x, int y, D3DCOLOR c )
{
	((D3DCOLOR*)Rect.pBits)[ x + (Rect.Pitch >> 2) * y ] = c;
}

void Graphics::DrawLine( int x1, int y1, int x2, int y2, D3DCOLOR c )
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
		DrawPixel( x1, y1, c );
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dx / (float)dy;
		float b = x1 - m*y1;
		for( int y = y1; y < y2; y++ )
		{
			int x = (int)(m*y + b + 0.5f);
			DrawPixel( x, y, c );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dy / (float)dx;
		float b = y1 - m*x1;
		for( int x = x1; x < x2; x++ )
		{
			int y = (int)(m*x + b + 0.5f);
			DrawPixel( x, y, c );
		}
	}
}

void Graphics::DrawSprite( float xoffset, float yoffset, Sprite* sprite, D3DCOLOR c )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			D3DCOLOR color = sprite->surface[ x + y * sprite->width ];
			if( color != sprite->key )
				DrawPixel( x + xoffset, y + yoffset, c );
		}
	}
}

void Graphics::DrawSprite( int xoffset, int yoffset, Sprite* sprite, D3DCOLOR c )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			D3DCOLOR color = sprite->surface[ x + y * sprite->width ];
			if( color != sprite->key )
				DrawPixel( x + xoffset, y + yoffset, c );
		}
	}
}

void Graphics::DrawChar( char Chars, float x, float y, Font* font, D3DCOLOR c )
{
	int Char = Chars - ' ';
	int Column = Char % font->charperrow;
	int Row = Char / font->charperrow;
	for( int Y = Row * font->charh; Y < (Row * font->charh) + font->charh; Y++ )
	{
		for( int X = Column * font->charw; X < (Column * font->charw) + font->charw; X++ )
		{
			if( font->Surface[ X + Y * font->charw * font->charperrow ] == D3DCOLOR_XRGB( 255,255,255 ) )
				DrawPixel( X + (x-Column * font->charw), Y + (y-Row * font->charh), c );
		}
	}
}

void Graphics::DrawString( const char* string, float center, float y, Font* font, D3DCOLOR c )
{
	int NumLetters = 0;
	for( int i = 0; string[i] != '\0'; i++ )
		NumLetters++;
	int x = center - (NumLetters*font->charw)/2 - (font->charw/2);
	for( int i = 0; string[i] != '\0'; i++ )
		DrawChar( string[i], x + i*font->charw, y, font, c );
}

void Graphics::DrawStringTimed( const char* string, float center, float y, Font* font, double timer, int pace, D3DCOLOR c )
{
	int NumLetters = 0;
	for( int i = 0; string[i] != '\0'; i++ )
		NumLetters++;
	int x = center - (NumLetters*font->charw)/2 - (font->charw/2);
	for( int i = 0; string[i] != '\0'; i++ )
	{
		if( timer + i*pace < clock() )
			DrawChar( string[i], x + i*font->charw, y, font, c );
	}
}

void Graphics::DrawInteger( int integer, int x, int y, Font* font, int type )
{
	if( integer < 0 )
		integer = 0;
	if( type == 1 )
	{
		int thousands = 0;
		int hundreds = 0;
		int tens = 0;
		while( integer >= 1000 )
		{
			integer -= 1000;
			thousands++;
		}
		while( integer >= 100 )
		{
			integer -= 100;
			hundreds++;
		}
		while( integer >= 10 )
		{
			integer -= 10;
			tens++;
		}
		DrawChar( thousands+48, x + 0*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
		DrawChar( hundreds+48, x + 1*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
		DrawChar( tens+48, x + 2*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
		DrawChar( integer+48, x + 3*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );\
	}
	else
	{
		if( integer >= 10000 )
			integer = 9999;
		int thousands = 0;
		int hundreds = 0;
		int tens = 0;
		int tmp = integer;
		while( integer >= 1000 )
		{
			integer -= 1000;
			thousands++;
		}
		while( integer >= 100 )
		{
			integer -= 100;
			hundreds++;
		}
		while( integer >= 10 )
		{
			integer -= 10;
			tens++;
		}
		if( tmp < 10 )
			DrawChar( integer+48, x + 0*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
		else if( tmp < 100 )
		{
			DrawChar( tens+48, x + 0*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
			DrawChar( integer+48, x + 1*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
		}
		else if( tmp < 1000 )
		{
			DrawChar( hundreds+48, x + 0*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
			DrawChar( tens+48, x + 1*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
			DrawChar( integer+48, x + 2*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
		}
		else if( tmp < 10000 )
		{
			DrawChar( thousands+48, x + 0*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
			DrawChar( hundreds+48, x + 1*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
			DrawChar( tens+48, x + 2*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
			DrawChar( integer+48, x + 3*font->charw, y, font, D3DCOLOR_XRGB( 255,255,255 ) );
		}
	}
}