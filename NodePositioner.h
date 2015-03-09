#pragma once
#include "ImageClassifier.h"
#include "NodeProperties.h"
#include "NodePropertiesGraph.h"
#include "ogdf/basic/Graph.h"

/**
 * @class	NodePositioner
 *
 * @brief	Positions a set of NodePropertieses through comparison of features from each NodeProperties.
 *
 * @author	Nick
 * @date	28/10/2014
 */

class NodePositioner
{
	
public:
	/**
	* @struct	Edge
	*
	* @brief	Represents an edge between two image classes.
	* 			
	*/

	struct Edge {
		Node node1;
		Node node2;
	};

	NodePositioner(NodePropertiesGraph* n);
	virtual ~NodePositioner();

	map<Node, cv::Point> NodePositioner::get_node_positions_tree(Node root_node, double node_width, double node_height);
	map<Node, cv::Point> NodePositioner::get_node_positions_fmmm(double node_width, double node_height);
	map<Node, cv::Point> NodePositioner::get_previous_node_positions() { return m_previous_positions; };
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
private:
	ogdf::Graph* setup_nodes(ogdf::Graph* graph);
	vector<NodePositioner::Edge> multimap_to_vector(multimap<Node, Node> map);

	NodePropertiesGraph* m_graph;
	/** @brief	Stores a list of edges calculated via the spanning tree. */
	vector<Edge> m_edges;
	map<Node, cv::Point> m_previous_positions;
	
};

