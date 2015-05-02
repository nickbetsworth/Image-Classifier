#pragma once
#include <opencv/cv.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "Feature.h"

enum class FeatureType { LOCAL_FEATURE, COLOUR_HISTOGRAM };

/**
 * @class	FeatureManager
 *
 * @brief	In charge of getting features from matrix data, (images)
 * 			Such as SURF / Colour Histograms.
 * 			Also provides a number of other features such as PCA of local descriptors.
 */

class FeatureManager {
public:
	/** @brief	The number of bins each colour channel of the histogram will have. */
	static const int HIST_BINS = 8;
	/** @brief	The number of colour channels the matrices have. */
	static const int NUM_CHANNELS = 3;

	static cv::Ptr<cv::FeatureDetector> get_detector();
	static cv::Ptr<cv::DescriptorExtractor> get_extractor();
	static cv::Ptr<cv::DescriptorMatcher> get_matcher();

	/**
	 * @fn	static Feature* FeatureManager::create_feature(cv::Mat feature_vector, FeatureType type);
	 *
	 * @brief	Creates the corresponding sub-class of Feature with respect to FeatureType
	 *
	 * @param	feature_vector	The feature vector.
	 * @param	type		  	The type of feature provided in feature_vector.
	 *
	 * @return	null if it fails, else the new feature.
	 */

	static Feature* create_feature(cv::Mat feature_vector, FeatureType type);

	/**
	 * @fn	static cv::Mat FeatureManager::calculate_histogram(cv::Mat image_data);
	 *
	 * @brief	Calculates a 1-D colour histogram for the provided image data.
	 *
	 * @param	image_data	Pixel data for the image.
	 *
	 * @return	The calculated histogram.
	 */

	static cv::Mat calculate_histogram(cv::Mat image_data);


	static std::vector<cv::KeyPoint> calculate_key_points(cv::Mat image_data);

	/**
	 * @fn	static cv::Mat FeatureManager::calculate_descriptors(cv::Mat image_data, std::vector<cv::KeyPoint>& key_points);
	 *
	 * @brief	Calculates a set of local feature descriptors for the provided image.
	 *
	 * @param	image_data		  	Pixel data for the image.
	 * @param [in,out]	key_points	The key points at which to calculate the descriptors.
	 *
	 * @return	A set of local feature descriptors.
	 */

	static cv::Mat calculate_descriptors(cv::Mat image_data, std::vector<cv::KeyPoint>& key_points);

	/**
	 * @fn	static cv::Mat FeatureManager::PCA_descriptors(cv::Mat descriptors, double ret_variance = 0.9);
	 *
	 * @brief	Performs PCA to reduce dimensionality on a set of descriptors
	 *
	 * @param	descriptors 	The set of feature descriptors in row vector form.
	 * @param	ret_variance	the amount of retained variance via projection, higher values retain more descriptors.
	 *
	 * @return	The reduced set of descriptors.
	 */

	static cv::Mat PCA_descriptors(cv::Mat descriptors, double ret_variance = 0.9);
private:
	static cv::Ptr<cv::FeatureDetector> detector;
	static cv::Ptr<cv::DescriptorExtractor> extractor;
	static cv::Ptr<cv::DescriptorMatcher> matcher;

	FeatureManager();
	~FeatureManager();
};