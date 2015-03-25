#include "ImageClustererKMeans.h"
#include "FeatureExtractor.h"
ImageClustererKMeans::ImageClustererKMeans(vector<Image*> images, int n_clusters) : ImageClusterer(images)
{
	m_n_clusters = n_clusters;
}


ImageClustererKMeans::~ImageClustererKMeans()
{
}

void ImageClustererKMeans::cluster_images() {
	cv::BOWKMeansTrainer trainer = cv::BOWKMeansTrainer(m_n_clusters);

	for (Image* image : get_images()) {
		trainer.add(image->get_descriptors());
	}
	
	cv::Mat vocabulary = trainer.cluster();
	std::cout << "Vocab: " << vocabulary << std::endl;
	cv::BOWImgDescriptorExtractor extractor(FeatureExtractor::get_extractor(), FeatureExtractor::get_matcher());
	extractor.setVocabulary(vocabulary);

	vector<ImageClass*> image_classes;

	// Create n clusters
	for (int i = 0; i < m_n_clusters; i++) {
		ImageClass* image_class = new ImageClass();
		image_classes.push_back(image_class);
	}

	for (Image* image : get_images()) {
		cv::Mat bow_descriptor;
		//cv::Mat descriptors;
		extractor.compute(image->get_fullres_image(), image->get_key_points(), bow_descriptor);

		// Find the cluster with the highest frequency
		int maxID[2];
		cv::minMaxIdx(bow_descriptor, 0, 0, 0, maxID);

		// Index we are interested in is maxID[1] and maxID[0] is always 0 (row)
		int max_index = maxID[1];
		image_classes[max_index]->add_image(image);

		//std::cout << "DESC: " << descriptors << std::endl;
		//std::cout << "Max ID: " << maxID[0] << ", " << maxID[1] << std::endl;
	}

	// Add all clusters that have at least one image
	for (ImageClass* image_class : image_classes) {
		if (image_class->get_image_count() > 0)
			this->create_cluster(image_class);
		else
			delete image_class;
	}
}