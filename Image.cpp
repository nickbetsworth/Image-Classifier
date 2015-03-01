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
	generate_thumbnail();
}


Image::~Image()
{
}

Mat Image::get_fullres_image() const {
	Mat fullres_image = imread(m_filepath);
	return fullres_image;
}

void Image::generate_thumbnail() {
	// First we need to convert the image in to a square
	// To do this we will crop the data in to a square
	
	Rect crop_sq;
	// Work out whether we need to crop the rows or columns
	// In this case we need to remove rows
	if (m_image_data.rows > m_image_data.cols) {
		int size = m_image_data.cols;
		int extra = m_image_data.rows - size;
		int x = 0;
		int y = extra / 2;
		crop_sq = Rect(x, y, size, size);
	}
	// In this case we need to remove columns
	// This also catches the case that it is already a square
	else {
		int size = m_image_data.rows;
		int extra = m_image_data.cols - size;
		int x = extra / 2;
		int y = 0;
		crop_sq = Rect(x, y, size, size);
	}
	m_image_data = m_image_data(crop_sq);

	resize(m_image_data, m_image_data, Size(THUMB_WIDTH, THUMB_HEIGHT));
}