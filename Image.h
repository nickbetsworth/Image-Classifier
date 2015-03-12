#pragma once
#include "NodeProperties.h"
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

class Image : public NodeProperties
{
public:
	/** @brief	Width and height of the thumbnail. */
	static const int THUMB_WIDTH = 200;
	static const int THUMB_HEIGHT = 200;
	/** @brief	The number of bins each colour channel of the histogram will have. */
	static const int HIST_BINS = 8;
	/** @brief	The number of colour channels the image has. */
	static const int NUM_CHANNELS = 3;
	static const int MAX_KEY_POINTS = 200;

	/**
	 * @fn	Image::Image(const string &filepath);
	 *
	 * @brief	Creates a new instance of an image from the specified file.
	 *
	 * @param	filepath	filepath of the image to be loaded in.
	 */

	Image(const std::string &filepath);
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
	 * @fn	string Image::get_filepath() const
	 *
	 * @brief	Returns the filepath of the loaded image.
	 *
	 * @return	The filepath from which this image was loaded.
	 */

	std::string get_filepath() const { return m_filepath; };

	/**
	 * @fn	Mat Image::get_image_data() const
	 *
	 * @brief	Gets image data.
	 *
	 * @return	A matrix containing image data.
	 */

	cv::Mat get_image_data() const { return m_image_data; };

	/**
	 * @fn	Mat Image::get_histogram() const
	 *
	 * @brief	Gets the histogram of this image.
	 *
	 * @return	The histogram.
	 */

	/**
	 * @fn	cv::Mat Image::get_fullres_image() const;
	 *
	 * @brief	Returns a full resolution image.
	 *
	 * @return	The full resolution image.
	 */

	cv::Mat get_fullres_image() const;

	// The reason the following calculations are static is because this Image class does not
	// store the entire image at runtime, after features have been calculated it stores only a thumbnail.
	// This is done to prevent an error where a call may be made to calculate_x after the initial
	// image data has been discarded.

	/**
	 * @fn	static cv::Mat Image::calculate_histogram(cv::Mat image_data);
	 *
	 * @brief	Calculates the histogram for the provided image data.
	 *
	 * @param	image_data	Information describing the image.
	 *
	 * @return	The calculated histogram.
	 *
	 * ### param	bins	The number of bins for each channel in the histogram.
	 */

	static cv::Mat calculate_histogram(cv::Mat image_data);


	static std::vector<cv::KeyPoint> calculate_key_points(cv::Mat image_data);

	/**
	 * @fn	static cv::Mat Image::calculate_SIFT_descriptors(cv::Mat image_data);
	 *
	 * @brief	Calculates a set of SIFT descriptors for 
	 *
	 * @param	image_data	Information describing the image.
	 *
	 * @return	A set of SIFT descriptors.
	 */

	static cv::Mat calculate_descriptors(cv::Mat image_data, std::vector<cv::KeyPoint>& key_points);

	static cv::Ptr<cv::FeatureDetector> get_detector();
	static cv::Ptr<cv::DescriptorExtractor> get_extractor();
	static cv::Ptr<cv::DescriptorMatcher> get_matcher();
private:
	void generate_thumbnail();
	
	static cv::Ptr<cv::FeatureDetector> detector;
	static cv::Ptr<cv::DescriptorExtractor> extractor;
	static cv::Ptr<cv::DescriptorMatcher> matcher;

	/** @brief	Stores the filepath this image belongs to */
	std::string m_filepath;
	/** @brief	Stores the image thumbnail data */
	cv::Mat m_image_data;
	//Mat m_histogram;
};

