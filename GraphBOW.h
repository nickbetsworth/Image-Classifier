#pragma once
#include "Graph.h"
#include "BOWExtractor.h"

#include <map>

/**
 * @class	GraphBOW
 *
 * @brief	Uses bag of words model to compare nodes which are local descriptors.
 * 			Upon initialisation of the graph, a bag of visual words is calculated.
 * 			Each new node is given a 1-d feature vector calculated from word frequencies
 * 			found from its local descriptors. This is efficiently used to calculate
 * 			distances between all existing nodes.
 */

class GraphBOW : public Graph {
public:
	GraphBOW(std::vector<Node> nodes);
	~GraphBOW();

	/**
	 * @fn	virtual bool GraphBOW::add_node(Node node);
	 *
	 * @brief	Adds a new node to the graph.
	 * 			Inherently calculates all distances to this node from existing nodes.
	 *
	 * @param	node	The new node.
	 *
	 * @return	true if the node does not already exist, else false.
	 */

	virtual bool add_node(Node node);

	/**
	 * @fn	void GraphBOW::generate_BOW_hist(Node node);
	 *
	 * @brief	Generates a BoW descriptor from the provided local feature descriptors.
	 *
	 * @param	node	A node representing local feature descriptors.
	 */

	void generate_BOW_hist(Node node);
private:
	/** @brief	The number of words to use in BoW vocabulary. */
	static const int NUM_WORDS = 100;

	/**
	* @fn	virtual void GraphBOW::calculate_edges(const Node node);
	*
	* @brief	Calculates the edges from all existing nodes to the specified node.
	*
	* @param	node	The node.
	*/

	virtual void calculate_edges(const Node node);

	/**
	 * @fn	Node GraphBOW::get_BOW(Node node);
	 *
	 * @brief	Gets the corresponding BoW  histogram from a map.
	 *
	 * @param	node	Local feature descriptors to get the appropriate BoW descriptor from.
	 *
	 * @return	BoW frequency histogram.
	 */

	Node get_BOW(Node node);

	/** @brief	Used to extract the BOW frequencies from new nodes. */
	BOWExtractor* m_BOW_extractor;
	/** @brief	Stores bag of word frequencies for each node in the graph. */
	std::map<Node, Node> m_node_bow_map;

	
};