#pragma once

#include <opencv\cv.h>
#include "Image.h"

/**
 * @fn	cv::Mat get_1d_histogram(cv::Mat image_data, int bins);
 *
 * @brief	Calculates a histogram with the given number of bins, from the matrix provided.
 *
 * @param	image_data	Pixel data for the image.
 * @param	bins	  	The number of bins in the histogram per colour channel.
 *
 * @return	The 1d histogram.
 */

cv::Mat get_1d_histogram(cv::Mat image_data, int bins);

/**
 * @fn	float compare_histograms(cv::Mat hist1, cv::Mat hist2);
 *
 * @brief	Compares two histograms via Earth mover's distance.
 *
 * @param	hist1	The first histogram in row vector form.
 * @param	hist2	The second histogram in row vector form.
 *
 * @return	The distance between the histograms.
 */

float compare_histograms(cv::Mat hist1, cv::Mat hist2);