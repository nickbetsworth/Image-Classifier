#pragma once
#include "Feature.h"
#include <string>
#include <opencv\cv.h>

/**
 * @class	Image
 *
 * @brief	Represents a single image.
 *			Stores essential information such as the image data, and file name.
 *			When attempting to create a new image, it is advised to use has_loaded()
 *			to check whether or not the image was successfully loaded.
 */

class Image
{
public:
	/** @brief	Width and height of the thumbnail. */
	static const int THUMB_WIDTH = 200;
	static const int THUMB_HEIGHT = 200;

	/**
	 * @fn	Image::Image(const std::string &filepath, Feature* feature);
	 *
	 * @brief	Creates a new instance of an image from the specified file.
	 *
	 * @param	filepath	   	filepath of the image to be loaded in.
	 * @param [in,out]	feature	A feature vector describing this image.
	 */

	Image(const std::string &filepath, Feature* feature);

	/**
	 * @fn	Image::Image(const std::string &filepath, cv::Mat image_data, Feature* feature);
	 *
	 * @brief	Image construct to allow the image data to be pre-loaded in.
	 *
	 * @param	filepath	   	The filepath.
	 * @param	image_data	   	Pixel data for this image.
	 * @param [in,out]	feature	A feature vector describing this image.
	 */

	Image(const std::string &filepath, cv::Mat image_data, Feature* feature);
	~Image();

	/**
	 * @fn	bool Image::has_loaded() const
	 *
	 * @brief	Query if this image was successfully loaded.
	 *
	 * @return	true if loaded, false if not.
	 */

	bool has_loaded() const { return m_image_data.data != 0; };

	/**
	 * @fn	std::string Image::get_filepath() const
	 *
	 * @brief	Returns the filepath of the loaded image.
	 *
	 * @return	The filepath from which this image was loaded.
	 */

	std::string get_filepath() const { return m_filepath; };

	/**
	 * @fn	cv::Mat Image::get_image_data() const
	 *
	 * @brief	Gets image data.
	 *
	 * @return	A matrix containing image data.
	 */

	cv::Mat get_image_data() const { return m_image_data; };

	/**
	 * @fn	cv::Mat Image::get_fullres_image() const;
	 *
	 * @brief	Returns a full resolution image.
	 *
	 * @return	The full resolution image.
	 */

	cv::Mat get_fullres_image() const;

	/**
	 * @fn	Feature* Image::get_feature() const
	 *
	 * @brief	Gets the feature that represents this image.
	 *
	 * @return	The feature.
	 */

	Feature* get_feature() const { return m_feature; };
private:

	/**
	 * @fn	void Image::generate_thumbnail();
	 *
	 * @brief	Scales the existing m_image_data to thumbnail size.
	 */

	void generate_thumbnail();

	/** @brief	Stores the filepath this image belongs to */
	std::string m_filepath;
	/** @brief	Stores the image thumbnail data */
	cv::Mat m_image_data;

	Feature* m_feature;
};

