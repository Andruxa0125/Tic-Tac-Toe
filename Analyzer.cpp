#include <iostream>
#include "Analyzer.h"
#include "Tic-Tac-Toe.h"

using namespace std;
double Analyzer::MIN_PROB = -1.0;

pair<int, int> Analyzer::getNextMove()
{
	pair<int, int> res(-1,- 1); // initiate with -1, -1. if after looping remains the same, means we are done.
	double maxRes = Analyzer::MIN_PROB;
	for (int j = 0; j < TicTacToe::GRID_SIZE; j++) // going over rows
	{
		for (int i = 0; i < TicTacToe::GRID_SIZE; i++) // going over columns
		{
			if (grid[j][i] == -1) // if wasn't filled
			{
				grid[j][i] = cross;
				double score = 0;
				double total = 0;
				evaluateThisMove(grid, !cross, score, total); // evaluate how well will this move perform
				double prob = score / total;
				cout << "probability of winning is " << prob << " for row " << j << " and col " << i << endl;
				cout << " maxRes is " << maxRes << endl;
				if (prob > maxRes)
				{
					cout << "updating probability as " << prob << " is bigger than " << maxRes << endl;
					res = make_pair(j, i); // update the result pair
					maxRes = prob;
				}
				grid[j][i] = -1; // backtrack.
			}
		}
	}
	return res;
}

void Analyzer::evaluateThisMove(vector<vector<int>> matrix, bool cross, double& score, double& total)
{
	pair <int, int> p;
	int result = checkGrid(matrix, cross, p);
	if (result >= 0) // the game is over
	{
		if (result == this->cross) // this is a victory 
		{
			score += 1;
		}
		total++; // total number of games.
		return;
	}

	for (int j = 0; j < TicTacToe::GRID_SIZE; j++)
	{
		for (int i = 0; i < TicTacToe::GRID_SIZE; i++)
		{
			if (matrix[j][i] == -1)
			{
				matrix[j][i] = cross;
				//Analyzer::printMatrix(matrix);
				evaluateThisMove(matrix, !cross, score, total);
				matrix[j][i] = -1;
			}
		}
	}
}

Analyzer::Analyzer(const vector<vector<int>>& matrix, bool cr)
{
	grid = matrix;
	cross = cr;
}

int Analyzer::checkGrid(const vector<vector<int>>& matrix, bool cross, pair<int, int> &p)
{
	// returns 2 if it is a draw
	// returns 1 if crosses won
	// returns 0 if circles won
	// returns -1 if no one has won yet

	int x = 0;
	int o = 0;

	// checks all horizontal rows
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++) {
			auto num = matrix[i][k];
			if (num == 1)
				x++;
			else if (num == 0)
				o++;
			else if (num == -1)
				p = make_pair(i, k);
		}
		if (x == 3)
			return 1;
		else if (o == 3)
			return 0;

		else if(cross && o == 2) // we play for cross=> check for two o's.
			bla
		else if(!cross && x == 2) // we play for o's and there are two crosses
			bla

		/*if (matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2] && matrix[i][0] != -1)
			return matrix[i][0];*/
	}

	// checks all vertical rows
	for (int i = 0; i < 3; i++)
	{
		if (matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i] && matrix[0][i] != -1)
			return matrix[0][i];
	}

	// checks one diagonal
	if (matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2] && matrix[0][0] != -1)
		return matrix[0][0];

	if (matrix[2][0] == matrix[1][1] && matrix[1][1] == matrix[0][2] && matrix[1][1] != -1)
		return matrix[1][1];

	// at this point we know that no one has won yet. either a draw or not finished yet.

	for (auto& row : matrix)
		for (auto& num : row)
			if (num == -1)
				return -1;

	return 2;
}

void Analyzer::printMatrix(const vector<vector<int>>& matrix)
{
	cout << endl;
	for (auto& row : matrix) {
		for (auto& num : row) {
			cout << num << " ";
		}
		cout << endl;
	}
	cout << endl;
}