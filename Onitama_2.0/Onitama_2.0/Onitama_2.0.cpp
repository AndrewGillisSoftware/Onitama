#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Piece.h"

using namespace std;
using namespace sf;

void setUpBackground(Texture*, Texture*, RectangleShape*, RectangleShape*, RenderWindow*);
void setUpCardHolders(RectangleShape cH[6], RenderWindow*);
void disCardHolders(RectangleShape cH[6], RenderWindow*);
void setUpBoard(RectangleShape[5][5], RenderWindow*);
void setPieces(Piece**, Piece**, RectangleShape board[5][5], RenderWindow*);
void movePiece(Piece*, Vector2i, RectangleShape[5][5]);
void disPieces(Piece**, Piece**, RenderWindow*);
Piece* findSelected(Vector2i, Piece**, Piece**, RectangleShape[5][5]);
void snapObj(Piece *, Vector2i lP);
Vector2i clickedSqr(Vector2i, RectangleShape board[5][5]);
bool snapToSquare(Piece *selected, Piece ** one, Piece ** two, Vector2i lP, RectangleShape board[5][5]);
void delPtrShift(Piece **, Piece*);

int main()
{
	RenderWindow window(VideoMode(600, 600), "Onitama", Style::Fullscreen);

	Texture T_background, T_board;
	RectangleShape background, aboard, cH[6];
	RectangleShape board[5][5];
	//blue
	Piece ** playerOne;

	//red
	Piece ** playerTwo;
	Piece *selected = nullptr;
	Vector2i lP;
	bool clicked = false;

	setUpBackground(&T_background, &T_board, &background, &aboard, &window);

	setUpBoard(board,&window);

	setUpCardHolders(cH, &window);

	playerOne = new Piece*[5];
	playerTwo = new Piece*[5];

	setPieces(playerOne, playerTwo, board, &window);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::MouseButtonPressed && !clicked)
			{
				lP = Mouse::getPosition(window);

				if (findSelected(lP, playerOne, playerTwo, board) != nullptr)
				{
					selected = findSelected(lP, playerOne, playerTwo, board);
					clicked = true;
				}
			}
			else if (event.type == Event::MouseButtonPressed && clicked && selected != nullptr)
			{
				lP = Mouse::getPosition(window);

				if (snapToSquare(selected, playerOne, playerTwo, lP, board))
				{
					clicked = false;
					selected = nullptr;
				}
			}
			if (event.type == Event::MouseMoved && selected != nullptr && clicked)
			{
				lP = Mouse::getPosition(window);

				snapObj(selected, lP);
			}
			
		}
		window.clear();
		window.draw(background);
		window.draw(aboard);
		disPieces(playerOne, playerTwo, &window);
	//	disCardHolders(cH, &window);
		window.display();
	}

	return 0;
}

void setUpBackground(Texture *T_background, Texture *T_board, RectangleShape *background, RectangleShape *aboard, RenderWindow *window)
{
	//background
	T_background->loadFromFile("background.jpg");
	T_background->setSmooth(true);
	background->setSize(Vector2f(window->getSize().x, window->getSize().y));
	background->setTexture(T_background);

	//board
	T_board->loadFromFile("board.png");
	T_board->setSmooth(true);
	aboard->setSize(Vector2f(1457 / (window->getSize().x * 0.0015), 2706 / (window->getSize().x * 0.0015)));
	aboard->setTexture(T_board);
	aboard->setPosition(Vector2f(window->getSize().x / 2 - aboard->getSize().x / 2, window->getSize().y / 2 - aboard->getSize().y / 2));
}
void setUpBoard(RectangleShape board[5][5], RenderWindow *window)
{
	double bSize = window->getSize().x / 24;
	double shiftX = 1.948;
	double shiftY = 1.5;
	double sep = 12.2;
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			board[x][y].setFillColor(Color::Cyan);
			board[x][y].setSize(Vector2f(bSize, bSize));
			board[x][y].setPosition(Vector2f((window->getSize().x * shiftX) / 5 + x * (bSize + bSize / sep), (window->getSize().y * shiftY) / 5 + y * (bSize + bSize / sep)));
		}
	}
}

void movePiece(Piece* p, Vector2i pos, RectangleShape board[5][5])
{
	p->setLocation(pos);
	p->setPosition(board[pos.x][pos.y].getPosition().x + (board[pos.x][pos.y].getLocalBounds().width/2 - p->getGlobalBounds().width/2), board[pos.x][pos.y].getPosition().y + (board[pos.x][pos.y].getLocalBounds().height / 2 - p->getGlobalBounds().height / 2));
}

void setPieces(Piece ** one, Piece ** two, RectangleShape board[5][5], RenderWindow *window)
{
	for (int i = 0; i < 5; i++)
	{
		if (i != 2)
		{
			one[i] = new Piece("blue", "Pawn", Vector2i(-1, -1), window);
			movePiece(one[i], Vector2i(i, 4), board);
			two[i] = new Piece("red", "Pawn", Vector2i(-1, -1), window);
			movePiece(two[i], Vector2i(i, 0), board);
		}
		else
		{
			one[i] = new Piece("blue", "King", Vector2i(-1, -1), window);
			movePiece(one[i], Vector2i(i, 4), board);
			two[i] = new Piece("red", "King", Vector2i(-1, -1), window);
			movePiece(two[i], Vector2i(i, 0), board);
		}
	} 
}

void disPieces(Piece ** one, Piece ** two, RenderWindow *window)
{
	for (int i = 0; i < 5; i++)
	{
		if (one[i]->isAlive())
		{
			window->draw(*one[i]);
		}
		if (two[i]->isAlive())
		{
			window->draw(*two[i]);
		}
	}
}

Vector2i clickedSqr(Vector2i lP, RectangleShape board[5][5])
{
	FloatRect currBox;
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			currBox = board[x][y].getGlobalBounds();
			if (currBox.contains((Vector2f)lP))
			{
				return Vector2i(x, y);
			}
		}
	}
	return Vector2i(-1, -1);
}

Piece* findSelected(Vector2i lP, Piece ** one, Piece ** two, RectangleShape board[5][5])
{
	Vector2i sPos = clickedSqr(lP, board);
	for (int i = 0; i < 5; i++)
	{

		if (one[i]->getLocation() == sPos)
		{
			return one[i];
		}
		else if (two[i]->getLocation() == sPos)
		{
			return two[i];
		}
	}
	return nullptr;
}

void snapObj(Piece *selected, Vector2i lP)
{
	selected->setPosition(lP.x - selected->getGlobalBounds().width/2, lP.y - selected->getGlobalBounds().height/2);
}

bool snapToSquare(Piece *selected, Piece ** one, Piece ** two, Vector2i lP, RectangleShape board[5][5])
{
	Vector2i sPos = clickedSqr(lP, board);
	Piece * target = findSelected(lP, one, two, board);
	//blue 1 red 2
	//checks if clicked object is a square
	if (sPos != Vector2i(-1,-1))
	{
		//piece on square
		if (target != nullptr)
		{
			//target player 1
			if (target->getTeam() == "blue")
			{
				if (selected->getTeam() == "blue")
				{
					//do nothing cause you cant place on your own piece
					return false;
				}
				else if (selected->getTeam() == "red")
				{
					target->kill();
					movePiece(selected, sPos, board);
					return true;
				}
			}
			//target player 2
			else if (target->getTeam() == "red")
			{
				if (selected->getTeam() == "blue")
				{
					target->kill();
					movePiece(selected, sPos, board);
					return true;
				}
				else if (selected->getTeam() == "red")
				{
					//do nothing cause you cant place on your own piece
					return false;
				}
			}
		}
		else
		{
			movePiece(selected, sPos, board);
			return true;
		}
	}
	else
	{
		return false;
	}
}
void setUpCardHolders(RectangleShape cH[6], RenderWindow* window)
{
	//775 450
	float factor = (float)window->getSize().x/600;
	float sizeX = 775/factor, sizeY = 450/factor;
	for (int i = 0; i < 6; i++)
	{
		cH[i].setFillColor(Color::Cyan);
		cH[i].setSize(Vector2f(sizeX, sizeY));
		switch (i)
		{
			case 0:
				cH[i].setPosition(Vector2f(0,0));
				break;
			case 1:
				cH[i].setPosition(Vector2f(500, 0));
				break;
			case 2:
				cH[i].setPosition(Vector2f(0, 300));
				break;
			case 3:
				cH[i].setPosition(Vector2f(1000, 300));
				break;
			case 4:
				cH[i].setPosition(Vector2f(0, 1000));
				break;
			case 5:
				cH[i].setPosition(Vector2f(300, 1000));
				break;
		}
	}
	return;
}

void disCardHolders(RectangleShape cH[6], RenderWindow* window)
{
	for (int i = 0; i < 6; i++)
	{
		window->draw(cH[i]);
	}
}