#pragma once
#include "ImageClassifier.h"
#include "NodeProperties.h"
#include "NodePropertiesGraph.h"
#include "Graph.h"

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
	* @author	Nick
	* @date	09/11/2014
	*/

	struct Edge {
		NodeProperties* node1;
		NodeProperties* node2;
	};

	NodePositioner(NodePropertiesGraph* n);
	virtual ~NodePositioner();

	map<NodeProperties*, Point> NodePositioner::get_node_positions(NodeProperties* root_node, double graph_scale = 1.0);

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
	NodePropertiesGraph* m_graph;
	/** @brief	Stores the graph of relations between NodeProperties. */
	//Graph<NodeProperties*> m_graph;
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

