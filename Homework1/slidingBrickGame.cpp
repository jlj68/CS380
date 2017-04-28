#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "slidingBrickGame.h"
#include "split.cpp"

using namespace std;

SlidingBrickGame::SlidingBrickGame(vector<vector<int> > state) {
	SlidingBrickGame::setState(state);
}

int SlidingBrickGame::getWidth(void) {
	return _board[0].size();
}
int SlidingBrickGame::getHeight(void) {
	return _board.size();
}

vector<vector<int> > SlidingBrickGame::readState(string fname) {
	vector<vector<int> > state;
	string line;
	ifstream infile(fname.c_str());
	if (infile.is_open()) {
		getline(infile, line); // first line contains unneeded dimensions
		while (getline(infile, line)) {
			vector<string> vals = split(line, ',');
			vector<int> ivals;
			for (int i = 0; i < vals.size(); i++) {
				ivals.push_back(stoi(vals[i]));
			}
			state.push_back(ivals);
		}
		infile.close();
	}
	else {
		cout << "Unable to open file" << endl;
	}
	return state;
}

vector<vector<int> > SlidingBrickGame::getState(void) {
	vector<vector<int> > rv = _board;
	return rv;
}

void SlidingBrickGame::setState(vector<vector<int> > state) {
	_board = state;
}

void SlidingBrickGame::printState(void) {
	cout << getWidth() << "," << getHeight() << "," << endl;
	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			cout << _board[i][j] << ",";
		}
		cout << endl;
	}
}

void SlidingBrickGame::printMove(pMove m) {
	string direction;
	switch (m.direction) {
	case 0:
		direction = "up";
		break;
	case 1:
		direction = "down";
		break;
	case 2:
		direction = "left";
		break;
	case 3:
		direction = "right";
		break;
	}

	cout << "(" << m.piece << "," << direction << ")" << endl;
}


bool SlidingBrickGame::isComplete(void) {
	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			if (_board[i][j] == -1)
				return false;
		}
	}
	return true;
}

vector<int> SlidingBrickGame::getAllPieces(void) {
	vector<int> pieces;

	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			if (_board[i][j] >= 2)
				pieces.push_back(_board[i][j]);
		}
	}

	sort(pieces.begin(), pieces.end());
	pieces.erase(unique(pieces.begin(), pieces.end()), pieces.end());

	return pieces;
}

vector<pMove> SlidingBrickGame::possibleMoves(int piece) {
	bool moves[4] = { true, true, true, true };
	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			if (_board[i][j] == piece) {
				moves[0] = moves[0] && (_board[i - 1][j] == 0 || _board[i - 1][j] == piece || (piece == 2 && _board[i - 1][j] == -1));
				moves[1] = moves[1] && (_board[i + 1][j] == 0 || _board[i + 1][j] == piece || (piece == 2 && _board[i + 1][j] == -1));
				moves[2] = moves[2] && (_board[i][j - 1] == 0 || _board[i][j - 1] == piece || (piece == 2 && _board[i][j - 1] == -1));
				moves[3] = moves[3] && (_board[i][j + 1] == 0 || _board[i][j + 1] == piece || (piece == 2 && _board[i][j + 1] == -1));
			}
		}
	}

	vector<pMove> possibleMoves;
	for (int i = 0; i<4; i++)
	if (moves[i]) {
		pMove m;
		m.piece = piece;
		m.direction = i;
		possibleMoves.push_back(m);
	}

	return possibleMoves;
}

vector<pMove> SlidingBrickGame::possibleMoves(void) {
	vector<int> allPieces = getAllPieces();
	vector<pMove> allMoves;

	for (int i = 0; i<allPieces.size(); i++) {
		vector<pMove> tmp = possibleMoves(allPieces[i]);
		allMoves.insert(allMoves.end(), tmp.begin(), tmp.end());
	}

	return allMoves;
}

void SlidingBrickGame::applyMove(pMove m) {
	setState(applyMoveCloning(m));
}

vector<vector<int> > SlidingBrickGame::applyMoveCloning(pMove m) {
	int dx = 0;
	int dy = 0;

	switch (m.direction) {
	case 0:
		dx = 0;
		dy = -1;
		break;
	case 1:
		dx = 0;
		dy = 1;
		break;
	case 2:
		dx = -1;
		dy = 0;
		break;
	case 3:
		dx = 1;
		dy = 0;
		break;
	}

	vector<vector<int> > pieceLocations;
	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			if (_board[i][j] == m.piece) {
				vector<int> loc;
				loc.push_back(i);
				loc.push_back(j);
				pieceLocations.push_back(loc);
			}
		}
	}

	vector<vector<int> > newState = _board;
	for (int i = 0; i < pieceLocations.size(); i++) {
		vector<int> loc;
		if (dx < 0 || dy < 0) {
			loc = pieceLocations[i];
		}
		else {
			loc = pieceLocations[pieceLocations.size() - (i + 1)];
		}

		int y = loc[0];
		int x = loc[1];
		newState[y + dy][x + dx] = m.piece;
		newState[y][x] = 0;
	}

	return newState;
}

bool SlidingBrickGame::compareStates(vector<vector<int> > a, vector<vector<int> > b) {
	if (a.size() != b.size())
		return false;
	for (int i = 0; i < a.size(); i++) {
		if (a[i].size() != b[i].size())
			return false;
		for (int j = 0; j < a[i].size(); j++)
		if (a[i][j] != b[i][j])
			return false;
	}

	return true;
}

vector<vector<int> > SlidingBrickGame::getNormalizedState(void) {
	return getNormalizedState(_board);
}

vector<vector<int> > SlidingBrickGame::getNormalizedState(vector<vector<int> > state) {
	int nextIdx = 3;
	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < state[0].size(); j++) {
			if (state[i][j] == nextIdx)
				nextIdx++;
			else if (state[i][j] > nextIdx) {
				for (int k = 0; k < state.size(); k++)
				for (int l = 0; l < state[0].size(); l++) {
					if (state[k][l] == nextIdx)
						state[k][l] = state[i][j];
					else if (state[k][l] == state[i][j])
						state[k][l] = nextIdx;
				}
				nextIdx++;
			}
		}
	}
	return state;
}

void SlidingBrickGame::normalizeState(void) {
	_board = getNormalizedState();
}

