#pragma once

#include <opencv2/features2d/features2d.hpp>

/**
 * @class	BOWExtractor
 *
 * @brief	Extracts normalised histogram of word frequencies with respect to the BoW provided
 */

class BOWExtractor {
public:

	/**
	 * @fn	BOWExtractor::BOWExtractor(cv::Mat vocabulary);
	 *
	 * @brief	Constructor.
	 *
	 * @param	vocabulary	The bag of words vocabulary (dictionary).
	 */

	BOWExtractor(cv::Mat vocabulary);
	~BOWExtractor();

	/**
	 * @fn	void BOWExtractor::set_vocabulary(cv::Mat vocabulary);
	 *
	 * @brief	Sets the BOW vocabulary.
	 *
	 * @param	vocabulary	The vocabulary.
	 */

	void set_vocabulary(cv::Mat vocabulary);
	cv::Mat get_vocabulary() { return m_vocabulary; };

	/**
	 * @fn	void BOWExtractor::get_BOW_hist(cv::Mat descriptors, cv::Mat& bow_hist);
	 *
	 * @brief	Extracts word frequencies with respect to the vocabulary.
	 *
	 * @param	descriptors			Descriptors in a row vector format.
	 * @param [in,out]	bow_hist	Frequencies of words w.r.t vocab found in descriptors.
	 */

	void get_BOW_hist(cv::Mat descriptors, cv::Mat& bow_hist);
private:

	/**
	 * @fn	int BOWExtractor::get_descriptor_size();
	 *
	 * @brief	Specifies the size of the vocabulary.
	 *
	 * @return	The number of words in the vocabulary.
	 */

	int get_descriptor_size();

	/**
	 * @fn	int BOWExtractor::get_descriptor_type();
	 *
	 * @brief	Gets the matrix type of each descriptor.
	 *
	 * @return	The descriptor type.
	 */

	int get_descriptor_type();

	/** @brief	The vocabulary used to extract the histogram frequencies. */
	cv::Mat m_vocabulary; 
	/** @brief	Finds closest matches between vocab and descriptors */
	cv::Ptr<cv::DescriptorMatcher> m_matcher;
};