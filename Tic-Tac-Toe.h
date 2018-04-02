#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

using namespace std;

class TicTacToe
{
private:
	unordered_map<int, pair<int, int>> map;
	static int computeIndex(int row, int col);
	void fillMap();
	static constexpr int THICKNESS = 20;
	sf::Color BASE_COLOR;
	int WIDTH;
	int HEIGHT;
	sf::Vector2f standardX;
	sf::Vector2f standardY;
	sf::Vector2f standardDiag;
	vector<sf::Drawable*> shapes;
	void  fillCell(int row, int col);
	vector<vector<int>> grid;
	bool cross;
public:
	static const int GRID_SIZE = 3;
	TicTacToe(int width, int height);
	~TicTacToe();
	void drawShapes(sf::RenderWindow& window);
	vector<sf::Drawable*> createShapes();
	void startGame();
};