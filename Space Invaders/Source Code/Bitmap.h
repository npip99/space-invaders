#pragma once

#include "Graphics.h"

struct BitmapFileHeader
{
	unsigned int filesize;
	unsigned short int reserved1, resereved2;
	unsigned int offset;
};

struct BitmapInfoHeader
{
	unsigned int infosize;
	int width, height;
	unsigned short int planes;
	unsigned short int bits;
	unsigned int compression;
	unsigned int imagesize;
	int xres, yres;
	unsigned int ncolors;
	unsigned int importantcolors;
};

struct Pixel
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

void LoadBmp( const char* filename, Sprite* sprite );
void LoadBmp( const char* filename, D3DCOLOR* surface );