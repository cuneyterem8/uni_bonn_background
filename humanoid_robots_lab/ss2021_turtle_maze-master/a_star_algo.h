#ifndef A_STAR_ALGO_H
#define A_STAR_ALGO_H

#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cmath>

class a_star_algo
{
public:
    a_star_algo();
};

struct Map {
public:
    size_t map_height;
    size_t map_width;
    std::vector<char> map_data;
    bool is_occupied(const int& x, const int& y) {
        if (x >= 0 && x < static_cast<int>(map_width)) {
            if(y >= 0 && y < static_cast<int>(map_height)) {
                if(map_data[y * map_width + x] == 1)
                    return true;
                else
                    return false;
            }
        }
        return false;
    }

    Map(size_t& map_width, size_t& map_height, std::vector<char>& map_data) : map_width(map_width), map_height(map_height), map_data(map_data) {};
};


//---------------------------------------------------------------

class Node {
public:
    double cost;
    void setPredecessor(Node *predecessor) {
        if (predecessor == this) {
            throw std::invalid_argument("error loop");
        }
        this->predecessor = predecessor;
    }
    Node* getPredecessor() {
        return this->predecessor;
    }

    Node() : cost(0.0), predecessor(NULL) {};
    virtual ~Node() {};

    int x;
    int y;
    static Node* get(const int& x, const int& y);
    Node(const int& x, const int& y) : x(x), y(y) {};

private:
    Node *predecessor;
    
    typedef std::pair<int, int> IndexType;
    typedef std::map<IndexType, Node*> map_pair;
    static map_pair nodes;
};

//---------------------------------------------------------------


class Open_list_class {
public:
    Open_list_class();
    virtual ~Open_list_class();

    void enqueue(Node* node, double cost);
    Node* remove_min();
    void update_cost(Node* node, double cost);
    bool is_empty();
    bool contains(Node* node);
    double get_open_cost(Node* node);

protected:
    struct Node_cost {
        Node* node;
        double cost;
        Node_cost(Node* node, double cost) : node(node), cost(cost) {}
    };
    struct Greater_node {
        bool operator()(Node_cost* node1, Node_cost* node2){
            return node1->cost > node2->cost;
        }
    };

    typedef std::priority_queue<Node_cost*, std::vector<Node_cost*>, Greater_node> Queue;
    mutable Queue open_list;
    bool warning_dup;
    bool warning_reinsert;

public:
    static Map *map;
    struct olist_data {
        enum { NEW, OPEN, CLOSED } state;
        double cost;
        olist_data() : state(NEW), cost(0.0) {}
    };

    mutable std::vector<olist_data> data;
    olist_data& get_data(Node * node);
};

//---------------------------------------------------------------



class PathPlan
{
public:
    PathPlan() {};
    ~PathPlan() {};
    static Map *map;
    static std::deque<Node*> plan_path(Node* startNode, Node* goalNode);
    static std::vector<Node*> get_neighbor(const Node* current_node);
    static double heuristic(Node* current_node, Node* goalNode);
    static double get_plan_cost(Node* current_node, Node* successorNode);

};


#endif // A_STAR_ALGO_H
