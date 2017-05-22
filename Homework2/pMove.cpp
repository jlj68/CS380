#ifndef __pMove_cpp__
#define __pMove_cpp__

#include <vector>
#include <iostream>

#include "Direction.cpp"

#include "pMove.h"

using namespace std;

pMove::pMove(int piece, int dir) {
	_piece = piece;
	_dir = dir;
}

int pMove::getPiece() {
	return _piece;
}

int pMove::getDirection() {
	return _dir;
}

vector<int> pMove::getPositionChange() {
	return getPositionChange(_dir);
}

vector<int> pMove::getPositionChange(int dir) {
	vector<int> rv = vector<int>();
	if (dir == UP) {
		rv.push_back(-1);
		rv.push_back(0);
		return rv;
	}
	else if (dir == DOWN) {
		rv.push_back(1);
		rv.push_back(0);
		return rv;
	}
	else if (dir == LEFT) {
		rv.push_back(0);
		rv.push_back(-1);
		return rv;
	}
	else if (dir == RIGHT) {
		rv.push_back(0);
		rv.push_back(1);
		return rv;
	}
	else {
		rv.push_back(0);
		rv.push_back(0);
		return rv;
	}
}

bool pMove::equals(pMove move) {
	return (_piece == move.getPiece() && _dir == move.getDirection());
}

void pMove::disp() {
	cout << "[" << _piece << ":" << _dir << "]";
}

bool pMove::operator==(pMove rhs) {
	return this->equals(rhs);
}

#endif

