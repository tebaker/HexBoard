/* PROGRAMMER  : TALON BAKER
 * CLASS       : CMPS 109
 * HW_04       : HEX BOARD
 * DUE DATE    : 11/10/17
 */
#ifndef HEXBOARD_H_
#define HEXBOARD_H_

#include<vector>
#include<sstream>
#include<iomanip>
#include<iostream>
#include<queue>
#include<map>
using namespace std;

enum tileStatus {
	RED,	//RED if tile is occupied by PLAYER RED
	BLUE,   //BLUE if occupied by PLAYER BLUE
	OPEN    //OPEN otherwise
};

struct piece {
	int		   x = 0;
	int		   y = 0;
	tileStatus status = OPEN;
	piece*     NW = nullptr;
	piece*     NE = nullptr;
	piece*     E  = nullptr;
	piece*     SE = nullptr;
	piece*     SW = nullptr;
	piece*     W  = nullptr;
	bool       eastWall  = false;
	bool	   southWall = false;
};

struct coord {
	int x;
	int y;
};


/*Class Hex Board:
 * This class will create a linked grid of pieces that all point to each other
 * via their cardinal directions*/
class HexBoard {
public:
	//default constructor
	HexBoard(int boardSize);
	//destructor
	virtual ~HexBoard(){};
/*MUTATORS*/
	void addPiece(int x, int y, tileStatus color);
/*ACCESSORS*/
	string     printBoard(bool = false);
	tileStatus calculateWinner();
	piece*     at(int, int);

private:
	piece* headPiece;	//holding all pieces on the board
	int    boardSize;	//holding board size
};

inline bool contained(int x, int y, vector<coord> closedList) {
	for(auto i: closedList) {
		if(i.x == x && i.y == y) return true;
	}
	return false;
}

/*Overloaded constructor:
 * Constructs a board of size n x n; where n = boardSize
 * Places a "blank" piece for every space on the board and links them
 * together via their cardinal directions*/
HexBoard::HexBoard(int boardSize) {
	this->boardSize = boardSize;
	this->headPiece = new piece;

	//creating piece pointer for traversal
	piece* currPiece = headPiece;
	piece* newPiece;

	//looping until one less than bordSize for special case (SOUTH WALL)
	for(int i = 0; i < boardSize - 1; i++) {
		//looping until one less than boardSize for special case (EAST WALL)
		for(int j = 0; j < boardSize - 1; j++) {
			newPiece = new piece;
			newPiece->x = i;
			newPiece->y = j + 1;

			//linking right/left
			currPiece->E = newPiece;
			newPiece->W  = currPiece;

			newPiece = new piece;
			newPiece->x = i + 1;
			newPiece->y = j;

			//linking up/down
			currPiece->SE = newPiece;
			newPiece->NW  = currPiece;

			//linking the two new pieces together
			currPiece->E->SW  = currPiece->SE;
			currPiece->SE->NE = currPiece->E;

			//incrementing place of currPiece to one right of itself
			currPiece = currPiece->E;
		}//end - inner for

		//special case: EAST wall
		newPiece = new piece;
		newPiece->x = i + 1;
		newPiece->y = boardSize - 1;
		newPiece->eastWall = true;

		//linking up/down
		currPiece->SE = newPiece;
		newPiece->NW  = currPiece;

		currPiece = this->at(i + 1, 0);
	}//end - outer for

	//looping until one less than boardSize for special case (SOUTH EAST CORNER)
	for(int i = 0; i < boardSize - 1; i++) {
		currPiece->NE->SE->W = currPiece;
		currPiece->E = currPiece->NE->SE;
		currPiece->southWall = true;

		//incrementing currPiece to one right or currPiece
		currPiece = currPiece->E;
	}
}//end - overloaded constructor

void HexBoard::addPiece(int x, int y, tileStatus color) {
	piece* holdPiece = headPiece;
	holdPiece = this->at(x, y);
	holdPiece->status = color;
}//end - addPiece

string HexBoard::printBoard(bool winPrint) {
	ostringstream outStr;
	outStr << "       A   B   C   D   E   F   G   H   I   J   K" << endl << endl;
	for(int i = 0; i < this->boardSize; i++) {
		for(int k = 0; k < i; k++) {
			outStr << "  ";
		}
		outStr << setw(3) << i + 1 << "   | ";
		for(int j = 0; j < this->boardSize; j++) {
			piece* holdPiece = this->at(i, j);
			//if space is not OPEN, then add color to print board
			if(holdPiece->status != OPEN) {
				if(holdPiece->status == RED) {
					outStr << "R | ";
				}
				else {
					outStr << "B | ";
				}//end - if piese is RED/BLUE
			}
			else {
				outStr << "  | ";
			}//end - if piece is OPEN
		}//end - inner for


		outStr << "  " << i + 1 << endl << endl;
	}//end - outer for
	outStr << "                            "
		   << "A   B   C   D   E   F   G   H   I   J   K" << endl;
	return outStr.str();
}//end = printBoard

/*determining a winner will be BLUE completing a line horizontally right/left
 * and RED completing a line vertically up/down. The program will return the
 * color of the winner
 * If path is optimal at every step (given the choices) AND reaches the end,
 * the path is optimal or TIED for optimal path*/
tileStatus HexBoard::calculateWinner() {
	vector<piece*> openList;
	vector<coord>  closedList;

	//searching for instance of BLUE on the first column
	for(int i = 0; i < this->boardSize; i++) {
		if(this->at(i, 0)->status == BLUE) {
			openList.push_back(this->at(i, 0));

			while(!openList.empty()) {
				piece* current = openList.back();
				coord newCoord;
				newCoord.x = openList.back()->x;
				newCoord.x = openList.back()->y;

				closedList.push_back(newCoord);
				openList.pop_back();

				//if current is BLUE piece on far right, return blue winner
				if(current->status == BLUE) {
					return tileStatus::BLUE;
				}

				//checking neighbors starting with optimal paths
				//E is optimal
				if(current->E->status == BLUE &&
						!contained(current->E->x, current->E->y, closedList)) {
					openList.push_back(current->E);
				}
				//NE, SE is sub optimal
				if(current->NE->status == BLUE &&
						!contained(current->NE->x, current->NE->y, closedList)) {
					openList.push_back(current->NE);
				}
				if(current->SE->status == BLUE &&
						!contained(current->SE->x, current->SE->y, closedList)) {
					openList.push_back(current->SE);
				}
				//NW, SW are bad
				if(current->NW->status == BLUE &&
						!contained(current->NW->x, current->NW->y, closedList)) {
					openList.push_back(current->NW);
				}
				if(current->SW->status == BLUE &&
						!contained(current->SW->x, current->SW->y, closedList)) {
					openList.push_back(current->SW);
				}
				//W is the worst
				if(current->W->status == BLUE &&
						!contained(current->W->x, current->W->y, closedList)) {
					openList.push_back(current->W);
				}
			}//end - inner while
		}//end - if
	}//end - inner while
	return tileStatus::RED;
}//end - calculateWinner

piece* HexBoard::at(int x, int y) {
	piece* returnPiece = headPiece;
	//setting returnPiece to x rows down and y columns right from headPiece
	for(int i = 0; i < x; i++) {
		returnPiece = returnPiece->SE;
	}
	for(int j = 0; j < y; j++) {
		returnPiece = returnPiece->E;
	}
	return returnPiece;
}//end - at

#endif /* HEXBOARD_H_ */

/*
 if(x == 0 && y == 0) {
		//excluding: SW, W, NW, NE
		//including: E, SE
	}
	//boundary check: NE corner
	else if(x == 0 && y == boardSize - 1) {
		//excluding: NW, NE, E
		//including: SE, SW, W
	}
	//boundary check: SE corner
	else if(x == boardSize - 1 && y == boardSize - 1) {
		//excluding: NE, E, SE, SW
		//including: W, NW
	}
	//boundary check: SW corner
	else if(x == boardSize - 1 && y == 0) {
		//excluding: SE, SW, W
		//including: NW, NE, E
	}
	//boundary check: N wall
	else if(x == 0) {
		//excluding: NW, NE
		//including: E, SE, SW, W
	}
	//boundary check: E wall
	else if(y == boardSize - 1) {
		//excluding: NE, E
		//including: SE, SW, W, NW
	}
	//boundary check: S wall
	else if(x == boardSize - 1) {
		//excluding: SE, SW
		//including: W, NW, NE, E
	}
	//boundary check: W wall
	else if(y == 0) {
		//excluding: SW, W
		//including: NW, NE, E, SE
	}
	//default case: somewhere in the middle
	else {
		//no boundary walls or corners. Including all adjacent neighbors
	}
 */






