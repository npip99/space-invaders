#pragma once

#include "Graphics.h"
#include "Keyboard.h"
#include "Bitmap.h"
#include <math.h>
class App
{
public:
	struct Bullet
	{
		int x;
		int y;
		bool shot;
	};
	struct Alien
	{
		float x;
		float y;
		bool dead;
		Bullet bullet[3];
		Sprite* sprite1;
		Sprite* sprite2;
	};
	struct Tank
	{
		float x;
		bool dead;
		Bullet bullet;
		Sprite* sprite;
	};
	struct Score
	{
		int score;
		char name[3];
	};

	App( HWND, const KeyboardServer& kServ );
	void UpdateFrame();
	~App();
private:
	Graphics gfx;
	Keyboard kbd;

	void UpdateTank();
	void UpdateAliens();
	void DrawSprites();
	void TestCollision();
	void IntroScene();
	void EndScene();
	void Reset();

	Score Player[2];
	Score Scores[10];
	Sprite ScreenBottom;
	Sprite Alien4ExplosionSprite;
	Sprite TankExplosion1Sprite;
	Sprite TankExplosion2Sprite;
	Sprite ExplosionSprite;
	Sprite BarrierSprite;
	Sprite Alien1Sprite1;
	Sprite Alien1Sprite2;
	Sprite Alien2Sprite1;
	Sprite Alien2Sprite2;
	Sprite Alien3Sprite1;
	Sprite Alien3Sprite2;
	Sprite Alien4Sprite;
	Sprite Brush1Sprite;
	Sprite Brush2Sprite;
	Sprite Brush3Sprite;
	Sprite Brush4Sprite;
	Sprite LivesSprite;
	Sprite TankSprite;
	D3DCOLOR ScreenBottomTexture[ 1000*5 ];
	D3DCOLOR Alien4ExplosionTexture[ 64*28 ];
	D3DCOLOR TankExplosion1Texture[ 52*32 ];
	D3DCOLOR TankExplosion2Texture[ 52*32 ];
	D3DCOLOR ExplosionTexture[ 44*36 ];
	D3DCOLOR BarrierTexture[ 88*64 ];
	D3DCOLOR Alien1Texture1[ 32*32 ];
	D3DCOLOR Alien1Texture2[ 32*32 ];
	D3DCOLOR Alien2Texture1[ 44*32 ];
	D3DCOLOR Alien2Texture2[ 44*32 ];
	D3DCOLOR Alien3Texture1[ 48*32 ];
	D3DCOLOR Alien3Texture2[ 48*32 ];
	D3DCOLOR Alien4Texture[ 64*28 ];
	D3DCOLOR Brush1Texture[ 30*24 ];
	D3DCOLOR Brush2Texture[ 30*24 ];
	D3DCOLOR Brush3Texture[ 30*24 ];
	D3DCOLOR Brush4Texture[ 30*24 ];
	D3DCOLOR FontTexture[ 512*84 ];
	D3DCOLOR LivesTexture[ 32*20 ];
	D3DCOLOR TankTexture[ 52*32 ];
	Font Font;
	Tank Tank;
	Alien Alien[56];

	int Lives;
	int Random;
	int NumAliens;
	int Direction;
	int Animation;
	int CurPlayer;
	int CurLetter;
	int NumPlayers;
	int TankExplosion;
	int BarrierState[4][16];
	double Timer;
	double Timer1;
	double Timer2;
	double Timer3;
	bool HighScore;
	bool Intro;
	bool End;
	bool Victory;
};