#include "ImageClass.h"
#include "NodePropertiesGraphBOW.h"

ImageClass::ImageClass()
{
	m_icon = 0;

	m_graph = new NodePropertiesGraph();
}

ImageClass::ImageClass(vector<Image*> images)
{
	m_icon = 0;

	// Convert the array of images to array of Nodes
	// So we can pass it to the Graph
	std::vector<Node> nodes;
	for (Image* image : images) {
		nodes.push_back(static_cast<Node>(image));
	}

	m_graph = new NodePropertiesGraphBOW(nodes);
	// Loop through each of the images so we can tally the histograms
	for (Image* image : images) {
		add_image(image);
	}

	
}

ImageClass::~ImageClass()
{
}

bool ImageClass::add_image(Image* image)
{
	// Only add successfully loaded images to the image list
	if (image->has_loaded()) {
		// Add up the histogram to the property sheet's total
		//this->set_histogram(this->get_histogram() + image->get_histogram());
		m_graph->add_node(image);
		m_images.push_back(image);
		return true;
	}
	else {
		cout << "Attempted to add non-loaded image to a class. Filepath: " << image->get_filepath() << endl;
		return false;
	}
}

void ImageClass::remove_image(Image* image) {
	//int initial_size = m_images.size();
	m_graph->remove_node(image);
	m_images.erase(remove(m_images.begin(), m_images.end(), image), m_images.end());
	/*int size_difference = m_images.size() - initial_size;
	// If the size has changed (the image existed in the class)
	if (size_difference > 0) {
		// It should typically be the case that only one image is removed
		// But in the event that the image was duplicated in the list, we
		// wish to remove the correct amount from the histogram
		this->set_histogram(this->get_histogram() - (image->get_histogram() * size_difference));
	}*/
}

// Calculates the icon
void ImageClass::calculate_icon() {
	if (m_images.size() == 0)
		return;

	// Get the first image from the list to determine what features this set of images has
	Image* ex_img = m_images.front();

	int dims = 0;
	
	if (ex_img->has_flag(Property::PCA_SURF)) {
		dims = ex_img->get_PCA_descriptors().cols;
	}
	else if (ex_img->has_flag(Property::SURF)) {
		dims = ex_img->get_descriptors().cols;
	}
	else if (ex_img->has_flag(Property::Histogram)) {
		dims = ex_img->get_histogram().rows;
	}
	else {
		std::cout << "Error: Attempting to calculate icon on image set with no features" << std::endl;
		return;
	}

	// Find min and max out of all images, for each dimension
	cv::Mat min = cv::Mat(1, dims, CV_32F);
	cv::Mat max = cv::Mat(1, dims, CV_32F);
	// Holds the center point of each dimension
	cv::Mat centers = cv::Mat(1, dims, CV_32F);

	Image* center_image = 0;
	float closest_distance = numeric_limits<float>::max();

	// Initialise min and max arrays
	for (int i = 0; i < dims; i++) {
		min.at<float>(i) = numeric_limits<float>::max();
		max.at<float>(i) = numeric_limits<float>::lowest();
	}

	cv::Mat data = cv::Mat(0, dims, CV_32F);

	// Generate a large matrix, where each row represents an observation
	// with `dims` number of columns
	for (Image* image : m_images) {
		if (ex_img->has_flag(Property::PCA_SURF)) {
			data.push_back(image->get_PCA_descriptors());
		}
		else if (ex_img->has_flag(Property::SURF)) {
			data.push_back(image->get_descriptors());
		}
		else if (ex_img->has_flag(Property::Histogram)) {
			cv::Mat hist_copy = image->get_histogram().clone();
			cv::Mat hist_t = hist_copy.t();
			data.push_back(hist_t);

			//hist_copy.release();
		}
	}

	for (int i = 0; i < dims; i++) {
		double min_val = 0;
		double max_val = 0;

		cv::minMaxIdx(data.col(i), &min_val, &max_val, 0, 0);
		min.at<float>(i) = min_val;
		max.at<float>(i) = max_val;
	}

	// Now calculate the center point of each dimension
	for (int i = 0; i < dims; i++) {
		centers.at<float>(i) = (max.at<float>(i) + min.at<float>(i)) / 2;
	}

	// Create a node which represents the center of this cluster
	NodeProperties* center_node = new NodeProperties();

	if (ex_img->has_flag(Property::PCA_SURF)) {
		center_node->set_PCA_descriptors(centers);
	}
	else if (ex_img->has_flag(Property::SURF)) {
		center_node->set_descriptors(centers);
	}
	else if (ex_img->has_flag(Property::Histogram)) {
		center_node->set_histogram(centers.t());
	}

	// For each image, calculate the distance to the center point
	// And determine if it is the closest image to the center point
	for (Image* image : m_images) {
		float distance = image->calculate_distance(center_node);

		//cout << "Distance: " << distance << endl;
		if (distance < closest_distance) {
			closest_distance = distance;
			center_image = image;
		}
	}

	// Free up all allocated memory
	min.release();
	max.release();
	centers.release();
	delete center_node;

	// Store the image which is closest to the center point of each dimension
	m_icon = center_image;


	if (ex_img->has_flag(Property::PCA_SURF)) {
		this->set_PCA_descriptors(m_icon->get_PCA_descriptors());
	}
	else if (ex_img->has_flag(Property::SURF)) {
		this->set_descriptors(m_icon->get_descriptors());
	}
	else if (ex_img->has_flag(Property::Histogram)) {
		this->set_histogram(m_icon->get_histogram());
	}
}

Image* ImageClass::get_icon() {
	// If the icon hasn't been calculated yet
	if (m_icon == 0) {
		calculate_icon();
	}

	return m_icon;
}
