#include "ClassifierMananger.h"

ClassifierMananger::ClassifierMananger(vector<string> file_paths)
{
	m_file_paths = file_paths;
	m_classifier = new ImageClassifierRF();
}


ClassifierMananger::~ClassifierMananger()
{
}

void ClassifierMananger::load_images() {
	cout << "Loading " << m_file_paths.size() << " images." << endl;
	for (string file_path : m_file_paths) {
		Image* image = new Image(file_path);
		if (image->has_loaded())
			m_images.push_back(image);
		else
			cout << "Unable to load image: " << file_path << endl;
	}
	
}

void ClassifierMananger::cluster_images(int n_clusters) {
	cout << "Using GMM with " << n_clusters << " clusters." << endl;
	ImageClassifier* classifier = new ImageClassifierGMM(m_images, n_clusters);
	classifier->classify_images();
	m_image_classes = classifier->get_image_classes();
}

void ClassifierMananger::train_classifier() {
	cout << "Training classifier" << endl;
	m_classifier->train(m_image_classes);
}

void ClassifierMananger::classify_images(vector<string> file_paths) {

}

void ClassifierMananger::remove_class(ImageClass* image_class) {
	m_image_classes.erase(	remove(
								m_image_classes.begin(), 
								m_image_classes.end(), 
								image_class),
							m_image_classes.end());
}