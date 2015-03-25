#include "Test_PCA.h"
#include "FeatureExtractor.h"

void test_PCA() {
	cv::Mat test_mat = cv::Mat(10, 8, CV_32F);

	for (int i = 0; i < test_mat.rows; i++) {
		for (int j = 0; j < test_mat.cols; j++) {
			float val = rand() % 255;
			test_mat.at<float>(i, j) = val;
		}
	}

	std::cout << "Before: " << test_mat << std::endl;

	std::cout << "After: " << FeatureExtractor::PCA_descriptors(test_mat, 0.90) << std::endl;
}