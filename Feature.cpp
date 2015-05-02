#include "Feature.h"

Feature::Feature(cv::Mat feature_vector) {
	m_feature_vector = feature_vector;
}

Feature::~Feature() {
	m_feature_vector.release();
}

float Feature::calculate_distance(Feature* feature2) {
	return cv::norm(this->get_feature_vector(), feature2->get_feature_vector(), cv::NORM_L2);
}
