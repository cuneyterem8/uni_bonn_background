#include "astar.hpp"

using namespace std::placeholders;

bool AStar::Vec2i::operator== (const Vec2i& c) const {
    return (x == c.x && y == c.y);
}

bool AStar::Vec2i::operator!= (const Vec2i& c) const {
    return (x != c.x && y != c.y);
}

AStar::Vec2i operator+ (const AStar::Vec2i& a, const AStar::Vec2i& b) {
    return{a.x + b.x, a.y + b.y};
}

AStar::Node::Node(Vec2i c, Node* par) {
    parent = par;
    coord = c;
    G = H = 0;
}

AStar::uint AStar::Node::getScore() const {
    return G + H;
}

AStar::Generator::Generator() {
    setDiagonalMovement(true);
    setHeuristic(&Heuristic::manhattan);
    direction = {
            { 0, 1}, {1,0}, { 0,-1}, {-1, 0},
            {-1,-1}, {1,1}, {-1, 1}, { 1,-1}
    };
}

void AStar::Generator::setWorldSize(Vec2i ws) {
    worldSize = ws;
}

void AStar::Generator::setDiagonalMovement(bool e) {
    directions = (e ? 8 : 4);
}

void AStar::Generator::setHeuristic(const heuristicFun& h) {
    heuristic = std::bind(h, _1, _2);
}

void AStar::Generator::addCollision(Vec2i c) {
    collisions.push_back(c);
}

void AStar::Generator::removeCollision(Vec2i c) {
    auto it = std::find(collisions.begin(), collisions.end(), c);
    if (it != collisions.end()) {
        collisions.erase(it);
    }
}

void AStar::Generator::clearCollisions() {
    collisions.clear();
}

AStar::coordVec AStar::Generator::findPath(Vec2i start, Vec2i finish) {
    Node* current = nullptr;
    NodeSet openList, closedList;
    openList.reserve(1000);
    closedList.reserve(1000);
    openList.push_back(new Node(start));

    while (!openList.empty()) {
        auto current_it = openList.begin();
        current = *current_it;

        for (auto it = openList.begin(); it != openList.end(); it++) {
            auto node = *it;
            if (node->getScore() <= current->getScore()) {
                current = node;
                current_it = it;
            }
        }

        if (current->coord == finish) {
            break;
        }

        closedList.push_back(current);
        openList.erase(current_it);

        for (uint i = 0; i < directions; ++i) {
            Vec2i newCoordinates(current->coord + direction[i]);
            if (detectCollision(newCoordinates) ||
                findNodeOnList(closedList, newCoordinates)) {
                continue;
            }

            uint totalCost = current->G + ((i < 4) ? 10 : 14);

            Node* successor = findNodeOnList(openList, newCoordinates);
            if (successor == nullptr) {
                successor = new Node(newCoordinates, current);
                successor->G = totalCost;
                successor->H = heuristic(successor->coord, finish);
                openList.push_back(successor);
            }
            else if (totalCost < successor->G) {
                successor->parent = current;
                successor->G = totalCost;
            }
        }
    }

    coordVec path;
    while (current != nullptr) {
        path.push_back(current->coord);
        current = current->parent;
    }

    releaseNodes(openList);
    releaseNodes(closedList);
    std::cout << "Path len: " << path.size() << '\n';
    return simplifyPath(path);
}

AStar::Node* AStar::Generator::findNodeOnList(NodeSet& n, Vec2i c) {
    for (auto node : n) {
        if (node->coord == c) {
            return node;
        }
    }
    return nullptr;
}

void AStar::Generator::releaseNodes(NodeSet& n) {
    for (auto it = n.begin(); it != n.end();) {
        delete* it;
        it = n.erase(it);
    }
}

bool AStar::Generator::detectCollision(Vec2i c) {
    if (c.x < 0 || c.x >= worldSize.x ||
        c.y < 0 || c.y >= worldSize.y ||
        std::find(collisions.begin(), collisions.end(), c) != collisions.end()) {
        return true;
    }
    return false;
}

AStar::Vec2i AStar::Heuristic::getDelta(Vec2i start, Vec2i finish) {
    return{abs(start.x - finish.x), abs(start.y - finish.y) };
}

AStar::uint AStar::Heuristic::manhattan(Vec2i start, Vec2i finish) {
    auto delta = getDelta(start, finish);
    return static_cast<uint>(10 * (delta.x + delta.y));
}

AStar::uint AStar::Heuristic::euclidean(Vec2i start, Vec2i finish) {
    auto delta = getDelta(start, finish);
    return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

void AStar::Generator::loadMap(const std::string& filename, int inflate_n) {
    std::ifstream file(filename, std::ifstream::in);

    if (file.is_open()) {
        std::string line;
        int dim_x = this->worldSize.x;
        int dim_y = this->worldSize.y;
        Vec2i newPoint{};
        int lineNum = 0;
        while(std::getline(file, line) || lineNum < 110) {
        //std::getline(file, line);
            for (int colNum = 0; colNum < 110; colNum++) {
                if (line[colNum] == '1' || line[colNum] == 'X') {
                    newPoint.x = colNum;
                    newPoint.y = 110-lineNum;
                    this->addCollision(newPoint);
                    for(int y = lineNum - inflate_n; y <= lineNum + inflate_n; y++) {
                        for(int x = colNum - inflate_n; x <= colNum + inflate_n; x++) {
                            if (y >= 0 && y < 110 && x >= 0 && x < 110) {
                                newPoint.x = x;
                                newPoint.y = 110-y;
                                this->addCollision(newPoint);
                            }
                        }
                    }
                }
            }
            lineNum++;
        }
        file.close();
    }
    else {
        std::cout << "Map file not found! " << std::endl;
        exit(0);
    }
}

AStar::coordVec AStar::Generator::simplifyPath(coordVec& path) {
    int first = 0;
    int second = 1;
    int third = 2;
    while (third < path.size()) {
        if (path[first].x * (path[second].y - path[third].y) + path[second].x * (path[third].y - path[first].y) + path[third].x * (path[first].y - path[second].y) == 0) {
            path.erase(path.begin() + second);
        }
        else {
            first++;
            second++;
            third++;
        }
    }
    std::cout << "Simplified path len: " << path.size() << '\n';
    return path;
}
