#include "NodeProperties.h"
#include "Histograms.h"

void NodeProperties::set_histogram(Mat hist) {

	m_histogram = hist;
}

Mat NodeProperties::get_histogram() {
	return m_histogram;
}

Mat NodeProperties::get_histogram_norm() {
	Mat hist_norm;
	normalize(m_histogram, hist_norm);
	return hist_norm;
}

Mat NodeProperties::get_feature_vector() {
	return get_histogram();
}

bool NodeProperties::has_flag(Property flags) const {
	return (m_properties & flags) == flags;
}
void NodeProperties::add_flag(Property flags) {
	m_properties |= flags;
}
void NodeProperties::remove_flag(Property flags) {
	m_properties = static_cast<Property>(m_properties & (~flags));
}

float NodeProperties::calculate_distance_histogram(NodeProperties* node2) {
	// Earth movers distance
	return compare_histograms(this->get_histogram(), node2->get_histogram());;
}

float NodeProperties::calculate_distance(NodeProperties* node2) {
	float total = 0;

	if (has_flag(Property::Histogram))
		total += calculate_distance_histogram(node2);
	//if (has_flag(Property::...))
		//total += calculate_distance_...
		
	return total;
}