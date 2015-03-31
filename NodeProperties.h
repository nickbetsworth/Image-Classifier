#pragma once
#include <opencv\cv.h>
#include <iostream>

enum Property {
	Histogram = 1,
	SIFT = 2,
	PCA_SIFT = 4,
	BOW = 8
};
inline Property operator|(const Property a, const Property b) {
	return static_cast<Property>(static_cast<int>(a) | static_cast<int>(b));
};
inline Property& operator|=(Property& a, const Property b) {
	return a = a | b;
}
inline int operator&(const Property a, const Property b) {
	return static_cast<int>(a) & static_cast<int>(b);
}

class NodeProperties
{
	
public:
	void set_histogram(cv::Mat hist);
	cv::Mat get_histogram() const;
	
	void set_keypoint_descriptors(std::vector<cv::KeyPoint> key_points, cv::Mat descriptors);
	std::vector<cv::KeyPoint> get_key_points() const;
	void set_descriptors(cv::Mat descriptors);
	cv::Mat get_descriptors() const;

	void set_PCA_descriptors(cv::Mat descriptors);
	cv::Mat get_PCA_descriptors() const;

	void set_BOW_histogram(cv::Mat bow_hist);
	cv::Mat get_BOW_histogram() const;

	cv::Mat get_feature_vector() const;
	//int get_feature_length(Property flags);

	float calculate_distance_histogram(NodeProperties* node2) const;
	float calculate_distance_descriptors(NodeProperties* node2) const;
	float calculate_distance_PCA_descriptors(NodeProperties* node2) const;
	float calculate_distance_BOW_histogram(NodeProperties* node2) const;
	float calculate_distance(NodeProperties* node2) const;

	bool has_flag(Property flags) const;
	void add_flag(Property flags);
	void remove_flag(Property flags);
protected:
	
private:
	/** @brief	Stores the properties that this node holds. */
	Property m_properties = static_cast<Property>(0);
	cv::Mat m_histogram;
	std::vector<cv::KeyPoint> m_key_points;
	cv::Mat m_descriptors;
	cv::Mat m_PCA_descriptors;
	cv::Mat m_BOW_histogram;
};

