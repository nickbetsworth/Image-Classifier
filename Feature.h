#pragma once
#include <opencv2/opencv.hpp>

class Feature {
public:
	Feature(cv::Mat feature_vector);
	virtual ~Feature();
	virtual float calculate_distance(Feature* feature2);
	cv::Mat get_feature_vector() { return m_feature_vector; };
private:
	cv::Mat m_feature_vector;
};