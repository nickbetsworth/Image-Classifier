#include "FeatureLocal.h"
#include "FeatureManager.h"

FeatureLocal::FeatureLocal(cv::Mat feature_vector) : Feature(feature_vector)
{
}


FeatureLocal::~FeatureLocal()
{
}

float FeatureLocal::calculate_distance(Feature* feature2) {
	cv::Ptr<cv::DescriptorMatcher> matcher = FeatureManager::get_matcher();

	std::vector<cv::DMatch> matches;

	// For each descriptore in this feature set, find the closest matches from feature2
	matcher->match(this->get_feature_vector(), feature2->get_feature_vector(), matches);

	// Average the distance of the closest matches
	float dist = 0;
	for (cv::DMatch match : matches) {
		dist += match.distance;
	}
	dist /= matches.size();

	return dist;
}
