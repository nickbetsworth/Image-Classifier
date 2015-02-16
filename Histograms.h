#pragma once

#include <opencv\cv.h>
#include "Image.h"

Mat get_1d_histogram(Mat image_data, int bins);
double compare_histograms(Mat hist1, Mat hist2);