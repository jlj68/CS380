#ifndef __Solution_cpp__
#define __Solution_cpp__

#include <vector>

#include "Board.h"
#include "pMove.h"

#include "Solution.h"

using namespace std;


Solution::Solution(Board finalState, vector<pMove> solution, int numNodesVisited) {
	_finalState = finalState;
	_solution = solution;
	_numNodesVisited = numNodesVisited;
}

Board Solution::getFinalState() {
	return _finalState;
}
	
vector<pMove> Solution::getSolution() {
	return _solution;
}

int Solution::getNumNodesVisited() {
	return _numNodesVisited;
}

#endif

