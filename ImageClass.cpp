#include "ImageClass.h"


ImageClass::ImageClass()
{
	// Initialise the property sheet histogram before adding to 
	Mat hist = Mat::zeros(Image::HIST_BINS * Image::NUM_CHANNELS, 1, CV_32F);
	this->set_histogram(hist);
	this->add_flag(Property::Histogram);
	m_icon = 0;

	m_graph = new NodePropertiesGraph();
}

ImageClass::ImageClass(vector<Image*> images) : ImageClass()
{
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
	int dims = Image::HIST_BINS * Image::NUM_CHANNELS;
	// Find min and max out of all images, for each dimension
	Mat min = Mat(Image::HIST_BINS * Image::NUM_CHANNELS, 1, CV_32F);
	Mat max = Mat(Image::HIST_BINS * Image::NUM_CHANNELS, 1, CV_32F);
	// Holds the center point of each dimension
	Mat centers = Mat(Image::HIST_BINS * Image::NUM_CHANNELS, 1, CV_32F);

	Image* center_image = 0;
	float closest_distance = numeric_limits<float>::max();

	// Initialise min and max arrays
	for (int i = 0; i < dims; i++) {
		min.at<float>(i) = numeric_limits<float>::max();
		max.at<float>(i) = numeric_limits<float>::lowest();
	}

	for (Image* image : m_images) {
		Mat hist = image->get_histogram();
		for (int i = 0; i < dims; i++) {
			float val = hist.at<float>(i);

			if (val > max.at<float>(i))
				max.at<float>(i) = val;
			if (val < min.at<float>(i))
				min.at<float>(i) = val;
		}
	}

	// Now calculate the center point of each dimension
	for (int i = 0; i < dims; i++) {
		centers.at<float>(i) = (max.at<float>(i) + min.at<float>(i)) / 2;
	}

	// Create a node which represents the center of this cluster
	NodeProperties* center_node = new NodeProperties();
	center_node->set_histogram(centers);
	// For each image, calculate the distance to the center point
	// And determine if it is the closest image to the center point
	for (Image* image : m_images) {
		Mat hist = image->get_histogram();
		float distance = center_node->calculate_distance(image);

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
	this->set_histogram(m_icon->get_histogram());
}

Image* ImageClass::get_icon() {
	// If the icon hasn't been calculated yet
	if (m_icon == 0) {
		calculate_icon();
	}

	return m_icon;
}
