#include"HexBoard.h"
#include<iostream>

/*Overloaded constructor:
 * Constructs a board of size n x n; where n = boardSize
 * Places a OPEN piece for every space on the board stored in a master vector*/
HexBoard::HexBoard(int boardSize) {
	this->boardSize = boardSize;
	//pushing all tiles into the board as OPEN
	for(int i = 0; i < boardSize * boardSize; i++) {
		this->masterList.push_back(make_pair((i), OPEN));
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
			tileStatus holdPiece = static_cast<tileStatus>(this->masterList.at(i * 11 + j).second);
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

	//checking for RED along NORTH wall
	for(int i = 0; i < this->boardSize; i++) {
		if(this->masterList.at(i * 11).second == RED) {
			northWall.push_back(i * 11);
		}
	}

	//checking for RED along SOUTH wall
	for(int i = 0; i < this->boardSize; i++) {
		if(this->masterList.at(i * 11 + 10).second == RED) {
			southWall.push_back(i * 11 + 10);
		}
	}

	//searching RED winner by seeing if any path connects in the N/S direction
	for(auto northWallRed: northWall) {
		int start = northWallRed;	//some RED along the top row

		priority_queue<weightedEdge> Q;
		this->dist[start] = 0;
		this->prev[start] = 0;
		Q.push({start, 0});

		//setting all distances to infinity and all previous distances to 0
		for(auto it: this->redNodes) {
			if(it != start){
				this->dist[it] = INT_LIMIT;
				this->prev[it] = 0;
			}
			Q.push({it, INT_LIMIT});
		}

		while(!Q.empty()) {
			//setting key to priority queue's first neighbor
			vertex key = Q.top().neighbor;
			Q.pop();

			pair<weightedAdjacencyList::iterator, weightedAdjacencyList::iterator> range;
			range = this->redEdges.equal_range(key);

			//range.first is start value for key, range.second is end value of key
			for(auto it = range.first; it != range.second; it++) {
				weight alt = it->second.dist;

				if(dist.find(key) != dist.end()) {
					alt += dist[key];
				}

				if(this->dist.find(it->second.neighbor) == this->dist.end() ||
						alt < this->dist[it->second.neighbor]) {
					this->dist[it->second.neighbor] = alt;
					this->prev[it->second.neighbor] = key;
					Q.push({it->second.neighbor, alt});
				}
			}
		}//end - while Q not empty
		//path not found, clearing dist and prev from HexBoard class for next iteration
		this->dist.clear();
		this->prev.clear();
	}//end - for all RED along NORTH wall

	cout << "dist:" << endl;

	for(auto i: this->dist) {
		cout << i.first << endl;
		cout << i.second << endl;
	}

	cout << "prev:" << endl;

	for(auto i: this->prev) {
		cout << i.first << endl;
		cout << i.second << endl;
	}

	return tileStatus::RED;
}//end - calculateWinner

//overloading operator < for use in Dijkstra's Algorithm
bool operator<( const weightedEdge& left, const weightedEdge& right ) {
	return left.dist > right.dist;
}

bool HexBoard::placePiece(vertex x, vertex y, tileStatus color) {
	int index = x * 11 + y;
	if(this->masterList.at(index).second == OPEN) {
		if(color == BLUE) {
			this->blueNodes.insert(index);
			this->masterList.at(index).second = color;
			//checking all neighbors of placedPiece for same color and in bounds
			//NW
			if(y - 1 >= 0) {
				if(this->masterList.at(x * 10 + (y - 1)).second == BLUE) {
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
				if(this->masterList.at((x + 1) * 10 + (y - 1)).second == BLUE) {
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
				if(this->masterList.at((x + 1) * 10 + y).second == BLUE) {
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
				if(this->masterList.at(x * 10 + (y + 1)).second == BLUE) {
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
				if(this->masterList.at((x - 1) * 10 + (y + 1)).second == BLUE) {
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
				if(this->masterList.at((x - 1) * 10 + y).second == BLUE) {
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
			this->masterList.at(index).second = color;
			//checking all neighbors of placedPiece for same color and in bounds
			//NW
			if(y - 1 >= 0) {
				if(this->masterList.at(x * 10 + (y - 1)).second == RED) {
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
				if(this->masterList.at((x + 1) * 10 + (y - 1)).second == RED) {
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
				if(this->masterList.at((x + 1) * 10 + y).second == RED) {
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
				if(this->masterList.at(x * 10 + (y + 1)).second == RED) {
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
				if(this->masterList.at((x - 1) * 10 + (y + 1)).second == RED) {
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
				if(this->masterList.at((x - 1) * 10 + y).second == RED) {
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
