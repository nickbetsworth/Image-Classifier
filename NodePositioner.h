#pragma once
#include "ImageClassifier.h"
#include "Feature.h"
#include "Graph.h"
#include "ogdf/basic/Graph.h"

/**
 * @class	NodePositioner
 *
 * @brief	Positions a set of Features with respect to their similarities.
 *
 */

class NodePositioner
{
public:
	/**
	* @struct	Edge
	*
	* @brief	Represents an edge between two nodes.
	* 			
	*/
	struct Edge {
		Node node1;
		Node node2;
	};

	/**
	 * @fn	NodePositioner::NodePositioner(Graph* g);
	 *
	 * @brief	Creates a new node positioner which positions nodes from graph g.
	 *
	 * @param [in,out]	g	Graph containing the nodes to be positioned.
	 */

	NodePositioner(Graph* g);
	virtual ~NodePositioner();

	/**
	 * @fn	map<Node, cv::Point> NodePositioner::get_node_positions_tree(Node root_node, double node_width, double node_height);
	 *
	 * @brief	Positions nodes with a tree layout algorithm.
	 *
	 * @param	root_node  	The root node.
	 * @param	node_width 	Width of each node.
	 * @param	node_height	Height of each node.
	 *
	 * @return	A map from nodes to their respective positions in 2D space.
	 */

	map<Node, cv::Point> NodePositioner::get_node_positions_tree(Node root_node, double node_width, double node_height);

	/**
	 * @fn	map<Node, cv::Point> NodePositioner::get_node_positions_fmmm(double node_width, double node_height);
	 *
	 * @brief	Positions nodes with an energy-based layout algorithm.
	 *
	 * @param	node_width 	Width of each node.
	 * @param	node_height	Height of each node.
	 *
	 * @return	A map from nodes to their respective positions in 2D space.
	 */

	map<Node, cv::Point> NodePositioner::get_node_positions_fmmm(double node_width, double node_height);

	/**
	 * @fn	map<Node, cv::Point> NodePositioner::get_previous_node_positions()
	 *
	 * @brief	Gets the node positions acquired from the previous positioning calculation.
	 *
	 * @return	The node positions from previous positioning calculation.
	 */

	map<Node, cv::Point> NodePositioner::get_previous_node_positions() { return m_previous_positions; };

	/**
	 * @fn	vector<Edge> NodePositioner::get_edges()
	 *
	 * @brief	Gets the edges from the last calculated spanning tree.
	 *
	 * @return	A list of edges that formed the last calculated graph.
	 */

	vector<Edge> get_edges() { return m_edges; };
private:
	/**
	 * @fn	vector<NodePositioner::Edge> NodePositioner::multimap_to_vector(multimap<Node, Node> map);
	 *
	 * @brief	Converts an adjacency matrix to a list of edges
	 *
	 * @param	map	Adjacency matrix.
	 *
	 * @return	A list of all edges from the matrix;
	 */

	vector<NodePositioner::Edge> multimap_to_vector(multimap<Node, Node> map);

	Graph* m_graph;
	/** @brief	Stores a list of edges calculated via the spanning tree. */
	vector<Edge> m_edges;
	map<Node, cv::Point> m_previous_positions;
	
};

