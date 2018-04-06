#pragma once

class KeyboardServer;

class Keyboard
{
public:
	Keyboard( const KeyboardServer& kServ );
	bool Up() const;
	bool Down() const;
	bool Left() const;
	bool Right() const;
	bool Space() const;
	bool Enter() const;
private:
	const KeyboardServer& Serv;
};

class KeyboardServer
{
	friend Keyboard;
public:
	KeyboardServer();

	void UpPress();
	void DownPress();
	void LeftPress();
	void RightPress();
	void SpacePress();
	void EnterPress();

	void UpRelease();
	void DownRelease();
	void LeftRelease();
	void RightRelease();
	void SpaceRelease();
	void EnterRelease();

private:
	bool Up;
	bool Down;
	bool Left;
	bool Right;
	bool Space;
	bool Enter;
};