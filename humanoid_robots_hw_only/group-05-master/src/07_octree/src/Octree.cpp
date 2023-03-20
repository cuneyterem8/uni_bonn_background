#include <octree/Octree.h>
#include <iostream>

namespace octree {

/**
 * \brief Inserts a new point into the octree and marks the corresponding cell as occupied.
 * \param[in] point The 3D point to insert into the octree.
 * \return true if the point has been successfully inserted, false otherwise
 *
 * This method implements the framework for your application. You don't have to
 * change anything here.
 */
bool Octree::insertPoint(const Eigen::Vector3d& point) {
	// Find existing node that contains the point
	Node *node = findNode(point);
	if (!node) {
		// This should not happen.
		std::cerr << "Could not find node containing the new point." << std::endl;
		return false;
	}

	// Is the node already marked as occupied? Then there's nothing do do.
	if (node->content == OCCUPIED) {
		return true;
	}

	// Is the node already at maximum depth? Then mark it as occupied
	if (node->depth == maxDepth) {
		node->content = OCCUPIED;
	} else {
		// Split the node recursively until the maximum depth is reached
		while (node->depth < maxDepth) {
			node = node->split(point);
			if (!node) {
				// This should not happen.
				std::cerr << "Splitting node returned NULL." << std::endl;
				return false;
			}
		}
	}

	// Traverse the tree towards the root and merge cells
	for (node = node->parent; node != NULL; node = node->parent) {
		const bool merged = node->merge();
		if (!merged) {
			// Could not merge -> nothing to do anymore
			break;
		}
	}
	return true;
}

/**
 * \brief Returns the index (0-7) of the child cube containing a given 3D point.
 * \param[in] point The 3D point
 * \return The index of the child cube containing the point.
 */
unsigned int Node::findIndex(const Eigen::Vector3d& point) const {
	unsigned int index = 0;
	/* TODO: Compute the index of the child node containing the given point.
	 *
	 * Member variables that you can use:
	 * - const Eigen::Vector3d corner1: first corner of the current cube
	 * - const Eigen::Vector3d corner2: opposite corner of the current cube
	 */
	double medium_x = (corner1(0) + corner2(0)) / 2;
	double medium_y = (corner1(1) + corner2(1)) / 2;
	double medium_z = (corner1(2) + corner2(2)) / 2;

	if ((point(0) <= medium_x) && (point(1) <= medium_y) && (point(2) <= medium_z)) {
		index = 0;
	}
	if ((point(0) <= medium_x) && (point(1) > medium_y) && (point(2) <= medium_z)) {
		index = 2;
	}
	if ((point(0) <= medium_x) && (point(1) <= medium_y) && (point(2) > medium_z)) {
		index = 4;
	}
	if ((point(0) <= medium_x) && (point(1) > medium_y) && (point(2) > medium_z)) {
		index = 6;
	}
	if ((point(0) > medium_x) && (point(1) <= medium_y) && (point(2) <= medium_z)) {
		index = 1;
	}
	if ((point(0) > medium_x) && (point(1) > medium_y) && (point(2) <= medium_z)) {
		index = 3;
	}
	if ((point(0) > medium_x) && (point(1) <= medium_y) && (point(2) > medium_z)) {
		index = 5;
	}	if ((point(0) > medium_x) && (point(1) > medium_y) && (point(2) > medium_z)) {
		index = 7;
	}
	return index;
}

/**
 * \brief Traverses the tree from the root to find existing node that contains a given point.
 * \param[in] point The 3D point
 * \return Node containing the given 3D point
 */
Node* Octree::findNode(const Eigen::Vector3d& point) const {
	Node *result = NULL;
	unsigned int index = 0;


	/* TODO: Find and return the leaf node containing the given point.
	 *
	 * Available member variables and methods:
	 * - Node * const root: the root node of the tree
	 * - node->children[8]: the 8 children of the node (all NULL if node is a leaf)
	 * - node->findIndex(const Eigen::Vector3d& point): method defined above
	 */
	Node *temp = root;
	while (temp) {
		index = temp->findIndex(point);
		if (temp->children[index]!= NULL) {
			result = temp->children[index];
			temp = temp->children[index];
		}
		else {
			result = temp;
			temp = NULL;
		}


	}
	return result;
}

/**
 * \brief Splits the current node into 8 child nodes and returns the child node containing the given point.
 * \param[in] point The 3D point.
 * \return The newly created child node containing the given point.
 */
Node* Node::split(const Eigen::Vector3d& point) {
	Node *result = NULL;

	/* TODO:
	 * 1. Set the current node's content to MIXED
	 * 2. Create 8 child nodes for children[0], ..., children[8].
	 *    The Node constructor has the signature
	 *    Node(Eigen::Vector3d corner1,         // = first corner
	 *         Eigen::Vector3d corner2,         // = opposite corner
	 *         Node *parent,                    // = this
	 *         unsigned int depth,              // = depth of the child node within the tree
	 *         Content content)                 // = FREE, OCCUPIED, or MIXED
	 * 3. Return the child node containing the point.
	 *
	 * Available member variables and methods:
	 * - Content content: label of the current node (FREE, OCCUPIED, or MIXED)
	 * - const unsigned int depth: depth of the current node within the tree (0 = root node)
	 * - const Eigen::Vector3d corner1: first corner of the current cube
	 * - const Eigen::Vector3d corner2: opposite corner of the current cube
	 * - Node * children[8]: child node pointers
	 * - Node * const parent: pointer to the parent node (NULL for the root node)
	 * - findIndex(const Eigen::Vector3d& point): method defined above
	 * - findNode(const Eigen::Vector3d& point): method defined above
	 */
	content = MIXED;
	Eigen::Vector3d corner1_temp;
	Eigen::Vector3d corner2_temp;
	double medium_x = (corner1(0) + corner2(0)) / 2;
	double medium_y = (corner1(1) + corner2(1)) / 2;
	double medium_z = (corner1(2) + corner2(2)) / 2;

	corner1_temp << corner1(0), corner1(1), corner1(2);
	corner2_temp << medium_x, medium_y, medium_z;
	children[0] = new Node(corner1_temp, corner2_temp, this, depth+1, FREE);
	corner1_temp << medium_x, corner1(1), corner1(2);
	corner2_temp << corner2(0), medium_y, medium_z;
	children[1] = new Node(corner1_temp, corner2_temp, this, depth + 1, FREE);
	corner1_temp << corner1(0), medium_y, corner1(2);
	corner2_temp << medium_x, corner2(1), medium_z;
	children[2] = new Node(corner1_temp, corner2_temp, this, depth + 1, FREE);
	corner1_temp << medium_x, medium_y, corner1(2);
	corner2_temp << corner2(0), corner2(1), medium_z;
	children[3] = new Node(corner1_temp, corner2_temp, this, depth + 1, FREE);
	corner1_temp << corner1(0), corner1(1), medium_z;
	corner2_temp << medium_x, medium_y, corner2(2);
	children[4] = new Node(corner1_temp, corner2_temp, this, depth + 1, FREE);
	corner1_temp << medium_x, corner1(1), medium_z;
	corner2_temp << corner2(0), medium_y, corner2(2);
	children[5] = new Node(corner1_temp, corner2_temp, this, depth + 1, FREE);
	corner1_temp << corner1(0), medium_y, medium_z;
	corner2_temp << medium_x, corner2(1), corner2(2);
	children[6] = new Node(corner1_temp, corner2_temp, this, depth + 1, FREE);
	corner1_temp << medium_x, medium_y, medium_z;
	corner2_temp << corner2(0), corner2(1), corner2(2);
	children[7] = new Node(corner1_temp, corner2_temp, this, depth + 1, FREE);


	unsigned int index = findIndex(point);
	children[index]->content = OCCUPIED;
	result = children[index];
	return result;
}
/**
 * \brief Tries to merge the current node in case all children have the same content.
 * \return True if the children have been merged, false otherwise.
 */
bool Node::merge() {
	bool merged = false;

	/* TODO:
	 * 1. Check if the children can be merged
	 * 2. Set the label of the current node
	 * 3. Delete all children with "delete children[i]"
	 * 4. Set all children to NULL
	 *
	 * Available member variables and methods:
	 * - Content content: label of the current node (FREE, OCCUPIED, or MIXED)
	 * - Node * children[8]: child node pointers
	 * - const unsigned int depth: depth of the current node within the tree (0 = root node)
	 */
	unsigned int count_occu = 0;
	unsigned int count_free = 0;

	for (size_t i = 0; i < 8; ++i) {
		if (children[i]->content == OCCUPIED) {
			count_occu = count_occu + 1;
		}
		if (children[i]->content == FREE) {
			count_free = count_free + 1;
		}
	}
	if (count_occu == 8) {
		merged = true;
		content = OCCUPIED;
		for (size_t i = 0; i < 8; ++i) {
			delete children[i];
			children[i] = NULL;
		}
	}
	if (count_free == 8) {
		merged = true;
		content = FREE;
		for (size_t i = 0; i < 8; ++i) {
			delete children[i];
			children[i] = NULL;

		}
	}
	return merged;
}

}  // namespace octree
