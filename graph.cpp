#include "graph.h"	

using namespace std; 

Node::Node(string n , int heur){ //constructor for Node object
	name = n; //sets name to n
	h = heur; //sets h to heur
	g = 0; //sets total pathcost to 0
	Status = NOT_VISITED; //sets status to NOT_VISITED
	}

string Node::getName(){ //returns name of node
	return name; 
}

string Node::getPath(){ //gets the total travelled path to get to the node
	return path; 
}

void Node::setPath(Edge e){ //sets the path to get to the node, has an edge e as a parameter
	string num; //creates string num
	stringstream out; //creates stringstream out
	out << e.getPathcost(); //makes out to be written on by pathcost of edge
	num = out.str(); //sets num to be out as a string...converts an integer to string

	path = e.getPNode()->getPath() + e.getPNode()->getName() + ' ' + num + ' ' + getName() + '\n';
	//sets the path as the path of parentNode of the edge annotated with a string that has the format:
	// <parentNode pathcost childnode> i.e. Arad 140 Sibiu, where 'num' is the pathcost getName() gets the name of 
	//the childNode which should be the node being looked at and e.getPNode()->getName() gets name of parentNode
	}

int Node::getHeur() {//returns heuristic value of the node
		return h;
}

int Node::getF() const{ //returns F value of the node
	return g + h; //F = g + h
}

void Node::setG(Edge e){ //sets the g(total pathcost to get to node), has a Edge e as the parameter
	g = e.getPathcost() + e.getPNode()->getG(); 
	//takes pathcost of the Edge and takes it to the pathcost of the parentNode to set pathcost of of this node(child node)
}

int Node::getG(){ //returns value of g
	return g; 
}

//adds edge to edge(children) vector of node. Takes parameters Node* child which is pointer to the child node and
//pc which is the pathcost
void Node::addchild(Node* child, int pc){ 
	Edge temp(this, child, pc); //creates an Edge called temp with this (stands for the node calling the function)
								//as parent node, child as child node, and pc as pathcost

	children.push_back(temp);	//Pushes the new edge into the children vector of edges
}

vector<Edge> Node::getChildren() //returns the children vector of edges
{
return children;
}


int Node::getStatus(){ //returns the sttaus of the node(city)
	return Status; 
}

void Node::setStatus(){ //sets status of the node to VISITED
	Status = VISITED; 
}

Edge::Edge(Node* pN, Node* cN, int pc){ //contructor for Edge object
	parentNode = pN; //sets parentNode to pointer to node pN
	childNode = cN;  //sets childNode to pointer to node cN
	pathcost = pc; //sets pathcost as pc

}

Node* Edge::getPNode(){ //returns pointer to node, parentNode
	return parentNode; 
}


Node* Edge::getCNode(){	//returns pointer to node, childNode
	return childNode; 
}; 

int Edge::getPathcost(){ //returns pathcost
	return pathcost; 
}


void graph::addNewNode(Node* n){ //adds a node* to its vector of node*s
	nList.push_back(n); //pushes Node* n into vector
}

Node* graph::findNode(string n){ //finds a node with the name n
	for(int i = 0 ; i < nList.size(); i++) //looks at each node* in vector
	{
		if(nList[i]->getName() == n) //if it finds a node with a name that is n
			return nList[i]; //return that node*
	}
	return NULL; //otherwise return NULL
}


int graph::getSL(){ //gets depthlimit
	return stepLimit; 
}

void graph::setSL(int limit){ //sets the depthlimit to int depth
	stepLimit = limit; 
}

