#ifndef __Heuristics_h__
#define __Heuristics_h__

#include <vector>

#include "Board.h"

using namespace std;

class Heuristics {
public:
	static int manhattanDistance(Board board);
	static int manhattanDistancePlusOne(Board board);
	static int minSwapsAndMoves(Board board);
	static int minNumSwapsInPath(Board board, vector<vector<vector<int> > >(*pathGetter)(Board));
	static vector<vector<vector<int> > > getPathsToGoal(Board board);
	static int minEdgeSwapsAndMoves(Board board);
	static vector<vector<vector<int> > > getEdgePathsToGoal(Board board);

private:
	static void getPaths(Board board, int xGoal, int yGoal, int xPos, int yPos, int pieceWidth, int pieceHeight, int xChange, int yChange, vector<vector<int> > path, vector<vector<vector<int> > > pathList);
	static vector<vector<int> > getBrickPositions(Board board, int xPos, int yPos, int brickWidth, int brickHeight);

};

#endif
