#pragma once
#include "ImageClass.h"

/**
 * @struct	ClassEdge
 *
 * @brief	Represents a weighted edge signifying the similarity between two ImageClasses.
 *
 * @author	Nick
 * @date	28/10/2014
 */

struct ClassEdge {
	ImageClass* class1;
	ImageClass* class2;
	double weight;
};