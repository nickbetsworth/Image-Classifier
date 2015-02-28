#pragma once
#include "NodeProperties.h"

#include <unordered_set>
#include <map>

using namespace std;
typedef NodeProperties* Node;
typedef map<Node, map<Node, float>> AdjacencyMatrix;
typedef multimap<Node, Node> SpanningTree;
typedef set<Node> NodeList;
class NodePropertiesGraph
{
public:
	static const int NO_EDGE = -1;
	NodePropertiesGraph();
	virtual ~NodePropertiesGraph();
	bool add_node(Node node);
	bool remove_node(Node node);
	AdjacencyMatrix get_adjacency_matrix() { return m_mat; };
	SpanningTree get_minimum_spanning_tree(Node root_node = 0);
	float get_edge_weight(Node node1, Node node2);

	NodeList get_nodes() { return m_nodes; };
	NodeList get_n_nearest_nodes(Node node, int n);
private:

	/**
	 * @fn	void NodePropertiesGraph::calculate_edges(NodeProperties* node);
	 *
	 * @brief	Calculates all edges between the given node and the existing nodes,
	 * 			and stores them in the adjacency matrix.
	 *
	 * @param [in,out]	node	The new node.
	 */

	void calculate_edges(Node node);

	/**
	 * @fn	void NodePropertiesGraph::remove_edges(NodeProperties* node);
	 *
	 * @brief	Removes all edges that link to the specified node.
	 *
	 * @param [in,out]	node	The node that has been removed.
	 */
	
	void remove_edges(Node node);

	void cache_minimum_spanning_tree(SpanningTree& st, Node root_node);
	// Specifies that the graph has changed, so we can not used cache data for MST
	void remove_cache() { m_has_changed = true; };
	// Stores whether or not the graph has changed since last MST calculation
	bool has_graph_changed() { return m_has_changed; }
	
	void set_previous_root(Node root_node) { m_previous_root = root_node; };
	// Specifies if the root of the last MST is different to the one specified
	bool is_previous_root(Node root_node) { return root_node == m_previous_root; };

	NodeList m_nodes;
	AdjacencyMatrix m_mat;
	// Determines whether or not it is suitable to used cached data about this graph
	bool m_has_changed;
	Node m_previous_root;
	SpanningTree m_cached_spanning_tree;
};

