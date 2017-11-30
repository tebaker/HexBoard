/* PROGRAMMER  : TALON BAKER
 * CLASS       : CMPS 109
 * HW_04       : HEX BOARD
 * DUE DATE    : 11/10/17
 */
#include "HexBoard.h"
#include "DrawingTools.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>	//needed for random_shuffle
#include <chrono>
#include <windows.h>    //for MS Windows
#include <GL/glut.h>    //GLUT, includes glu.h and gl.h

using namespace std;

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutCreateWindow("Hex Board"); // Create a window with the given title
	glutReshapeWindow(800, 800);

	// obtain a time-based seed:
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	HexBoard    board(BOARD_SIZE);
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
	shuffle(randArray.begin(), randArray.end(), default_random_engine(seed));

	//adding pieces from randomly shuffled array to the board
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++) {
			board.placePiece(i, j, static_cast<tileStatus>(randArray.back()));
			randArray.pop_back();

			glutDisplayFunc(drawHexBoard);

		}
	}

	cout << board.printBoard();

	if(board.calculateWinner() == BLUE) {
		cout << "BLUE!"<< endl;
	}
	else {
		cout << "RED!" << endl;
	}

	glutMainLoop();

	return 0;
}//end - main
