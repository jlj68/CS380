#ifndef __SearchNode_cpp__
#define __SearchNode_cpp__

#include <vector>
#include <cstddef>

#include "Board.h"
#include "pMove.h"

#include "SearchNode.h"

using namespace std;

SearchNode::SearchNode(void) { }

SearchNode::SearchNode(SearchNode* p, pMove* m, Board b) {
	SearchNode(p, m, b, 0, 0);
}

SearchNode::SearchNode(SearchNode* p, pMove* m, Board b, int g, int h) {
	_parent = p;
	_move = m;
	_board = b;

	_children = vector<SearchNode>();

	if (_parent == NULL) {
		_depth = 0;
	}
	else {
		_depth = (*_parent).getDepth() + 1;
	}

	_gCost = g;
	_hCost = h;
}

SearchNode SearchNode::normalize(void) {
	return SearchNode(_parent, _move, _board.normalize(), _gCost, _hCost);
}

pMove* SearchNode::getMove() {
	return _move;
}

Board SearchNode::getBoard() {
	return _board;
}

SearchNode* SearchNode::getParent() {
	return _parent;
}

vector<SearchNode> SearchNode::getChildren() {
	return _children;
}

int SearchNode::getDepth() {
	return _depth;
}

int SearchNode::getGCost() {
	return _gCost;
}

int SearchNode::getHCost() {
	return _hCost;
}

const int SearchNode::getCost() {
	return _gCost + _hCost;
}

void SearchNode::addChild(SearchNode t) {
	_children.push_back(t);
}

void SearchNode::addChild(pMove m, Board b) {
	_children.push_back(SearchNode(this, &m, b));
}

bool SearchNode::equals(SearchNode tn) {
	return getBoard().equals(tn.getBoard());
}

#endif

