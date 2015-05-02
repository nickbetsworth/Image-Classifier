#pragma once
#include <opencv2/opencv.hpp>

/**
 * @class	Feature
 *
 * @brief	Represents a feature vector, or set of local feature descriptors from an image.
 * 			Provides an interface for calculation between different Feature vectors.
 * 			Feature vectors should be in the form of a row vector. 
 */

class Feature {
public:
	Feature(cv::Mat feature_vector);
	virtual ~Feature();

	/**
	 * @fn	virtual float Feature::calculate_distance(Feature* feature2);
	 *
	 * @brief	Calculates L2 distance to feature2.
	 *
	 * @param [in,out]	feature2	The feature to calculate distance to.
	 *
	 * @return	The calculated distance.
	 */

	virtual float calculate_distance(Feature* feature2);
	cv::Mat get_feature_vector() { return m_feature_vector; };
private:
	cv::Mat m_feature_vector;
};