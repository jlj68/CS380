#ifndef __SlidingBrickGame_h__
#define __SlidingBrickGame_h__

#include <vector>
#include <string>

using namespace std;

struct pMove {
	int piece;
	int direction;
};

class SlidingBrickGame
{
public:
	SlidingBrickGame(vector<vector<int> > state);
	int getWidth(void);
	int getHeight(void);
	static vector<vector<int> > readState(string fname);
	vector<vector<int> > getState(void);
	void setState(vector<vector<int> > state);
	void printState(void);
	static void printMove(pMove m);
	bool isComplete(void);
	vector<int> getAllPieces(void);
	vector<pMove> possibleMoves(int piece);
	vector<pMove> possibleMoves(void);
	void applyMove(pMove m);
	vector<vector<int> > applyMoveCloning(pMove m);
	static bool compareStates(vector<vector<int> > a, vector<vector<int> > b);
	vector<vector<int> > getNormalizedState(void);
	vector<vector<int> > getNormalizedState(vector<vector<int> > state);
	void normalizeState(void);
private:
	vector<vector<int> > _board;
};

#endif

