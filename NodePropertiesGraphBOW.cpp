#include "NodePropertiesGraphBOW.h"

NodePropertiesGraphBOW::NodePropertiesGraphBOW(std::vector<Node> nodes) : NodePropertiesGraph() {
	assert(nodes.size() > 0);

	// Get an example node, to work out how many columns / what type of mat we need
	Node example_node = nodes.front();
	assert(example_node->has_flag(Property::SIFT));

	cv::Mat descs = cv::Mat(0, example_node->get_descriptors().cols, example_node->get_descriptors().type());

	// Construct bag of words from 
	for (Node node : nodes) {
		assert(node->has_flag(Property::SIFT));
		descs.push_back(node->get_descriptors());
	}

	cv::BOWKMeansTrainer kmt = cv::BOWKMeansTrainer(NUM_WORDS);
	cv::Mat vocab = kmt.cluster(descs);

	m_BOW_extractor = new BOWExtractor(vocab);
}

NodePropertiesGraphBOW::~NodePropertiesGraphBOW() {

}

bool NodePropertiesGraphBOW::add_node(Node node) {
	// Before calculating all edges, we need to generate the BOW frequencies for the new node
	generate_BOW_hist(node);
	return NodePropertiesGraph::add_node(node);
}

void NodePropertiesGraphBOW::generate_BOW_hist(Node node) {
	assert(node->has_flag(Property::SIFT));
	cv::Mat bow_hist;
	m_BOW_extractor->get_BOW_hist(node->get_descriptors(), bow_hist);
	node->set_BOW_histogram(bow_hist);
}