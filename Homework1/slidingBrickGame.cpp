#include "slidingBrickGame.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "direction.h"
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
			for (int i=0; i < vals.size(); i++) {
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

void SlidingBrickGame::setState(vector<vector<int> > state) {
	_board = state;
}

void SlidingBrickGame::printState(void) {
	cout << getWidth() << "," << getHeight() << "," << endl;
	for (int i=0; i < getHeight(); i++) {
		for (int j=0; j < getWidth(); j++)
			cout << _board[i][j] << ",";
		cout << endl;
	}
}

bool SlidingBrickGame::isComplete(void) {
	for (int i=0; i < getHeight(); i++)
		for (int j=0; j < getWidth(); j++)
			if (_board[i][j] == -1)
				return false;
	return true;
}

bool SlidingBrickGame::possibleMoves(int piece) {
	bool moves[4] = {true, true, true, true};
	for (int i=0; i < getHeight(); i++)
		for (int j=0; j < getWidth(); j++)
			if (_board[i][j] == piece) {
				moves[0] = moves[0] && (_board[i-1][j] == 0 || _board[i-1][j] == -1);
				moves[1] = moves[1] && (_board[i+1][j] == 0 || _board[i+1][j] == -1);
				moves[2] = moves[2] && (_board[i][j-1] == 0 || _board[i][j-1] == -1);
				moves[3] = moves[3] && (_board[i][j+1] == 0 || _board[i][j+1] == -1);
			}

	vector<direction> d;
	for (int i=0; i<4; i++)
		if (moves[i])
			d.push_back(static_cast<direction>(i));

	return &d[0];
}

