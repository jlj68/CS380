#ifndef __Heuristics_cpp__
#define __Heuristics_cpp__

#include <vector>
#include <climits>
#include <cstdlib>
#include <set>

#include "Board.h"
#include "pMove.h"
#include "CellTypes.cpp"
#include "Direction.cpp"

#include "Heuristics.h"

using namespace std;

int Heuristics::manhattanDistance(Board board) {
	int xDist;
	int yDist;
	vector<vector<int> > goalBrickPos = board.findPiece(GOALBRICK);
	vector<vector<int> > goalPos = board.findPiece(GOAL);

	if (goalPos.size() == 0) {
		return 0;
	}

	vector<int> goalBrickTopLeft = vector<int>();
	goalBrickTopLeft.push_back(goalBrickPos[0][0]);
	goalBrickTopLeft.push_back(goalBrickPos[0][1]);

	vector<int> goalTopLeft = vector<int>();
	goalTopLeft.push_back(goalPos[0][0]);
	goalTopLeft.push_back(goalPos[0][1]);

	yDist = abs(goalBrickTopLeft[0] - goalTopLeft[0]);
	xDist = abs(goalBrickTopLeft[1] - goalTopLeft[1]);

	return xDist + yDist;
}

int Heuristics::minSwapsAndMoves(Board board) {
	return manhattanDistance(board) + minNumSwapsInPath(board, getPathsToGoal);
}

int Heuristics::minNumSwapsInPath(Board board, vector<vector<vector<int> > >(*pathGetter)(Board)) {
	vector<vector<vector<int> > > pathsToGoal = pathGetter(board);
	int currentCell;
	int currentPieceCount;
	int minNumPieces = INT_MAX;
	bool invalidPath = false;
	set<int> foundPieces = {};

	if (pathsToGoal.empty()) {
		return 0;
	}

	for (int i = 0; i < pathsToGoal.size(); i++) {
		vector<vector<int> > path = pathsToGoal[i];
		currentPieceCount = 0;
		for (int j = 0; j < path.size(); j++) {
			vector<int> cell = path[j];
			currentCell = board.getCells()[cell[0]][cell[1]];
			if (currentCell == WALL) {
				invalidPath = true;
				break;
			}
			if (currentCell != GOALBRICK
				&& currentCell != EMPTY
				&& currentCell != GOAL
				&& foundPieces.find(currentCell) == foundPieces.end()) {
				currentPieceCount++;
				foundPieces.insert(currentCell);
			}
		}

		if (!invalidPath && currentPieceCount < minNumPieces) {
			minNumPieces = currentPieceCount;
		}

		if (minNumPieces == 0) {
			return 0;
		}
	}

	return minNumPieces < INT_MAX ? minNumPieces : 0;
}

vector<vector<vector<int> > > Heuristics::getPathsToGoal(Board board) {
	vector<vector<int> > goalBrickPos = board.findPiece(GOALBRICK);
	vector<vector<int> > goalPos = board.findPiece(GOAL);

	int LR = -1;
	int UD = -1;

	if (goalPos.size() == 0) {
		return vector<vector<vector<int> > >();
	}

	vector<int> goalBrickTopLeft = vector<int>();
	goalBrickTopLeft.push_back(goalBrickPos[0][0]);
	goalBrickTopLeft.push_back(goalBrickPos[0][1]);
	vector<int> goalTopLeft = vector<int>();
	goalTopLeft.push_back(goalPos[0][0]);
	goalTopLeft.push_back(goalPos[0][1]);

	vector<int> goalBrickBottomRight = vector<int>();
	goalBrickBottomRight.push_back(goalBrickPos[goalBrickPos.size() - 1][0]);
	goalBrickBottomRight.push_back(goalBrickPos[goalBrickPos.size() - 1][1]);

	int goalBrickHeight = goalBrickBottomRight[0] - goalBrickTopLeft[0] + 1;
	int goalBrickWidth = goalBrickBottomRight[1] - goalBrickTopLeft[1] + 1;

	if (goalBrickTopLeft[0] > goalTopLeft[0]) {
		UD = UP;
	}
	else if (goalBrickTopLeft[0] < goalTopLeft[0]) {
		UD = DOWN;
	}

	if (goalBrickTopLeft[1] > goalTopLeft[1]) {
		LR = LEFT;
	}
	else if (goalBrickTopLeft[1] < goalTopLeft[1]) {
		LR = RIGHT;
	}

	int xGoal = goalTopLeft[1];
	int yGoal = goalTopLeft[0];
	int xStart = goalBrickTopLeft[1];
	int yStart = goalBrickTopLeft[0];
	int xChange = (LR == -1) ? 0 : pMove::getPositionChange(LR)[1];
	int yChange = (UD == -1) ? 0 : pMove::getPositionChange(UD)[0];

	vector<vector<vector<int> > > pathList = vector<vector<vector<int> > >();
	getPaths(board, xGoal, yGoal, xStart, yStart, goalBrickWidth, goalBrickHeight, xChange, yChange, vector<vector<int> >(), pathList);
	return pathList;
}

int Heuristics::minEdgeSwapsAndMoves(Board board) {
	return manhattanDistance(board) + minNumSwapsInPath(board, Heuristics::getEdgePathsToGoal);
}

vector<vector<vector<int> > > Heuristics::getEdgePathsToGoal(Board board) {
	vector<vector<vector<int> > > pathList = vector<vector<vector<int> > >();
	vector<vector<int> > goalBrickPos = board.findPiece(GOALBRICK);
	vector<vector<int> > goalPos = board.findPiece(GOAL);

	int LR = -1;
	int UD = -1;

	if (goalPos.size() == 0) {
		return vector<vector<vector<int> > >();
	}

	vector<int> goalBrickTopLeft = vector<int>();
	goalBrickTopLeft.push_back(goalBrickPos[0][0]);
	goalBrickTopLeft.push_back(goalBrickPos[0][1]);
	vector<int> goalTopLeft = vector<int>();
	goalTopLeft.push_back(goalPos[0][0]);
	goalTopLeft.push_back(goalPos[0][1]);

	vector<int> goalBrickBottomRight = vector<int>();
	goalBrickBottomRight.push_back(goalBrickPos[goalBrickPos.size() - 1][0]);
	goalBrickBottomRight.push_back(goalBrickPos[goalBrickPos.size() - 1][1]);

	int goalBrickHeight = goalBrickBottomRight[0] - goalBrickTopLeft[0] + 1;
	int goalBrickWidth = goalBrickBottomRight[1] - goalBrickTopLeft[1] + 1;

	if (goalBrickTopLeft[0] > goalTopLeft[0]) {
		UD = UP;
	}
	else if (goalBrickTopLeft[0] < goalTopLeft[0]) {
		UD = DOWN;
	}

	if (goalBrickTopLeft[1] > goalTopLeft[1]) {
		LR = LEFT;
	}
	else if (goalBrickTopLeft[1] < goalTopLeft[1]) {
		LR = RIGHT;
	}

	int xGoal = goalTopLeft[1];
	int yGoal = goalTopLeft[0];
	int xStart = goalBrickTopLeft[1];
	int yStart = goalBrickTopLeft[0];
	int xChange = (LR == -1) ? 0 : pMove::getPositionChange(LR)[1];
	int yChange = (UD == -1) ? 0 : pMove::getPositionChange(UD)[0];
	int xPos = xStart;
	int yPos = yStart;
	vector<vector<int> > path1 = vector<vector<int> >();

	int xGoalWall;
	int yGoalWall;

	if (xGoal == 0) {
		xGoalWall = xGoal - xChange;
		yGoalWall = yGoal;
	}
	else {
		xGoalWall = xGoal;
		yGoalWall = yGoal - yChange;
	}

	while (xPos != xGoalWall) {
		xPos += xChange;
		vector<vector<int> > tmp = getBrickPositions(board, xPos, yPos, goalBrickWidth, goalBrickHeight);
		for (int i = 0; i < tmp.size(); i++) {
			path1.push_back(tmp[i]);
		}
	}

	while (yPos != yGoal) {
		yPos += yChange;
		vector<vector<int> > tmp = getBrickPositions(board, xPos, yPos, goalBrickWidth, goalBrickHeight);
		for (int i = 0; i < tmp.size(); i++) {
			path1.push_back(tmp[i]);
		}
	}
	if (xGoalWall != xGoal) {
		vector<vector<int> > tmp = getBrickPositions(board, xPos + xChange, yPos, goalBrickWidth, goalBrickHeight);
		for (int i = 0; i < tmp.size(); i++) {
			path1.push_back(tmp[i]);
		}
	}
	pathList.push_back(path1);

	xPos = xStart;
	yPos = yStart;
	vector<vector<int> > path2 = vector<vector<int> >();
	while (yPos != yGoalWall) {
		yPos += yChange;
		vector<vector<int> > tmp = getBrickPositions(board, xPos, yPos, goalBrickWidth, goalBrickHeight);
		for (int i = 0; i < tmp.size(); i++) {
			path2.push_back(tmp[i]);
		}
	}

	while (xPos != xGoal) {
		xPos += xChange;
		vector<vector<int> > tmp = getBrickPositions(board, xPos, yPos, goalBrickWidth, goalBrickHeight);
		for (int i = 0; i < tmp.size(); i++) {
			path2.push_back(tmp[i]);
		}
	}

	if (yGoalWall != yGoal) {
		vector<vector<int> > tmp = getBrickPositions(board, xPos, yPos + yChange, goalBrickWidth, goalBrickHeight);
		for (int i = 0; i < tmp.size(); i++) {
			path2.push_back(tmp[i]);
		}
	}
	pathList.push_back(path2);

	return pathList;
}

void Heuristics::getPaths(Board board, int xGoal, int yGoal, int xPos, int yPos, int pieceWidth, int pieceHeight, int xChange, int yChange, vector<vector<int> > path, vector<vector<vector<int> > > pathList){
	path = vector<vector<int> >(path);

	for (int i = 0; i < pieceWidth; i++) {
		for (int j = 0; j < pieceHeight; j++) {
			if (xPos + i < board.getWidth() && yPos + j < board.getHeight()) {
				vector<int> tmp = vector<int>();
				tmp.push_back(yPos + j);
				tmp.push_back(xPos + i);
				path.push_back(tmp);
			}
		}
	}
	if (xPos == xGoal && yPos == yGoal) {
		pathList.push_back(path);
	}
	else if ((xChange > 0 && xPos > xGoal) || (xChange < 0 && xPos < xGoal)) {
		return;
	}
	else if ((yChange > 0 && yPos > yGoal) || (yChange < 0 && yPos < yGoal)) {
		return;
	}
	else {
		if (xChange != 0 && xPos != xGoal) {
			getPaths(board, xGoal, yGoal, xPos + (xChange * pieceWidth), yPos, pieceWidth, pieceHeight, xChange, yChange, path, pathList);

		}
		if (yChange != 0 && yPos != yGoal) {
			getPaths(board, xGoal, yGoal, xPos, yPos + (yChange * pieceHeight), pieceWidth, pieceHeight, xChange, yChange, path, pathList);
		}
	}
}

vector<vector<int> > Heuristics::getBrickPositions(Board board, int xPos, int yPos, int brickWidth, int brickHeight) {
	vector<vector<int> > brickPositions = vector<vector<int> >();
	for (int i = 0; i < brickWidth; i++) {
		for (int j = 0; j < brickHeight; j++) {
			if (xPos + i < board.getWidth() && yPos + j < board.getHeight()) {
				vector<int> tmp = vector<int>();
				tmp.push_back(yPos + j);
				tmp.push_back(xPos + i);
				brickPositions.push_back(tmp);
			}
		}
	}
	return brickPositions;
}

#endif

