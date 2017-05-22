#ifndef __Solution_h__
#define __Solution_h__

#include <vector>

#include "Board.h"
#include "pMove.h"

using namespace std;


class Solution {
public:
	Solution(Board finalState, vector<pMove> solution, int numNodesVisited);
	Board getFinalState();
	vector<pMove> getSolution();
	int getNumNodesVisited();

private:
	Board _finalState;
	vector<pMove> _solution;
	int _numNodesVisited;

};

#endif
