#include "BOWExtractor.h"
#include <iostream>

BOWExtractor::BOWExtractor(cv::Mat vocabulary) {
	m_matcher = cv::DescriptorMatcher::create("BruteForce-L1");
	this->set_vocabulary(vocabulary);
}

BOWExtractor::~BOWExtractor() {

}

void BOWExtractor::set_vocabulary(cv::Mat vocabulary) {
	m_vocabulary = vocabulary;
	m_matcher->clear();
	m_matcher->add(std::vector<cv::Mat>(1, vocabulary));
}

int BOWExtractor::get_descriptor_size() {
	return m_vocabulary.empty() ? 0 : m_vocabulary.rows;
}

int BOWExtractor::get_descriptor_type() {
	return CV_32FC1;
}

void BOWExtractor::get_BOW_hist(cv::Mat descriptors, cv::Mat& bow_hist) {
	int clusterCount = this->get_descriptor_size(); // = vocabulary.rows

	// Match keypoint descriptors to cluster center (to vocabulary)
	std::vector<cv::DMatch> matches;
	m_matcher->match(descriptors, matches);

	bow_hist.create(1, clusterCount, this->get_descriptor_type());
	bow_hist.setTo(cv::Scalar::all(0));


	float *dptr = bow_hist.ptr<float>();
	for (size_t i = 0; i < matches.size(); i++)
	{
		int queryIdx = matches[i].queryIdx;
		int trainIdx = matches[i].trainIdx; // cluster index
		CV_Assert(queryIdx == (int)i);

		dptr[trainIdx] = dptr[trainIdx] + 1.f;

	}
	
	// Normalize image descriptor.
	bow_hist /= descriptors.size().height;
}