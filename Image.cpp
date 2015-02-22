#include "Image.h"
#include <opencv\highgui.h>
#include "Histograms.h"

Image::Image(const string &filepath)
{
	m_filepath = filepath;
	// If we have been provided with an empty string
	if (filepath.length() == 0) {
		cout << "Error: Empty filename provided for image creation" << endl;
		return;
	}
	
	m_image_data = imread(filepath);

	// Do our histogram calculation before resizing
	this->set_histogram(get_1d_histogram(m_image_data, HIST_BINS));
	this->add_flag(Property::Histogram);
	// Resize our image to a thumbnail size
	resize(m_image_data, m_image_data, Size(THUMB_WIDTH, THUMB_HEIGHT));
}


Image::~Image()
{
}

Mat Image::get_fullres_image() const {
	Mat fullres_image = imread(m_filepath);
	return fullres_image;
}

