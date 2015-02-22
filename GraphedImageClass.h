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
public:
	GraphedImageClass();
	~GraphedImageClass();
};

