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
	weight dist;
};

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


private:
	int                        boardSize;

	set<vertex>                redNodes;
	weightedAdjacencyList      redEdges;

	set<vertex>                blueNodes;
	weightedAdjacencyList      blueEdges;

	multimap<vertex, vertex>   mst;
	map<vertex,int>            dist;
	map<vertex,vertex>         prev;

	vector<pair<vertex, tileStatus> > masterList;
};

#endif /* HEXBOARD_H_ */
