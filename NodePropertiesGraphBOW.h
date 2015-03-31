#pragma once
#include "NodePropertiesGraph.h"
#include "BOWExtractor.h"

class NodePropertiesGraphBOW : public NodePropertiesGraph {
public:
	NodePropertiesGraphBOW(std::vector<Node> nodes);
	~NodePropertiesGraphBOW();
	virtual bool add_node(Node node);
	void generate_BOW_hist(Node node);
private:
	static const int NUM_WORDS = 16;
	BOWExtractor* m_BOW_extractor;
};