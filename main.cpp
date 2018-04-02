#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Tic-Tac-Toe.h"
#include "Analyzer.h"

/////    |--->
////     |
////     v 

// x goes to from left to rigth, y goes from top to bottom
//
//int WIDTH = 800;
//int HEIGHT = 500;
//const int THICKNESS = 20;
//const sf::Vector2f standardX(WIDTH, THICKNESS);
//const sf::Vector2f standardY(HEIGHT, THICKNESS);
using namespace std;
int main()
{
	TicTacToe game(800, 800);
	game.startGame();
	//vector<vector<int>> grid;
	//vector<int> temp(3, -1);
	//for (int i = 0; i < 3; i++)
	//	grid.push_back(temp);
	//grid[0][0] = 1;
	//grid[0][1] = 0;
	//grid[0][2] = 1;
	//grid[1][1] = 0;
	//grid[1][0] = 0;
	//grid[1][1] = 0;
	//grid[2][1] = 1;
	//Analyzer::printMatrix(grid);
	//Analyzer a(grid, false);
	//pair<int, int> res = a.getNextMove();
	//cout << "the final result is " << res.first << " and " << res.second << endl;

	return 0;
}