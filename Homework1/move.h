#ifndef __move_h__
#define __move_h__

#include "direction.h"

using namespace std;

class move {
	public:
		move(int piece, direction d);
		int getPiece(void);
		direction getDirection(void);
	private:
		int _piece;
		direction _direction;
};

#endif

