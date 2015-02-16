#include "imageclassifierwindow.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <opencv\cv.h>
#include <opencv\highgui.h>
// Include Windows.h to temporarily display console
#include <Windows.h>
#include <sstream>
#include <iostream>
#include "Graph.h"

void ShowConsole()
{
	AllocConsole();
	FILE *pFileCon = NULL;
	pFileCon = freopen("CONOUT$", "w", stdout);

	COORD coordInfo;
	coordInfo.X = 130;
	coordInfo.Y = 9000;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
}

void test_graph() {
	Graph<int> g;
	for (int i = 0; i <= 5; i++) {
		g.add_node(i);
	}

	g.add_edge(0, 1, 1);
	g.add_edge(1, 2, 6);
	g.add_edge(0, 3, 3);
	g.add_edge(3, 1, 5);
	g.add_edge(4, 1, 1);
	g.add_edge(4, 2, 5);
	g.add_edge(5, 2, 2);
	g.add_edge(3, 4, 1);
	g.add_edge(4, 5, 4);
	//cout << "Edge weight: " << g.get_edge_weight(4, 2);
	auto spanning_tree_edges = g.get_minimum_spanning_tree();
	for (auto edge : spanning_tree_edges) {
		cout << edge.first << " to " << edge.second << endl;
	}
}


int main(int argc, char *argv[])
{
	ShowConsole();
	
	QApplication a(argc, argv);
	ImageClassifierWindow w;
	w.show();
	w.showMaximized();

	return a.exec();
}

