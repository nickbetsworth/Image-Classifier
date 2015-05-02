#pragma once
#include "Feature.h"
class FeatureLocal :
	public Feature
{
public:
	FeatureLocal(cv::Mat feature_vector);
	virtual ~FeatureLocal();
	virtual float calculate_distance(Feature* feature2);

};

