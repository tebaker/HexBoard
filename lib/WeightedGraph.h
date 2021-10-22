/* PROGRAMMER  : TALON BAKER
 * CLASS       : CMPS 109
 * HW_03       : MST
 * DUE DATE    : 10/30/17
 */

#ifndef WEIGHTEDGRAPH_H_
#define WEIGHTEDGRAPH_H_


/*INCLUDE*/
#include <iostream>	//Console input/output
#include <map>      //For use of map container
#include <set>      //For use of set container
#include <math.h>	//For using complex math functions
#include <string>	//For string manipulations
#include <sstream>	//For use of StringStream
#include <queue>	//For use of qriority_queue
#include <limits>	//For use of largest integer value in C++
#include <random>	//For random number generation
#include <stack>	//For use of stack library

/*USING*/
using namespace std;  //Using standard namespace

/*CONSTANT DECLARATIONS*/
const int COMPLETE_GRAPH = 2500;	//Value of K_50 complete graph (eg. 50x50)
const int INT_LIMIT = numeric_limits<int>::max();

/*TYPEDEF*/
typedef int vertex;
typedef int weight;
typedef multimap<vertex, vertex> adjacencyList;
typedef multimap<vertex, struct weightedEdge> weightedAdjacencyList;

/*STRUCT*/
struct graph {
	set<vertex>   nodes;
	adjacencyList edges;
};

struct weightedEdge {
	vertex neighbor;
	weight dist;
};

class WeightedGraph {
/*MEMBERS*/
	private:
		set<vertex>           nodes;	//Holding all the nodes the graph
		weightedAdjacencyList edges;	//Holding all the edges of the graph

		int density;					//Holding the density of the graph
		int numEdges;					//Holding number of edges in graph
		int numNode;					//Holding number of vertices in graph

		multimap<vertex, vertex> mst;	//Holding Minimum Spanning Tree when it's ready to be created
		map<vertex,int> dist;			//dist and prev for calculating paths during D'sA and MST
		map<vertex,vertex> prev;
	public:

		//default constructor
		WeightedGraph();
		/*Overloaded Constructor - Initializing random graph:
		* Will generate three random numbers: from-vertices, to-vertices,
		* and a random distance [1, 10] between them.
		* If there is not already a connection made between the two, the loop will
		* generate a connection and assign a random distance between the two vectors.
		* The loop will generate connections until the density is satisfied.*/
		WeightedGraph(float density, default_random_engine generator);
		//Default destructor
		virtual ~WeightedGraph(){};

/*ACCESSORS*/
		//returns string contents of entire graph starting with vertex 1 going to vertex 50
		string printGraph();
		//returns string contents of graph only at specified vertex
		string printGraph(vertex node);
		/*Dijkstra's Algorithm will be passed a start node and calculate shortest path for every combination
		 * of (i, j) where i is the start node and j goes from 1 - 50 excluding i.
		 * This program will modify a minimum distance from i map that will hold all the path calculations*/
		void dijkstrasAlgorithm(vertex start);
		/*Calculating MST*/
		void calcMST(vertex start);
		/*Printing MST nicely formatted*/
		void printMST(vertex start, int indent);

/*MUTATORS*/
		//inserts an edge and weight into the graph
		void insertEdge(vertex fromVert, vertex toVert, weight dist);
		//inserts a node into the graph
		void insertNode(vertex fromVert);
};//end - class WeightedGraph

WeightedGraph::WeightedGraph(){
	this->density = 0;
	this->numEdges = 0;
	this->numNode = 0;
}

/*Overloaded Constructor - Initializing random graph:
* Will generate three random numbers: from-vertices, to-vertices,
* and a random distance [1, 10] between them.
* If there is not already a connection made between the two, the loop will
* generate a connection and assign a random distance between the two vectors.
* The loop will generate connections until the density is satisfied.*/
WeightedGraph::WeightedGraph(float density, default_random_engine generator){//density of graph [0, 1]
	this->density = density;
	this->numEdges = 0;
	this->numNode = 0;

	int calculatedDensity = ceil((float)COMPLETE_GRAPH * density);

	uniform_int_distribution<int> distribution1to25(1,25);
	uniform_int_distribution<int> distribution1to50(1,50);
	uniform_int_distribution<int> distribution26to50(26,50);
	uniform_int_distribution<int> distribution1to10(1,10);

	//loading the graph with initial two edges
	//fromVert [1,25], toVery [26, 50]
	int fromVert = distribution1to25(generator);		//[1, 25]
	int toVert   = distribution26to50(generator);		//[26, 25]
	int randDist = distribution1to10(generator);		//[1, 10]

	//adding fromVert and toVert and the random distance between them
	this->insertNode(fromVert);
	this->numNode++;
	this->insertNode(toVert);
	this->numNode++;
	this->insertEdge(fromVert, toVert, randDist);
	this->numEdges++;
	this->insertEdge(toVert, fromVert, randDist);
	this->numEdges++;

	while(calculatedDensity > 0){
		//Generating random from-vertex, to-vertex [1, 50]
		fromVert = distribution1to50(generator);
		toVert   = distribution1to50(generator);

		//If the vertices aren't the same, continue
		if(fromVert != toVert){
			auto it = this->edges.find(fromVert);

			//If fromVert is not contained in g.edges; add it
			if(it == this->edges.end()){
				randDist = distribution1to10(generator);
				this->insertNode(fromVert);
				this->numNode++;
				this->insertEdge(fromVert, toVert, randDist);
				this->numEdges++;
				this->insertEdge(toVert, fromVert, randDist);
				this->numEdges++;
				//Decrementing density
				calculatedDensity--;
			}
			else{
				//If fromVert is contained but no neighbor in g.edges is toVert; insert new edge
				while(it->first == fromVert){
					//if one of the neighbors is toVert, break out of the while loop
					if(it->second.neighbor == toVert){
						break;
					}
					//Advancing the iterator
					it++;
					//If the next iteration is the end, then toVert is not contained; add it
					if(it->first != fromVert){
						randDist = distribution1to10(generator);
						this->insertEdge(fromVert, toVert, randDist);
						this->numEdges++;
						this->insertEdge(toVert, fromVert, randDist);
						this->numEdges++;
						calculatedDensity--;
					}
				}
			}
		}
	}//end - creation of random graph
}//end - overloaded constructor (density)

//returns string contents of entire graph starting with vertex 1 going to vertex 50
string WeightedGraph::printGraph(){
	stringstream outStr;
	//Outputting noted and edges from 1 to 50
	for(int i = 1; i <= 50; i++){
		for(auto it = this->edges.find(i); it->first == i; it++){
			outStr << it->first << " -> " << it->second.neighbor << ": "
				   << it->second.dist << "\n";
		}
	}
	return outStr.str();
}//end - printGraph

//returns string contents of graph only at specified vertex
string WeightedGraph::printGraph(vertex node){
	stringstream outStr;
	//Outputting noted and edges from 1 to 50
	for(auto it = this->edges.find(node); it->first == node; it++){
		outStr << it->first << " -> " << it->second.neighbor << ": "
			   << it->second.dist << "\n";
	}
	return outStr.str();
}//end - printGraph

/*Dijkstra's Algorithm will be passed a start node and calculate shortest path for every combination
 * of (i, j) where i is the start node and j goes from 1 - 50 excluding i.
 * This program will modify a minimum distance from i map that will hold all the path calculations*/
void WeightedGraph::dijkstrasAlgorithm(vertex start){
	priority_queue<weightedEdge> Q;
	this->dist[start] = 0;
	this->prev[start] = 0;
	Q.push({start, 0});

	for(auto it = this->nodes.begin(); it != this->nodes.end(); it++){
		if(*it != start){
			this->dist[*it] = INT_LIMIT; //setting creating dist at [*it] to 'infinity'
			this->prev[*it] = 0;
		}
		Q.push({*it, INT_LIMIT});
	}

	while(!Q.empty()){
		vertex u = Q.top().neighbor;
		Q.pop();

		pair<weightedAdjacencyList::iterator, weightedAdjacencyList::iterator> range;
		range = this->edges.equal_range(u);

		for(auto it = range.first; it != range.second; it++){
			int alt = it->second.dist;

			if(dist.find(u) != dist.end()){
				alt += dist[u];
			}

			if(this->dist.find(it->second.neighbor) == this->dist.end() ||
					alt < this->dist[it->second.neighbor]){
				this->dist[it->second.neighbor] = alt;
				this->prev[it->second.neighbor] = u;
				Q.push({it->second.neighbor, alt});
			}
		}
	}
}//end - dijkstrasAlgorithm

void WeightedGraph::calcMST(vertex start){
	for(auto it = this->nodes.begin(); it != this->nodes.end(); ++it ){
		stack<vertex> path;
		vertex temp = *it;
		while(this->prev.find(temp) != this->prev.end()){
			path.push(temp);
			bool insert = true;
			if(this->prev[temp] != 0){
				pair<multimap<vertex, vertex>::iterator, multimap<vertex, vertex>::iterator> range;
				range =	mst.equal_range(this->prev[temp]);
				for (auto child = range.first; child != range.second; ++child) {
					if (child->second == temp) {
						insert = false;
						break;
					}
				}
				if(insert){
					mst.insert(pair<vertex, vertex>(this->prev[temp], temp));
				}
			}
			temp = this->prev[temp];
		}
		cout << "From " << start << " to " << *it << right << ": ";
		while(!path.empty()){
			cout << path.top();
			path.pop();
			if(!path.empty()){
				cout << "->";
			}
			else{
				cout << endl;
			}
		}
	}
}//end - calcMST

void WeightedGraph::printMST(vertex start, int indent){
	pair<multimap<vertex,vertex>::iterator, multimap<vertex,vertex>::iterator> range;
	range = this->mst.equal_range(start);
	for(int i = 0; i < indent; i++){
		cout << " |" ;
	}
	cout << " " << start << endl;
	for(auto child = range.first; child != range.second; ++child){
		if(child->second != start)
			printMST(child->second, indent+1);
	}
}

void WeightedGraph::insertEdge(vertex fromVert, vertex toVert, weight dist){
	this->edges.insert(pair<vertex, weightedEdge>(fromVert, {toVert, dist}));
}//end - insertEdge

void WeightedGraph::insertNode(vertex fromVert){
	this->nodes.insert(fromVert);
}//end - insertNode

//overloading operator < for use in Dijkstra's Algorithm
bool operator<( const weightedEdge& left, const weightedEdge& right ) {
	return left.dist > right.dist;
}
#endif /* WEIGHTEDGRAPH_H_ */
