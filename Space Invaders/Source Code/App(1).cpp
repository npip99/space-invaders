#include "App.h"
#include "Bitmap.h"
App::App( HWND hWnd,const KeyboardServer& kServer )
:	gfx( hWnd ),
	kbd( kServer )
{
	LoadSprite( "Application Files\\BottomScreen.bmp", &ScreenBottom, ScreenBottomTexture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\Alien4Explosion.bmp", &Alien4ExplosionSprite, Alien4ExplosionTexture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\TankExplosion1.bmp", &TankExplosion1Sprite, TankExplosion1Texture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\TankExplosion2.bmp", &TankExplosion2Sprite, TankExplosion2Texture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\Explosion.bmp", &ExplosionSprite, ExplosionTexture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\Barrier.bmp", &BarrierSprite, BarrierTexture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\1Alien1.bmp", &Alien1Sprite1, Alien1Texture1, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\1Alien2.bmp", &Alien2Sprite1, Alien2Texture1, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\1Alien3.bmp", &Alien3Sprite1, Alien3Texture1, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\2Alien1.bmp", &Alien1Sprite2, Alien1Texture2, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\2Alien2.bmp", &Alien2Sprite2, Alien2Texture2, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\2Alien3.bmp", &Alien3Sprite2, Alien3Texture2, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\Alien4.bmp", &Alien4Sprite, Alien4Texture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\Brush1.bmp", &Brush1Sprite, Brush1Texture, D3DCOLOR_XRGB( 255,255,255 ) );
	LoadSprite( "Application Files\\Brush2.bmp", &Brush2Sprite, Brush2Texture, D3DCOLOR_XRGB( 255,255,255 ) );
	LoadSprite( "Application Files\\Brush3.bmp", &Brush3Sprite, Brush3Texture, D3DCOLOR_XRGB( 255,255,255 ) );
	LoadSprite( "Application Files\\Brush4.bmp", &Brush4Sprite, Brush4Texture, D3DCOLOR_XRGB( 255,255,255 ) );
	LoadSprite( "Application Files\\Lives.bmp", &LivesSprite, LivesTexture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadSprite( "Application Files\\Tank.bmp", &TankSprite, TankTexture, D3DCOLOR_XRGB( 0,0,0 ) );
	LoadFont( "Application Files\\Font.bmp", &Font, FontTexture, 16, 28, 32 );
	Reset();
	NumPlayers = 1;
	CurPlayer = 0;
	Player[0].score = 0;
	Player[1].score = 0;
	Player[0].name[0] = 'A';
	Player[0].name[1] = 'A';
	Player[0].name[2] = 'A';
	Player[1].name[0] = 'A';
	Player[1].name[1] = 'A';
	Player[1].name[2] = 'A';
	Intro = true;
	End = false;
	for( int i = 0; i < 10; i++ )
	{
		Scores[i].score = -1;
		for( int j = 0; j < 3; j++ )
			Scores[i].name[j] = 'A';
	}
	FILE* HiScoreFile;
	fopen_s( &HiScoreFile, "hiscore.txt","r");
	if( HiScoreFile )
	{
		int i;
		do for( i = 0; i < 10; i++ )
		{
			fscanf_s( HiScoreFile, "%d %c%c%c", &Scores[i].score, &Scores[i].name[0], &Scores[i].name[1],& Scores[i].name[2] );
		} while ( !feof( HiScoreFile ) && i < 10 );
		fclose( HiScoreFile );
	}
}

App::~App()
{
	FILE* HiScoreFile;
	fopen_s( &HiScoreFile, "hiscore.txt","w");
	for( int i = 0; i < 10 && Scores[i].score >= 0; i++ )
		fprintf( HiScoreFile,"%d\t\t%c%c%c\n", Scores[i].score, Scores[i].name[0], Scores[i].name[1], Scores[i].name[2] );
	fclose( HiScoreFile );
}

void App::UpdateFrame()
{
	gfx.StartFrame();
	gfx.DrawString( "SCORE<1> HI-SCORE SCORE<2>", 500, 0, &Font, D3DCOLOR_XRGB( 255,255,255 ) );
	gfx.DrawSprite( 0, 670, &ScreenBottom, D3DCOLOR_XRGB( 0,255,0 ) );
	gfx.DrawInteger( Lives, 25, 675, &Font, 2 );
	gfx.DrawInteger( Player[0].score, 350-16*3, 30, &Font, 1);

	if( Player[0].score > Scores[0].score )
		gfx.DrawInteger( Player[0].score, 500-16*3, 30, &Font, 1);
	else if( Player[1].score > Scores[0].score )
		gfx.DrawInteger( Player[1].score, 500-16*3, 30, &Font, 1);
	else
		gfx.DrawInteger( Scores[0].score, 500-16*3, 30, &Font, 1);

	gfx.DrawInteger( Player[1].score, 650-16*3, 30, &Font, 1);


	if( Intro == true )
		IntroScene();	
	else if( End == true )
		EndScene();
	else
	{
		if( Timer + (NumAliens+15)/25 < clock() )
		{
			UpdateTank();
			UpdateAliens();
			TestCollision();
		}
		DrawSprites();
	}
	gfx.FinishFrame();
}

void App::Reset()
{
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 16; j++ )
			BarrierState[i][j] = 0;
	}
	srand( (unsigned int)time(NULL) );
	Random = rand() % 10;
	Victory = true;
	NumAliens = 55;
	Timer = clock();
	Timer1 = clock();
	Timer2 = clock();
	Timer3 = clock();
	TankExplosion = 1;
	HighScore = false;
	Tank.bullet.shot = false;
	Tank.bullet.x = 500;
	Tank.bullet.y = 600;
	Tank.dead = false;
	CurLetter = 2;
	Direction = 1;
	Tank.x = 474;
	Lives = 3;

	Alien[ 55 ].dead = true;
	Alien[ 55 ].y = 100;
	Alien[ 55 ].x = 0;
	Alien[ 55 ].sprite1 = &Alien4Sprite;
	Alien[ 55 ].sprite2 = &Alien4Sprite;
	for( int i = 0; i < 55; i++ )
	{
		Alien[i].y = (75 + (float)((i/11)*70));
		Alien[i].x = 125 + (float)(i*70) - 11*70*(i/11);
		Alien[i].dead = false;
		if( i < 11 )
			Alien[i].x += 8;
		else if( i < 33 )
			Alien[i].x += 2;
		if( i < 11 )
			Alien[i].sprite1 = &Alien1Sprite1;
		else if( i < 33 )
			Alien[i].sprite1 = &Alien2Sprite1;
		else if( i < 55 )
			Alien[i].sprite1 = &Alien3Sprite1;
		if( i < 11 )
			Alien[i].sprite2 = &Alien1Sprite2;
		else if( i < 33 )
			Alien[i].sprite2 = &Alien2Sprite2;
		else if( i < 55 )
			Alien[i].sprite2 = &Alien3Sprite2;

		for( int j = 0; j < 3; j++ )
			Alien[i].bullet[j].shot = false;
	}
}

void App::TestCollision()
{
	if( Tank.bullet.shot == true )
	{
		if( Tank.bullet.y <= 5 )
			Tank.bullet.shot = false;

		Tank.bullet.y -= 10;

		for( int i = 0; i < 11 && Tank.bullet.shot == true; i++ )
		{
			if( Tank.bullet.x - 01 >= Alien[ i ].x &&
				Tank.bullet.x <= Alien[ i ].x + 32 &&
				Tank.bullet.y + 20 >= Alien[ i ].y &&
				Tank.bullet.y <= Alien[ i ].y + 32 &&
				Alien[ i ].dead != true )
			{
				Alien[ i ].dead = true;
				Tank.bullet.shot = false;
				gfx.DrawSprite( Alien[i].x - 6, Alien[i].y - 2, &ExplosionSprite, D3DCOLOR_XRGB( 255,255,255 ) );
				NumAliens--;
				Player[CurPlayer].score += 30;
			}
		}
		for( int i = 11; i < 33 && Tank.bullet.shot == true; i++ )
		{
			if( Tank.bullet.x - 01 >= Alien[ i ].x &&
				Tank.bullet.x <= Alien[ i ].x + 44 &&
				Tank.bullet.y + 20 >= Alien[ i ].y &&
				Tank.bullet.y <= Alien[ i ].y + 32 &&
				Alien[ i ].dead != true )
			{
				Alien[ i ].dead = true;
				Tank.bullet.shot = false;
				gfx.DrawSprite( Alien[i].x, Alien[i].y - 2, &ExplosionSprite, D3DCOLOR_XRGB( 255,255,255 ) );
				NumAliens--;
				Player[CurPlayer].score += 20;
			}
		}
		for( int i = 33; i < 55 && Tank.bullet.shot == true; i++ )
		{
			if( Tank.bullet.x - 01 >= Alien[ i ].x &&
				Tank.bullet.x <= Alien[ i ].x + 48 &&
				Tank.bullet.y + 20 >= Alien[ i ].y &&
				Tank.bullet.y <= Alien[ i ].y + 32 &&
				Alien[ i ].dead != true )
			{
				Alien[ i ].dead = true;
				Tank.bullet.shot = false;
				gfx.DrawSprite( Alien[i].x + 2, Alien[i].y - 2, &ExplosionSprite, D3DCOLOR_XRGB( 255,255,255 ) );
				NumAliens--;
				Player[CurPlayer].score += 10;
			}
		}

		if( Tank.bullet.shot == true &&
			Tank.bullet.x >= Alien[ 55 ].x &&
			Tank.bullet.x <= Alien[ 55 ].x + 64 &&
			Tank.bullet.y >= Alien[ 55 ].y &&
			Tank.bullet.y <= Alien[ 55 ].y + 28 &&
			Alien[ 55 ].dead != true )
		{
			Tank.bullet.shot = false;
			Alien[ 55 ].dead = true;
			Alien[ 55 ].y = 100;
			Alien[ 55 ].x = 0;
			gfx.DrawSprite( Alien[55].x, Alien[55].y, &Alien4ExplosionSprite, D3DCOLOR_XRGB( 255,255,255 ) );
			int random = rand() % 4;
			if( random == 0 )
				Player[CurPlayer].score += 50;
			else if( random == 1 )
				Player[CurPlayer].score += 100;
			else if( random == 2 )
				Player[CurPlayer].score += 150;
			else if( random ==3 )
				Player[CurPlayer].score += 300;
		}
		if( Tank.bullet.shot == true)
		{
			if( Tank.bullet.x >= 156 &&
				Tank.bullet.x <= 756 + 88 &&
				Tank.bullet.y >= 550 &&
				Tank.bullet.y <= 550 + 64 )
			{
				for( int i = 0; i < 4; i++ )
				{
					for( int j = 0; j < 16; j++ )
					{
						if( Tank.bullet.x <= 156 + (i*200) + 22*((j%4)+1) &&
							Tank.bullet.x >= 156 + (i*200) + 22*(j%4) &&
							Tank.bullet.y <= 550 + 16*((j/4)+1) &&
							Tank.bullet.y >= 550 + 16*(j/4) &&
							BarrierState[i][j] < 4 && Tank.bullet.shot == true )
						{
							Tank.bullet.shot = false;
							BarrierState[i][j]++;
						}
					}
				}
			}
		}

		if( NumAliens < 1 )
		{
			Lives++;
			Victory = true;
			End = true;
		}
	}
	for( int i = 0; i < 55; i++ )
	{
		if( Alien[ i ].y + 32 > 625 && Alien[ i ].dead != true )
		{
			Victory = false;
			End = true;
		}
		for( int j = 0; j < 3; j++ )
		{
			if( Alien[ i ].bullet[ j ].shot == true )
			{
				if( Alien[ i ].bullet[ j ].x >= Tank.x &&
					Alien[ i ].bullet[ j ].x <= Tank.x + 52 &&
					Alien[ i ].bullet[ j ].y >= 625 &&
					Alien[ i ].bullet[ j ].y <= 625 + 32 + 15 &&
					Tank.dead != true )
				{
					Alien[ i ].bullet[ j ].shot = false;
					Tank.dead = true;
					Lives--;
				}
				else if( Alien[ i ].bullet[ j ].x >= 156 &&
						Alien[ i ].bullet[ j ].x <= 756 + 88 &&
						Alien[ i ].bullet[ j ].y >= 550 &&
						Alien[ i ].bullet[ j ].y <= 550 + 64 )
				{
					for( int a = 0; a < 4; a++ )
					{
						for( int b = 0; b < 16; b++ )
						{
							if( Alien[ i ].bullet[ j ].x <= 156 + (a*200) + 22*((b%4)+1) &&
								Alien[ i ].bullet[ j ].x >= 156 + (a*200) + 22*(b%4) &&
								Alien[ i ].bullet[ j ].y <= 550 + 16*((b/4)+1) &&
								Alien[ i ].bullet[ j ].y >= 550 + 16*(b/4) &&
								BarrierState[a][b] < 4 && Alien[ i ].bullet[ j ].shot == true )
							{
								Alien[ i ].bullet[ j ].shot = false;
								BarrierState[a][b]++;
							}
						}
					}
				}
				if( Tank.bullet.shot == true &&
					Tank.bullet.y <= Alien[ i ].bullet[ j ].y &&
					(Tank.bullet.x == Alien[ i ].bullet[ j ].x ||
					Tank.bullet.x + 1 == Alien[ i ].bullet[ j ].x) )
				{
					Alien[ i ].bullet[ j ].shot = false;
					Tank.bullet.shot = false;
				}
			}
		}
	}
}

void App::DrawSprites()
{
	gfx.DrawSprite( 156, 550, &BarrierSprite, D3DCOLOR_XRGB( 0,255,0 ) );
	gfx.DrawSprite( 356, 550, &BarrierSprite, D3DCOLOR_XRGB( 0,255,0 ) );
	gfx.DrawSprite( 556, 550, &BarrierSprite, D3DCOLOR_XRGB( 0,255,0 ) );
	gfx.DrawSprite( 756, 550, &BarrierSprite, D3DCOLOR_XRGB( 0,255,0 ) );

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 16; j++ )
		{
			if( BarrierState[i][j] == 1)
			{
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush1Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush1Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
			}
			else if( BarrierState[i][j] == 2)
			{
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush2Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush2Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
			}
			else if( BarrierState[i][j] == 3)
			{
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush3Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush3Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
			}
			else if( BarrierState[i][j] == 4)
			{
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush4Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
				gfx.DrawSprite( 152 + i*200 + 22*(j%4), 546 + 16*(j/4), &Brush4Sprite, D3DCOLOR_XRGB( 0,0,0 ) );
			}
		}
	}
	for( int i = 0; i < 55; i++ )
	{
		if( Alien[i].dead != true )
		{
			if( Animation == 1 )
			{
				if( Alien[i].y + 32 > 550 )
					gfx.DrawSprite( Alien[i].x, Alien[i].y, Alien[i].sprite1, D3DCOLOR_XRGB( 0,255,0 ) );
				else
					gfx.DrawSprite( Alien[i].x, Alien[i].y, Alien[i].sprite1, D3DCOLOR_XRGB( 255,255,255 ) );
			}
			else
			{
				if( Alien[i].y + 32 > 550 )
					gfx.DrawSprite( Alien[i].x, Alien[i].y, Alien[i].sprite2, D3DCOLOR_XRGB( 0,255,0 ) );
				else
					gfx.DrawSprite( Alien[i].x, Alien[i].y, Alien[i].sprite2, D3DCOLOR_XRGB( 255,255,255 ) );
			}
		}
	}
	if( Alien[55].dead != true )
		gfx.DrawSprite( Alien[55].x, Alien[55].y, Alien[55].sprite1, D3DCOLOR_XRGB( 255,0,0 ) );

	if( Tank.bullet.shot == true )
	{
		gfx.DrawLine( Tank.bullet.x - 1, Tank.bullet.y, Tank.bullet.x - 1, Tank.bullet.y + 20, D3DCOLOR_XRGB( 255,255,255 ) );
		gfx.DrawLine( Tank.bullet.x, Tank.bullet.y, Tank.bullet.x, Tank.bullet.y + 20, D3DCOLOR_XRGB( 255,255,255 ) );
	}

	for( int i = 0; i < 55; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			if( Alien[ i ].bullet[ j ].shot == true )
			{
				gfx.DrawLine( Alien[ i ].bullet[ j ].x - 2, Alien[ i ].bullet[ j ].y, 
					Alien[ i ].bullet[ j ].x - 2, Alien[ i ].bullet[ j ].y - 20, D3DCOLOR_XRGB( 255,255,255 ) );
				gfx.DrawLine( Alien[ i ].bullet[ j ].x - 1, Alien[ i ].bullet[ j ].y, 
					Alien[ i ].bullet[ j ].x - 1, Alien[ i ].bullet[ j ].y - 20, D3DCOLOR_XRGB( 255,255,255 ) );
			}
		}
	}

	if( Tank.dead != true )
		gfx.DrawSprite( Tank.x, (float)625, &TankSprite, D3DCOLOR_XRGB( 0,255,0 ) );
	else
	{
		if( TankExplosion == 1)
			gfx.DrawSprite( Tank.x, 625.0f, &TankExplosion1Sprite, D3DCOLOR_XRGB( 0,255,0 ) );
		else
			gfx.DrawSprite( Tank.x, 625.0f, &TankExplosion2Sprite, D3DCOLOR_XRGB( 0,255,0 ) );
	}
	for( int i = 1; i < Lives && i < 20; i++ )
		gfx.DrawSprite( (i*50) + (Lives/10)*16, 677, &LivesSprite, D3DCOLOR_XRGB( 0,255,0 ) );
}

void App::IntroScene() 
{
	if( Timer + 7500 > clock() )
		gfx.DrawString( "Space Invaders", 500, 100, &Font, D3DCOLOR_XRGB( 255,255,255 ) );
	if( Timer + 750 < clock() && Timer + 7500 > clock() )
		gfx.DrawString( "*Score Advance Table*", 500, 200, &Font, D3DCOLOR_XRGB( 255,255,255 ) );
	if( Timer + 1500 < clock() && Timer + 7500 > clock() )
	{
		gfx.DrawSprite( 350, 300, &Alien4Sprite, D3DCOLOR_XRGB( 255,0,0 ) );
		gfx.DrawStringTimed( "   = ? Mystery", 500, 300, &Font, Timer + 1500, 100, D3DCOLOR_XRGB( 255,255,255 ) );
	}
	if( Timer + 3000 < clock() && Timer + 7500 > clock() )
	{
		gfx.DrawSprite( 350 + 16, 350, &Alien1Sprite1, D3DCOLOR_XRGB( 255,255,255 ) );
		gfx.DrawStringTimed( "   = 30 Points", 500, 350, &Font, Timer + 3000, 100, D3DCOLOR_XRGB( 255,255,255 ) );
	}
	if( Timer + 4500 < clock() && Timer + 7500 > clock() )
	{
		gfx.DrawSprite( 350 + 10, 400, &Alien2Sprite1, D3DCOLOR_XRGB( 255,255,255 ) );
		gfx.DrawStringTimed( "   = 20 Points", 500, 400, &Font, Timer + 4500, 100, D3DCOLOR_XRGB( 255,255,255 ) );
	}
	if( Timer + 6000 < clock() && Timer + 7500 > clock() )
	{
		gfx.DrawSprite( 350 + 8, 450, &Alien3Sprite1, D3DCOLOR_XRGB( 0,255,0 ) );
		gfx.DrawStringTimed( "   = 10 Points", 500, 450, &Font, Timer + 6000, 100, D3DCOLOR_XRGB( 0,255,0 ) );
	}
	if( Timer + 7500 < clock() )
	{
		if( HighScore == true )
		{
			for( int i = 0; i < 10 && Scores[i].score >= 0; i++ )
			{
				gfx.DrawInteger( i+1, 338, 102 + (i*50), &Font, 2 );
				gfx.DrawChar( ')', 350.0f, float(100 + (i*50)), &Font, D3DCOLOR_XRGB( 255,255,255 ) );
				gfx.DrawInteger( Scores[i].score, 550, 100 + (i*50), &Font, 2 );
				for( int j = 0; j < 3; j++ )
					gfx.DrawChar( Scores[i].name[j], float(400 + ((j-1)*25)), float(100 + (i*50)), &Font, D3DCOLOR_XRGB( 255,255,255 ) );
			}
		}
		else
		{
			gfx.DrawString( "Space Invaders",500,100,&Font, D3DCOLOR_XRGB( 255,255,255 ) );

			gfx.DrawLine( 425 - ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50),
				425 - ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50) + 30, D3DCOLOR_XRGB( 255,255,255 ) );
			gfx.DrawLine( 425 - ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50) + 30,
				558 + ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50) + 30, D3DCOLOR_XRGB( 255,255,255 ) );
			gfx.DrawLine( 558 + ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50) + 30,
				558 + ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50), D3DCOLOR_XRGB( 255,255,255 ) );
			gfx.DrawLine( 558 + ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50),
				425 - ((NumPlayers/3)*25), 350 + ((NumPlayers-2)*50), D3DCOLOR_XRGB( 255,255,255 ) );

			gfx.DrawStringTimed( "1 Player", 500, 300, &Font, Timer + 6000, 100, D3DCOLOR_XRGB( 255,255,255 ) );
			gfx.DrawStringTimed( "2 Player", 500, 350, &Font, Timer + 6000, 100, D3DCOLOR_XRGB( 255,255,255 ) );
			gfx.DrawStringTimed( "High Scores", 500, 400, &Font, Timer + 6000, 100, D3DCOLOR_XRGB( 255,255,255 ) );
		}
		if( Timer1 + 100 < clock() && HighScore == false )
		{
			if( kbd.Up() && NumPlayers > 1 )
				NumPlayers--;
			if( kbd.Down() && NumPlayers < 3 )
				NumPlayers++;
			Timer1 = clock();
		}
		if( kbd.Enter() && Timer2 + 500 < clock() )
		{
			if( NumPlayers == 3 && HighScore == false )
					HighScore = true;
			else if( HighScore == true )
				HighScore = false;
			else
				Intro = false;
			Timer2 = clock();
		}
	}
	for( int i = 1;i < Lives && Lives > 1 && i < 20; i++ )
		gfx.DrawSprite( ((i)*50 + (Lives/10)*16), 677, &LivesSprite, D3DCOLOR_XRGB( 0,255,0 ) );
}

void App::EndScene()
{
	if( Victory == true )
	{
		Timer = clock();
		gfx.DrawStringTimed( "Well Done Earthling", 500, 300, &Font, Timer, 100, D3DCOLOR_XRGB( 255,255,255 ) );
		gfx.DrawStringTimed( "This Time You Win", 500, 325, &Font, Timer + 3000, 100, D3DCOLOR_XRGB( 255,255,255 ) );
		Timer1 = clock();
		while( Timer1 + 500 > clock() );

		gfx.DrawString( "Continue?", 500, 375, &Font, D3DCOLOR_XRGB( 255,255,255 ) );

		if( kbd.Enter() )
		{
			Alien[ 55 ].dead = true;
			Alien[ 55 ].y = 100;
			Alien[ 55 ].x = 0;
			for( int i = 0; i < 55; i++ )
			{
				Alien[i].y = 75 + (float)((i/11)*70);
				Alien[i].x = 125 + (float)(i*70) - 11*70*(i/11);
				Alien[i].dead = false;
				if( i < 11 )
					Alien[i].x += 8;
				else if( i < 33 )
					Alien[i].x += 2;

				for( int j = 0; j < 3; j++ )
					Alien[i].bullet[j].shot = false;
			}
			End = false;
			NumAliens = 55;
		}
	}
	else
	{
		gfx.DrawStringTimed( "Game Over", 500, 250, &Font, Timer2, 250, D3DCOLOR_XRGB( 255,255,255 ) );
		gfx.DrawChar( Player[CurPlayer].name[0], 450-16, 300, &Font, D3DCOLOR_XRGB( 255,255,255 ) );
		gfx.DrawChar(Player[CurPlayer].name[1], 500-16, 300, &Font, D3DCOLOR_XRGB( 255,255,255 ) );
		gfx.DrawChar( Player[CurPlayer].name[2], 550-16, 300, &Font, D3DCOLOR_XRGB( 255,255,255 ) );
		if( Timer1 + 250 < clock() && ( kbd.Left() || kbd.Right() ) )
		{
			if( kbd.Left() && CurLetter != 1 )
				CurLetter--;
			if( kbd.Right() && CurLetter != 3 )
				CurLetter++;
			Timer1 = clock();
		}
		gfx.DrawString( "< >", float(500 + ((CurLetter-2)*50)), 300.0f, &Font, D3DCOLOR_XRGB( 255,255,255 ) );
		if( Timer + 150 < clock() ) {
			if( kbd.Down() )
			{
				if( Player[CurPlayer].name[CurLetter-1] == 'Z' )
					Player[CurPlayer].name[CurLetter-1] = 'A';
				else
					Player[CurPlayer].name[CurLetter-1]++;
				Timer = clock();
			}
			if( kbd.Up() )
			{
				if( Player[CurPlayer].name[CurLetter-1] == 'A' )
					Player[CurPlayer].name[CurLetter-1] = 'Z';
				else
					Player[CurPlayer].name[CurLetter-1]--;
				Timer = clock();
			}
		}
		if( kbd.Enter() )
		{
			int i;
			for( i = 0; i < 10 && Scores[i].score >= Player[CurPlayer].score; i++ );
			for( int j = 0; j < 3; j++ )
				Scores[i].name[j] = Player[CurPlayer].name[j];
			if( i < 10 )
			{
				for( int shufflei = 10-2; shufflei >= i; shufflei-- )
					Scores[i+1].score = Scores[i].score;
				Scores[i].score = Player[CurPlayer].score;
			}
			Reset();
			End = false;
			Intro = false;
			if( NumPlayers == 2 )
			{
				if( CurPlayer == 1 )
				{
					Player[CurPlayer].score = 0;
					CurPlayer = 0;
					Player[CurPlayer].score = 0;
					End = false;
				}
				else
					CurPlayer = 1;
			}
			else
				Player[CurPlayer].score = 0;
		}
	}
	for( int i = 1; i < Lives && i < 20; i++ )
		gfx.DrawSprite( (i*50) + (Lives/10)*16, 677, &LivesSprite, D3DCOLOR_XRGB( 0,255,0 ) );
}

void App::UpdateAliens()
{
	for( int i = 0; i < 55; i ++ )
	{
		/*if( Direction == 1 )
			Alien[ i ].x += 50/(float)( pow( float(NumAliens), 1.15f ) + 25 );
		else
			Alien[ i ].x -= 50/(float)( pow( float(NumAliens), 1.15f ) + 25 );*/
		if( Direction == 1 )
			Alien[ i ].x += 50/float(pow( float(3 - (2/NumAliens)), 5.0f ) + 25 );
		else
			Alien[ i ].x -= 50/float(pow( float(3 - (2/NumAliens)), 5.0f ) + 25 );


		if( Alien[ i ].x >= 900 && Alien[ i ].dead != true )
		{
			for( int j = 0; j < 55; j ++ )
			{
				Alien[ j ].y += 18;
				if( Alien[ i ].y >= 600 )
				{
					Victory = false;
					End = true;
				}
				Alien[ j ].x -= 5;
			}
			Direction = 0;
		}
		else if( Alien[ i ].x <= 50 && Alien[ i ].dead != true )
		{
			for( int j = 0; j < 55; j ++ )
			{
				Alien[ j ].y += 18;
				if( Alien[ i ].y >= 600 )
				{
					Victory = false;
					End = true;
				}
				Alien[ j ].x += 5;
			}
			Direction = 1;
		}
		if( Alien[ 55 ].dead != true )
		{
			Alien[ 55 ].x += 0.05f;
			if( Alien[ 55 ].x >= 1000 )
			{
				Alien[ 55 ].dead = true;
				Alien[ 55 ].x = 0;
			}
		}
		for( int j = 0; j < 3; j ++ )
		{
			if( Alien[ i ].bullet[ j ].shot != true && Random == rand() % 5000 &&
				Alien[ i ].dead != true && Animation == 1 )
			{
				Alien[ i ].bullet[ j ].shot = true;
				if( i < 11 )
				{
					Alien[ i ].bullet[ j ].x = int(Alien[ i ].x) + 16;
					Alien[ i ].bullet[ j ].y = int(Alien[ i ].y) + 16;
				}
				if( i < 33 )
				{
					Alien[ i ].bullet[ j ].x = int(Alien[ i ].x) + 22;
					Alien[ i ].bullet[ j ].y = int(Alien[ i ].y) + 16;
				}
				if( i < 55 )
				{
					Alien[ i ].bullet[ j ].x = int(Alien[ i ].x) + 24;
					Alien[ i ].bullet[ j ].y = int(Alien[ i ].y) + 16;
				}
			}
			if( Alien[ i ].bullet[ j ].shot == true )
			{
				Alien[ i ].bullet[ j ].y += 5;
				if( Alien[ i ].bullet[ j ].y >= 675 )
					Alien[ i ].bullet[ j ].shot = false;
			}
		}
	}
	if( Timer1 + (NumAliens+20)*10 < clock() )
	{
		if( Animation == 0 )
			Animation = 1;
		else
			Animation = 0;
		Timer1 = clock();
	}
	if( Alien[ 0 ].y > 100 + 30 && Alien[ 55 ].dead == true && Random == rand() % 5000 )
		Alien[ 55 ].dead = false;
}

void App::UpdateTank()
{
	if( kbd.Space() && Tank.bullet.shot != true && Tank.dead != true )
	{
		Tank.bullet.shot = true;
		Tank.bullet.x = (int)Tank.x + 26;
		Tank.bullet.y = 645;
	}
	if( kbd.Left() && Tank.dead != true && Tank.x > 5 )
		Tank.x-=5;
	if( kbd.Right() && Tank.dead != true && Tank.x < 995 - 52)
		Tank.x+=5;

	if( Tank.dead == true )
	{
		if( Timer2 + 75 < clock() )
		{
			if( TankExplosion == 1)
				TankExplosion = 2;
			else
				TankExplosion = 1;
			Timer2 = clock();
		}
		if( Timer3 + 1000 < clock() )
		{
			if( Lives < 1 )
			{
				End = true;
				Victory = false;
			}
			else
				Tank.dead = false;
		}
	}
	else
		Timer3 = clock();
	Timer = clock();
}
