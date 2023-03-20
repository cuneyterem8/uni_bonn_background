#ifndef ASTAR_TEST_ASTAR_HPP
#define ASTAR_TEST_ASTAR_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <set>
#include <string>

namespace AStar {
    struct Vec2i {
        Vec2i() = default;
        Vec2i(int x_, int y_) : x(x_), y(y_) {};
        int x{}, y{};

        bool operator == (const Vec2i& c) const;
        bool operator != (const Vec2i& c) const;
    };

    using uint = unsigned int;
    using heuristicFun = std::function<uint(Vec2i, Vec2i)>;
    using coordVec = std::vector<Vec2i>;

    struct Node {
        uint G, H;
        Vec2i coord{};
        Node* parent;

        explicit Node(Vec2i coord, Node* par = nullptr);
        uint getScore() const;
    };

    using NodeSet = std::vector<Node*>;

    class Generator {
        bool detectCollision(Vec2i c);
        static Node* findNodeOnList(NodeSet& n, Vec2i c);
        static void releaseNodes(NodeSet& n);

    public:
        Generator();
        void setWorldSize(Vec2i ws);
        void setDiagonalMovement(bool e);
        void setHeuristic(const heuristicFun& h);
        coordVec findPath(Vec2i start, Vec2i finish);
        void addCollision(Vec2i c);
        void removeCollision(Vec2i c);
        void clearCollisions();
        void loadMap(const std::string& filename, int inflate_n);
        static coordVec simplifyPath(coordVec& path);

    private:
        heuristicFun heuristic;
        coordVec direction, collisions;
        Vec2i worldSize{};
        uint directions{};

    };

    class Heuristic {
        static Vec2i getDelta(Vec2i start, Vec2i finish);

    public:
        static uint manhattan(Vec2i start, Vec2i finish);
        static uint euclidean(Vec2i start, Vec2i finish);
    };
}

#endif //ASTAR_TEST_ASTAR_HPP
