#include "ImageClassifierGMM.h"
#include <opencv\cv.h>
#include "Histograms.h"

ImageClassifierGMM::ImageClassifierGMM(vector<Image*> images, int n_clusters) : ImageClassifier(images)
{
	m_em = new EM(n_clusters);
	m_n_clusters = n_clusters;
}


ImageClassifierGMM::~ImageClassifierGMM()
{
	delete m_em;
}

void ImageClassifierGMM::classify_images() {
	int img_count = get_images().size();
	int current_row = 0;
	const int bins = Image::HIST_BINS;
	const int channels = Image::NUM_CHANNELS;
	Mat samples = Mat::zeros(img_count, bins * channels, CV_32F);

	for (Image* image : this->get_images()) {
		Mat hist = image->get_histogram();
		Mat hist_t = hist.t();
		//cout << hist_t << endl;
		// Copy this row to the current_rowTH row of the samples matrix
		hist_t.copyTo(samples.row(current_row));
		hist.release();
		current_row++;

		/*if (image->get_filepath().find("Car6") != string::npos) {
			cout << "Filename: " << image->get_filepath() << endl;
			cout << "Hist_t:" << hist_t << endl;
		}*/
	}
	
	Mat likelihoods;
	Mat labels;
	Mat probs;

	m_em->train(samples, likelihoods, labels, probs);
	
	map<int, ImageClass*> label_to_class_map;

	// Create a mapping from each available label [1..m_n_clusters] to a separate image class
	for (int i = 0; i < m_n_clusters; i++) {
		label_to_class_map[i] = new ImageClass();
	}
	for (int i = 0; i < img_count; i++) {
		int label = labels.at<int>(i, 0);
		ImageClass* image_class = label_to_class_map[label];
		
		/*Image* img = get_images().at(i);
		if (img->get_filepath().find("Car6") != string::npos) {
			cout << img->get_filepath() << " label " << label << endl;
		}*/
		

		image_class->add_image(get_images().at(i));
	}

	for (int i = 0; i < m_n_clusters; i++) {
		ImageClass* image_class = label_to_class_map[i];
		if (image_class->get_images().size() > 0)
			this->add_image_class(image_class);
	}
	//cout << "Likelihoods:" << likelihoods << endl;
	//cout << "Labels:" << labels << endl;
	//cout << "Probs:" << probs << endl;
}