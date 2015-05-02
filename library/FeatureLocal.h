#pragma once
#include "Feature.h"

/**
 * @class	FeatureLocal
 *
 * @brief	Represents a local set of feature descriptors.
 * 			Features should be in row vector form.
 */

class FeatureLocal :
	public Feature
{
public:
	FeatureLocal(cv::Mat feature_vector);
	virtual ~FeatureLocal();

	/**
	 * @fn	virtual float FeatureLocal::calculate_distance(Feature* feature2);
	 *
	 * @brief	Calculates the distance via an average of closest matches of descriptors.
	 *
	 * @param [in,out]	feature2	The feature to calculate distance to.
	 *
	 * @return	The calculated distance.
	 */

	virtual float calculate_distance(Feature* feature2);

};

