#pragma once
#include "Feature.h"
class FeatureHistogram :
	public Feature
{
public:
	FeatureHistogram(cv::Mat feature_vector);
	virtual ~FeatureHistogram();
	virtual float calculate_distance(Feature* feature2);
};

