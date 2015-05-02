#include "FeatureHistogram.h"
#include "Histograms.h"

FeatureHistogram::FeatureHistogram(cv::Mat feature_vector) : Feature(feature_vector)
{
}


FeatureHistogram::~FeatureHistogram()
{
}

float FeatureHistogram::calculate_distance(Feature* feature2) {
	return compare_histograms(this->get_feature_vector(), feature2->get_feature_vector());
}
