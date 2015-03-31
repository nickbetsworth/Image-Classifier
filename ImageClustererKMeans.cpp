#include "ImageClustererKMeans.h"
#include "FeatureExtractor.h"
#include "BOWExtractor.h"
#include <opencv\ml.h>

ImageClustererKMeans::ImageClustererKMeans(vector<Image*> images, int n_clusters) : ImageClusterer(images)
{
	m_n_clusters = n_clusters;
}


ImageClustererKMeans::~ImageClustererKMeans()
{
}

void ImageClustererKMeans::cluster_images() {
	Image* example_image = get_images().front();
	cv::Mat data = cv::Mat(0, example_image->get_descriptors().cols, example_image->get_descriptors().type());

	int n = 0;
	int size = get_images().size();
	for (Image* image : get_images()) {
		data.push_back(image->get_descriptors());
		std::cout << "Added image " << n++ << " of " << size << " rows: " << data.rows << std::endl;
	}

	cv::Mat labels;
	cv::TermCriteria term_crit = cv::TermCriteria();
	cv::Mat vocabulary;

	std::cout << "Val: " << cv::kmeans(data, m_n_clusters, labels, term_crit, 3, cv::KMEANS_PP_CENTERS, vocabulary);
	//cv::Mat vocabulary = trainer.cluster();
	std::cout << "Vocab: " << vocabulary << std::endl;

	BOWExtractor extractor = BOWExtractor(vocabulary);
	

	vector<vector<Image*>> image_classes;

	// Create n clusters
	for (int i = 0; i < m_n_clusters; i++) {
		image_classes.push_back(std::vector<Image*>());
	}

	for (Image* image : get_images()) {
		cv::Mat bow_descriptor;
		extractor.get_BOW_hist(image->get_descriptors(), bow_descriptor);

		// Find the cluster with the highest frequency
		int maxID[2];
		cv::minMaxIdx(bow_descriptor, 0, 0, 0, maxID);

		// Index we are interested in is maxID[1] and maxID[0] is always 0 (row)
		int max_index = maxID[1];
		image_classes[max_index].push_back(image);
	}

	// Add all clusters that have at least one image
	for (std::vector<Image*> images : image_classes) {
		// Only add the cluster if it has at least one image
		if (images.size() > 0) {
			ImageClass* image_class = new ImageClass(images);
			this->create_cluster(image_class);
		}
	}
}