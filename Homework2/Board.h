#ifndef __Board_h__
#define __Board_h__

#include <vector>
#include "pMove.h"

using namespace std;

class Board {
public:
	Board(void);
	Board(vector<vector<int> > cells);
	int getWidth(void);
	int getHeight(void);
	vector<vector<int> > getCells(void);
	bool isNormalized(void);
	void setCell(int h, int w, int val);
	void setNormalized(bool n);
	Board clone(void);
	bool isSolved(void);
	vector<vector<int> > findPiece(int piece);
	vector<pMove> findMoves(int piece);
	vector<pMove> findAllMoves(void);
	bool applyMove(pMove m);
	Board applyMoveCloning(pMove m);
	void randomWalk(int n);
	Board normalize(void);
	bool equals(Board b);
	void disp(void);

private:
	vector<vector<int> > _cells;
	bool _isNormalized;

	vector<vector<int> > _cloneCells(void);
	bool _canMoveUp(vector<int> topLeft, int pieceWidth, bool isGoalBrick);
	bool _canMoveDown(vector<int> bottomRight, int pieceWidth, bool isGoalBrick);
	bool _canMoveLeft(vector<int> topLeft, int pieceHeight, bool isGoalBrick);
	bool _canMoveRight(vector<int> bottomRight, int pieceHeight, bool isGoalBrick);
	void _swapIdx(Board b, int idx1, int idx2);

};


#endif
