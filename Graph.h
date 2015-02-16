#pragma once
#include <map>
#include <set>
#include <vector>
#include <ogdf\energybased\FMMMLayout.h>
#include <ogdf\\misclayout\BalloonLayout.h>
#include <opencv\cv.h>
#include <ctime>

/**
 * @class	Graph
 *
 * @brief	Represents an undirected graph with weighted edges.
 * 			Nodes may be of an arbitrary data type.
 * 			This class is designed around the basis that every node has a weighted edge to every other node.
 *
 * @author	Nick
 * @date	30/10/2014
 *
 * @tparam	T	The data type of the nodes.
 */

using namespace std;
template <class T> class Graph
{
public:
	typedef map<T, map<T, double>> AdjacencyMatrix;
	struct Edge {	T first;
					T second;
					double weight; };

	Graph() {

	}

	~Graph() {

	}

	/**
	 * @fn	multimap<T, T> Graph::get_minimum_spanning_tree(T root_node)
	 *
	 * @brief	Calculates a minimum spanning tree from the current graph.
	 *
	 * @author	Nick
	 * @date	30/10/2014
	 *
	 * @param	root_node	The root node of the spanning tree.
	 *
	 * @return	A multimap from a single node to its connected nodes.
	 */

	multimap<T, T> get_minimum_spanning_tree(T root_node) {
		// Normalize our set of edges
		AdjacencyMatrix norm_mat = this->normalize(1);

		// Create a multimap to store the spanning tree
		multimap<T, T> edge_map;

		// Construct a new list containing all nodes in the graph (converts from set to vector)
		vector<T> remaining_nodes(m_nodes.begin(), m_nodes.end());
		// Randomly choose a starting node
		//int index = rand() % remaining_nodes.size();
		//T start_node = remaining_nodes.at(index);
		// Remove the node we just selected from the list of remaining nodes
		remaining_nodes.erase(find(remaining_nodes.begin(), remaining_nodes.end(), root_node));
		//remaining_nodes.erase(remaining_nodes.begin() + index);

		// Create a list to store nodes that have already been visited
		vector<T> visited_nodes;
		// We have already visited the starting node
		visited_nodes.push_back(root_node);
		double total_distance = 0;
		
		// While there are still nodes remaining
		while (remaining_nodes.size() > 0) {
			// A boolean which stores whether or not a potential edge has been found
			// If no edge is found, then we have an unconnected graph
			bool edge_found = false;
			// Give min_value some arbitrarily large value initially
			double min_value = pow(2, 32);
			// Stores the first node of an edge when we find a new minimum value
			T node1;
			// Stores the position of the second node of the edge 
			// (enables efficient deletion of the node later when removing from remaining_nodes)
			vector<T>::iterator min_node_it;

			// Loop through all visited nodes
			for (T n : visited_nodes) {
				// For each of these nodes, loop through all unvisited nodes
				// and search for the edge with the smallest weight
				vector<T>::iterator it;
				for (it = remaining_nodes.begin(); it != remaining_nodes.end(); ++it) {
					double weight = norm_mat[n][*it];

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
				T new_node = *min_node_it;
				// Remove the node from the remaining list
				remaining_nodes.erase(min_node_it);
				// Add the node to the visited node list
				visited_nodes.push_back(new_node);

				// Add the new edge to the final list
				edge_map.insert(pair<T, T>(node1, new_node));
				// We will not add the edge in both directions to avoid duplicate edges
				//edge_map.insert(pair<T, T>(new_node, node1));
				//edges.push_back({ node1, new_node, get_edge_weight(node1, new_node) });
			}
		}

		cout << "Total distance in MST: " << total_distance << endl;
		
		return edge_map;
	}

	multimap<T, T> get_graph(T root_node, double edge_threshold) {
		// Normalize our set of edges
		AdjacencyMatrix norm_mat = this->normalize(1);

		// Create a multimap to store the spanning tree
		multimap<T, T> edge_map;

		for (auto v = m_nodes.begin(); v != m_nodes.end() - 1; v++) {
			for (auto k = v + 1; k != m_nodes.end(); k++) {
				T node1 = *v;
				T node2 = *k;

				//double weight = get_edge_weight(node1, node2);
				double weight = norm_mat[node1][node2];
				cout << "Weight: " << weight << endl;
				if (weight <= edge_threshold) {
					edge_map.insert(pair<T, T>(node1, node2));
				}
			}
		}

		return edge_map;
	}

	/**
	 * @fn	map<T, cv::Point> Graph::calculate_node_positions(multimap<T, T> spanning_tree)
	 *
	 * @brief	Calculates the positions of a set of nodes given a spanning tree connecting all nodes.
	 *
	 * @author	Nick
	 * @date	09/11/2014
	 *
	 * @param	spanning_tree	The spanning tree.
	 *
	 * @return	The calculated node positions.
	 */

	map<T, cv::Point> calculate_node_positions(multimap<T, T> spanning_tree, double graph_scale) {
		// Normalize our set of edges
		AdjacencyMatrix norm_mat = this->normalize(1);

		//std::clock_t    start;

		//start = std::clock();

		// Create the map that will store the node positions
		map<T, cv::Point> pos_map;

		// Create the graph and graph attributes
		ogdf::Graph g;
		ogdf::GraphAttributes ga(g, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
		// Stores the weighting of edges
		ogdf::EdgeArray<double> weight(g);

		// Create maps from the ogdf node to the local node, and vice versa
		map<ogdf::node, T> ogdf_to_node_map;
		map<T, ogdf::node> node_to_ogdf_map;

		// Generate a new node in the ogdf graph for each existing node
		for (T n : m_nodes) {
			ogdf::node o_node = g.newNode();
			ogdf_to_node_map[o_node] = n;
			node_to_ogdf_map[n] = o_node;
		}
		
		ga.setAllWidth(200 * graph_scale);
		ga.setAllHeight(200 * graph_scale);

		// Now add all of the edges to the ogdf graph
		for (T n : m_nodes) {
			pair<multimap<T, T>::iterator, multimap<T, T>::iterator> node_it = spanning_tree.equal_range(n);

			for (auto it = node_it.first; it != node_it.second; ++it) {
				// The two nodes which form an edge
				T node1 = it->first;
				T node2 = it->second;

				ogdf::edge e = g.newEdge(node_to_ogdf_map[node1], node_to_ogdf_map[node2]);
				weight[e] = norm_mat[node1][node2] * graph_scale;
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

		/*ogdf::BalloonLayout sm;
		sm.call(ga);*/

		// Create a map from each node to its respective position
		for (T n : m_nodes) {
			ogdf::node v = node_to_ogdf_map[n];
			double x = ga.x(v);
			double y = ga.y(v);
			
			pos_map[n] = cv::Point(x, y);
			
			//cout << "Position: (" << x << ", " << y << ") P1: " << ga.imageWidthNode(v) << " P2: " << ga.imageHeightNode(v) << endl;
		}
		
		//std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

		return pos_map;
	}

	/**
	 * @fn	map<T, cv::Point> Graph::calculate_node_positions(T root_node)
	 *
	 * @brief	Calculates the node positions given a root node.
	 * 			A minimum spanning tree will be calculated starting at the specified
	 * 			root node, and this tree will then be used to determine the positioning
	 * 			of the nodes.
	 * 			
	 * @author	Nick
	 * @date	09/11/2014
	 *
	 * @param	root_node	The root node.
	 *
	 * @return	The calculated node positions.
	 */

	map<T, cv::Point> calculate_node_positions(T root_node, double graph_scale) {
		// Calculate the minimum spanning tree
		multimap<T, T> spanning_tree = get_minimum_spanning_tree(root_node);

		return calculate_node_positions(spanning_tree, graph_scale);
	}

	/**
	 * @fn	void Graph::add_edge(T node1, T node2, double weight);
	 *
	 * @brief	Adds a new edge to the graph.
	 *
	 * @author	Nick
	 * @date	30/10/2014
	 *
	 * @param	node1 	The first node.
	 * @param	node2 	The second node.
	 * @param	weight	The weight of the edge.
	 */

	void add_edge(T node1, T node2, double weight) {
		m_mat[node1][node2] = weight;
		m_mat[node2][node1] = weight;
	}

	/**
	 * @fn	void Graph::add_node(T node)
	 *
	 * @brief	Adds a new node to the graph.
	 *
	 * @author	Nick
	 * @date	30/10/2014
	 *
	 * @param	node	The new node.
	 */

	void add_node(T node) {
		m_nodes.push_back(node);
	}

	/**
	 * @fn	void Graph::set_adjacency_matrix(AdjacencyMatrix mat)
	 *
	 * @brief	Sets the adjacency matrix.
	 *
	 * @author	Nick
	 * @date	30/10/2014
	 *
	 * @param	mat	The weigthed adjacency matrix.
	 */

	void set_adjacency_matrix(AdjacencyMatrix mat) { m_mat = mat; };

	/**
	 * @fn	double Graph::get_edge_weight(T node1, T node2)
	 *
	 * @brief	Gets the weight of an edge within the graph.
	 *
	 * @author	Nick
	 * @date	30/10/2014
	 *
	 * @param	node1	The first node.
	 * @param	node2	The second node.
	 *
	 * @return	The weight of the specified edge.
	 */

	double get_edge_weight(T node1, T node2) { return m_mat[node1][node2]; };

	AdjacencyMatrix normalize(double new_max) {
		AdjacencyMatrix norm_mat;

		double max = -1 * numeric_limits<double>::max();
		
		// Find the minimum and maximum edge values
		for (auto v = m_nodes.begin(); v != m_nodes.end() - 1; v++) {
			for (auto k = v + 1; k != m_nodes.end(); k++) {
				T node1 = *v;
				T node2 = *k;

				double weight = get_edge_weight(node1, node2);

				if (weight > max) {
					max = weight;
				}
			}
		}

		//cout << "Min: " << min << " Max: " << max << endl;

		// Now normalize each of the edges
		for (auto v = m_nodes.begin(); v != m_nodes.end() - 1; v++) {
			for (auto k = v + 1; k != m_nodes.end(); k++) {
				T node1 = *v;
				T node2 = *k;

				double weight = get_edge_weight(node1, node2);
				double normalized_weight = ((weight) / max) * new_max;
				norm_mat[node1][node2] = normalized_weight;
				norm_mat[node2][node1] = normalized_weight;
				//cout << "Normalized " << weight << " to " << normalized_weight << endl;
			}
		}

		return norm_mat;
	}
protected:

private:
	/** @brief	Stores the list of nodes */
	vector<T> m_nodes;
	/** @brief	Stores the adjacency matrix consisting of weighted edges between nodes. */
	AdjacencyMatrix m_mat;
};