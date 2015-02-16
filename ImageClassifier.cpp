#include "ImageClassifier.h"
#include <iostream>

ImageClassifier::ImageClassifier()
{
}
ImageClassifier::ImageClassifier(vector<Image*> images)
{
	m_images = images;
}


ImageClassifier::~ImageClassifier()
{
}

void ImageClassifier::add_image_class(ImageClass* image_class) {
	m_classes.push_back(image_class);
}

vector<ImageClass*> ImageClassifier::get_image_classes() {
	return m_classes;
}

int ImageClassifier::get_class_count() {
	return m_classes.size();
}

vector<Image*> ImageClassifier::get_images() {
	return m_images;
}

void ImageClassifier::add_image_to_class(Image* image, ImageClass* image_class) {
	// Create a mapping from key value image, to the specified image class
	m_class_map[image] = image_class;
	// Add the image to the list of images within the specified ImageClass
	image_class->add_image(image);
}