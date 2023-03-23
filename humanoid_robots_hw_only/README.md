# Humanoid Robots


Most of the exercises include heavily practical coding and some theoretical math


## Content

Linear algebra and the Eigen library, Odometry calibration with least squares, Projective geometry and camera parameters, Forward kinematics, kd-trees, Octree representation, Signed distance function, ICP, Particle filter, Path planning, Footstep planning, Anytime Repairing A* (ARA*), Zero Moment Point (ZMP), Inverse kinematics, Rapidly exploring Random Trees (RRT), Inverse reachability maps (IRM)


### C++ example from task

```cpp
#include <deneme1/deneme1.h>
#include <cmath>
```
```cpp
Node::MapType Node::nodes;

int hash_value(Node &node) {
	return node.x * 1000000 + node.y;
}

Node* Node::get(int& x, int&y) {
	Node *result = NULL;
	IndexType index = std::make_pair(x, y);
	MapType::iterator it = nodes.find(index);

	if (it == nodes.end()) {
		result = new Node(x, y);
		nodes[index] = result;
	} else {
		result = it->second;
	}
	return result;
}



FileIO *ClosedList::fileIO = NULL;

ClosedList::ClosedList() : duplicateWarning(false) {
}

ClosedList::~ClosedList() {
}

void ClosedList::add(Node* node) {
	if (!node) {
		throw std::invalid_argument("error");
	}
	if (fileIO) {
		fileIO->logCloseNode(node);
	}
	if (!duplicateWarning && list.find(node) != list.end()) {
		std::cerr << "error" << std::endl;
		duplicateWarning = true;
	}
	list.insert(node);
}
bool ClosedList::contains(Node* node) {
	if (node) {
		return list.find(node) != list.end();
	}
	return false;
}

```
```cpp
//euclidean
double GridPathPlanning::getCosts(Node* currentNode, Node* successorNode) {
	double result = 0.0;
    result = sqrt(pow(currentNode->x - successorNode->x, 2) + pow(currentNode->y - successorNode->y, 2));

	return result;
}

//straight line distance
double StraightLineDistanceHeuristic::heuristic(Node* currentNode, Node* goalNode) {
	double result = 0.0;
	result= sqrt(pow(currentNode->x - goalNode->x, 2) + pow(currentNode->y - goalNode->y, 2));

	return result;
}

//manhattan
double ManhattanDistanceHeuristic::heuristic(Node* currentNode, Node* goalNode) {
	double result = 0.0;
    result = abs(currentNode->x - goalNode->x) + abs(currentNode->y - goalNode->y);

	return result;
}

//neighbors
std::vector<Node*> GridPathPlanning::getNeighborNodes(Node* currentNode, Map& map) {
	std::vector<Node*> result;

        if(currentNode->x > 0 && currentNode->y > 0 && (map.isOccupied(currentNode->x-1,currentNode->y-1) == false)){
        	result.push_back( Node::get(currentNode->x-1,currentNode->y-1));
        }

        if(currentNode->y > 0 && (map.isOccupied(currentNode->x,currentNode->y-1) == false)){
        	result.push_back( Node::get(currentNode->x,currentNode->y-1));
        }

        if(currentNode->x < map.height-1 && currentNode->y > 0 && (map.isOccupied(currentNode->x+1,currentNode->y-1) == false)){
        	result.push_back( Node::get(currentNode->x+1,currentNode->y-1));
        }

        if(currentNode->x < map.height-1 && (map.isOccupied(currentNode->x+1,currentNode->y) == false)){
        	result.push_back( Node::get(currentNode->x+1,currentNode->y));
        }

        if(currentNode->x < map.height-1 && currentNode->y < map.width-1 && (map.isOccupied(currentNode->x+1,currentNode->y+1) == false)){
        	result.push_back( Node::get(currentNode->x+1,currentNode->y+1));
        }

        if(currentNode->y < map.width-1 && (map.isOccupied(currentNode->x,currentNode->y+1) == false)){
        	result.push_back( Node::get(currentNode->x,currentNode->y+1));
        }

        if(currentNode->x > 0 && currentNode->y < map.width-1 && (map.isOccupied(currentNode->x-1,currentNode->y+1) == false)){
        	result.push_back( Node::get(currentNode->x-1,currentNode->y+1));
        }

        if (currentNode->x > 0 && (map.isOccupied(currentNode->x-1,currentNode->y) == false)){
        	result.push_back( Node::get(currentNode->x-1,currentNode->y));
        }

	return result;
}

//is close to goal
bool GridPathPlanning::isCloseToGoal(Node* currentNode, Node* goalNode) {
	if((currentNode->x == goalNode->x) && (currentNode->y == goalNode->y))
		return true;
	else
		return false;
}
```
```cpp
std::deque<Node*> PathPlanning::planPath(Node* startNode, Node* goalNode) {
   	OpenList openList;
   	ClosedList closedList;
   	std::deque<Node*> resultPath;

	openList.enqueue(startNode, heuristic(startNode, goalNode));
	while (openList.isEmpty() == false) {
		Node* currentNode = openList.removeMin();
		if (isCloseToGoal(currentNode, goalNode)) {
			std::deque<Node*> path;
			path.push_front(currentNode);
			Node* temp = currentNode;

			while(temp->getPredecessor()!= nullptr){
		        temp = temp->getPredecessor();
			    path.push_front(temp);
			}

			return path;
		}
		closedList.add(currentNode);


		std::vector<Node*> neighbours = getNeighborNodes(currentNode);

	    for(int i=0 ;i< neighbours.size(); i++){

	    	double cost = getCosts(neighbours[i], currentNode) + currentNode->costs;
	    	if(openList.contains(neighbours[i]) && cost < getCosts(neighbours[i], currentNode))




			double fcost = currentNode->costs + getCosts(neighbours[i], currentNode) + heuristic(neighbours[i], goalNode);
	        if (openList.contains(neighbours[i])){
				
	            if( fcost < neighbours[i]->costs + heuristic(neighbours[i], goalNode)) {
	                neighbours[i]->setPredecessor(currentNode);
					neighbours[i]->costs = currentNode->costs + getCosts(neighbours[i], currentNode);
	                openList.updateCosts(neighbours[i], fcost);
	            }

	        }
	        else {
	            if (!closedList.contains(neighbours[i])) {

	                openList.enqueue(neighbours[i], fcost);
	                neighbours[i]->setPredecessor(currentNode);
	                neighbours[i]->costs = currentNode->costs + getCosts(neighbours[i], currentNode);
	            }
	        }
	    }

	} ;
	
	return resultPath;
}
```

Example from exercise
