#pragma once
#include "Image.h"
#include "FeatureManager.h"

// In charge of generating the required image with the specified features
class ImageFactory {
public:
	/** @brief	The version of feature currently being used,  
	/** change this if local feature descriptor generation changes for example. */
	static const int FEATURE_VERSION = 2;

	/**
	 * @fn	static Image* ImageFactory::create_image(const std::string &filepath, FeatureType type);
	 *
	 * @brief	Checks if the specified features are already in database, and re-uses them if so.
	 *
	 * @param	filepath	The filepath.
	 * @param	type		The type of feature to be used.
	 *
	 * @return	An image created from the content specified in filepath.
	 */

	static Image* create_image(const std::string &filepath, FeatureType type);
	
private:
	ImageFactory();
	~ImageFactory();

	/**
	 * @fn	static cv::Mat ImageFactory::get_image_data(const std::string &filepath);
	 *
	 * @brief	Loads in image pixel data from filepath.
	 *
	 * @param	filepath	The filepath.
	 *
	 * @return	The image data.
	 */

	static cv::Mat get_image_data(const std::string &filepath);

	/**
	 * @fn	static std::string ImageFactory::get_feature_string(FeatureType type);
	 *
	 * @brief	Converts a feature type to a string.
	 *
	 * @param	type	The type of feature.
	 *
	 * @return	The feature type string.
	 */

	static std::string get_feature_string(FeatureType type);
	
};