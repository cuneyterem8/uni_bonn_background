#include <deneme1/deneme1.h>
#include <cmath>

namespace deneme1 {


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





/**
 * @brief Plans a path from a start node to a goal node.
 * @param[in] startNode The start node.
 * @param[in] goalNode The goal node.
 * @return The optimal path from the start node to the end node.
 */
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




//deneme.h; pathplanning altına

	virtual double heuristic(Node* currentNode, Node* goalNode) {
		if (!currentNode && !goalNode) {
			throw std::invalid_argument("error null");
		}
		return heuristic(static_cast<Node*>(currentNode), static_cast<Node*>(goalNode));
	}
	virtual double getCosts(Node* currentNode, Node* successorNode) {
		if (!currentNode && !successorNode) {
			throw std::invalid_argument("error null getcosts");
		}
		return getCosts(static_cast<Node*>(currentNode), static_cast<Node*>(successorNode));
	}
	virtual std::vector<Node*> getNeighborNodes(Node* currentNode, GridMap& map) {
		if (!currentNode && !map) {
			throw std::invalid_argument("error null neighbors");
		}
		return getNeighborNodes(static_cast<Node*>(currentNode), static_cast<GridMap*>(map));
	}
	virtual bool isCloseToGoal(Node* currentNode, Node* goalNode) {
		if (!currentNode && !goalNode) {
			throw std::invalid_argument("error null isclosetogoal");
		}
		return isCloseToGoal(static_cast<Node*>(currentNode), static_cast<Node*>(goalNode));
	}











}  // namespace deneme1