#include "move.h"

#include "direction.h"

using namespace std;

move::move(int piece, direction d) {
	_piece = piece;
	_direction = d;
}

int move::getPiece(void) {
	return _piece;
}

direction move::getDirection(void) {
	return _direction;
}

