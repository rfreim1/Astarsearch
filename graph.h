#ifndef GRAPH_H
#define GRAPH_H

#include <iostream> //allows inout from keyboard/output to screen
#include <fstream> //allows reading/writing to files
#include <vector> //allows use of vectors
#include <string>//allows use of strings
#include <sstream> //allows use of stringstream type to turn integer into string
#include <queue> //allows use of sort function to sort vector
#include <direct.h> //allows use of _getcwd function

using namespace std; //allows use of standard C++ library functions, classes and objects 


#define NOT_VISITED 0; //defines STATUS of NOT_VISITED as 0
#define VISITED 1;  //defines STATUS of VISITED as 1

class Edge; //declare Edge class so it can be used when defining Node class

class Node{ //class of City nodes
private: 
	int h; //hueristic value
	int g; //pathcost value
	string name; //name of node
	string path; //holds string of overall path travelled
	vector<Edge> children; //holds Edges that connect this node to others
	int Status; //Status of whether this city has been visited or not

public:  
	Node(string n , int heur); //constructor for node

	string getName(); //gets name of node
	string getPath(); //gets overall path travelled to get to node
	void setPath(Edge e); //sets the path travelled to get to node
	int getHeur(); //gets hueristic value
	void setG(Edge e); //sets value of pathcost to get to node
	int getG(); //gets pathcost to get to node
	int getF() const; //gets value of F

	int getStatus(); //gets the Status of the node
	void setStatus(); //sets the status of the node to VISITED
	
	vector<Edge> getChildren(); //gets the vector of edge of the node
	void addchild(Node* child, int pc); //gets an Edge(child) to the Node
	

}; 

//structure used to compare the F values of node for sorting vector(priority list)
struct CompareNodeF : public std::binary_function<Node*, Node*, bool>
{
    bool operator()(const Node* lhs, const Node* rhs) const //return a boolean for the operator
    {
        return lhs->getF() < rhs->getF(); //returns true if F of the rhs > than F of lhs
    }
}; 


class Edge{ //class of connecting edges between cities
private: 
	int pathcost; //pathcost
	Node* parentNode; //node edge starts from
	Node* childNode;  //node edge ends at
public: 
	Edge(Node* pN, Node* cN, int pc); //constructor for edge
	Node* getPNode(); //gets parentNode
	Node* getCNode(); //gets childNode
	int getPathcost(); //gets pathcost of the edge

};


class graph //class of the complete graph
{
private: 
	vector<Node*> nList; //list of cities(nodes)
	int stepLimit; //limit as to how many times the astar search should look through the graph to try to find the path
					//to the goal
public: 
	int getSL(); //gets stepLimit
	void setSL(int limit); //sets the steplimit
	void addNewNode(Node* n); //adds a node to the vector of cities(nodes)
	Node* findNode(string n); //finds a node in the vector of cities(nodes) 
};
#endif