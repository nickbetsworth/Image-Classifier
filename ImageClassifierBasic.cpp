#include "ImageClassifierBasic.h"
#include <string>

ImageClassifierBasic::ImageClassifierBasic(vector<Image*> images) : ImageClassifier(images)
{
}


ImageClassifierBasic::~ImageClassifierBasic()
{
}

void ImageClassifierBasic::classify_images() {
	for (Image* image : this->get_images()) {
		ImageClass* image_class = get_image_class(image);
		// If the class we have been given is non-NULL
		if (image_class != 0) {
			// Add the image to the relevant class
			add_image_to_class(image, image_class);
		}
		else {
			cout << "Unable to classify image: " << image->get_filepath() << endl;
		}
	}
}

ImageClass* ImageClassifierBasic::get_image_class(Image* image) {
	string filepath = image->get_filepath();
	// Work out the position of the last back-slash
	size_t pos = filepath.find_last_of('\\') + 1;
	// Get all text after the last back-slash, which should leave us with the filename and extension
	string filename = filepath.substr(pos);
	// Remove any digits from the remaining string
	auto it = remove_if(filename.begin(), filename.end(), [](char c) { return isdigit(c); });
	filename.erase(it, filename.end());

	// Obtain the current ImageClass for this filename, if any
	ImageClass* image_class = m_file_to_class_map[filename];

	// If there is no existing ImageClass for this filename
	if (image_class == 0) {
		//cout << "No existing image class was found, creating new class" << endl;
		// Create a new class and add it to the map
		image_class = new ImageClass();
		m_file_to_class_map[filename] = image_class;
		// Add the new class to the class list
		add_image_class(image_class);
	}

	return image_class;
}

