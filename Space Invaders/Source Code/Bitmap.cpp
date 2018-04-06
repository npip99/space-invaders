#include "Bitmap.h"
#include <stdio.h>

void LoadBmp( const char* filename, Sprite* sprite )
{
	FILE* pFile = fopen( filename, "rb" );

	char Sig[2];
	fread( Sig, sizeof( char ), 2, pFile );
	BitmapFileHeader FileHeader;
	fread( &FileHeader, sizeof( FileHeader ), 1, pFile );
	BitmapInfoHeader InfoHeader;
	fread( &InfoHeader, sizeof( InfoHeader ), 1, pFile );

	fseek( pFile, FileHeader.offset, SEEK_SET );

	int Pad = (4 - ((InfoHeader.width * 3) % 4)) % 4;

	sprite->width = InfoHeader.width;
	sprite->height = InfoHeader.height;

	for( int y = InfoHeader.height - 1; y >= 0; y-- )
	{
		for( int x = 0; x < InfoHeader.width; x++ )
		{
			Pixel Pixel;
			fread( &Pixel, sizeof( Pixel ), 1, pFile );
			sprite->surface[ x + y * InfoHeader.width ] = D3DCOLOR_XRGB( Pixel.red, Pixel.green, Pixel.blue );
		}
		fseek( pFile, Pad, SEEK_CUR );
	}
	fclose( pFile );
}

void LoadBmp( const char* filename, D3DCOLOR* surface )
{
	FILE* pFile = fopen( filename, "rb" );

	char Sig[2];
	fread( Sig, sizeof( char ), 2, pFile );
	BitmapFileHeader FileHeader;
	fread( &FileHeader, sizeof( FileHeader ), 1, pFile );
	BitmapInfoHeader InfoHeader;
	fread( &InfoHeader, sizeof( InfoHeader ), 1, pFile );

	fseek( pFile, FileHeader.offset, SEEK_SET );

	int Pad = (4 - ((InfoHeader.width * 3) % 4)) % 4;

	for( int y = InfoHeader.height - 1; y >= 0; y-- )
	{
		for( int x = 0; x < InfoHeader.width; x++ )
		{
			Pixel Pixel;
			fread( &Pixel, sizeof( Pixel ), 1, pFile );
			surface[ x + y * InfoHeader.width ] = D3DCOLOR_XRGB( Pixel.red, Pixel.green, Pixel.blue );
		}
		fseek( pFile, Pad, SEEK_CUR );
	}
	fclose( pFile );
}