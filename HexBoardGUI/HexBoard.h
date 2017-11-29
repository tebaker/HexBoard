/* PROGRAMMER  : TALON BAKER
 * CLASS       : CMPS 109
 * HW_04       : HEX BOARD
 * DUE DATE    : 11/10/17
 */
#ifndef HEXBOARD_H_
#define HEXBOARD_H_

#include<map>
#include<set>
#include<limits>
#include<vector>
#include<sstream>
#include<iomanip>
#include<queue>
#include<math.h>
using namespace std;

/*CONSTANTS*/
const int INT_LIMIT = numeric_limits<int>::max();

/*TYPEDEF*/
typedef int vertex;
typedef int weight;
typedef multimap<vertex, vertex> adjacencyList;
typedef multimap<vertex, struct weightedEdge> weightedAdjacencyList;

/*ENUMERATIONS*/
enum tileStatus {
	RED,	//RED if tile is occupied by PLAYER RED
	BLUE,   //BLUE if occupied by PLAYER BLUE
	OPEN    //OPEN otherwise
};

/*STRUCT*/
struct graph {
	set<vertex>   nodes;
	adjacencyList edges;
};

struct weightedEdge {
	vertex neighbor;
	float  dist;
};

//global masterList for DrawingTools
vector<pair<vertex, tileStatus>> masterList;

/*HexBoard:
 *  Boundaries checks for in-play will be:
 *  	0 <= x <= boardSize - 1
 *  	y <= 0 <= boardSize - 1
 */
class HexBoard {
public:
	//default constructor
	HexBoard(int boardSize);
	//destructor
	virtual ~HexBoard(){};

	/*MUTATORS*/
	bool placePiece(vertex x, vertex y, tileStatus color);

	/*ACCESSORS*/
	string printBoard(bool winPrint = false);
	tileStatus calculateWinner();
	bool aStar(vertex start, vertex goal);
	float distBetween(vertex start, vertex goal);


private:
	int                      boardSize;

	set<vertex>              redNodes;
	weightedAdjacencyList    redEdges;

	set<vertex>              blueNodes;
	weightedAdjacencyList    blueEdges;

	map<vertex,vertex>       cameFrom;
};

/*Overloaded constructor:
 * Constructs a board of size n x n; where n = boardSize
 * Places a OPEN piece for every space on the board stored in a master vector*/
HexBoard::HexBoard(int boardSize) {
	this->boardSize = boardSize;
	//pushing all tiles into the board as OPEN
	for(int i = 0; i < boardSize * boardSize; i++) {
		masterList.push_back(make_pair((i), OPEN));
	}
}//end - overloaded constructor

string HexBoard::printBoard(bool winPrint) {
	ostringstream outStr;
	outStr << "       0   1   2   3   4   5   6   7   8   9   10" << endl << endl;
	for(int i = 0; i < this->boardSize; i++) {
		for(int k = 0; k < i; k++) {
			outStr << "  ";
		}
		outStr << setw(3) << i << "   | ";
		for(int j = 0; j < this->boardSize; j++) {
			tileStatus holdPiece = static_cast<tileStatus>(masterList.at(i * 11 + j).second);
			//if space is not OPEN, then add color to print board
			if(holdPiece != OPEN) {
				if(holdPiece == RED) {
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


		outStr << "  " << i << endl << endl;
	}//end - outer for
	outStr << "                            "
		   << "0   1   2   3   4   5   6   7   8   9   10" << endl;
	return outStr.str();
}//end = printBoard


tileStatus HexBoard::calculateWinner() {
	//collecting every instance of a RED piece along the top and bottom.
	vector<vertex> northWall;
	vector<vertex> southWall;

	//all RED along NORTH wall
	for(int i = 0; i < this->boardSize; i++) {
		if(masterList.at(i * 11).second == RED) {
			northWall.push_back(i * 11);
		}
	}

	//all RED along SOUTH wall
	for(int i = 0; i < this->boardSize; i++) {
		if(masterList.at(i * 11 + 10).second == RED) {
			southWall.push_back(i * 11 + 10);
		}
	}

	for(auto start: northWall) {
		for(auto end: southWall) {
			if(this->aStar(start, end)) {
				return tileStatus::RED;
			}
		}
	}

	return tileStatus::BLUE;
}//end - calculateWinner

float HexBoard::distBetween(vertex start, vertex goal) {
	int x1, y1, x2, y2;

	y1 = start % this->boardSize;
	x1 = (start - y1) / this->boardSize;

	y2 = goal % this->boardSize;
	x2 = (goal - y2) / this->boardSize;

	return sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2));
}

bool HexBoard::aStar(vertex start, vertex goal) {
	priority_queue<weightedEdge> openSet;
	map<vertex, float>		     fScore;
	map<vertex, int>             gScore;
	set<vertex>					 openSetHold;
	set<vertex>                  closedSet;

	//for every RED node, setting gScore, fScore to infinity and cameFrom to 0
	for(auto it: this->redNodes) {
		gScore[it]   = INT_LIMIT;
		fScore[it]   = INT_LIMIT;
		this->cameFrom[it] = 0;
	}

	//setting gScore, cameFrom to 0, pushing start node to openSet
	gScore[start]   = 0;
	fScore[start]   = distBetween(start, goal);
	this->cameFrom[start] = 0;
	openSet.push({start, fScore[start]});
	openSetHold.insert(start);

	while(!openSet.empty()) {
		//setting current to top of openSet
		vertex current = openSet.top().neighbor;
		openSet.pop();
		openSetHold.erase(current);

		if(current == goal) {
			return true;
		}

		//adding current to closedSet
		closedSet.insert(current);

		//getting all neighbors of the current node
		pair<weightedAdjacencyList::iterator, weightedAdjacencyList::iterator> range;
		range = this->redEdges.equal_range(current);

		//looping through every neighbor of the current node
		for(auto it = range.first; it != range.second; it++) {
			//if neighbor is not in the closed set & neighbor not in open set
			if(closedSet.find(it->first) != closedSet.end() &&
					openSetHold.find(it->first) != openSetHold.end()) {
				openSet.push({it->first, distBetween(it->first, goal)});
			}

			int alt = gScore[current];

			if(alt < gScore[it->second.neighbor]) {
				//setting gScore at current neighbor to alt and from at current neighbor to current
				gScore[it->second.neighbor]   = alt;
				this->cameFrom[it->second.neighbor] = current;
			}
		}
	}//end - while openSet is not empty

	//path not found, clearing dist and prev from HexBoard class for next iteration
	this->cameFrom.clear();

	return false;
}//end - aStar

//overloading operator < for use in Dijkstra's Algorithm
bool operator<( const weightedEdge& left, const weightedEdge& right ) {
	return left.dist > right.dist;
}

bool HexBoard::placePiece(vertex x, vertex y, tileStatus color) {
	int index = x * 11 + y;
	if(masterList.at(index).second == OPEN) {
		if(color == BLUE) {
			this->blueNodes.insert(index);
			masterList.at(index).second = color;
			//checking all neighbors of placedPiece for same color and in bounds
			//NW
			if(y - 1 >= 0) {
				if(masterList.at(x * 10 + (y - 1)).second == BLUE) {
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(index, {(x * 10 + y - 1), 1})
					);
					this->blueEdges.insert(
						pair<vertex, weightedEdge>((x * 10 + y - 1), {index, 1})
					);
				}
			}
			//NE
			if(x + 1 >= 0 && y - 1 <= 10) {
				if(masterList.at((x + 1) * 10 + (y - 1)).second == BLUE) {
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(index, {(x + 1 * 10 + y - 1), 1})
					);
					this->blueEdges.insert(
						pair<vertex, weightedEdge>((x + 1 * 10 + y - 1), {index, 1})
					);
				}
			}
			//E
			if(x + 1 <= 10) {
				if(masterList.at((x + 1) * 10 + y).second == BLUE) {
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(index, {((x + 1) * 10 + y), 1})
					);
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(((x + 1) * 10 + y), {index, 1})
					);
				}
			}
			//SE
			if(y + 1 <= 10) {
				if(masterList.at(x * 10 + (y + 1)).second == BLUE) {
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(index, {(x * 10 + y + 1), 1})
					);
					this->blueEdges.insert(
						pair<vertex, weightedEdge>((x * 10 + y + 1), {index, 1})
					);
				}
			}
			//SW
			if(x - 1 >= 0 && y + 1 <= 10) {
				if(masterList.at((x - 1) * 10 + (y + 1)).second == BLUE) {
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(index, {((x - 1) * 10 + y + 1), 1})
					);
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(((x - 1) * 10 + y + 1), {index, 1})
					);
				}
			}
			//W
			if(x - 1 >= 0) {
				if(masterList.at((x - 1) * 10 + y).second == BLUE) {
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(index, {((x - 1) * 10 + y), 1})
					);
					this->blueEdges.insert(
						pair<vertex, weightedEdge>(((x - 1) * 10 + y), {index, 1})
					);
				}
			}
		}//end - if BLUE
		//checking RED for neighbors
		else {
			this->redNodes.insert(index);
			masterList.at(index).second = color;
			//checking all neighbors of placedPiece for same color and in bounds
			//NW
			if(y - 1 >= 0) {
				if(masterList.at(x * 10 + (y - 1)).second == RED) {
					this->redEdges.insert(
						pair<vertex, weightedEdge>(index, {(x * 10 + y - 1), 1})
					);
					this->redEdges.insert(
						pair<vertex, weightedEdge>((x * 10 + y - 1), {index, 1})
					);
				}
			}
			//NE
			if(x + 1 >= 0 && y - 1 <= 10) {
				if(masterList.at((x + 1) * 10 + (y - 1)).second == RED) {
					this->redEdges.insert(
						pair<vertex, weightedEdge>(index, {(x + 1 * 10 + y - 1), 1})
					);
					this->redEdges.insert(
						pair<vertex, weightedEdge>((x + 1 * 10 + y - 1), {index, 1})
					);
				}
			}
			//E
			if(x + 1 <= 10) {
				if(masterList.at((x + 1) * 10 + y).second == RED) {
					this->redEdges.insert(
						pair<vertex, weightedEdge>(index, {((x + 1) * 10 + y), 1})
					);
					this->redEdges.insert(
						pair<vertex, weightedEdge>(((x + 1) * 10 + y), {index, 1})
					);
				}
			}
			//SE
			if(y + 1 <= 10) {
				if(masterList.at(x * 10 + (y + 1)).second == RED) {
					this->redEdges.insert(
						pair<vertex, weightedEdge>(index, {(x * 10 + y + 1), 1})
					);
					this->redEdges.insert(
						pair<vertex, weightedEdge>((x * 10 + y + 1), {index, 1})
					);
				}
			}
			//SW
			if(x - 1 >= 0 && y + 1 <= 10) {
				if(masterList.at((x - 1) * 10 + (y + 1)).second == RED) {
					this->redEdges.insert(
						pair<vertex, weightedEdge>(index, {((x - 1) * 10 + y + 1), 1})
					);
					this->redEdges.insert(
						pair<vertex, weightedEdge>(((x - 1) * 10 + y + 1), {index, 1})
					);
				}
			}
			//W
			if(x - 1 >= 0) {
				if(masterList.at((x - 1) * 10 + y).second == RED) {
					this->redEdges.insert(
						pair<vertex, weightedEdge>(index, {((x - 1) * 10 + y), 1})
					);
					this->redEdges.insert(
						pair<vertex, weightedEdge>(((x - 1) * 10 + y), {index, 1})
					);
				}
			}
		}//end - else
		return true;
	}//end - if tile is OPEN
	return false;
}//end - insertEdge


#endif /* HEXBOARD_H_ */
