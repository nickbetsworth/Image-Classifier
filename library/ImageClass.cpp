#include "ImageClass.h"
#include "GraphBOW.h"
#include "FeatureManager.h"

ImageClass::ImageClass(vector<Image*> images, FeatureType type)
{
	m_type = type;
	m_icon = 0;

	// Determine which type of graph we will use, with respect to the type of feature used
	// (Local feature descriptors are better compared with BOW Graph)
	switch (type) {
	case FeatureType::COLOUR_HISTOGRAM:
		m_graph = new Graph();
		break;
	case FeatureType::LOCAL_FEATURE:
		// Convert the array of images to array of Nodes
		// So we can pass it to the Graph
		std::vector<Node> nodes;
		for (Image* image : images) {
			nodes.push_back(image->get_feature());
		}
		m_graph = new GraphBOW(nodes);
		break;
	}

	// Add all images to the graph
	for (Image* image : images) {
		add_image(image);
	}
}

ImageClass::~ImageClass()
{
	delete m_graph;

	// Release all images
	for (Image* image : m_images) {
		delete image;
	}
}

bool ImageClass::add_image(Image* image)
{
	// Only add successfully loaded images to the image list
	if (image->has_loaded()) {
		m_graph->add_node(image->get_feature());
		m_images.push_back(image);
		return true;
	}
	else {
		cout << "Attempted to add non-loaded image to a class. Filepath: " << image->get_filepath() << endl;
		return false;
	}
}

void ImageClass::remove_image(Image* image) {
	m_graph->remove_node(image->get_feature());
	m_images.erase(remove(m_images.begin(), m_images.end(), image), m_images.end());
}

void ImageClass::calculate_icon() {
	if (m_images.size() == 0)
		return;

	// Get the first image from the list to determine what features this set of images has
	Image* ex_img = m_images.front();

	int dims = ex_img->get_feature()->get_feature_vector().cols;

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
		data.push_back(image->get_feature()->get_feature_vector());
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
	Feature* center_node = FeatureManager::create_feature(centers, m_type);

	// For each image, calculate the distance to the center point
	// And determine if it is the closest image to the center point
	for (Image* image : m_images) {
		float distance = image->get_feature()->calculate_distance(center_node);

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
}

Image* ImageClass::get_icon() {
	// If the icon hasn't been calculated yet
	if (m_icon == 0) {
		calculate_icon();
	}

	return m_icon;
}
