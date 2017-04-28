#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "slidingBrickGame.h"
#include "mapIntToDir.cpp"

using namespace std;

bool oppositeMove(pMove a, pMove b) {
	if (a.piece != b.piece)
		return false;
	vector<int> aDir = mapIntToDir(a.direction);
	vector<int> bDir = mapIntToDir(b.direction);
	return (aDir[0] + bDir[0] == 0 && aDir[1] + bDir[1] == 0);
}

bool doneBefore(pair<vector<vector<int> >, pMove> a, vector<pair<vector<vector<int> >, pMove>> l) {
	for (int i = 0; i < l.size(); i++) {
		if (SlidingBrickGame::compareStates(a.first, l[i].first))
			if (a.second.direction == l[i].second.direction && a.second.piece == l[i].second.direction)
				return true;
	}
	return false;
}

void randomWalk(int n, SlidingBrickGame sbg) {
	int i = 0;
	srand(time(NULL));
	while (!sbg.isComplete() && i <= n) {
		vector<pMove> possibleMoves = sbg.possibleMoves();
		pMove currentMove;
		if (possibleMoves.size() > 1) {
			currentMove = possibleMoves[rand() % possibleMoves.size()];
		}
		else if (possibleMoves.size() == 0) {
			currentMove = possibleMoves[0];
		}
		else {
			break;
		}
		SlidingBrickGame::printMove(currentMove);
		sbg.applyMove(currentMove);
		sbg.normalizeState();
		sbg.printState();
	}
}

vector<pMove> BFS(SlidingBrickGame sbg, pMove lastMove, vector<pair<vector<vector<int> >, pMove>> previousMoves, bool firstCall = false) {
	sbg.printState();
	vector<pMove> moves;
	vector<pMove> possibleMoves = sbg.possibleMoves();
	for (int i = 0; i < possibleMoves.size(); i++) {
		pMove currentMove = possibleMoves[i];
		pair<vector<vector<int> >, pMove> lastStateMove(sbg.getState(), currentMove);
		if ((!firstCall && oppositeMove(lastMove, currentMove)) || doneBefore(lastStateMove, previousMoves)) {
			continue;
		}
		vector<vector<int> > newState = sbg.applyMoveCloning(currentMove);
		SlidingBrickGame sbg1(newState);
		sbg1.normalizeState();
		if (sbg1.isComplete()) {
			moves.push_back(currentMove);
			return moves;
		}
	}
	for (int i = 0; i < possibleMoves.size(); i++) {
		pMove currentMove = possibleMoves[i];
		pair<vector<vector<int> >, pMove> lastStateMove(sbg.getState(), currentMove);
		if ((!firstCall && oppositeMove(lastMove, currentMove)) || doneBefore(lastStateMove, previousMoves)) {
			continue;
		}
		vector<vector<int> > newState = sbg.applyMoveCloning(currentMove);
		SlidingBrickGame sbg1(newState);
		sbg1.normalizeState();
		vector<pair<vector<vector<int> >, pMove> > usedMoves = previousMoves;
		usedMoves.push_back(lastStateMove);
		vector<pMove> nextMoves = BFS(sbg1, currentMove, usedMoves);
		if (nextMoves.size() > 0) {
			moves.push_back(currentMove);
			for (int j = 0; j < nextMoves.size(); j++) {
				moves.push_back(nextMoves[j]);
			}
		}
	}
}

vector<pMove> DFS(SlidingBrickGame sbg, pMove lastMove, vector<pair<vector<vector<int> >, pMove>> previousMoves, bool firstCall = false) {
	sbg.printState();
	vector<pMove> moves;
	vector<pMove> possibleMoves = sbg.possibleMoves();
	for (int i = 0; i < possibleMoves.size(); i++) {
		pMove currentMove = possibleMoves[i];
		pair<vector<vector<int> >, pMove> lastStateMove(sbg.getState(), currentMove);
		if ((!firstCall && oppositeMove(lastMove, currentMove)) || doneBefore(lastStateMove, previousMoves)) {
			continue;
		}
		vector<vector<int> > newState = sbg.applyMoveCloning(currentMove);
		SlidingBrickGame sbg1(newState);
		sbg1.normalizeState();
		vector<pair<vector<vector<int> >, pMove> > usedMoves = previousMoves;
		usedMoves.push_back(lastStateMove);
		if (sbg1.isComplete()) {
			moves.push_back(currentMove);
			return moves;
		}
		else {
			vector<pMove> nextMoves = DFS(sbg1, currentMove, usedMoves);
			if (nextMoves.size() > 0) {
				moves.push_back(currentMove);
				for (int i = 0; i < nextMoves.size(); i++) {
					moves.push_back(nextMoves[0]);
				}
			}
		}

	}
}

void IDS(SlidingBrickGame sbg) {

}

int main(int argc, char *argv[]) {
	string fname("inputFiles/SBP-level0.txt");
	vector<vector<int> > s = SlidingBrickGame::readState(fname);
	SlidingBrickGame g(s);
	cout << "Random walk:" << endl;
	g.printState();
	randomWalk(10, g);

	//pMove startingMove;
	//startingMove.piece = 0;
	//startingMove.direction = 0;
	//vector<pair<vector<vector<int> >, pMove>> previousMoves;
	//BFS(g, startingMove, previousMoves, true);
}

