#include "ImageFactory.h"
#include "FeatureExtractor.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

Image* ImageFactory::create_image(const std::string &filepath, Property properties) {
	// Load in the image data from the provided filepath
	cv::Mat image_data = get_image_data(filepath);

	Image* image = new Image(filepath, image_data);

	if (properties & (Property::SURF)) {
		std::string data_filepath = filepath + ".yml";
		// Attempt to load in descriptors
		cv::FileStorage r_fs(data_filepath, cv::FileStorage::READ);
		cv::Mat descriptors;

		// If the file exists and we are able to open it
		if (r_fs.isOpened()) {
			r_fs["surf_descriptors"] >> descriptors;
		}
		else {
			std::vector<cv::KeyPoint> key_points = FeatureExtractor::calculate_key_points(image_data);
			descriptors = FeatureExtractor::calculate_descriptors(image_data, key_points);

			// Save the descriptors to the file
			cv::FileStorage w_fs(data_filepath, cv::FileStorage::WRITE);
			w_fs << "descriptors" << descriptors;

			w_fs.release();
		}

		r_fs.release();
		image->set_descriptors(descriptors);

		if (properties & (Property::PCA_SURF)) {
			cv::Mat PCA_descriptors = FeatureExtractor::PCA_descriptors(descriptors);
			image->set_PCA_descriptors(PCA_descriptors);
		}
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

