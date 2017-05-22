#ifndef __Board_cpp__
#define __Board_cpp__

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

#include "pMove.h"
#include "CellTypes.cpp"
#include "Direction.cpp"

#include "Board.h"


Board::Board(void) { }
	
Board::Board(vector<vector<int> > cells) {
	_cells = cells;
	_isNormalized = false;
}

int Board::getWidth(void) {
	return _cells[0].size();
}

int Board::getHeight(void) {
	return _cells.size();
}

vector<vector<int> > Board::getCells(void) {
	return _cells;
}

bool Board::isNormalized(void) {
	return _isNormalized;
}

void Board::setCell(int h, int w, int val) {
	_cells[h][w] = val;
}

void Board::setNormalized(bool n) {
	_isNormalized = n;
}

Board Board::clone(void) {
	Board b = Board(_cloneCells());
	return b;
}

bool Board::isSolved(void) {
	for (int i = 0; i < _cells.size(); i++) {
		for (int j = 0; j < _cells[i].size(); j++) {
			if (_cells[i][j] == GOAL) {
				return false;
			}
		}
	}
	return true;
}

vector<vector<int> > Board::findPiece(int piece) {
	vector<vector<int> > piecePositions = vector<vector<int> >();
	for (int i = 0; i < _cells.size(); i++) {
		for (int j = 0; j < _cells[i].size(); j++) {
			if (_cells[i][j] == piece) {
				vector<int> tmp = vector<int>();
				tmp.push_back(i);
				tmp.push_back(j);
				piecePositions.push_back(tmp);
			}
		}
	}
	return piecePositions;
}

vector<pMove> Board::findMoves(int piece) {
	vector<pMove> moves = vector<pMove>();
	vector<int> topLeft;
	vector<int> bottomRight;
	int pieceWidth;
	int pieceHeight;
	bool isGoalBrick;

	if (piece < 2) {
		cout << "Piece " << piece << " is not a piece that can move." << endl;
		return moves;
	}

	if (isSolved()) {
		return moves;
	}

	if (piece == GOALBRICK) {
		isGoalBrick = true;
	}
	else {
		isGoalBrick = false;
	}

	vector<vector<int> > piecePositions = findPiece(piece);

	topLeft = vector<int>();
	topLeft.push_back(piecePositions[0][0]);
	topLeft.push_back(piecePositions[0][1]);

	bottomRight = vector<int>();
	bottomRight.push_back(piecePositions[piecePositions.size() - 1][0]);
	bottomRight.push_back(piecePositions[piecePositions.size() - 1][1]);

	pieceHeight = bottomRight[0] - topLeft[0] + 1;
	pieceWidth = bottomRight[1] - topLeft[1] + 1;

	if (_canMoveUp(topLeft, pieceWidth, isGoalBrick)) {
		moves.push_back(pMove(piece, UP));
	}

	if (_canMoveDown(bottomRight, pieceWidth, isGoalBrick)) {
		moves.push_back(pMove(piece, DOWN));
	}

	if (_canMoveLeft(topLeft, pieceHeight, isGoalBrick)) {
		moves.push_back(pMove(piece, LEFT));
	}

	if (_canMoveRight(bottomRight, pieceHeight, isGoalBrick)) {
		moves.push_back(pMove(piece, RIGHT));
	}

	return moves;
}

vector<pMove> Board::findAllMoves(void) {
	vector<pMove> moves = vector<pMove>();
	vector<int> pieces = vector<int>();
	int piece;

	for (int i = 0; i < _cells.size(); i++) {
		for (int j = 0; j < _cells[i].size(); j++) {
			piece = _cells[i][j];
			if (find(pieces.begin(), pieces.end(), piece) == pieces.end() && piece > 1) {
				pieces.push_back(piece);
			}
		}
	}

	for (int i = 0; i < pieces.size(); i++) {
		int p = pieces[i];
		vector<pMove> tmp = findMoves(p);
		for (int j = 0; j < tmp.size(); j++) {
			moves.push_back(tmp[j]);
		}
	}

	return moves;
}

bool Board::applyMove(pMove m) {
	vector<pMove> moves = findAllMoves();
	int newX;
	int newY;

	if (find(moves.begin(), moves.end(), m) == moves.end()) {
		cout << "Move ";
		m.disp();
		cout << " is invalid." << endl;
		return false;
	}

	vector<vector<int> > piecePositions = findPiece(m.getPiece());
	if (piecePositions.size() < 1) {
		cout << "Piece " << m.getPiece() << " was not found on the board." << endl;
		return false;
	}

	for (int i = 0; i < piecePositions.size(); i++) {
		vector<int> pos = piecePositions[i];
		_cells[pos[0]][pos[1]] = EMPTY;
	}

	for (int i = 0; i < piecePositions.size(); i++) {
		vector<int> pos = piecePositions[i];
		newY = pos[0] + m.getPositionChange()[0];
		newX = pos[1] + m.getPositionChange()[1];

		_cells[newY][newX] = m.getPiece();
	}

	return true;
}

Board Board::applyMoveCloning(pMove m) {
	Board c = clone();
	c.applyMove(m);
	return c;
}

void Board::randomWalk(int n) {
	vector<pMove> moves;
	int chosenMove;

	Board toDisp = normalize();
	toDisp.disp();
	cout << endl;

	for (int i = 0; i < n; i++) {
		moves = findAllMoves();
		if (isSolved()) {
			cout << "Puzzle was solved in " << i << " moves" << endl;
			break;
		}
		if (moves.size() == 0) {
			break;
		}

		srand(time(NULL));
		chosenMove = rand() % moves.size();

		applyMove(moves[chosenMove]);
		cout << "Move #" << (i + 1) << ") ";
		moves[chosenMove].disp();
		cout << "  applied successfully" << endl;

		toDisp = normalize();

		cout << endl;
		toDisp.disp();
		cout << endl;
	}
}

Board Board::normalize(void) {
	Board b = clone();
	int nextIdx = 3;

	for (int i = 0; i < b.getCells().size(); i++) {
		for (int j = 0; j < b.getCells()[i].size(); j++) {
			if (b.getCells()[i][j] == nextIdx) {
				nextIdx++;
			}
			else if (b.getCells()[i][j] > nextIdx) {
				_swapIdx(b, nextIdx, b.getCells()[i][j]);
				nextIdx++;
			}
		}
	}
	b.setNormalized(true);
	return b;
}

bool Board::equals(Board b) {
	Board a = *this;

	if (!isNormalized()) {
		a = normalize();
	}
	if (!b.isNormalized()) {
		b = b.normalize();
	}

	if (a.getWidth() != b.getWidth() || a.getHeight() != b.getHeight()) {
		return false;
	}

	for (int i = 0; i < a.getCells().size(); i++) {
		for (int j = 0; j < a.getCells()[i].size(); j++) {
			if (a.getCells()[i][j] != b.getCells()[i][j]) {
				return false;
			}
		}
	}
	return true;
}

void Board::disp(void) {
	if (_cells.size() > 0) {
		cout << _cells[0].size() << "," << _cells.size() << endl;
		for (int i = 0; i < _cells.size(); i++) {
			for (int j = 0; j < _cells[i].size(); j++) {
				cout << setw(3) << _cells[i][j];
			}
			cout << endl;
		}
	}
	else {
		cout << "The board does not contain anything to display." << endl;
	}
}

vector<vector<int> > Board::_cloneCells(void) {
	vector<vector<int> > cellsClone = vector<vector<int> >();
	for (int i = 0; i < _cells.size(); i++) {
		vector<int> currentRow = vector<int>();
		for (int j = 0; j < _cells[i].size(); j++) {
			currentRow.push_back(_cells[i][j]);
		}
		cellsClone.push_back(currentRow);
	}

	return cellsClone;
}
	
bool Board::_canMoveUp(vector<int> topLeft, int pieceWidth, bool isGoalBrick) {
	int abovePiece;

	if (topLeft[0] == 0) {
		return false;
	}

	for (int i = topLeft[1]; i < topLeft[1] + pieceWidth; i++) {
		abovePiece = _cells[topLeft[0] - 1][i];
		if (abovePiece != 0) {
			if (!(isGoalBrick && abovePiece == -1)) {
				return false;
			}
		}
	}
	return true;
}

bool Board::_canMoveDown(vector<int> bottomRight, int pieceWidth, bool isGoalBrick) {
	int belowPiece;

	if (bottomRight[0] == getHeight() - 1) {
		return false;
	}

	for (int i = bottomRight[1]; i > bottomRight[1] - pieceWidth; i--) {
		belowPiece = _cells[bottomRight[0] + 1][i];
		if (belowPiece != 0) {
			if (!(isGoalBrick && belowPiece == -1)) {
				return false;
			}
		}
	}
	return true;
}

bool Board::_canMoveLeft(vector<int> topLeft, int pieceHeight, bool isGoalBrick) {
	int leftPiece;

	if (topLeft[1] == 0) {
		return false;
	}

	for (int i = topLeft[0]; i < topLeft[0] + pieceHeight; i++) {
		leftPiece = _cells[i][topLeft[1] - 1];
		if (leftPiece != 0) {
			if (!(isGoalBrick && leftPiece == -1)) {
				return false;
			}
		}
	}
	return true;
}

bool Board::_canMoveRight(vector<int> bottomRight, int pieceHeight, bool isGoalBrick) {
	int rightPiece;

	if (bottomRight[1] == getWidth() - 1) {
		return false;
	}

	for (int i = bottomRight[0]; i > bottomRight[0] - pieceHeight; i--) {
		rightPiece = _cells[i][bottomRight[1] + 1];
		if (rightPiece != 0) {
			if (!(isGoalBrick && rightPiece == -1)) {
				return false;
			}
		}
	}
	return true;
}

void Board::_swapIdx(Board b, int idx1, int idx2) {
	for (int i = 0; i < b.getCells().size(); i++) {
		for (int j = 0; j < b.getCells()[i].size(); j++) {
			if (b.getCells()[i][j] == idx1) {
				b.getCells()[i][j] = idx2;
			}
			else if (b.getCells()[i][j] == idx2) {
				b.getCells()[i][j] = idx1;
			}
		}
	}
}

#endif

