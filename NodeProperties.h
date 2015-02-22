#pragma once
#include <opencv\cv.h>
#include <iostream>
using namespace cv;

enum Property {
	Histogram = 1,
	SIFT = 2,
	Test = 4
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
	/**
	 * @enum	Feature
	 *
	 * @brief	Enum that represents which properties a node may hold
	 */

	
public:
	void set_histogram(Mat hist);

	Mat get_histogram();

	Mat get_feature_vector();

	float calculate_distance_histogram(NodeProperties* node2);
	float calculate_distance(NodeProperties* node2);

	bool has_flag(Property flags) const;
	void add_flag(Property flags);
	void remove_flag(Property flags);
protected:
	
private:
	/** @brief	Stores the properties that this node holds. */
	Property m_properties = static_cast<Property>(0);
	Mat m_histogram;
};

