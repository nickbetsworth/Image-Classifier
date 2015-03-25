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

cv::Ptr<cv::FeatureDetector> FeatureExtractor::get_detector() {
	if (detector == 0) {
		cv::initModule_nonfree();
		detector = cv::FeatureDetector::create("SURF");
		//detector = new cv::SiftFeatureDetector(Image::MAX_KEY_POINTS);
		//detector->set("nfeatures", Image::MAX_KEY_POINTS);
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