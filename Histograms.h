#pragma once

#include <opencv\cv.h>
#include "Image.h"

// Calculates a histogram with the given number of bins, from the matrix provided.
cv::Mat get_1d_histogram(cv::Mat image_data, int bins);
// Compares two histograms via EMD.
float compare_histograms(cv::Mat hist1, cv::Mat hist2);