#include "NodePositioner.h"
#include "Histograms.h"
#include "ogdf/basic/GraphAttributes.h"
#include "ogdf/tree/TreeLayout.h"
#include "ogdf/energybased/FMMMLayout.h"
NodePositioner::NodePositioner(NodePropertiesGraph* graph)
{
	m_graph = graph;
}


NodePositioner::~NodePositioner()
{
}

map<Node, Point> NodePositioner::get_node_positions_tree(Node root_node, double node_width, double node_height) {
	auto spanning_tree = m_graph->get_minimum_spanning_tree(root_node);

	// Store the spanning tree as a list of edges, for later use
	m_edges = multimap_to_vector(spanning_tree);

	//map<NodeProperties*, Point> class_positions = m_graph.calculate_node_positions(root_node, graph_edges, graph_scale);
	// Create the map that will store the node positions
	map<Node, cv::Point> pos_map;

	// Create the graph and graph attributes
	ogdf::Graph g;
	ogdf::GraphAttributes ga(g, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);

	// Create maps from the ogdf node to the local node, and vice versa
	map<Node, ogdf::node> node_to_ogdf_map;

	// Generate a new node in the ogdf graph for each existing node
	for (Node  n : m_graph->get_nodes()) {
		ogdf::node o_node = g.newNode();
		node_to_ogdf_map[n] = o_node;
	}

	ga.setAllWidth(node_width);
	ga.setAllHeight(node_height);

	// Now add all of the edges to the ogdf graph
	for (Node n : m_graph->get_nodes()) {
		pair<multimap<Node, Node>::iterator, multimap<Node, Node>::iterator> node_it = spanning_tree.equal_range(n);

		for (auto it = node_it.first; it != node_it.second; ++it) {
			Node node1 = it->first;
			Node node2 = it->second;;

			ogdf::edge e = g.newEdge(node_to_ogdf_map[node1], node_to_ogdf_map[node2]);
		}
	}

	ogdf::TreeLayout layout;
	//layout.rootSelection(ogdf::RadialTreeLayout::rootIsSource);
	//layout.connectedComponentDistance(0.5);
	layout.call(ga);

	// Create a map from each node to its respective position
	for (Node n : m_graph->get_nodes()) {
		ogdf::node v = node_to_ogdf_map[n];
		double x = ga.x(v);
		double y = -ga.y(v);

		pos_map[n] = cv::Point(x, y);

		//cout << "Position: (" << x << ", " << y << ") P1: " << ga.imageWidthNode(v) << " P2: " << ga.imageHeightNode(v) << endl;
	}

	m_previous_positions = pos_map;

	return pos_map;
}

map<Node, Point> NodePositioner::get_node_positions_fmmm(double node_width, double node_height) {
	map<Node, cv::Point> pos_map;

	// Create the graph and graph attributes
	ogdf::Graph g;
	ogdf::GraphAttributes ga(g, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
	// Stores the weighting of edges
	ogdf::EdgeArray<double> weight(g);

	// Create maps from the ogdf node to the local node, and vice versa
	map<Node, ogdf::node> node_to_ogdf_map;

	// Generate a new node in the ogdf graph for each existing node
	for (Node n : m_graph->get_nodes()) {
		ogdf::node o_node = g.newNode();
		node_to_ogdf_map[n] = o_node;
	}

	ga.setAllWidth(node_width);
	ga.setAllHeight(node_height);

	// Now add all of the edges to the ogdf graph
	NodeList nodes = m_graph->get_nodes();
	auto endMinusOne = std::prev(nodes.end(), 1);
	for (auto v = nodes.begin(); v != endMinusOne; v++) {
		auto startPlusOne = std::next(v, 1);
		for (auto k = startPlusOne; k != nodes.end(); k++) {
			Node node1 = *v;
			Node node2 = *k;

			ogdf::edge e = g.newEdge(node_to_ogdf_map[node1], node_to_ogdf_map[node2]);
			weight[e] = m_graph->get_edge_weight(node1, node2);
		}
	}

	// Use a FMMM layout to calculate the node positions
	ogdf::FMMMLayout fm;
	fm.useHighLevelOptions(true);
	// Do not use a new initial placement each time,
	// this makes the graph deterministic
	fm.newInitialPlacement(false);
	fm.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);
	// Call the layout manager with the weight of each edge
	fm.call(ga, weight);

	// Create a map from each node to its respective position
	for (Node n : m_graph->get_nodes()) {
		ogdf::node v = node_to_ogdf_map[n];
		double x = ga.x(v);
		double y = -ga.y(v);

		pos_map[n] = cv::Point(x, y);

		//cout << "Position: (" << x << ", " << y << ") P1: " << ga.imageWidthNode(v) << " P2: " << ga.imageHeightNode(v) << endl;
	}

	m_previous_positions = pos_map;
	return pos_map;
}

vector<NodePositioner::Edge> NodePositioner::multimap_to_vector(multimap<Node, Node> map) {
	vector<NodePositioner::Edge> edges;

	for (auto it = map.begin(); it != map.end(); ++it) {
		Node edge1 = it->first;
		Node edge2 = it->second;
		NodePositioner::Edge e = { edge1, edge2 };
		edges.push_back(e);
	}

	return edges;
}