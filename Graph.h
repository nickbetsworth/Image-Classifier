#pragma once
#include "Feature.h"

#include <unordered_set>
#include <map>

using namespace std;
typedef Feature* Node;
typedef map<Node, map<Node, float>> AdjacencyMatrix;
typedef multimap<Node, Node> SpanningTree;
typedef set<Node> NodeList;

/**
 * @class	Graph
 *
 * @brief	Models a complete graph where nodes are represented by image features.
 */

class Graph
{
public:
	/** @brief	Constant to indicate there is no edge between two nodes */
	static const int NO_EDGE = -1;

	Graph();
	virtual ~Graph();

	/**
	 * @fn	virtual bool Graph::add_node(Node node);
	 *
	 * @brief	Adds a new node to the graph.
	 * 			This inherently calculates the edges between the new node and all existing nodes.
	 *
	 * @param	node	The node to be added.
	 *
	 * @return	true if this node is not already in the graph, false otherwise.
	 */

	virtual bool add_node(Node node);

	/**
	 * @fn	virtual bool Graph::remove_node(Node node);
	 *
	 * @brief	Removes a node from the graph
	 *
	 * @param	node	The node to be removed.
	 *
	 * @return	true if the node is found within the graph, false otherwise.
	 */

	virtual bool remove_node(Node node);

	/**
	 * @fn	virtual AdjacencyMatrix Graph::get_adjacency_matrix()
	 *
	 * @brief	Gets adjacency matrix for the current state of the graph.
	 *
	 * @return	The adjacency matrix.
	 */

	virtual AdjacencyMatrix get_adjacency_matrix() { return m_mat; };

	/**
	 * @fn	virtual SpanningTree Graph::get_minimum_spanning_tree(Node root_node = 0);
	 *
	 * @brief	Calculates a MST from the provided root node.
	 * 			Inherently stores a cached version of the MST for later use.
	 *
	 * @param	root_node	The root node.
	 *
	 * @return	The minimum spanning tree.
	 */

	virtual SpanningTree get_minimum_spanning_tree(Node root_node = 0);

	/**
	 * @fn	virtual float Graph::get_edge_weight(Node node1, Node node2);
	 *
	 * @brief	Gets edge weight between the two specified nodes.
	 *
	 * @param	node1	The first node.
	 * @param	node2	The second node.
	 *
	 * @return	The edge weight.
	 */

	virtual float get_edge_weight(Node node1, Node node2);

	/**
	 * @fn	virtual NodeList Graph::get_nodes()
	 *
	 * @brief	Gets a list of the graph's nodes.
	 *
	 * @return	The nodes.
	 */

	virtual NodeList get_nodes() { return m_nodes; };

	/**
	 * @fn	virtual NodeList Graph::get_n_nearest_nodes(Node node, int n);
	 *
	 * @brief	Gets n nearest nodes to the provided node.
	 *
	 * @param	node	The node to find nearest neighbours.
	 * @param	n   	the number of nodes to find.
	 *
	 * @return	The n nearest nodes.
	 */

	virtual NodeList get_n_nearest_nodes(Node node, int n);
protected:
	NodeList m_nodes;
	AdjacencyMatrix m_mat;
private:

	/**
	 * @fn	virtual void Graph::calculate_edges(Node node);
	 *
	 * @brief	Calculates all edges between the given node and the existing nodes, and stores them
	 * 			in the adjacency matrix.
	 *
	 * @param	node	The new node.
	 */

	virtual void calculate_edges(const Node node);

	/**
	 * @fn	void Graph::remove_edges(Node node);
	 *
	 * @brief	Removes all edges that link to the specified node.
	 *
	 * @param	node	The node that has been removed.
	 */
	
	void remove_edges(Node node);

	/**
	 * @fn	void Graph::cache_minimum_spanning_tree(SpanningTree& st, Node root_node);
	 *
	 * @brief	Stores the provided MST for later use.
	 *
	 * @param [in,out]	st	The minimum spanning tree.
	 * @param	root_node 	The root node of the tree.
	 */

	void cache_minimum_spanning_tree(SpanningTree& st, Node root_node);

	/**
	 * @fn	void Graph::remove_cache()
	 *
	 * @brief	Specifies that the graph has changed, so we can not used cache data for MST.
	 */

	void remove_cache() { m_has_changed = true; };

	/**
	 * @fn	bool Graph::has_graph_changed()
	 *
	 * @brief	Stores whether or not the graph has changed since last MST calculation.
	 *
	 * @return	true if graph changed, false if not.
	 */

	bool has_graph_changed() { return m_has_changed; }
	
	void set_previous_root(Node root_node) { m_previous_root = root_node; };

	/**
	 * @fn	bool Graph::is_previous_root(Node root_node)
	 *
	 * @brief	Specifies if the root of the last MST is different to the one specified.
	 *
	 * @param	root_node	The root node.
	 *
	 * @return	true if previous root, false if not.
	 */

	bool is_previous_root(Node root_node) { return root_node == m_previous_root; };

	/** @brief	Determines whether or not it is suitable to used cached data about this graph. */
	bool m_has_changed;
	/** @brief	Stores the root node of the last used MST. */
	Node m_previous_root;
	/** @brief	The cached minimum spanning tree. */
	SpanningTree m_cached_spanning_tree;
};

