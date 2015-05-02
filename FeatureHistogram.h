#pragma once
#include "Feature.h"

/**
 * @class	FeatureHistogram
 *
 * @brief	Represents a feature vector which is a continuous histogram. 
 */

class FeatureHistogram :
	public Feature
{
public:
	FeatureHistogram(cv::Mat feature_vector);
	virtual ~FeatureHistogram();

	/**
	 * @fn	virtual float FeatureHistogram::calculate_distance(Feature* feature2);
	 *
	 * @brief	Calculates the distance between another histogram via Earth mover's distance.
	 *
	 * @param [in,out]	feature2	The feature to calculate distance to.
	 *
	 * @return	The calculated distance.
	 */

	virtual float calculate_distance(Feature* feature2);
};

