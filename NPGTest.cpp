#include "NPGTest.h"
#include "Graph.h"
#include "Image.h"
#include <QDir>
#include <QFileInfo>

#include <iostream>

using namespace std;

void test_NPG() {
	/*Graph* g = new Graph();
	vector<Image*> images;
	QDir dir = QDir("C:/data/ProjectImages/Test/");
	QStringList filters;
	filters << "*.png" << "*.jpg" << "*.gif";

	QFileInfoList list = dir.entryInfoList(filters);
	
	for (QFileInfo info : list) {
		string filepath = info.absoluteFilePath().toStdString();
		Image* image = new Image(filepath);
		if (image->has_loaded())
			images.push_back(image);
		else
			cout << "Unable to load image: " << filepath << endl;
	}

	cout << images.size() << " images loaded" << endl;
	// Load in some images
	// Add the last n - 1 as nodes
	for (auto k = images.begin() + 1; k != images.end(); k++) {
		g->add_node((*k)->get_feature());
	}
	
	AdjacencyMatrix m = g->get_adjacency_matrix();
	NodeList node_list = g->get_nodes();
	cout << node_list.size() << " nodes added to graph:" << endl;
	for (Feature* node1 : node_list) {
		for (Feature* node2 : node_list) {
			cout << node1 << " to " << node2 << ": " << m[node1][node2] << endl;
		}
	}
	cout << endl << endl;

	cout << "Adding new node: " << images.front() << endl;
	g->add_node(images.front());

	m = g->get_adjacency_matrix();
	node_list = g->get_nodes();
	cout << node_list.size() << " nodes in graph:" << endl;
	for (Feature* node1 : node_list) {
		for (Feature* node2 : node_list) {
			cout << node1 << " to " << node2 << ": " << m[node1][node2] << endl;
		}
	}
	cout << endl << endl;

	cout << "Attempting to add existing node: " << images.back() << endl;
	g->add_node(images.back());
	m = g->get_adjacency_matrix();
	node_list = g->get_nodes();
	cout << node_list.size() << " nodes in graph:" << endl;
	for (Feature* node1 : node_list) {
		for (Feature* node2 : node_list) {
			cout << node1 << " to " << node2 << ": " << m[node1][node2] << endl;
		}
	}

	cout << endl << endl;

	cout << "Remove node: " << images.back() << endl;
	g->remove_node(images.back());

	m = g->get_adjacency_matrix();
	node_list = g->get_nodes();
	cout << node_list.size() << " nodes in graph:" << endl;
	for (Feature* node1 : node_list) {
		for (Feature* node2 : node_list) {
			cout << node1 << " to " << node2 << ": " << m[node1][node2] << endl;
		}
	}*/
}