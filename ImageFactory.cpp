#include "ImageFactory.h"
#include "FeatureExtractor.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

Image* ImageFactory::create_image(const std::string &filepath, Property properties) {
	// Load in the image data from the provided filepath
	cv::Mat image_data = get_image_data(filepath);

	Image* image = new Image(filepath, image_data);

	if (properties & (Property::BOW | Property::SIFT | Property::PCA_SIFT)) {
		std::vector<cv::KeyPoint> key_points = FeatureExtractor::calculate_key_points(image_data);
		cv::Mat descriptors = FeatureExtractor::calculate_descriptors(image_data, key_points);

		image->set_keypoint_descriptors(key_points, descriptors);

		cv::Mat PCA_descriptors = FeatureExtractor::PCA_descriptors(descriptors, 0.95);
		image->set_PCA_descriptors(PCA_descriptors);
	}

	if (properties & Property::Histogram) {
		image->set_histogram(FeatureExtractor::calculate_histogram(image_data));
	}

	// We can safely release the image data here, as Image makes a clone of it
	image_data.release();
	return image;
}

cv::Mat ImageFactory::get_image_data(const std::string &filepath) {
	cv::Mat image_data = cv::imread(filepath);
	
	return image_data;
}

