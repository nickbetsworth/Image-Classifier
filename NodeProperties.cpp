#include "NodeProperties.h"
#include "Histograms.h"

void NodeProperties::set_histogram(cv::Mat hist) {
	// If this is not an empty histogram, set the histogram flag to true
	// Otherwise set the histogram flag to false
	if (!hist.empty())
		add_flag(Property::Histogram);
	else
		remove_flag(Property::Histogram);
	
	m_histogram = hist;
}

cv::Mat NodeProperties::get_histogram() const {
	if (has_flag(Property::Histogram)) {
		return m_histogram;
	}
	else {
		std::cout << "Error: attempted to retrieve histogram from NodeProperties whilst histogram flag is disabled" << std::endl;
		return cv::Mat();
	}
}

void NodeProperties::set_keypoint_descriptors(std::vector<cv::KeyPoint> key_points, cv::Mat descriptors) {
	if (key_points.size() > 0 && !descriptors.empty())
		add_flag(Property::SIFT);
	else
		remove_flag(Property::SIFT);

	m_key_points = key_points;
	m_descriptors = descriptors;
}

cv::Mat NodeProperties::get_descriptors() const {
	if (has_flag(Property::SIFT)) {
		return m_descriptors;
	}
	else {
		std::cout << "Error: attempted to retrieve descriptors from NodeProperties whilst SIFT flag is disabled" << std::endl;
		return cv::Mat();
	}
}

std::vector<cv::KeyPoint> NodeProperties::get_key_points() const {
	if (has_flag(Property::SIFT)) {
		return m_key_points;
	}
	else {
		std::cout << "Error: attempted to retrieve key points from NodeProperties whilst SIFT flag is disabled" << std::endl;
		return std::vector<cv::KeyPoint>();
	}
}


cv::Mat NodeProperties::get_feature_vector() const {
	return get_histogram();
}

//int get_feature_length(Property flags) {
	//if ()
//}

bool NodeProperties::has_flag(Property flags) const {
	return (m_properties & flags) == flags;
}
void NodeProperties::add_flag(Property flags) {
	m_properties |= flags;
}
void NodeProperties::remove_flag(Property flags) {
	m_properties = static_cast<Property>(m_properties & (~flags));
}

float NodeProperties::calculate_distance_histogram(NodeProperties* node2) const {
	// Earth movers distance
	return compare_histograms(this->get_histogram(), node2->get_histogram());
}

float NodeProperties::calculate_distance_descriptors(NodeProperties* node2) const {
	float dist = 0;

	cv::Ptr<cv::DescriptorMatcher> matcher = Image::get_matcher();
	std::vector<std::vector<cv::DMatch>> matches;
	matcher->knnMatch(node2->get_descriptors(), this->get_descriptors(), matches, 16);

	int n = 0;
	for (std::vector<cv::DMatch> match_list : matches) {
		for (cv::DMatch match : match_list) {
			n++;
			dist += match.distance;
		}
	}

	dist /= n;
	std::cout << "Distance from " << this << " to " << node2 << ": " << dist << " with " << n << " matches." << std::endl;
	return dist;
}

float NodeProperties::calculate_distance(NodeProperties* node2) const {
	float total = 0;
	
	//if (has_flag(Property::Histogram) && node2->has_flag(Property::Histogram))
		//total += calculate_distance_histogram(node2);
	if (has_flag(Property::SIFT) && node2->has_flag(Property::SIFT))
		total += calculate_distance_descriptors(node2);
		
	return total;
}