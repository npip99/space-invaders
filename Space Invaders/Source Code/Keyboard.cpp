#include "Keyboard.h"


Keyboard::Keyboard( const KeyboardServer& kServ )
:	Serv( kServ )
{}

bool Keyboard::Up() const
{
	return Serv.Up;
}

bool Keyboard::Down() const
{
	return Serv.Down;
}

bool Keyboard::Left() const
{
	return Serv.Left;
}

bool Keyboard::Right() const
{
	return Serv.Right;
}

bool Keyboard::Space() const
{
	return Serv.Space;
}

bool Keyboard::Enter() const
{
	return Serv.Enter;
}

KeyboardServer::KeyboardServer()
:	Up( false ),
	Down( false ),
	Left( false ),
	Right( false ),
	Space( false ),
	Enter( false )
{}

void KeyboardServer::UpPress()
{
	Up = true;
}
void KeyboardServer::DownPress()
{
	Down = true;
}
void KeyboardServer::LeftPress()
{
	Left = true;
}
void KeyboardServer::RightPress()
{
	Right = true;
}
void KeyboardServer::SpacePress()
{
	Space = true;
}
void KeyboardServer::EnterPress()
{
	Enter = true;
}

void KeyboardServer::UpRelease()
{
	Up = false;
}
void KeyboardServer::DownRelease()
{
	Down = false;
}
void KeyboardServer::LeftRelease()
{
	Left = false;
}
void KeyboardServer::RightRelease()
{
	Right = false;
}
void KeyboardServer::SpaceRelease()
{
	Space = false;
}
void KeyboardServer::EnterRelease()
{
	Enter = false;
}