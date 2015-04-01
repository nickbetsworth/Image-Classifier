#include "FeatureExtractor.h"
#include "Histograms.h"

cv::Mat FeatureExtractor::calculate_histogram(cv::Mat image_data) {
	return get_1d_histogram(image_data, Image::HIST_BINS);
}

std::vector<cv::KeyPoint> FeatureExtractor::calculate_key_points(cv::Mat image_data) {
	cv::Ptr<cv::FeatureDetector> detector = get_detector();
	std::vector<cv::KeyPoint> key_points;
	detector->detect(image_data, key_points);

	cv::KeyPointsFilter::retainBest(key_points, Image::MAX_KEY_POINTS);

	return key_points;
}

cv::Mat FeatureExtractor::calculate_descriptors(cv::Mat image_data, std::vector<cv::KeyPoint>& key_points) {
	cv::Ptr<cv::DescriptorExtractor> extractor = get_extractor();

	cv::Mat descriptors;

	std::cout << "Key points size: " << key_points.size() << std::endl;
	extractor->compute(image_data, key_points, descriptors);

	return descriptors;
}

cv::Mat FeatureExtractor::PCA_descriptors(cv::Mat descriptors, double ret_variance) {
	cv::PCA pca = cv::PCA(descriptors, cv::Mat(), CV_PCA_DATA_AS_COL, ret_variance);
	cv::Mat reduced = pca.project(descriptors);
	
	return reduced;
}

float FeatureExtractor::calculate_descriptor_distance(cv::Mat descriptors1, cv::Mat descriptors2, bool both_dist) {
	float dist = 0;

	if (both_dist)
		dist += calculate_descriptor_distance(descriptors2, descriptors1, false);

	cv::Ptr<cv::DescriptorMatcher> matcher = FeatureExtractor::get_matcher();

	std::vector<cv::DMatch> matches;

	matcher->match(descriptors1, descriptors2, matches);

	for (cv::DMatch match : matches) {
		dist += match.distance;
	}

	dist /= matches.size();

	// Halve the distance if we calculated it in both directions
	if (both_dist)
		dist /= 2;

	return dist / 10;
}



cv::Ptr<cv::FeatureDetector> FeatureExtractor::get_detector() {
	if (detector == 0) {
		cv::initModule_nonfree();
		detector = cv::FeatureDetector::create("SURF");
	}

	return detector;
}
cv::Ptr<cv::DescriptorExtractor> FeatureExtractor::get_extractor() {
	if (extractor == 0) {
		cv::initModule_nonfree();
		extractor = cv::DescriptorExtractor::create("SURF");
	}

	return extractor;
}
cv::Ptr<cv::DescriptorMatcher> FeatureExtractor::get_matcher() {
	if (matcher == 0)
		matcher = cv::DescriptorMatcher::create("BruteForce-L1");

	return matcher;
}

cv::Ptr<cv::FeatureDetector> FeatureExtractor::detector = 0;
cv::Ptr<cv::DescriptorExtractor> FeatureExtractor::extractor = 0;
cv::Ptr<cv::DescriptorMatcher> FeatureExtractor::matcher = 0;