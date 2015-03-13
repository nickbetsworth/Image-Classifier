#pragma once
#include <opencv\cv.h>
#include <iostream>

enum Property {
	Histogram = 1,
	SIFT = 2
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
	cv::Mat get_descriptors() const;

	cv::Mat get_feature_vector() const;
	//int get_feature_length(Property flags);

	float calculate_distance_histogram(NodeProperties* node2) const;
	float calculate_distance_descriptors(NodeProperties* node2) const;
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
};

