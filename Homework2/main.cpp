#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "Board.h"
#include "split.cpp"
#include "Heuristics.h"
#include "Solver.h"
#include "Solution.h"

using namespace std;

Board* loadBoard(string fname) {
	vector<vector<int> > state;
	string line;
	ifstream infile(fname.c_str());
	if (infile.is_open()) {
		getline(infile, line); // first line contains unneeded dimensions
		while (getline(infile, line)) {
			vector<string> vals = split(line, ',');
			vector<int> ivals;
			for (int i = 0; i < vals.size(); i++) {
				ivals.push_back(stoi(vals[i]));
			}
			state.push_back(ivals);
		}
		infile.close();
	}
	else {
		cout << "Unable to open file" << endl;
		return NULL;
	}
	Board* b = new Board(state);
	return b;
}

static void printResults(Solution* solution, chrono::milliseconds searchTime) {
	if (solution != NULL || solution->getSolution().size() > 0) {
		Solver::printMoves(solution->getSolution());
		solution->getFinalState().disp();
		cout << "#" << solution->getNumNodesVisited() << " " << searchTime.count() / 1000 << " " << solution->getSolution().size() << endl;
	}
	else {
		cout << "No solution found!" << endl;
		cout << "Time to run: " << searchTime.count() << " ms" << endl;
	}
}

int main(int argc, char* argv[]) {
	string baseDir = "./inputFiles/";
	int n = atoi(argv[1]);

	if (n == 1) {
		Board* b = loadBoard(baseDir + "SBP-level1.txt");
		if (b != NULL) {
			chrono::milliseconds searchStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			Solution* solution = Solver::aStar(*b, Heuristics::manhattanDistance);
			chrono::milliseconds searchEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			cout << "SBP-level1.txt" << " " << "Manhattan distance" << endl;
			printResults(solution, searchEnd - searchStart);
		}
	}
	else if (n == 2) {
		Board* b = loadBoard(baseDir + "SBP-level1.txt");
		if (b != NULL) {
			chrono::milliseconds searchStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			Solution* solution = Solver::aStar(*b, Heuristics::minEdgeSwapsAndMoves);
			chrono::milliseconds searchEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			cout << "SBP-level1.txt" << " " << "Custom" << endl;
			printResults(solution, searchEnd - searchStart);
		}
	}
	else if (n == 3) {
		Board* b = loadBoard(baseDir + "SBP-level2.txt");
		if (b != NULL) {
			chrono::milliseconds searchStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			Solution* solution = Solver::aStar(*b, Heuristics::manhattanDistance);
			chrono::milliseconds searchEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			cout << "SBP-level2.txt" << " " << "Manhattan distance" << endl;
			printResults(solution, searchEnd - searchStart);

			cout << endl;

			searchStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			solution = Solver::aStar(*b, Heuristics::minEdgeSwapsAndMoves);
			searchEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			cout << "SBP-level2.txt" << " " << "Custom" << endl;
			printResults(solution, searchEnd - searchStart);
		}
	}
}

