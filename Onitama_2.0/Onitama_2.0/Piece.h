#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Piece : public Sprite
{
public:
	Piece();
	~Piece();

	Piece(string team, string type, Vector2i pos, RenderWindow *window);

	void setLocation(Vector2i);
	Vector2i getLocation();
	string getTeam();
	string getType();
	void kill();
	bool isAlive();

protected:
	string team, typ;
	Texture type;
	Vector2i location;
	bool alive;
};

