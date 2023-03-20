#include <iostream>
#include <fstream>
#include <deque>

#include "a_star_algo.h"
#include "a_star_algo.cpp"

int main(int /* argc */, char ** /*argv*/) 
{
	const std::string packagePath = "C:/Users/Bayezit Fatih EREM/Desktop/cpp_deneme/11_path_planning";
	std::string str (packagePath + "/data/map.txt");
	Map* map;

	
    std::vector<char> data;
    char byte;

    std::ifstream input_file(str);
	size_t width = 11, height = 8;
    if (!input_file.is_open()) {
        return EXIT_FAILURE;
    }

    while (input_file.get(byte)) {
        data.push_back((int)byte);
    }
    input_file.close();

	//for (const auto &i : data) {
    //    std::cout << i << "-";
    //}
	
	map = new Map(width, height, data);


	if (!map) {
		return 1;
	}
	Open_list_class::map = map;
	PathPlan::map = map;

	Node* startNode = Node::get(0, 0);
	Node* goalNode = Node::get(10, 7);
	PathPlan planning;
	std::deque<Node*> path = planning.plan_path(startNode, goalNode);

	if (path.empty()) {
		std::cout << "empty" << std::endl;
	} else {
		std::cout << std::endl;
		for (std::deque<Node*>::const_iterator it = path.begin(); it != path.end(); ++it) {
			if (!*it) {
				throw std::runtime_error("path is null");
			}
			std::stringstream ss;
			ss << "(" << (*it)->x << ", " << (*it)->y << ")";
			std::cout << ss.str() << std::endl;
		}
	}

	std::cout << "state 4" << std::endl;

	delete map;
	map = NULL;

	return 0;
}


