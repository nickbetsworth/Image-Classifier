#pragma once
#include <opencv/cv.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

// In charge of getting features from matrix data, (images)
// Such as SURF / Colour Histograms
class FeatureExtractor {
public:
	static cv::Ptr<cv::FeatureDetector> get_detector();
	static cv::Ptr<cv::DescriptorExtractor> get_extractor();
	static cv::Ptr<cv::DescriptorMatcher> get_matcher();

	// The reason the following calculations are static is because this Image class does not
	// store the entire image at runtime, after features have been calculated it stores only a thumbnail.
	// This is done to prevent an error where a call may be made to calculate_x after the initial
	// image data has been discarded.

	/**
	* @fn	static cv::Mat Image::calculate_histogram(cv::Mat image_data);
	*
	* @brief	Calculates the histogram for the provided image data.
	*
	* @param	image_data	Information describing the image.
	*
	* @return	The calculated histogram.
	*
	*/

	static cv::Mat calculate_histogram(cv::Mat image_data);


	static std::vector<cv::KeyPoint> calculate_key_points(cv::Mat image_data);

	/**
	* @fn	static cv::Mat Image::calculate_SIFT_descriptors(cv::Mat image_data);
	*
	* @brief	Calculates a set of SIFT descriptors for
	*
	* @param	image_data	Information describing the image.
	*
	* @return	A set of SIFT descriptors.
	*/

	static cv::Mat calculate_descriptors(cv::Mat image_data, std::vector<cv::KeyPoint>& key_points);
private:
	static cv::Ptr<cv::FeatureDetector> detector;
	static cv::Ptr<cv::DescriptorExtractor> extractor;
	static cv::Ptr<cv::DescriptorMatcher> matcher;

	FeatureExtractor();
	~FeatureExtractor();
};