#include "ClassifierManager.h"
#include "ImageClustererKMeans.h"
ClassifierManager::ClassifierManager()
{
	m_classifier = new ImageClassifierRF();
}


ClassifierManager::~ClassifierManager()
{
}

Image* ClassifierManager::load_image(string file_path) {
	Image* image = new Image(file_path);
	if (image->has_loaded()) {
		return image;
	}
	else {
		delete image;
		cout << "Error: Unable to load image from: " << file_path << endl;
		return 0;
	}
}

void ClassifierManager::load_images(vector<string> file_paths) {
	cout << "Loading " << file_paths.size() << " images." << endl;
	for (string file_path : file_paths) {
		Image* loaded_image = this->load_image(file_path);
		if (loaded_image != 0) {
			m_images.push_back(loaded_image);
		}
	}
	
}

void ClassifierManager::cluster_images(int n_clusters) {
	// Ensure that images have been loaded in
	if (m_images.size() == 0) {
		cout << "Error: call to cluster_images with no images loaded in.." << endl;
		cout << "Exiting function.." << endl;
		return;
	}

	cout << "Using GMM with " << n_clusters << " clusters." << endl;
	ImageClusterer* clusterer = new ImageClustererKMeans(m_images, n_clusters);
	clusterer->cluster_images();
	m_image_classes = clusterer->get_clusters();
}

void ClassifierManager::train_classifier() {
	// Ensure that we have at least one image class
	if (m_image_classes.size() == 0) {
		cout << "Error: call to train_classifier with no existing data" << endl;
		cout << "Exiting function.." << endl;
		return;
	}

	// If the classifier has already trained, create a new instance of it before training
	if (m_classifier->has_trained()) {
		delete m_classifier;
		m_classifier = new ImageClassifierRF();
	}

	cout << "Training classifier" << endl;
	m_classifier->train(m_image_classes);
}

ImageClass* ClassifierManager::classify_image(Image* image) {
	// Ensure that we have been given a valid image
	if (image == 0) {
		cout << "Error: call to classify_image; null image provided" << endl;
		cout << "Exiting function.." << endl;
		return 0;
	}

	ImageClass* predicted_class = m_classifier->predict(image);

	// If the prediction was successful, add the image to the class
	if (predicted_class != 0)
		predicted_class->add_image(image);

	return predicted_class;
}

void ClassifierManager::remove_class(ImageClass* image_class) {
	m_image_classes.erase(	remove(
								m_image_classes.begin(), 
								m_image_classes.end(), 
								image_class),
							m_image_classes.end());
}