#include "ImageFactory.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

Image* ImageFactory::create_image(const std::string &filepath, FeatureType type) {
	const bool is_PCA_Enabled = false;

	// Load in the image data from the provided filepath
	cv::Mat image_data = get_image_data(filepath);
	cv::Mat feature_vector;
	
	std::string data_filepath = filepath + ".yml";
	// Attempt to load in descriptors
	cv::FileStorage r_fs(data_filepath, cv::FileStorage::READ);

	// If the file exists and we are able to open it
	if (r_fs.isOpened()) {
		// Ensure that we have the correct version of features
		int version = 0;
		std::string saved_type;
		r_fs["version"] >> version;
		r_fs["type"] >> saved_type;
		if (version == FEATURE_VERSION && saved_type == get_feature_string(type))
			r_fs["descriptors"] >> feature_vector;

		r_fs.release();
	}

	// If the descriptors are still empty, that means either the file does not exist
	// or it has incorrect formatting
	if (feature_vector.empty()) {
		if (type == FeatureType::LOCAL_FEATURE) {
			
			std::vector<cv::KeyPoint> key_points = FeatureManager::calculate_key_points(image_data);
			feature_vector = FeatureManager::calculate_descriptors(image_data, key_points);

			if (is_PCA_Enabled) {
				feature_vector = FeatureManager::PCA_descriptors(feature_vector);
			}
		}
		else if (type == FeatureType::COLOUR_HISTOGRAM) {
			feature_vector = FeatureManager::calculate_histogram(image_data);
		}
		
		// Save descriptors to file
		cv::FileStorage w_fs(data_filepath, cv::FileStorage::WRITE);
		w_fs << "version" << FEATURE_VERSION;
		w_fs << "type" << get_feature_string(type);
		w_fs << "descriptors" << feature_vector;

		w_fs.release();
	}

	Feature* feature = FeatureManager::create_feature(feature_vector, type);
	Image* image = new Image(filepath, image_data, feature);

	// We can safely release the image data here, as Image makes a clone of it
	image_data.release();
	return image;
}

cv::Mat ImageFactory::get_image_data(const std::string &filepath) {
	cv::Mat image_data = cv::imread(filepath);
	
	return image_data;
}

std::string ImageFactory::get_feature_string(FeatureType type) {
	switch (type) {
	case FeatureType::COLOUR_HISTOGRAM:
		return "histogram";
	case FeatureType::LOCAL_FEATURE:
		return "local";
	}
}