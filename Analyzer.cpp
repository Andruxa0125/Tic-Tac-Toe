#include <iostream>
#include <functional>
#include <chrono>
#include "Analyzer.h"
#include "Tic-Tac-Toe.h"
#include "ThreadPool.h"

using namespace std;
double Analyzer::MIN_PROB = -1.0;


/////////////////// MULTITHREADING IMPLEMENTATION BEGIN //////////////////////////
pair<int, int> Analyzer::getNextMoveUsingThreads()
{
	vector<vector<double>> v; // every vector will have following entries: score, total, row, col.
	vector<std::function<void()>> jobs;

	for (int j = 0; j < TicTacToe::GRID_SIZE; j++) // going over rows
	{
		for (int i = 0; i < TicTacToe::GRID_SIZE; i++) // going over columns
		{
			if (grid[j][i] == -1) // if wasn't filled
			{
				grid[j][i] = cross;
				v.push_back({ 0, 0, double(j), double(i) });
				int index = v.size() - 1;
				double& score = v[index][0];
				double& total = v[index][1];
				std::function<void()> task = bind(&Analyzer::evaluateThisMove, this, grid, !cross, ref(score), ref(total));
				jobs.push_back(task);
				grid[j][i] = -1;
			}
		}
	}
	auto start = std::chrono::high_resolution_clock::now();
	pool->addJobs(jobs);
	
	pool->waitForJobs();

	// adds around 1.25 sec to execution
	//while (!pool->areJobsDone()) {}

	cout << "MAIN THREAD: finished waiting";

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = (finish - start);
	elapsedTime += elapsed;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	double bestProb = Analyzer::MIN_PROB;
	pair<int, int> choosenCell;
	for (auto& vec : v)
	{
		//cout << "for pair (" << vec[2] << ", " << vec[3] << ") probability is " << vec[0] / vec[1] << endl;
		double winProb = vec[0] / vec[1];
		if (winProb > bestProb)
		{
			bestProb = winProb;
			choosenCell = make_pair(int(vec[2]), int(vec[3]));
		}
	}
	return choosenCell;
}
/////////////////// MULTITHREADING IMPLEMENTATION END///////////////////////

pair<int, int> Analyzer::getNextMove()
{
	auto start = std::chrono::high_resolution_clock::now();
	pair<int, int> res(-1,- 1); // initiate with -1, -1. if after looping remains the same, means we are done.
	pair<int, int> funcRes(-1, -1);
	double maxRes = Analyzer::MIN_PROB;
	int TOTAL = 0;
	amIAboutToLose(grid, !cross, funcRes); // if there is a move that gives a chance of winning with 1, use it.
	int fol = 0;
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
				fol += total;
				TOTAL += total;
				cout << "probability of winning is " << prob << " for row " << j << " and col " << i << endl;
				if (prob > maxRes)
				{
					res = make_pair(j, i); // update the result pair
					maxRes = prob;
				}
				grid[j][i] = -1; // backtrack.
			}
		}
	}
	cout << "TOTAL IS " << TOTAL << endl;
	if (funcRes != make_pair(-1, -1))
	{
		return maxRes == 1 ? res : funcRes;
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = (finish - start);
	elapsedTime += elapsed;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";
	return res;
}

void Analyzer::amIAboutToLose(const vector<vector<int>> &matrix, bool cr, pair<int, int>& p)
{
	// the purpose of this function is to check if an opponent can win
	// with his next move. e.g. if count is 2.
	// if returned pair isn't (-1, -1) then
	// we need to fill this cell.

	Analyzer::printMatrix(matrix);
	int count = 0;
	int cross = cr ? 1 : 0;

	for (int j = 0; j < TicTacToe::GRID_SIZE; j++)
	{
		for (int i = 0; i < TicTacToe::GRID_SIZE; i++)
		{
			if (matrix[j][i] == cross)
				count++;
			else if (matrix[j][i] == -1)
				p = make_pair(j, i);
		}
		if (count == 2 && p != make_pair(-1, -1))
			return;
		count = 0;
		p = make_pair(-1, -1);
	}

	for (int j = 0; j < TicTacToe::GRID_SIZE; j++)
	{
		for (int i = 0; i < TicTacToe::GRID_SIZE; i++)
		{
			if (matrix[i][j] == cross)
				count++;
			else if (matrix[i][j] == -1)
				p = make_pair(i, j);
		}
		if (count == 2 && p != make_pair(-1, -1))
			return;
		p = make_pair(-1, -1);
		count = 0;
	}


	for (int k = 0; k < TicTacToe::GRID_SIZE; k++)
	{
		if (matrix[k][k] == cross)
			count++;
		else if (matrix[k][k] == -1)
			p = make_pair(k, k);
	}
	if (count == 2 && p != make_pair(-1, -1))
		return;

	p = make_pair(-1, -1);
	count = 0;

	// 0 2; 1, 1; 2 0
	int col = 0;
	for (int k = TicTacToe::GRID_SIZE - 1; k >= 0; k--)
	{
		if (matrix[k][col] == cross)
			count++;
		else if (matrix[k][col] == -1)
			p = make_pair(k, col);
		col++;
	}

	if (count == 2 && p != make_pair(-1, -1))
		return;

	p = make_pair(-1, -1);
}

void Analyzer::evaluateThisMove(vector<vector<int>> matrix, bool cross, double& score, double& total)
{
	int result = Analyzer::checkGrid(matrix);
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
	Analyzer();
	grid = matrix;
	cross = cr;
}

Analyzer::Analyzer()
{
	pool = new ThreadPool(NUM_THREADS);
}

void Analyzer::setMatrixAndPlayer(const vector<vector<int>>& matrix, bool cr)
{
	grid = matrix;
	cross = cr;
}

int Analyzer::checkGrid(const vector<vector<int>>& matrix)
{
	// returns 2 if it is a draw
	// returns 1 if crosses won
	// returns 0 if circles won
	// returns -1 if no one has won yet
	// checks all horizontal rows
	for (int i = 0; i < 3; i++) {
		if (matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2] && matrix[i][0] != -1)
			return matrix[i][0];
	}

	// check all vertical rows
	for (int i = 0; i < 3; i++) {
		if (matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i] && matrix[0][i] != -1)
			return matrix[0][i];
	}

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