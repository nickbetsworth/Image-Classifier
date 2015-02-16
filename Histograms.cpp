#include "Histograms.h"

Mat get_1d_histogram(Mat image_data, int bins = 8) {
	const int dims = 1;
	const int sizes[] = { bins, bins, bins };
	const int num_channels = 3;
	const int channels[] = { 0, 1, 2 };
	float rRange[] = { 0, 256 };
	float gRange[] = { 0, 256 };
	float bRange[] = { 0, 256 };
	const float *ranges[] = { rRange, gRange, bRange };
	Mat mask = Mat();

	Mat hist = Mat(0, 1, CV_32F);
	Mat temp;
	for (int c = 0; c < num_channels; c++) {
		// Calculate the histogram for the current color channel
		calcHist(&image_data, 1, &channels[c], mask, temp, dims, sizes, &ranges[c], true, true);
		// Append the current channel's histogram
		hist.push_back(temp);
		//cout << hist;
	}

	normalize(hist, hist);
	return hist;
}

double compare_histograms(Mat hist1, Mat hist2) {
	// Normalize each of the histograms
	Mat norm_hist1, norm_hist2;
	normalize(hist1, norm_hist1);
	normalize(hist2, norm_hist2);

	// Append an index of the corresponding row of each histogram bin, for EMD
	Mat emd_cols = Mat::zeros(Image::HIST_BINS * Image::NUM_CHANNELS, 1, CV_32F);
	for (int i = 0; i < Image::HIST_BINS * Image::NUM_CHANNELS; i++) {
		emd_cols.at<float>(i, 0) = i + 1;
	}

	hconcat(norm_hist1, emd_cols, norm_hist1);
	hconcat(norm_hist2, emd_cols, norm_hist2);

	double val = EMD(norm_hist1, norm_hist2, CV_DIST_L1);
	//cout << "EMD Value: " << val << endl;

	return val;
}