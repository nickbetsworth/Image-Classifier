#pragma once

#include <opencv2/features2d/features2d.hpp>

class BOWExtractor {
public:
	BOWExtractor(cv::Mat vocabulary);
	~BOWExtractor();

	void set_vocabulary(cv::Mat vocabulary);
	cv::Mat get_vocabulary() { return m_vocabulary; };

	void get_BOW_hist(cv::Mat descriptors, cv::Mat& bow_hist);
private:
	int get_descriptor_size();
	int get_descriptor_type();

	cv::Mat m_vocabulary; 
	cv::Ptr<cv::DescriptorMatcher> m_matcher;
};