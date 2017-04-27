#include "slidingBrickGame.h"
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
	string fname(argv[1]);
	vector<vector<int> > s = SlidingBrickGame::readState(fname);
	SlidingBrickGame g(s);
	g.printState();
}

