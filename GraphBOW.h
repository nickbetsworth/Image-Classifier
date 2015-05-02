#pragma once
#include "Graph.h"
#include "BOWExtractor.h"
#include <map>

class GraphBOW : public Graph {
public:
	GraphBOW(std::vector<Node> nodes);
	~GraphBOW();
	virtual bool add_node(Node node);
	void generate_BOW_hist(Node node);
private:
	static const int NUM_WORDS = 100;
	BOWExtractor* m_BOW_extractor;
	// Stores bag of word frequencies for each node in the graph
	std::map<Node, Node> m_node_bow_map;

	virtual void calculate_edges(const Node node);
	Node get_BOW(Node node);
};