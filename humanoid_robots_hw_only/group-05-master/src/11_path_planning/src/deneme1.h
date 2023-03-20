
#include <stdexcept>
#include <string>
#include <map>
#include <sstream>
#include <vector>

namespace path_planning {

struct Map {
public:
	size_t width;
	size_t height;
	std::vector<bool> data;

	bool isOccupied(int& x, int& y) {
		if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= static_cast<int>(width)) {
			throw std::out_of_range("error");
		}
		return data[y * width + x];
	}

	Map(size_t& width, size_t& height, std::vector<bool>& data) : width(width), height(height), data(data) {};
};


class Node {
public:
	double costs; //g
	int x;
	int y;

   	void setPredecessor(Node *predecessor) {
   		if (predecessor != this) {
   			this->predecessor = predecessor;
   		}
   	}

   	Node* getPredecessor() {
   		return this->predecessor;
   	}

	Node() : costs(0.0), predecessor(NULL) {};
	Node(int& x, int& y) : x(x), y(y) {};
	virtual ~Node() {};

	Node *predecessor;
	static Node* get(int& x, int& y);
	typedef std::pair<int, int> IndexType;
	typedef std::map<IndexType, Node*> MapType;
	static MapType nodes;
};

int hash_value(Node &node);


class ClosedList {
public:
	ClosedList();
	virtual ~ClosedList();

	void add(Node* node);
	bool contains(Node* node);
	std::set<Node*> list;
	bool duplicateWarning;
	static FileIO *fileIO; 
};



class OpenList {
public:
	OpenList();
	virtual ~OpenList();
	void enqueue(const AbstractNode * const node, const double costs);
	const AbstractNode * removeMin();
	void updateCosts(const AbstractNode * const node, const double costs);
	bool isEmpty() const;
	bool contains(const AbstractNode * const node) const;
	double getCosts(const AbstractNode * const node) const;

protected:
	struct NodeWrapper;
	struct CompareNode {
		bool operator()(const NodeWrapper* const node1, const NodeWrapper* const node2) const {
			return node1->costs > node2->costs;
		}
	} compareNode;

	struct NodeWrapper {
		const AbstractNode * node;
		double costs;
		NodeWrapper(const AbstractNode * const node, const double costs) : node(node), costs(costs) {}
	};

	/**
	 * @brief Priority queue, partially ordered (heap)
	 */
	typedef std::priority_queue<NodeWrapper*, std::vector<NodeWrapper*>, CompareNode> Queue;

	/**
	 * @brief Hash map mapping for looking up the wrapped node corresponding to an AbstractNode pointer.
	 */
	typedef std::map<const AbstractNode *, NodeWrapper*> HashMap;

	mutable Queue openList;  ///< Priority queue for the open list.
	bool duplicateWarning;   ///< True if a node has been added twice to the open list.
	bool reinsertWarning;    ///< True if a node has been inserted to the open list that has already been closed.

public:
	static FileIO *fileIO;  ///< Helper for logging data.
	static bool logToStdout;      ///< Write log messages to stdout.
	static const GridMap *map;

private:
	struct OpenListData {
	    enum { NEW, OPEN, CLOSED } state;
	    double cost;
	    OpenListData() : state(NEW), cost(0.0) {}
	};
	mutable std::vector<OpenListData> data;
	OpenListData& getData(AbstractNode * node);
	OpenListData& getData(const AbstractNode * const node) const;
};




} // namespace deneme1

