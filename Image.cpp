#include "Image.h"
#include <opencv\highgui.h>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include "Histograms.h"

Image::Image(const std::string &filepath)
{
	m_filepath = filepath;
	// If we have been provided with an empty string
	if (filepath.length() == 0) {
		std::cout << "Error: Empty filename provided for image creation" << std::endl;
		return;
	}
	
	m_image_data = cv::imread(filepath);

	// Do our histogram calculation before resizing
	this->set_histogram(calculate_histogram(m_image_data));
	std::vector<cv::KeyPoint> key_points = calculate_key_points(m_image_data);
	cv::Mat descriptors = calculate_descriptors(m_image_data, key_points);

	this->set_keypoint_descriptors(key_points, descriptors);

	// Resize our image to a thumbnail size
	generate_thumbnail();
}


Image::~Image()
{
	m_image_data.release();
}

cv::Mat Image::get_fullres_image() const {
	cv::Mat fullres_image = cv::imread(m_filepath);
	return fullres_image;
}

cv::Mat Image::calculate_histogram(cv::Mat image_data) {
	return get_1d_histogram(image_data, HIST_BINS);
}

std::vector<cv::KeyPoint> Image::calculate_key_points(cv::Mat image_data) {
	cv::Ptr<cv::FeatureDetector> detector = get_detector();
	std::vector<cv::KeyPoint> key_points;
	detector->detect(image_data, key_points);

	return key_points;
}

cv::Mat Image::calculate_descriptors(cv::Mat image_data, std::vector<cv::KeyPoint>& key_points) {
	cv::Ptr<cv::DescriptorExtractor> extractor = get_extractor();

	cv::Mat descriptors;

	std::cout << "Key points size: " << key_points.size() << std::endl;
	extractor->compute(image_data, key_points, descriptors);

	return descriptors;
}

void Image::generate_thumbnail() {
	// First we need to convert the image in to a square
	// To do this we will crop the data in to a square
	
	cv::Rect crop_sq;
	// Work out whether we need to crop the rows or columns
	// In this case we need to remove rows
	if (m_image_data.rows > m_image_data.cols) {
		int size = m_image_data.cols;
		int extra = m_image_data.rows - size;
		int x = 0;
		int y = extra / 2;
		crop_sq = cv::Rect(x, y, size, size);
	}
	// In this case we need to remove columns
	// This also catches the case that it is already a square
	else {
		int size = m_image_data.rows;
		int extra = m_image_data.cols - size;
		int x = extra / 2;
		int y = 0;
		crop_sq = cv::Rect(x, y, size, size);
	}
	m_image_data = m_image_data(crop_sq);

	resize(m_image_data, m_image_data, cv::Size(THUMB_WIDTH, THUMB_HEIGHT));
}

cv::Ptr<cv::FeatureDetector> Image::get_detector() {
	if (detector == 0) {
		cv::initModule_nonfree();
		detector = cv::FeatureDetector::create("SURF");
	}

	return detector;
}
cv::Ptr<cv::DescriptorExtractor> Image::get_extractor() {
	if (extractor == 0) {
		cv::initModule_nonfree();
		extractor = cv::DescriptorExtractor::create("SURF");
	}

	return extractor;
}
cv::Ptr<cv::DescriptorMatcher> Image::get_matcher() {
	if (matcher == 0)
		matcher = cv::DescriptorMatcher::create("FlannBased");

	return matcher;
}

cv::Ptr<cv::FeatureDetector> Image::detector = 0;
cv::Ptr<cv::DescriptorExtractor> Image::extractor = 0;
cv::Ptr<cv::DescriptorMatcher> Image::matcher = 0;