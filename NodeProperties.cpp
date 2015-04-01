#include "NodeProperties.h"
#include "Histograms.h"
#include "FeatureExtractor.h"
#include <chrono>

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

void NodeProperties::set_descriptors(cv::Mat descriptors) {
	if (!descriptors.empty())
		add_flag(Property::SIFT);
	else
		remove_flag(Property::SIFT);

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

void NodeProperties::set_PCA_descriptors(cv::Mat descriptors) {
	if (!descriptors.empty())
		add_flag(Property::PCA_SIFT);

	m_PCA_descriptors = descriptors;
}

cv::Mat NodeProperties::get_PCA_descriptors() const {
	if (has_flag(Property::PCA_SIFT)) {
		return m_PCA_descriptors;
	}
	else {
		std::cout << "Error: attempted to retrieve descriptors from NodeProperties whilst PCA_SIFT flag is disabled" << std::endl;
		return cv::Mat();
	}
}

void NodeProperties::set_BOW_histogram(cv::Mat bow_hist) {
	if (!bow_hist.empty())
		add_flag(Property::BOW);

	m_BOW_histogram = bow_hist;
}
cv::Mat NodeProperties::get_BOW_histogram() const {
	if (has_flag(Property::BOW)) {
		return m_BOW_histogram;
	}
	else {
		std::cout << "Error: attempted to retrieve descriptors from NodeProperties whilst BOW flag is disabled" << std::endl;
		return cv::Mat();
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
	float dist = FeatureExtractor::calculate_descriptor_distance(this->get_descriptors(), node2->get_descriptors());

	return dist;
}

float NodeProperties::calculate_distance_PCA_descriptors(NodeProperties* node2) const {
	float dist = FeatureExtractor::calculate_descriptor_distance(this->get_PCA_descriptors(), node2->get_PCA_descriptors(), true);

	return dist;
}

float NodeProperties::calculate_distance_BOW_histogram(NodeProperties* node2) const {
	// Compare histogram function takes 2 column vectors, so we need to transpose our histograms
	cv::Mat node1T = this->get_BOW_histogram().clone().t();
	cv::Mat node2T = node2->get_BOW_histogram().clone().t();


	float dist = compare_histograms(node1T, node2T);

	node1T.release();
	node2T.release();

	return dist;
}

float NodeProperties::calculate_distance(NodeProperties* node2) const {
	float total = 0;
	
	if (has_flag(Property::BOW) && node2->has_flag(Property::BOW))
		total = calculate_distance_BOW_histogram(node2);
	else if (has_flag(Property::PCA_SIFT) && node2->has_flag(Property::PCA_SIFT))
		total = calculate_distance_PCA_descriptors(node2);
	// Only calculate the SIFT distance if the PCA distance is not calculated
	// This is because PCA Sift is typically done to reduce calculation times
	else if (has_flag(Property::SIFT) && node2->has_flag(Property::SIFT))
		total = calculate_distance_descriptors(node2);
	else if (has_flag(Property::Histogram) && node2->has_flag(Property::Histogram))
		total = calculate_distance_histogram(node2);
		
	return total;
}