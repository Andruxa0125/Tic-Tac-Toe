#pragma once
#include <vector>
#include "ThreadPool.h"

using namespace std;

class Analyzer
{
private:
	static const int NUM_THREADS = 16;
	vector<vector<int>> grid;
	bool cross;
	ThreadPool* pool;
	chrono::duration<double> elapsedTime;
	static double MIN_PROB;
public:
	double getTotalTime() { return elapsedTime.count(); }
	//int checkGrid(const vector<vector<int>>& matrix, bool cross, pair<int, int>& pos);
	void amIAboutToLose(const vector<vector<int>> &matrix, bool cross, pair<int, int>& p);
	static int checkGrid(const vector<vector<int>>& matrix);
	static void printMatrix(const vector<vector<int>>& matrix);
	Analyzer(const vector<vector<int>>& matrix, bool cr);
	Analyzer();
	void setMatrixAndPlayer(const vector<vector<int>>& matrix, bool cr);
	pair<int, int> getNextMove();
	pair<int, int> getNextMoveUsingThreads();
	void evaluateThisMove(vector<vector<int>> matrix, bool cross, double& score, double& total);
	~Analyzer() { delete pool; };
};