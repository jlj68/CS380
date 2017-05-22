#ifndef __pMove_h__
#define __pMove_h__

#include <vector>

using namespace std;

class pMove {
public:
	pMove(int piece, int dir);
	int getPiece();
	int getDirection();
	vector<int> getPositionChange();
	static vector<int> getPositionChange(int dir);
	bool equals(pMove move);
	void disp();
	bool operator==(pMove rhs);

private:
	int _piece;
	int _dir;

};

#endif
