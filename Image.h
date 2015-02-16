#pragma once
#include "NodeProperties.h"
#include <string>
#include <opencv\cv.h>

/**
 * @class	Image
 *
 * @brief	Represents a single image
 *			Stores essential information such as the image data, and file name.
 *			When attempting to create a new image, it is advised to use has_loaded()
 *			to check whether or not the image was successfully loaded.
 * @author	Nick
 * @date	25/10/2014
 */

using namespace std;
using namespace cv;

class Image : public NodeProperties
{
public:
	// Width and height of the thumbnail
	static const int THUMB_WIDTH = 200;
	static const int THUMB_HEIGHT = 200;
	// The number of bins each colour channel of the histogram will have
	static const int HIST_BINS = 8;
	// The number of colour channels the image has
	static const int NUM_CHANNELS = 3;

	/**
	 * @fn	Image::Image(const string &filepath);
	 *
	 * @brief	Creates a new instance of an image from the specified file.
	 *
	 * @author	Nick
	 * @date	25/10/2014
	 *
	 * @param	filepath	filepath of the image to be loaded in.
	 */
	Image(const string &filepath);
	~Image();

	/**
	 * @fn	bool Image::has_loaded();
	 *
	 * @brief	Query if this image was successfully loaded.
	 *
	 * @author	Nick
	 * @date	25/10/2014
	 *
	 * @return	true if loaded, false if not.
	 */
	bool has_loaded() const { return m_image_data.data != 0; };

	/**
	 * @fn	string Image::get_filepath()
	 *
	 * @brief	Returns the filepath of the loaded image.
	 *
	 * @author	Nick
	 * @date	25/10/2014
	 *
	 * @return	The filepath from which this image was loaded
	 */
	string get_filepath() const { return m_filepath; };

	/**
	 * @fn	Mat Image::get_image_data()
	 *
	 * @brief	Gets image data.
	 *
	 * @author	Nick
	 * @date	25/10/2014
	 *
	 * @return	A matrix containing image data.
	 */
	Mat get_image_data() const { return m_image_data; };
	Mat get_histogram() const { return m_histogram; };
	// Returns a full resolution image
	Mat get_fullres_image() const;
private:
	/** @brief	Stores the filepath this image belongs to */
	string m_filepath;
	/** @brief	Stores the image thumbnail data */
	Mat m_image_data;
	Mat m_histogram;
};

