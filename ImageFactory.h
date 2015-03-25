#pragma once
#include "Image.h"


// In charge of generating the required image with the specified features
class ImageFactory {
public:
	// Checks if the specified features are already in database
	// If so re-uses them
	static Image* create_image(const std::string &filepath, Property properties);
private:
	ImageFactory();
	~ImageFactory();

	static cv::Mat get_image_data(const std::string &filepath);

	
};