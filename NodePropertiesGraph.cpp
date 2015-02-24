#include "NodePropertiesGraph.h"


NodePropertiesGraph::NodePropertiesGraph()
{
	m_previous_root = 0;
	remove_cache();
}


NodePropertiesGraph::~NodePropertiesGraph()
{
}

bool NodePropertiesGraph::add_node(NodeProperties* node) {
	pair<unordered_set<NodeProperties*>::const_iterator, bool> result = m_nodes.insert(node);
	// If the new node was successfully added
	// (It did not already exist within the list)
	if (result.second) {
		remove_cache();
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
		remove_cache();
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

	// Set the edge to itself to -1 (no edge)
	m_mat[node][node] = NO_EDGE;
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
	remove_cache();
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



	/**
	* @fn	multimap<T, T> Graph::get_minimum_spanning_tree()
	*
	* @brief	Gets minimum spanning tree.
	*
	* @return	A multimap from a single node to its connected nodes.
	*/

float NodePropertiesGraph::get_edge_weight(Node node1, Node node2) {
	try {
		return m_mat.at(node1).at(node2);
	}
	// If one of the nodes does not exist
	catch (std::out_of_range) {
		cout << "Attempted to access edge that does not exist" << endl;
		return -1;
	}
}

SpanningTree NodePropertiesGraph::get_minimum_spanning_tree(Node root_node) {
	// Check if there is a cached version of the requested spanning tree
	if (!has_graph_changed() && is_previous_root(root_node)) {
		cout << "Loading spanning tree from cache" << endl;
		return m_cached_spanning_tree;
	}
	else {
		// Create a multimap to store the spanning tree
		SpanningTree edge_map;

		// Construct a new list containing all nodes in the graph (converts from set to vector)
		vector<Node> remaining_nodes(m_nodes.begin(), m_nodes.end());

		Node start_node;

		// If the caller did not specify a root node
		if (root_node == 0) {
			start_node = *(m_nodes.begin());
		}
		else {
			start_node = root_node;
		}

		// Remove the node we just selected from the list of remaining nodes
		remaining_nodes.erase(find(remaining_nodes.begin(), remaining_nodes.end(), start_node));

		// Create a list to store nodes that have already been visited
		vector<Node> visited_nodes;
		// We have already visited the starting node
		visited_nodes.push_back(start_node);
		float total_distance = 0;

		// While there are still nodes remaining
		while (remaining_nodes.size() > 0) {
			// A boolean which stores whether or not a potential edge has been found
			// If no edge is found, then we have an unconnected graph
			bool edge_found = false;
			// Give min_value some arbitrarily large value initially
			double min_value = pow(2, 32);
			// Stores the first node of an edge when we find a new minimum value
			Node node1;
			// Stores the position of the second node of the edge 
			// (enables efficient deletion of the node later when removing from remaining_nodes)
			vector<Node>::iterator min_node_it;

			// Loop through all visited nodes
			for (Node n : visited_nodes) {
				// For each of these nodes, loop through all unvisited nodes
				// and search for the edge with the smallest weight
				vector<Node>::iterator it;
				for (it = remaining_nodes.begin(); it != remaining_nodes.end(); ++it) {
					//double weight = norm_mat[n][*it];
					float weight = get_edge_weight(n, *it);

					// If this edge has the lowest weight yet
					// And does not equal -1 (indicates this edge is to be ignored, or no edge exists)
					if (weight < min_value && weight != -1.0) {
						edge_found = true;
						min_value = weight;
						node1 = n;
						min_node_it = it;
					}

				}
			}

			// If either node 1 or node 2 is uninitialized, we have an unconnected graph
			if (!edge_found) {
				cout << "Error: The graph provided is not connected" << endl;
				// Break out of the program or else we will end up in an infinite loop
				exit(0);
			}
			else {
				// Increase the total distance/weight of the spanning tree thus far
				total_distance += min_value;
				Node new_node = *min_node_it;
				// Remove the node from the remaining list
				remaining_nodes.erase(min_node_it);
				// Add the node to the visited node list
				visited_nodes.push_back(new_node);

				// Add the new edge to the final list
				edge_map.insert(pair<Node, Node>(node1, new_node));
				// We will not add the edge in both directions to avoid duplicate edges
				//edge_map.insert(pair<T, T>(new_node, node1));
				//edges.push_back({ node1, new_node, get_edge_weight(node1, new_node) });
			}
		}

		cout << "Total distance in MST: " << total_distance << endl;
		cache_minimum_spanning_tree(edge_map, root_node);
		return edge_map;
	}
	
}

void NodePropertiesGraph::cache_minimum_spanning_tree(SpanningTree& tree, Node root_node) {
	m_cached_spanning_tree = tree;
	set_previous_root(root_node);
	m_has_changed = false;
}