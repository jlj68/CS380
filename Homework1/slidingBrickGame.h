#ifndef __SlidingBrickGame_h__
#define __SlidingBrickGame_h__

#include <vector>
#include <string>

using namespace std;

class SlidingBrickGame {
	public:
		SlidingBrickGame(vector<vector<int> > state);
		int getWidth(void);
		int getHeight(void);
		static vector<vector<int> > readState(string fname);
		void setState(vector<vector<int> > state);
		void printState(void);
		bool isComplete(void);
		bool possibleMoves(int piece);
	private:
		vector<vector<int> > _board;
};

#endif

