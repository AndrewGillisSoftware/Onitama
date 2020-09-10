#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Card : public Sprite
{
public:
	Card();
	~Card();

	void display(RectangleShape* state, RenderWindow* window);
	bool ** getpMoves();

private:
	string name;
	string botTxt;
	bool ** pMoves;
	Font font;
	Texture cardTemp;
	Texture image;
};

