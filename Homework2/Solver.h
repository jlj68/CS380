#ifndef __Solver_h__
#define __Solver_h__

#include <vector>
#include <map>

#include "Board.h"
#include "Solution.h"
#include "pMove.h"
#include "SearchNode.h"

using namespace std;


class Solver {
public:
	static Solution* BreadthFirstSearch(Board board);
	static Solution* DepthFirstSearch(Board board);
	static Solution* IterativeDeepeningSearch(Board board);
	static Solution* DepthLimitedSearch(Board board, int depthLimit);
	static Solution* aStar(Board board, int(*heuristic)(Board));
	static void printMoves(vector<pMove> solution);

private:
	static bool shouldActOnNode(SearchNode node, map<SearchNode, int> table);
	static vector<pMove> backTrack(SearchNode goal);

};

#endif
