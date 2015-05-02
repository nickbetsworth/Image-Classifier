#include "ImageClustererGMM.h"
#include <opencv\cv.h>
#include "Histograms.h"
#include "FeatureManager.h"

ImageClustererGMM::ImageClustererGMM(vector<Image*> images, int n_clusters, FeatureType type) : ImageClusterer(images, type)
{
	m_em = new cv::EM(n_clusters);
	m_n_clusters = n_clusters;
}


ImageClustererGMM::~ImageClustererGMM()
{
	delete m_em;
}

void ImageClustererGMM::cluster_images() {
	Feature* ex_fv = get_images().front()->get_feature();
	int num_cols = ex_fv->get_feature_vector().cols;
	int img_count = get_images().size();
	int current_img = 0;

	cv::Mat samples = cv::Mat::zeros(0, num_cols, CV_32F);
	cv::Mat sampleIDToImg = cv::Mat(0, 1, CV_32SC1);

	for (Image* image : this->get_images()) {
		cv::Mat fv = image->get_feature()->get_feature_vector();
		samples.push_back(fv);

		cv::Mat response = cv::Mat(fv.rows, 1, CV_32SC1, cv::Scalar(current_img));

		sampleIDToImg.push_back(response);
		// Copy this row to the current_rowTH row of the samples matrix
		current_img++;
	}

	cv::Mat likelihoods;
	cv::Mat labels;
	cv::Mat probs;

	m_em->train(samples, likelihoods, labels, probs);

	// Create a mapping from each available label [1..m_n_clusters] to a separate image class
	map<int, std::vector<Image*>> label_to_class_map;

	cv::Mat gmm_votes = cv::Mat::zeros(img_count, m_n_clusters, CV_32SC1);
	for (int i = 0; i < samples.rows; i++) {
		// Get the voting for this row
		int vote = labels.at<int>(i, 0);
		// Convert the label to an image number
		int image_no = sampleIDToImg.at<long>(i);
		gmm_votes.at<long>(image_no, vote)++;
	}

	// Find the max bin for each image
	for (int i = 0; i < img_count; i++) {
		cv::Mat row = gmm_votes.row(i);

		// Find the cluster with the highest frequency
		int maxID[2];
		cv::minMaxIdx(row, 0, 0, 0, maxID);

		// Index we are interested in is maxID[1] and maxID[0] is always 0 (row)
		int max_index = maxID[1];

		int label = labels.at<int>(i, 0);
		label_to_class_map[max_index].push_back(get_images().at(i));
	}

	for (int i = 0; i < m_n_clusters; i++) {
		if (label_to_class_map[i].size() > 0) {
			ImageClass* image_class = new ImageClass(label_to_class_map[i], get_feature_type());
			this->create_cluster(image_class);
		}
	}

	//cout << "Likelihoods:" << likelihoods << endl;
	//cout << "Labels:" << labels << endl;
	//cout << "Probs:" << probs << endl;
}