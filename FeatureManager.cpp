#include "FeatureManager.h"
#include "Histograms.h"
#include "FeatureHistogram.h"
#include "FeatureLocal.h"

cv::Mat FeatureManager::calculate_histogram(cv::Mat image_data) {
	return get_1d_histogram(image_data, HIST_BINS);
}

std::vector<cv::KeyPoint> FeatureManager::calculate_key_points(cv::Mat image_data) {
	cv::Ptr<cv::FeatureDetector> detector = get_detector();
	std::vector<cv::KeyPoint> key_points;
	detector->detect(image_data, key_points);

	//cv::KeyPointsFilter::retainBest(key_points, Image::MAX_KEY_POINTS);

	return key_points;
}

cv::Mat FeatureManager::calculate_descriptors(cv::Mat image_data, std::vector<cv::KeyPoint>& key_points) {
	cv::Ptr<cv::DescriptorExtractor> extractor = get_extractor();

	cv::Mat descriptors;

	std::cout << "Key points size: " << key_points.size() << std::endl;
	extractor->compute(image_data, key_points, descriptors);

	return descriptors;
}

cv::Mat FeatureManager::PCA_descriptors(cv::Mat descriptors, double ret_variance) {
	cv::PCA pca = cv::PCA(descriptors, cv::Mat(), CV_PCA_DATA_AS_COL, ret_variance);
	cv::Mat reduced = pca.project(descriptors);
	
	return reduced;
}

cv::Ptr<cv::FeatureDetector> FeatureManager::get_detector() {
	if (detector == 0) {
		cv::initModule_nonfree();
		//detector = cv::FeatureDetector::create("SURF");
		detector = new cv::SurfFeatureDetector(800);
	}

	return detector;
}
cv::Ptr<cv::DescriptorExtractor> FeatureManager::get_extractor() {
	if (extractor == 0) {
		cv::initModule_nonfree();
		extractor = cv::DescriptorExtractor::create("SURF");
	}

	return extractor;
}
cv::Ptr<cv::DescriptorMatcher> FeatureManager::get_matcher() {
	if (matcher == 0)
		matcher = cv::DescriptorMatcher::create("BruteForce-L1");

	return matcher;
}

Feature* FeatureManager::create_feature(cv::Mat feature_vector, FeatureType type) {
	switch (type) {
	case FeatureType::COLOUR_HISTOGRAM:
		return new FeatureHistogram(feature_vector);
	case FeatureType::LOCAL_FEATURE:
		return new FeatureLocal(feature_vector);
	}
}

cv::Ptr<cv::FeatureDetector> FeatureManager::detector = 0;
cv::Ptr<cv::DescriptorExtractor> FeatureManager::extractor = 0;
cv::Ptr<cv::DescriptorMatcher> FeatureManager::matcher = 0;