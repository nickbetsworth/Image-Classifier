#include "NodePositioner.h"
#include "Histograms.h"

NodePositioner::NodePositioner(vector<NodeProperties*> nodes)
{
	// Store the image classifier for later use
	m_nodes = nodes;
}


NodePositioner::~NodePositioner()
{
}

map<NodeProperties*, Point> NodePositioner::get_node_positions(double graph_scale) {
	for (NodeProperties* node : get_nodes()) {
		// Create a new node in the graph
		m_graph.add_node(node);
	}

	// Generate the adjacency matrix and provide it to the graph class
	m_graph.set_adjacency_matrix(get_adjacency_matrix());
	
	auto graph_edges = m_graph.get_minimum_spanning_tree();

	// Store the spanning tree as a list of edges, for later use
	m_edges = multimap_to_vector(graph_edges);

	map<NodeProperties*, Point> class_positions = m_graph.calculate_node_positions(graph_edges, graph_scale);

	return class_positions;
}

AdjacencyMatrix NodePositioner::get_adjacency_matrix(double edge_threshold) {
	// Store all of the classes we will be comparing
	vector<NodeProperties*> nodes = get_nodes();
	// Define our adjacency matrix
	AdjacencyMatrix a_mat;

	// Calculate the difference between each possible histogram combination
	for (auto v = nodes.begin(); v != nodes.end() - 1; v++) {
		for (auto k = v + 1; k != nodes.end(); k++) {
			// Calculate the difference between the classes' histograms
			double weight = compare_histograms((*v)->get_histogram_norm(), (*k)->get_histogram_norm());
			// If the weight is not within the threshold, then we will not consider this edge in the graph
			if (weight > edge_threshold) 
				weight = -1.0;
			// Add a weighted edge between these two classes
			a_mat[*v][*k] = weight;
			a_mat[*k][*v] = weight;
		}
	}

	return a_mat;
}

vector<NodePositioner::Edge> NodePositioner::multimap_to_vector(multimap<NodeProperties*, NodeProperties*> map) {
	vector<NodePositioner::Edge> edges;

	for (auto it = map.begin(); it != map.end(); ++it) {
		NodeProperties* edge1 = it->first;
		NodeProperties* edge2 = it->second;
		NodePositioner::Edge e = { edge1, edge2 };
		edges.push_back(e);
	}

	return edges;
}