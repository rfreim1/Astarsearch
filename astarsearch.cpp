#include "graph.h" //includes Node/edge/graph class and their member functions and data members as well as any files/directories needed to be included


//fills in the graph's vector of Nodes using the heuristic file given
void fillgraph(char* hfile, graph* g){ //takes filename and ptr to graph to create nodes on as parameters
	string name; //name of Node
	int h; //hueristic

	Node* temp; //temporary node
	ifstream heuristic(hfile); //use ifstream to read hfile

	while(heuristic >> name){  //while the heuristic can write to write to name, write first word from line to name 
		heuristic >> h; //write heuristic # to h
		temp = new Node(name, h); //create a new node using name and heuristic from file
		g->addNewNode(temp);  //add to graph's node vector
	} 
}

//fills in the edges for each Node in the graph
void fillEdges(char* cfile, graph* g){//takes connectivity file name and ptr to graph with nodes to write edges to
	string pName; //name of parent node
	string cName; //name of child node
	int pc; //path cost
	ifstream connect(cfile); //uses ifstream to read cfile 

	while (connect >> pName){ //while it can write to pName variable
		Node* temp = g->findNode(pName); //create temp ptr to node pointing to node with name given by pName
		connect >> pc; //writes pathcost to pc
		connect >> cName; //writes child node name to cName
		Node* temp2 = g->findNode(cName); //creates another temp ptr to node pointing to node with name given by cName
		temp->addchild(temp2, pc); //adds a edge with child node as temp2 and pathcost as pc
	}
}


//searches to try to see if a node is already in the frontier
int searchFrontier(vector<Node*> pq, string name){ //takes vector of Node*(frontier) and name of node to find as parameters
	if (pq.empty()) //if vector is empty return -1
		return -1;
	else{ //otherwise search through vector one element at a time
		for (int i = 0; i < pq.size(); i++)
			if (pq[i]->getName() == name) //if the name of node in the frontier is the name of node to find
				return i; //return the place in the frontier that the node is located
		return -1; //if no node with the name entered is in frontier return -1
	}	
}


void astarsearch(char* argv[]){
	int num_steps = 0; //step counter seeing how many times a node is taken out of the frontier, not counting removing the Goal from the priority queue
	int path_cost = 0; //int to hold total pathcost to get to goal 

	graph* romCity = new graph();  //creates a new graph called romCity(romanian cities)
	romCity->setSL(15);  //sets the value of d to an arbituary value of 15 since no path should be longer than 9 however the function may take more than that many steps to find the goal

	fillgraph(argv[2], romCity); //fills the graphs vector of nodes by reading from heuristic file
	fillEdges(argv[1], romCity); //files every nodes vector of Edges by reading from connectivity file

	ofstream outputfile ("lab1.txt"); //creates a output file called lab1.txt
	if (outputfile.is_open()) { //while the file is open
		outputfile << "Initial State = " << argv[3] << endl; //enters intial state as 3rd argument on cmdline (excluding call to .exe)
		outputfile << "Goal state = " << argv[4] << endl; //enters goal state as 4th agrument on cmdline
	}
	else
		cerr << "ERROR. Unable to open newly created file."; //if it cannot open the file for some reason output an error

	vector<Node*> frontier; //sets up a vector of Node*s which will be used as the priority queue

	Node* Start = romCity->findNode(argv[3]); //finds pointer to starting node and sets Start to it
	Node* Goal = romCity->findNode(argv[4]);  //finds pointer to goal node and sets Goal to it

	frontier.push_back(Start); //adds the first node to the frontier vector

	while(num_steps < romCity->getSL()){ //while the number of steps is less than the depthlimit

		if(frontier.empty()){ //if the frontier ever becomes empty then report failure and end loop
			outputfile << "Path found = false\n"; //writes to lab1.txt that no path has been found
			outputfile << "Steps to solve = " << num_steps; //writes number of steps to find failure to lab1.txt
			outputfile.close(); //closes the file
			break; //breaks the while loop to get out of it
		} 

		std::sort(frontier.begin(), frontier.end(), CompareNodeF()); //sorts the entire frontier vector using parameters of CompareNodeF() to see which F value is lowest and sorting it accordingly 
		Node* lowest = frontier.front(); //sets top to the first Node* in the vector, i.e. the one with the lowest F value

		path_cost = path_cost + lowest->getG(); //sets the pathcost to be the already accumulated path cost and the pathcost of the top node
		lowest->setStatus(); //sets the status of the lowest F node as VISITED



		frontier.erase(frontier.begin()); //erases top element, i.e. the one with the lowest F value, from the frontier
		

		if (lowest->getName() == Goal->getName()){ //if the node just removed from the frontier has the same name as the goal node
			outputfile << "Path found = true\n"; //writes that a path has been found to the goal
			outputfile << "Steps to solve = " << num_steps << endl; //writes number of steps to find failure to lab1.txt
			outputfile << "Path cost = " << lowest->getG() << endl; //writes path cost to goal
			outputfile << lowest->getPath(); //writes out lowest cost path to goal
			outputfile.close(); //closes file
			break; //breaks while loop
		}

		num_steps++; //increments the number of steps
		vector<Edge>CList = lowest->getChildren(); //get children(edges) of lowest F node(just popped from frontier)

		for (int i = 0; i < CList.size(); i++){ //for every child(edge) of the node just taken off the frontier
			Node* Child = CList[i].getCNode(); //sets childNode of edge to Child
			int iter = searchFrontier(frontier, Child->getName()); //check and see if that childNode has already been added to the frontier before
			Node* temp = new Node(Child->getName(), Child->getHeur());  //sets up a temporary node so that compare two pathcosts if a node is already in the frontier
			temp->setG(CList[i]); //gets the pathcost of the temporary node

			if (Child->getStatus() == 1){ //if the childNode of the edge has been VISITED 
				continue; //skip it and go to next child(edge)
			}
			else if(iter > -1){ //else if the node is already in the frontier
				if (Child->getG() > temp->getG()){ //check to see if the node in the frontier has a higher pathcost value than the childNode of this edge(temp)
					Child->setG(CList[i]); //if the one in the frontier is greater, set the childNodes pathcost
					Child->setPath(CList[i]); //and set the childNodes path travelled
				}
				else
					continue; //otherwise skip that child node from being added to the frontier/updated
			}
			else{ //otherwise
				Child->setG(CList[i]); //set the pathcost of the Node
				frontier.push_back(Child); //put the childNode Child into the frontier 
				Child->setPath(CList[i]); //set the path of the childNode
			}
		}
	}

	if (num_steps == romCity->getSL()){ //if the while loop runs its course and too many steps are taken
		outputfile << "Path found = false\n"; //writes to lab1.txt that no path has been found
		outputfile << "Steps to solve = " << num_steps; //writes number of steps to find failure to lab1.txt
		outputfile.close(); //closes the file
	}
}


int main(int argc, char* argv[]){  
	char* buffer; //buffer used to hold value of current working directory

	astarsearch(argv); 

	// Get the current working directory: 
	if( (buffer = _getcwd( NULL, 0 )) == NULL ) //if an error getting cwd
		perror( "_getcwd error" ); //print error
	else
	{
		cout << buffer; //output directory
		cout << "\\lab1.txt"; //add output file name to output of directory
		free(buffer); //free memory in buffer
	}

	return 0; 
}