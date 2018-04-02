#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Tic-Tac-Toe.h"
#include "Analyzer.h"

using namespace std;

//class TicTacToe
//{
//private:
//	static constexpr int THICKNESS = 20;
//	int WIDTH;
//	int HEIGHT;
//	sf::Vector2f standardX;
//	sf::Vector2f standardY;
//public:
//	TicTacToe(int width, int height);
//	void startGame();
//};

void TicTacToe::fillMap()
{
	map[0] = make_pair(0, 0);
	map[1] = make_pair(WIDTH / 3, 0);
	map[2] = make_pair(2 * WIDTH / 3, 0);
	map[3] = make_pair(0, HEIGHT / 3 + THICKNESS);
	map[4] = make_pair(WIDTH / 3, HEIGHT / 3 + THICKNESS);
	map[5] = make_pair(2 * WIDTH / 3, HEIGHT / 3 + THICKNESS);
	map[6] = make_pair(0, 2 * HEIGHT / 3 + THICKNESS);
	map[7] = make_pair(WIDTH / 3, 2 * HEIGHT / 3 + THICKNESS);
	map[8] = make_pair(2 * WIDTH / 3, 2 * HEIGHT / 3 + THICKNESS);
	//fillCell(0, HEIGHT / 3 + THICKNESS, 1, 0);
	//fillCell(0, 2 * HEIGHT / 3 + THICKNESS, 2, 0);
	//fillCell(WIDTH / 3, 0, 0, 1);
	//fillCell(WIDTH / 3, HEIGHT / 3 + THICKNESS, 1, 1);
	//fillCell(WIDTH / 3, 2 * HEIGHT / 3 + THICKNESS, 2, 1);
	//fillCell(2 * WIDTH / 3, 0, 0, 2);
	//fillCell(2 * WIDTH / 3, 2 * HEIGHT / 3 + THICKNESS, 2, 2);
	//fillCell(2 * WIDTH / 3, HEIGHT / 3 + THICKNESS, 1, 2);
}

TicTacToe::TicTacToe(int width, int height)
{
	BASE_COLOR = sf::Color::Cyan;
	cross = true;
	WIDTH = width;
	HEIGHT = height;
	fillMap();
	standardX = {float(WIDTH), float(THICKNESS) };
	standardY = { float(HEIGHT), float(THICKNESS) };
	standardDiag = { float(sqrt((HEIGHT / 3)*(HEIGHT / 3) + (WIDTH / 3) * (WIDTH / 3)) - 70), float(THICKNESS/2) };
	vector<int> temp(3, -1);
	for (int i = 0; i < 3; i++)
		grid.push_back(temp);
}

TicTacToe::~TicTacToe()
{
	// destructor
	for (sf::Drawable* pointer : shapes)
		delete pointer;
	shapes.clear();
}

void  TicTacToe::fillCell(int row, int col)
{
	cout << "position is " << TicTacToe::computeIndex(row, col) << endl;
	auto leftCorner = map[TicTacToe::computeIndex(row, col)];
	int leftCornerX = leftCorner.first;
	int leftCornerY = leftCorner.second;
	cross = !cross;
	if (cross) // it was O
	{
		grid[row][col] = 0;
		sf::CircleShape* circle = new sf::CircleShape();
		circle->setPosition(leftCornerX + 10, leftCornerY + 10);
		circle->setRadius(WIDTH / 6 - 20);
		circle->setFillColor(BASE_COLOR);
		circle->setOutlineColor(sf::Color::Red);
		circle->setOutlineThickness(THICKNESS / 2);
		shapes.push_back(circle);
		return;
	}

	// it was X
	grid[row][col] = 1;
	sf::RectangleShape* diag1 = new sf::RectangleShape();
	diag1->setSize(standardDiag); // length, width
	diag1->setPosition(sf::Vector2f(leftCornerX + 20, leftCornerY + 20)); //first is x, second is y  ---> is x 
	diag1->setFillColor(sf::Color::Red);
	diag1->rotate(45);

	sf::RectangleShape* diag2 = new sf::RectangleShape();
	diag2->setSize(standardDiag); // length, width
	diag2->setPosition(sf::Vector2f(leftCornerX + WIDTH / 3 - THICKNESS - 10, leftCornerY + 30)); //first is x, second is y  ---> is x 
	diag2->setFillColor(sf::Color::Red);
	diag2->rotate(135);

	shapes.push_back(diag1);
	shapes.push_back(diag2);
}

vector<sf::Drawable*> TicTacToe::createShapes()
{
	vector<sf::Drawable*> objects;
	sf::RectangleShape* firstHor = new  sf::RectangleShape();
	firstHor->setSize(standardX); // length, width
	firstHor->setPosition(sf::Vector2f(0, HEIGHT / 3)); //first is x, second is y  ---> is x
	firstHor->setFillColor(sf::Color::Black);

	sf::RectangleShape* secondHor = new sf::RectangleShape();
	secondHor->setSize(standardX); // length, width
	secondHor->setPosition(sf::Vector2f(0, 2 * HEIGHT / 3)); //first is x, second is y  ---> is x 
	secondHor->setFillColor(sf::Color::Black);

	sf::RectangleShape* firstVert = new sf::RectangleShape();
	firstVert->setSize(standardY);
	firstVert->setPosition(sf::Vector2f(WIDTH / 3, 0)); //first is x, second is y  ---> is x
	firstVert->setFillColor(sf::Color::Black);
	firstVert->rotate(90);

	sf::RectangleShape* secondVert = new  sf::RectangleShape();
	secondVert->setSize(standardY);
	secondVert->setPosition(sf::Vector2f(2 * WIDTH / 3, 0));
	secondVert->setFillColor(sf::Color::Black);
	secondVert->rotate(90);

	objects.push_back(firstHor);
	objects.push_back(secondHor);
	objects.push_back(firstVert);
	objects.push_back(secondVert);
	return objects;
}

void TicTacToe::drawShapes(sf::RenderWindow& window)
{
	for (auto& shape : shapes)
	{
		window.draw(*shape);
	}
}

void TicTacToe::startGame()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Tic-Tac-Toe");
	window.clear(sf::Color::Green);

	vector<sf::Drawable*> objects = createShapes(); // this will create all the lines for Tic-Tac-Toe
	for (int i = 0; i < objects.size(); i++)
	{
		shapes.push_back(objects[i]);
	}

	bool compute = false;
	while (window.isOpen())
	{
		sf::Event event;
		WIDTH = window.getSize().x;
		HEIGHT = window.getSize().y;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				int mouseX = event.mouseButton.x;
				int mouseY = event.mouseButton.y;
				if (mouseX > 0 && mouseX < (WIDTH / 3 - THICKNESS)) // region 0, 3, or 6
				{
					if (mouseY > 0 && mouseY < HEIGHT / 3)
					{
						//region 0
						if (grid[0][0] == -1) // has not been filled yet
						{
							fillCell(0, 0);
						}
					}
					else if (mouseY > HEIGHT / 3 + THICKNESS && mouseY < 2 * HEIGHT / 3)
					{
						// region 3
						if (grid[1][0] == -1)
						{
							fillCell(1, 0);
						}
					}
					else if (mouseY > 2 * HEIGHT / 3 + THICKNESS && mouseY < HEIGHT)
					{
						// region 6
						if (grid[2][0] == -1)
						{
							fillCell(2, 0);
						}
					}
				}

				else if (mouseX > WIDTH / 3 && mouseX < (2 * WIDTH / 3 - THICKNESS)) // region 1, 4, or 7
				{
					if (mouseY > 0 && mouseY < HEIGHT / 3)
					{
						// region 1
						if (grid[0][1] == -1) // has not been filled yet
						{
							fillCell(0, 1);
							//grid[0][0] = 1;
						}
					}
					else if (mouseY > HEIGHT / 3 + THICKNESS && mouseY < 2 * HEIGHT / 3)
					{
						// region 4
						if (grid[1][1] == -1) // has not been filled yet
						{
							fillCell(1, 1);
						}
					}
					else if (mouseY > 2 * HEIGHT / 3 + THICKNESS && mouseY < HEIGHT)
					{
						// region 7
						if (grid[2][1] == -1) // has not been filled yet
						{
							fillCell(2, 1);
						}
					}
				}
				else if (mouseX > (2 * WIDTH / 3) && mouseX < WIDTH) // region 2, 5, or 9
				{
					if (mouseY > 0 && mouseY < HEIGHT / 3)
					{
						// region 2
						if (grid[0][2] == -1) // has not been filled yet
						{
							fillCell(0, 2);
						}
					}
					else if (mouseY > HEIGHT / 3 + THICKNESS && mouseY < 2 * HEIGHT / 3)
					{
						// region 5
						if (grid[1][2] == -1) // has not been filled yet
						{
							fillCell(1, 2);
						}
					}
					else if (mouseY > 2 * HEIGHT / 3 + THICKNESS && mouseY < HEIGHT)
					{
						// region 9
						if (grid[2][2] == -1) // has not been filled yet
						{
							fillCell(2, 2);
						}
					}
				}
				compute = true;
			}
		}
		window.clear(BASE_COLOR);
		drawShapes(window);
		window.display();
		if (compute)
		{
			compute = false;
			Analyzer a(grid, cross);
			pair<int, int> res = a.getNextMove();
			cout << "the final result is " << res.first << " and " << res.second << endl;
			fillCell(res.first, res.second);
			window.clear(BASE_COLOR);
			drawShapes(window);
			window.display();
		}
	}
}

inline int TicTacToe::computeIndex(int row, int col)
{
	return  row * TicTacToe::GRID_SIZE + col;
}