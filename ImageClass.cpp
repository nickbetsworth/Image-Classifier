#include "ImageClass.h"


ImageClass::ImageClass()
{
	// Initialise the property sheet histogram before adding to 
	Mat hist = Mat::zeros(Image::HIST_BINS * Image::NUM_CHANNELS, 1, CV_32F);
	this->set_histogram(hist);

	m_icon = 0;
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

void ImageClass::calculate_icon() {
	int dims = Image::HIST_BINS * Image::NUM_CHANNELS;
	// Find min and max out of all images, for each dimension
	float* min = new float[dims];
	float* max = new float[dims];
	// Holds the center point of each dimension
	float* centers = new float[dims];

	Image* center_image = 0;
	float closest_distance = numeric_limits<float>::max();

	// Initialise min and max arrays
	for (int i = 0; i < dims; i++) {
		min[i] = numeric_limits<float>::max();
		max[i] = numeric_limits<float>::lowest();
	}
	for (Image* image : m_images) {
		Mat hist = image->get_histogram();
		for (int i = 0; i < dims; i++) {
			float val = hist.at<float>(i);

			if (val > max[i])
				max[i] = val;
			if (val < min[i])
				min[i] = val;
		}
	}

	// Now calculate the center point of each dimension
	for (int i = 0; i < dims; i++) {
		centers[i] = (max[i] + min[i]) / 2;
	}

	// For each image, calculate the distance to the center point
	// And determine if it is the closest image to the center point
	for (Image* image : m_images) {
		Mat hist = image->get_histogram();
		float distance = 0;

		for (int i = 0; i < dims; i++) {
			distance += powf(centers[i] - hist.at<float>(i), 2);
		}

		//cout << "Distance: " << distance << endl;
		if (distance < closest_distance) {
			closest_distance = distance;
			center_image = image;
		}
	}

	//cout << "Closest distance squared found: " << closest_distance << endl;

	// Free up all allocated memory
	delete[] min;
	delete[] max;
	delete[] centers;

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