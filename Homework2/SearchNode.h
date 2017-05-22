#ifndef __SearchNode_h__
#define __SearchNode_h__

#include <vector>

#include "Board.h"
#include "pMove.h"

using namespace std;


class SearchNode {
public:
	SearchNode(void);
	SearchNode(SearchNode* p, pMove* m, Board b);
	SearchNode(SearchNode* p, pMove* m, Board b, int g, int h);
	SearchNode normalize(void);
	pMove* getMove();
	Board getBoard();
	SearchNode* getParent();
	vector<SearchNode> getChildren();
	int getDepth();
	int getGCost();
	int getHCost();
	const int getCost();
	void addChild(SearchNode t);
	void addChild(pMove m, Board b);
	bool equals(SearchNode tn);

private:
	pMove* _move;
	Board _board;
	SearchNode* _parent;
	vector<SearchNode> _children;
	int _depth;
	int _gCost;
	int _hCost;

};

#endif
