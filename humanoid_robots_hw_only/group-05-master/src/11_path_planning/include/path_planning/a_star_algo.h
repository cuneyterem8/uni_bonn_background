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

//#include <path_plan/FileIO.h>

class a_star_algo
{
public:
    a_star_algo();
};


struct Map {
public:
    const size_t map_height;
    const size_t map_width;
    std::vector<bool> map_data;
    bool is_occupied(const int& x, const int& y) const{
        if (x >= 0 || x < static_cast<int>(map_width)) {
            if(y >= 0 || y < static_cast<int>(map_width)) {
                return map_data[y * map_width + x];
            }
            throw std::out_of_range("error boundary");
        }
        else {
            throw std::out_of_range("error boundary");
        }
    }
    Map(const size_t& map_width, const size_t& map_height, const std::vector<bool>& map_data) : map_width(map_width), map_height(map_height), map_data(map_data) {};

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

private:
    Node *predecessor;
    Node(const int& x, const int& y) : x(x), y(y) {};
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
//    static FileIO *fileIO;
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
    virtual ~PathPlan() {};
    virtual std::deque<Node*> plan_path(Node* startNode, Node* goalNode, const Map& map);
    virtual std::vector<Node*> get_neighbor(const Node* current_node, const Map& map) = 0;
    virtual double heuristic(Node* current_node, Node* goalNode) = 0;
    virtual double get_plan_cost(Node* current_node, Node* successorNode) = 0;

};


#endif // A_STAR_ALGO_H
