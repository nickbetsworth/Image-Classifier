#pragma once
#include "ImageClass.h"

/**
 * @class	GraphedImageClass
 *
 * @brief	GraphedImageClass adds extra graphing elements to ImageClass, 
 * 			such as the edges between each of the images, for later use in the
 * 			positioning stage.
 */

class GraphedImageClass : public ImageClass
{
	typedef map<Image*, map<Image*, float>> AdjacencyMatrix;
public:
	GraphedImageClass();
	~GraphedImageClass();
	void add_node(Image* node);
	void add_edge(Image* nodeA, Image* nodeB);
};

