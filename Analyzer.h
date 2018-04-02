#include <vector>
using namespace std;

class Analyzer
{
private:
	vector<vector<int>> grid;
	bool cross;
	static double MIN_PROB;
public:
	int checkGrid(const vector<vector<int>>& matrix, bool cross, pair<int, int>& pos);
	static void printMatrix(const vector<vector<int>>& matrix);
	Analyzer(const vector<vector<int>>& matrix, bool cr);
	pair<int, int> getNextMove();
	void evaluateThisMove(vector<vector<int>> matrix, bool cross, double& score, double& total);
};