#pragma once
#include "ImageClassifier.h"
#include "NodeProperties.h"
#include "Graph.h"

/**
 * @class	NodePositioner
 *
 * @brief	Positions a set of NodePropertieses through comparison of features from each NodeProperties.
 *
 * @author	Nick
 * @date	28/10/2014
 */

typedef map<NodeProperties*, map<NodeProperties*, double>> AdjacencyMatrix;
class NodePositioner
{
	
public:
	/**
	* @struct	Edge
	*
	* @brief	Represents an edge between two image classes.
	*
	* @author	Nick
	* @date	09/11/2014
	*/

	struct Edge {
		NodeProperties* node1;
		NodeProperties* node2;
	};

	NodePositioner(vector<NodeProperties*> nodes);
	virtual ~NodePositioner();

	/**
	 * @fn	map<NodeProperties*, Point> NodePositioner::get_class_positions(NodeProperties* root_node);
	 *
	 * @brief	Calculates a map from an NodeProperties, to its respective position on screen.
	 *
	 * @author	Nick
	 * @date	09/11/2014
	 *
	 * @param 	root_node	The root node of the graph.
	 *
	 * @return	A mapping from NodeProperties to its position on screen.
	 */

	map<NodeProperties*, Point> NodePositioner::get_node_positions_MST(NodeProperties* root_node, double graph_scale = 1.0);
	map<NodeProperties*, Point> NodePositioner::get_node_positions_graph(NodeProperties* root_node, double edge_threshold, double graph_scale = 1.0);
	map<NodeProperties*, Point> NodePositioner::get_node_positions_graph2(NodeProperties* root_node, double edge_length);
	/**
	 * @fn	vector<Edge> NodePositioner::get_edges()
	 *
	 * @brief	Gets the edges from the last calculated spanning tree.
	 *
	 * @author	Nick
	 * @date	09/11/2014
	 *
	 * @return	A list of edges that formed the last calculated graph.
	 */

	vector<Edge> get_edges() { return m_edges; };
protected:
	vector<NodeProperties*> get_nodes() { return m_nodes; };

	/**
	 * @fn	virtual AdjacencyMatrix NodePositioner::get_adjacency_matrix() = 0;
	 *
	 * @brief	Calculates a matrix of weighted edges using a set of features derived from each node.
	 * 			The edges are stored as an adjacency matrix.
	 *
	 * @author	Nick
	 * @date	28/10/2014
	 *
	 * @return	A weighted adjacency matrix.
	 */
	// Any value below threshold will be considered an edge
	// Any value above will be given an edge length of -1
	AdjacencyMatrix get_adjacency_matrix(double threshold = numeric_limits<double>::max());
private:
	/** @brief	Stores the nodes being used */
	vector<NodeProperties*> m_nodes;
	/** @brief	Stores the graph of relations between NodePropertieses. */
	Graph<NodeProperties*> m_graph;
	/** @brief	Stores a list of edges calculated via the spanning tree. */
	vector<Edge> m_edges;

	/**
	 * @fn	vector<NodePositioner::Edge> NodePositioner::multimap_to_vector(multimap<NodeProperties*, NodeProperties*> map);
	 *
	 * @brief	Converts a multimap of edges to a single vector.
	 *
	 * @author	Nick
	 * @date	09/11/2014
	 *
	 * @param 	map	The multimap to be converted.
	 *
	 * @return	A Vector generated from the map;
	 */

	vector<NodePositioner::Edge> multimap_to_vector(multimap<NodeProperties*, NodeProperties*> map);
};

