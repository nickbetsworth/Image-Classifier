#pragma once
#include <opencv\cv.h>

using namespace cv;

class NodeProperties
{
public:
	void set_histogram(Mat hist) {
		m_histogram = hist;
	}

	Mat get_histogram() {
		return m_histogram;
	}

	Mat get_histogram_norm() {
		Mat hist_norm;
		normalize(m_histogram, hist_norm);
		return hist_norm;
	}
private:
	Mat m_histogram;
};

