/* PROGRAMMER  : TALON BAKER
 * CLASS       : CMPS 109
 * HW_04       : HEX BOARD
 * DUE DATE    : 11/10/17
 */

#include "HexBoard.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>	//needed for random_shuffle
#include <chrono>
using namespace std;

const int BOARD_SIZE = 11;

int main() {
	// obtain a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	HexBoard board(BOARD_SIZE);

	vector<int> randArray;

	//populating an array with alternating REDs and BLUEs
	for(int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
		//using static_cast to safely convert from enum to int
		if(i % 2 == 0) {
			randArray.push_back(static_cast<int>(RED));
		}
		else {
			randArray.push_back(static_cast<int>(BLUE));
		}
	}

	//randomly shuffling array
	shuffle(randArray.begin(), randArray.end(), std::default_random_engine(seed));

	//adding pieces from randomly shuffled array to the board
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++) {
			board.addPiece(i, j, static_cast<tileStatus>(randArray.back()));
			randArray.pop_back();
		}
	}

	cout << board.printBoard();

	cout << "Winner is: ";
	if(board.calculateWinner() == BLUE) {
		cout << "BLUE!"<< endl;
	}
	else {
		cout << "RED!" << endl;
	}

	return 0;
}//end - main
