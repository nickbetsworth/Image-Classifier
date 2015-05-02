#pragma once
#include "Image.h"
#include "FeatureManager.h"

// In charge of generating the required image with the specified features
class ImageFactory {
public:
	static const int FEATURE_VERSION = 2;
	// Checks if the specified features are already in database
	// If so re-uses them
	static Image* create_image(const std::string &filepath, FeatureType type);
	
private:
	ImageFactory();
	~ImageFactory();

	static cv::Mat get_image_data(const std::string &filepath);
	static std::string get_feature_string(FeatureType type);
	
};