#include "ImageClassifierRF.h"


ImageClassifierRF::ImageClassifierRF()
{
	m_rt = new CvRTrees();
	m_rt_params = new CvRTParams(
		MAX_DEPTH,
		MIN_SAMPLE_COUNT,
		REGRESSION_ACCURACY,
		USE_SURROGATES,
		MAX_CATEGORIES,
		0, 
		CALC_VAR_IMPORTANCE,
		NACTIVE_VARS,
		MAX_TREES,
		FOREST_ACCURACY,
		TERMCRIT_TYPE);
	m_has_trained = false;
}

ImageClassifierRF::~ImageClassifierRF()
{
}

void ImageClassifierRF::classify_images() {

}

void ImageClassifierRF::train(vector<ImageClass*> training_data) {
	// Calculate the total number of images we have
	int num_images = 0;
	for (ImageClass* image_class : training_data) 
		num_images += image_class->get_images().size();

	//cout << "Training forest with " << num_images << " images" << endl;
	// Construct the training data from the Image Classes
	int feature_vec_length = Image::HIST_BINS * Image::NUM_CHANNELS;
	Mat trainData = Mat(num_images, feature_vec_length, CV_32FC1);
	Mat responses = Mat(num_images, 1, CV_32SC1);

	int current_class = 0;
	int current_row = 0;
	for (ImageClass* image_class : training_data) {
		m_label_to_class[current_class] = image_class;

		for (Image* img : image_class->get_images()) {
			// Returns histogram as column vector
			Mat hist = img->get_histogram();
			// Transpose to row vector
			Mat hist_t = hist.t();

			hist_t.copyTo(trainData.row(current_row));
			responses.at<int>(current_row, 0) = current_class;

			current_row++;
		}

		current_class++;
	}

	m_rt->train(trainData, CV_ROW_SAMPLE, responses, Mat(), Mat(), Mat(), Mat(), *m_rt_params);

	m_has_trained = true;
}

ImageClass* ImageClassifierRF::predict(Image* image) {
	if (m_has_trained) {
		Mat hist = image->get_histogram();
		Mat hist_t = hist.t();
		int label = m_rt->predict(hist_t);
		//cout << "Label: " << label << endl;

		return m_label_to_class[label];
	}
	else {
		return 0;
	}
}