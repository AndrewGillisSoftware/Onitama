#include "stdafx.h"
#include "Piece.h"


Piece::Piece()
{
}


Piece::~Piece()
{
}

Piece::Piece(string t, string sTyp, Vector2i pos, RenderWindow *window)
{
	double scale = window->getSize().x/500;
	team = t;
	typ = sTyp;
	alive = true;
	location = pos;
	type.loadFromFile(t + typ + ".png");
	setTexture(type);
	setScale(0.4 / scale, 0.4 / scale);
	type.setSmooth(true);

}

void Piece::setLocation(Vector2i pos)
{
	location = pos;
}

Vector2i Piece::getLocation()
{
	return location;
}

string Piece::getTeam()
{
	return team;
}

string Piece::getType()
{
	return typ;
}

void Piece::kill()
{
	alive = false;
	team = "NULL";
	typ = "NULL";
	location = Vector2i(-1, -1);
}

bool Piece::isAlive()
{
	return alive;
}
