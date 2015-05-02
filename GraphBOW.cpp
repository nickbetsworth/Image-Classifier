#include "GraphBOW.h"

GraphBOW::GraphBOW(std::vector<Node> nodes) : Graph() {
	assert(nodes.size() > 0);

	// Get an example node, to work out how many columns / what type of mat we need
	Node example_node = nodes.front();

	// Add all descriptors, from all nodes, to a single matrix
	cv::Mat descs = cv::Mat(0, example_node->get_feature_vector().cols, example_node->get_feature_vector().type());

	for (Node node : nodes) {
		descs.push_back(node->get_feature_vector());
	}

	// Extract a vocabulary via k-means
	cv::BOWKMeansTrainer kmt = cv::BOWKMeansTrainer(NUM_WORDS);
	cv::Mat vocab = kmt.cluster(descs);

	// Provide the vocabulary to the feature extractor
	m_BOW_extractor = new BOWExtractor(vocab);
}

GraphBOW::~GraphBOW() {

}

bool GraphBOW::add_node(Node node) {
	// Before calculating all edges, we need to generate the BOW frequencies for the new node
	generate_BOW_hist(node);
	return Graph::add_node(node);
}

void GraphBOW::calculate_edges(const Node node) {
	if (node == 0)
		return;

	// Set the edge to itself to -1 (no edge)
	m_mat[node][node] = NO_EDGE;

	for (Node existing_node : get_nodes()) {
		// Ensure that we do not calculate the edge to itself
		if (node != existing_node) {
			float val = get_BOW(node)->calculate_distance(get_BOW(existing_node));
			m_mat[node][existing_node] = val;
			m_mat[existing_node][node] = val;
		}
	}
}

void GraphBOW::generate_BOW_hist(Node node) {
	// Replace the image's feature vector with a BOW hist
	cv::Mat bow_hist;
	m_BOW_extractor->get_BOW_hist(node->get_feature_vector(), bow_hist);

	Feature* new_bow_feature = new Feature(bow_hist);
	m_node_bow_map[node] = new_bow_feature;
	
}

Node GraphBOW::get_BOW(Node node) {
	return m_node_bow_map[node];
}