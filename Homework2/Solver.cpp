#ifndef __Solver_cpp__
#define __Solver_cpp__

#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <climits>

#include "Board.h"
#include "Solution.h"
#include "pMove.h"
#include "SearchNode.h"

#include "Solver.h"

using namespace std;


Solution* Solver::BreadthFirstSearch(Board board) {
	queue<SearchNode> openQueue = queue<SearchNode>();
	set<SearchNode> closedSet = {};

	vector<pMove> availableMoves;

	SearchNode root = SearchNode(NULL, NULL, board);
	openQueue.push(root);

	while (!openQueue.empty()) {
		SearchNode currentNode = openQueue.front();
		openQueue.pop();

		if (closedSet.find(currentNode.normalize()) != closedSet.end()) {
			// Node was already closed
			continue;
		}

		// close current node
		closedSet.insert(currentNode.normalize());

		Board currentBoard = currentNode.getBoard();
		if (currentBoard.isSolved()) {
			return new Solution(currentBoard, backTrack(currentNode), closedSet.size());
		}

		// Add all unique children to currentNode
		availableMoves = currentBoard.findAllMoves();
		for (int i = 0; i < availableMoves.size(); i++) {
			pMove currentMove = availableMoves[i];
			SearchNode currentChild = SearchNode(&currentNode, &currentMove, currentBoard.applyMoveCloning(currentMove));
			if (closedSet.find(currentChild.normalize()) == closedSet.end()) {
				currentNode.addChild(currentChild.normalize());
			}
		}

		vector<SearchNode> currentChildren = currentNode.getChildren();
		for (int i = 0; i < currentChildren.size(); i++) {
			openQueue.push(currentChildren[i]);
		}
	}

	return NULL;
}

Solution* Solver::DepthFirstSearch(Board board) {
	stack<SearchNode> openStack = stack<SearchNode>();
	set<SearchNode> closedSet = {};

	vector<pMove> availableMoves;

	SearchNode root = SearchNode(NULL, NULL, board);
	openStack.push(root);

	while (!openStack.empty()) {
		SearchNode currentNode = openStack.top();
		openStack.pop();

		if (closedSet.find(currentNode.normalize()) != closedSet.end()) {
			// Node was already closed
			continue;
		}

		// Add current node to the closed set
		closedSet.insert(currentNode.normalize());

		Board currentBoard = currentNode.getBoard();

		if (currentBoard.isSolved()) {
			return new Solution(currentBoard, backTrack(currentNode), closedSet.size());
		}

		// Add all unique children to currentNode
		availableMoves = currentBoard.findAllMoves();
		for (int i = 0; i < availableMoves.size(); i++) {
			pMove currentMove = availableMoves[i];
			SearchNode currentChild = SearchNode(&currentNode, &currentMove, currentBoard.applyMoveCloning(currentMove));
			if (closedSet.find(currentChild.normalize()) == closedSet.end()) {
				currentNode.addChild(currentChild.normalize());
			}
		}

		// Add all of the unique children to the openStack
		vector<SearchNode> currentChildren = currentNode.getChildren();
		for (int i = 0; i < currentChildren.size(); i++) {
			openStack.push(currentChildren[i]);
		}
	}

	return NULL;
}

Solution* Solver::IterativeDeepeningSearch(Board board) {
	int numNodesVisited = 0;
	for (int i = 0; i < INT_MAX; i++) {
		Solution* solution = DepthLimitedSearch(board, i);
		if (solution != NULL) {
			numNodesVisited += (*solution).getNumNodesVisited();
		}

		if (solution != NULL && !(*solution).getSolution().empty()) {
			return new Solution((*solution).getFinalState(), (*solution).getSolution(), numNodesVisited);
		}
	}
	return NULL;
}

Solution* Solver::DepthLimitedSearch(Board board, int depthLimit) {
	stack<SearchNode> openStack = stack<SearchNode>();
	set<SearchNode> closedSet = {};
	map<SearchNode, int> closedTable;

	vector<pMove> availableMoves;
	bool depthLimitReached = false;

	SearchNode root = SearchNode(NULL, NULL, board);
	openStack.push(root);

	while (!openStack.empty()) {
		depthLimitReached = false;
		SearchNode currentNode = openStack.top();
		openStack.pop();

		if (closedSet.find(currentNode.normalize()) != closedSet.end()) {
			// Node was already closed
			continue;
		}

		// Add current node to the closed set
		closedSet.insert(currentNode.normalize());
		closedTable[currentNode.normalize()] = currentNode.getDepth();

		Board currentBoard = currentNode.getBoard();

		if (currentBoard.isSolved()) {
			return new Solution(currentBoard, backTrack(currentNode), closedSet.size());
		}

		// Add all unique children to currentNode
		availableMoves = currentBoard.findAllMoves();
		for (int i = 0; i < availableMoves.size(); i++) {
			pMove currentMove = availableMoves[i];
			SearchNode currentChild = SearchNode(&currentNode, &currentMove, currentBoard.applyMoveCloning(currentMove));
			if (currentChild.getDepth() <= depthLimit) {
				if (shouldActOnNode(currentChild, closedTable)) {
					currentNode.addChild(currentChild.normalize());
				}
			}
			else {
				depthLimitReached = true;
			}
		}

		vector<SearchNode> currentChildren = currentNode.getChildren();
		for (int i = 0; i < currentChildren.size(); i++) {
			openStack.push(currentChildren[i]);
		}
	}

	if (depthLimitReached) {
		cout << "Depth limit reached." << endl;
	}

	return NULL;
}

bool operator<(SearchNode lhs, SearchNode rhs) {
	return lhs.getCost() > rhs.getCost(); // opposite of < to force priority queue to be a min_queue
}

bool contains(vector<SearchNode> v, SearchNode x) {
	for (int i = 0; i < v.size(); i++) {
		SearchNode n = v[i];
		if (n.equals(x)) {
			return true;
		}
	}
	return false;
}

Solution* Solver::aStar(Board board, int(*heuristic)(Board)) {
	int MOVE_COST = 1;

	priority_queue<SearchNode> openQueue;
	vector<SearchNode> closedSet = {};

	vector<pMove> availableMoves;

	SearchNode* root = new SearchNode(NULL, NULL, board, 0, heuristic(board));
	openQueue.push(*root);

	while (!openQueue.empty()) {
		SearchNode* currentNode = new SearchNode(openQueue.top());
		openQueue.pop();
		
		if (contains(closedSet, *currentNode)) {
			// Node was already closed
			continue;
		}

		closedSet.push_back(currentNode->normalize());

		Board currentBoard = currentNode->getBoard();

		if (currentBoard.isSolved()) {
			return new Solution(currentBoard, backTrack(*currentNode), closedSet.size());
		}

		// Add all unique children to currentNode
		availableMoves = currentBoard.findAllMoves();
		for (int i = 0; i < availableMoves.size(); i++) {
			pMove* currentMove = new pMove(availableMoves[i]);
			Board movedBoard = currentBoard.applyMoveCloning(*currentMove);

			SearchNode* currentChild = new SearchNode(currentNode, currentMove, movedBoard, currentNode->getGCost() + MOVE_COST, heuristic(movedBoard));
			currentChild = new SearchNode(currentChild->normalize());
			if (!contains(closedSet, *currentChild)) {
				currentNode->addChild(*currentChild);
			}
		}
		
		vector<SearchNode> currentChildren = currentNode->getChildren();
		for (int i2 = 0; i2 < currentChildren.size(); i2++) {
			openQueue.push(currentChildren[i2]);
		}
	}

	return NULL;
}

void Solver::printMoves(vector<pMove> solution) {
	for (int i = 0; i < solution.size(); i++) {
		pMove move = solution[i];
		move.disp();
		cout << endl;
	}
}

bool Solver::shouldActOnNode(SearchNode node, map<SearchNode, int> table) {
	int otherDepth = table[SearchNode(node.getParent(), node.getMove(), node.getBoard().normalize())];
	if ((otherDepth == NULL || node.getDepth() < otherDepth)) {
		return true;
	}
	return false;
}

vector<pMove> Solver::backTrack(SearchNode goal) {
	vector<pMove> solution = vector<pMove>();
	SearchNode currentNode = goal;

	while (currentNode.getParent() != NULL) {
		solution.push_back(*currentNode.getMove());
		currentNode = *(currentNode.getParent());
	}
	reverse(solution.begin(), solution.end()); // need to reverse since they were added in reverse order
	return solution;
}

#endif

