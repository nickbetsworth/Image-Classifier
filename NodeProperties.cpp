#include "NodeProperties.h"
#include "Histograms.h"

void NodeProperties::set_histogram(Mat hist) {
	// If this is not an empty histogram, set the histogram flag to true
	// Otherwise set the histogram flag to false
	if (!hist.empty())
		add_flag(Property::Histogram);
	else
		remove_flag(Property::Histogram);
	
	m_histogram = hist;
}

Mat NodeProperties::get_histogram() {
	if (has_flag(Property::Histogram)) {
		return m_histogram;
	}
	else {
		cout << "Error: attempted to retrieve histogram from NodeProperties whilst histogram flag is disabled" << endl;
		return Mat();
	}
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
	return compare_histograms(this->get_histogram(), node2->get_histogram());
}

float NodeProperties::calculate_distance(NodeProperties* node2) {
	float total = 0;

	if (has_flag(Property::Histogram))
		total += calculate_distance_histogram(node2);
	//if (has_flag(Property::...))
		//total += calculate_distance_...
		
	return total;
}