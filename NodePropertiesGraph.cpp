#include "NodePropertiesGraph.h"


NodePropertiesGraph::NodePropertiesGraph()
{
}


NodePropertiesGraph::~NodePropertiesGraph()
{
}

bool NodePropertiesGraph::add_node(NodeProperties* node) {
	pair<unordered_set<NodeProperties*>::const_iterator, bool> result = m_nodes.insert(node);
	// If the new node was successfully added
	// (It did not already exist within the list)
	if (result.second) {
		calculate_edges(node);
		return true;
	}
	else {
		return false;
	}
}

bool NodePropertiesGraph::remove_node(NodeProperties* node) {
	int num_removed = m_nodes.erase(node);
	// If the element was removed
	if (num_removed) {
		remove_edges(node);
		return true;
	}
	else {
		return false;
	}
}

void NodePropertiesGraph::calculate_edges(NodeProperties* node) {
	if (node == 0)
		return;

	// Loop through all existing nodes
	for (NodeProperties* existing_node : m_nodes) {
		// Ensure that we do not calculate the edge to itself
		if (node != existing_node) {
			float val = node->calculate_distance(existing_node);
			m_mat[node][existing_node] = val;
			m_mat[existing_node][node] = val;
		}
	}
}

void NodePropertiesGraph::remove_edges(NodeProperties* node) {
	// First remove the key for this node
	m_mat.erase(node);

	// Now remove the secondary key referred to by all other nodes
	for (NodeProperties* existing_node : m_nodes) {
		// Ensure that we do not calculate the edge to itself
		if (node != existing_node) {
			m_mat[existing_node].erase(node);
		}
	}
}