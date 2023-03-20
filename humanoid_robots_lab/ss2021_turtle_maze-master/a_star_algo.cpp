
#include <iostream>
#include <set>

#include "a_star_algo.h"

a_star_algo::a_star_algo()
{

}

bool fileIO = true;
Map *Open_list_class::map = NULL;
Map *PathPlan::map = NULL;

Open_list_class::Open_list_class() : warning_dup(false), warning_reinsert(false), data(map->map_width * map->map_height) {
}

Open_list_class::~Open_list_class() {
}

//---------------------------------------------------------------

Open_list_class::olist_data& Open_list_class::get_data(Node* node) {
    Node* node_data_i = dynamic_cast<Node*>(node);

    if (node_data_i) {
        if((node_data_i->y * map->map_width + node_data_i->x) > 0 && (node_data_i->y * map->map_width + node_data_i->x) < data.size())
            return data[node_data_i->y * map->map_width + node_data_i->x];
    } else {
        throw std::invalid_argument("error");
    }

    Open_list_class::olist_data cost2 = data[node_data_i->y * map->map_width + node_data_i->x];

    return data[node_data_i->y * map->map_width + node_data_i->x];
}


void Open_list_class::enqueue(Node* node, double cost) {
    if (node) {
        fileIO == true;
    }
    else {
        throw std::invalid_argument("error");
    }

    Open_list_class::olist_data& node_data = get_data(node);

    if (node_data.state == olist_data::OPEN && warning_dup == false) {
        warning_dup = true;
    }
    else if (node_data.state == olist_data::CLOSED && warning_reinsert == false) {
        warning_reinsert = true;
    }
    
    

    node_data.state = olist_data::OPEN;
    node_data.cost = cost;

    Node_cost *node_cost = new Node_cost(node, cost);
    open_list.push(node_cost);
}


Node* Open_list_class::remove_min() {
    while (!open_list.empty()) {
        Node_cost *node_cost = open_list.top();
        Node *node = node_cost->node;
        open_list.pop();
        delete node_cost;

        Open_list_class::olist_data& node_data = get_data(node);
        if (node_data.state == olist_data::OPEN) {
            node_data.state = olist_data::CLOSED;
            return node;
        }
    }
    if(open_list.empty())
        throw std::invalid_argument("error");
    return nullptr;
}


void Open_list_class::update_cost(Node* node, double cost) {
    if (node) {
        Open_list_class::olist_data& node_data = get_data(node);
        if (node_data.state == olist_data::OPEN) {
            node_data.cost = cost;

            Node_cost *node_cost = new Node_cost(node, cost) ;
            open_list.push(node_cost);
        }
        else
            throw std::invalid_argument("error");
    }
    else
        throw std::invalid_argument("error");
}

double Open_list_class::get_open_cost(Node* node) {
    if (node) {
        Open_list_class::olist_data& node_data = get_data(node);

        if (node_data.state == olist_data::OPEN) {
            return node_data.cost;
        }
        else
            throw std::invalid_argument("error");
    }
    else
        throw std::invalid_argument("error");
    return get_data(node).cost;
}

bool Open_list_class::is_empty() {
    while(!open_list.empty() && get_data(open_list.top()->node).state == olist_data::CLOSED) {
        open_list.pop();
    }
    return open_list.empty();
}


//---------------------------------------------------------------


Node::map_pair Node::nodes;

Node* Node::get(const int& x, const int& y) {
    Node *get_node = NULL;
    IndexType pair_i = std::make_pair(x, y);
    map_pair::const_iterator pair = nodes.find(pair_i);

    if (pair == nodes.end()) {
        get_node = new Node(x, y);
        nodes[pair_i] = get_node;
    } else {
        get_node = pair->second;
    }
    return get_node;
}

//---------------------------------------------------------------

//euclidean
double PathPlan::get_plan_cost(Node* current_node, Node* successorNode) {
    double result = 0.0;
    result = sqrt(pow(current_node->x - successorNode->x, 2) + pow(current_node->y - successorNode->y, 2));
    return result;
}

//manhattan
double PathPlan::heuristic(Node* current_node, Node* goalNode) {
    double result = 0.0;
    result = abs(current_node->x - goalNode->x) + abs(current_node->y - goalNode->y);
    return result;
}

//neighbors
std::vector<Node*> PathPlan::get_neighbor(const Node* current_node) {
    std::vector<Node*> result;

    if(current_node->x > 0 && current_node->y > 0 && (map->is_occupied(current_node->x-1,current_node->y-1) == false)){
        result.push_back( Node::get(current_node->x-1,current_node->y-1));
    }

    if(current_node->y > 0 && (map->is_occupied(current_node->x,current_node->y-1) == false)){
        result.push_back( Node::get(current_node->x,current_node->y-1));
    }

    if(current_node->x < map->map_height-1 && current_node->y > 0 && (map->is_occupied(current_node->x+1,current_node->y-1) == false)){
        result.push_back( Node::get(current_node->x+1,current_node->y-1));
    }

    if(current_node->x < map->map_height-1 && (map->is_occupied(current_node->x+1,current_node->y) == false)){
        result.push_back( Node::get(current_node->x+1,current_node->y));
    }

    if(current_node->x < map->map_height-1 && current_node->y < map->map_width-1 && (map->is_occupied(current_node->x+1,current_node->y+1) == false)){
        result.push_back( Node::get(current_node->x+1,current_node->y+1));
    }

    if(current_node->y < map->map_width-1 && (map->is_occupied(current_node->x,current_node->y+1) == false)){
        result.push_back( Node::get(current_node->x,current_node->y+1));
    }

    if(current_node->x > 0 && current_node->y < map->map_width-1 && (map->is_occupied(current_node->x-1,current_node->y+1) == false)){
        result.push_back( Node::get(current_node->x-1,current_node->y+1));
    }

    if (current_node->x > 0 && (map->is_occupied(current_node->x-1,current_node->y) == false)){
        result.push_back( Node::get(current_node->x-1,current_node->y));
    }

    return result;
}


std::deque<Node*> PathPlan::plan_path(Node* startNode, Node* goalNode) {
    Open_list_class open_list_plan;
    std::set<Node*> closed_list_plan;
    bool warning_dup;
    std::deque<Node*> resultPath;

    open_list_plan.enqueue(startNode, heuristic(startNode, goalNode));
    while (!open_list_plan.is_empty()) {
        Node* current_node = open_list_plan.remove_min();

        if((current_node->x == goalNode->x) && (current_node->y == goalNode->y)) {
            std::deque<Node*> path;
            path.push_front(current_node);

            Node* temp = current_node;
            while(temp->getPredecessor()!= nullptr){
                temp = temp->getPredecessor();
                path.push_front(temp);
            }
            resultPath = path;

            std::cout << "resultPath: " << resultPath.size() << std::endl;

            return path;
        }
        if (current_node) {
            fileIO == true;
        }
        else {
            throw std::invalid_argument("closed error add node");
        }

        if (warning_dup == false && (closed_list_plan.find(current_node) != closed_list_plan.end())) {
            warning_dup = true;
        }
        if(warning_dup == false) {
            closed_list_plan.insert(current_node);
        }

        std::vector<Node*> neighbours = get_neighbor(current_node);

        //from this point

        for(int i=0; i< neighbours.size(); i++){
            double cost = current_node->cost + get_plan_cost(neighbours[i], current_node) + heuristic(neighbours[i], goalNode);
            if (open_list_plan.get_data(neighbours[i]).state == Open_list_class::olist_data::OPEN){
                if( cost < neighbours[i]->cost + heuristic(neighbours[i], goalNode)) {
                    neighbours[i]->setPredecessor(current_node);
                    neighbours[i]->cost = current_node->cost + get_plan_cost(neighbours[i], current_node);
                    open_list_plan.update_cost(neighbours[i], cost);
                }
            }
            else {
                if (closed_list_plan.find(neighbours[i]) == closed_list_plan.end()) {

                    open_list_plan.enqueue(neighbours[i], cost);
                    neighbours[i]->setPredecessor(current_node);
                    neighbours[i]->cost = current_node->cost + get_plan_cost(neighbours[i], current_node);
                }
            }
        }

        //std::cout << "neighbours: " << neighbours.size() << std::endl;
        //std::cout << "open_list_plan: " << open_list_plan.data.size() << std::endl;
        //std::cout << "closed_list_plan: " << closed_list_plan.size() << std::endl;

    };

    return resultPath;
}
