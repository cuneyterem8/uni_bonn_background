#include <iostream>
#include "a_star_algo.h"

using namespace path_planning;

int main(int /* argc */, char ** /*argv*/) 
{
	const std::string packagePath = "C:/Users/Bayezit Fatih EREM/Desktop/cpp_deneme/11_path_planning";
	std::string str (packagePath + "/data/map.pbm");
	const Map * map;

	std::ifstream ifs(str.c_str());
	if (ifs.good()) {
		std::string header;
		ifs >> header;

		if (header == "P1") {
			size_t width = 0, height = 0;
			ifs >> width >> height;
			if (width <= 100 || height <= 100) {
				std::vector<bool> data;
				data.reserve(width * height);
				char c;
				while (ifs.get(c)) {
					switch (c) {
					case '0': data.push_back(false); break;
					case '1': data.push_back(true); break;
					default:
						break;
					}
				}
				ifs.close();

				map = Map(width, height, data);
			}
			throw std::runtime_error("map size");
		}
		map = NULL;
	}
	map = NULL;
	
	if (!map) {
		return 1;
	}
	OpenList::map = map;

	Node* startNode = Node::get(4, 9);
	Node* goalNode = Node::get(2, 7);
	PathPlan heuristic = PathPlan::heuristic(startNode, goalNode);
	PathPlan planning(*map, heuristic);
	std::deque<Node*> path = planning.plan_path(startNode, goalNode, *map);

	if (path.empty()) {
		std::cout << "empty" << std::endl;
	} else {
		std::cout << std::endl;
		for (std::deque<Node*>::const_iterator it = path.begin(); it != path.end(); ++it) {
			if (!*it) {
				throw std::runtime_error("path is null");
			}
			std::cout << (*it)->toString() << std::endl;
		}
	}

	delete map;
	map = NULL;

	return 0;
}