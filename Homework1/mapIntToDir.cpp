#include <vector>

using namespace std;

vector<int> mapIntToDir(int n) {
	int dx, dy;
	switch (n) {
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
	vector<int> rv;
	rv.push_back(dx);
	rv.push_back(dy);
	return rv;
}

