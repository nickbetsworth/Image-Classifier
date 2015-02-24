#pragma once
#include "NodeProperties.h"

#include <unordered_set>
#include <map>

using namespace std;
class NodePropertiesGraph
{
	typedef map<NodeProperties*, map<NodeProperties*, float>> AdjacencyMatrix;
public:
	NodePropertiesGraph();
	virtual ~NodePropertiesGraph();
	bool add_node(NodeProperties* node);
	bool remove_node(NodeProperties* node);
	AdjacencyMatrix get_adjacency_matrix() { return m_mat; };
	unordered_set<NodeProperties*> get_nodes() { return m_nodes; };
private:

	/**
	 * @fn	void NodePropertiesGraph::calculate_edges(NodeProperties* node);
	 *
	 * @brief	Calculates all edges between the given node and the existing nodes,
	 * 			and stores them in the adjacency matrix.
	 *
	 * @param [in,out]	node	The new node.
	 */

	void calculate_edges(NodeProperties* node);

	/**
	 * @fn	void NodePropertiesGraph::remove_edges(NodeProperties* node);
	 *
	 * @brief	Removes all edges that link to the specified node.
	 *
	 * @param [in,out]	node	The node that has been removed.
	 */

	void remove_edges(NodeProperties* node);

	unordered_set<NodeProperties*> m_nodes;
	AdjacencyMatrix m_mat;
};

