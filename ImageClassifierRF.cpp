#include "ImageClassifierRF.h"


ImageClassifierRF::ImageClassifierRF() : ImageClassifier()
{
	m_num_classes = 0;
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
}

ImageClassifierRF::~ImageClassifierRF()
{
}

void ImageClassifierRF::train(const vector<ImageClass*> training_data) {
	assert(training_data.size() > 0);

	m_num_classes = training_data.size();

	// Calculate the total number of images we have
	int num_images = 0;
	for (ImageClass* image_class : training_data) 
		num_images += image_class->get_images().size();

	// Construct the training data from the Image Classes
	// Example feature
	Feature* ex_fv = training_data.front()->get_images().front()->get_feature();
	int feature_vec_cols = ex_fv->get_feature_vector().cols;

	cv::Mat train_data = cv::Mat(0, feature_vec_cols, CV_32FC1);
	cv::Mat responses = cv::Mat(0, 1, CV_32SC1);

	int current_class = 0;
	for (ImageClass* image_class : training_data) {
		m_label_to_class[current_class] = image_class;

		for (Image* img : image_class->get_images()) {
			cv::Mat fv = img->get_feature()->get_feature_vector();
			train_data.push_back(fv);
			cv::Mat response = cv::Mat(fv.rows, 1, CV_32SC1, cv::Scalar(current_class));
			responses.push_back(response);
		}

		current_class++;
	}

	m_rt->train(train_data, CV_ROW_SAMPLE, responses, cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(), *m_rt_params);

	this->set_trained_state(true);
}

ImageClass* ImageClassifierRF::predict(const Image* image) {
	// Create a histogram of predictions (one for each descriptor)
	cv::Mat pred_hist = cv::Mat::zeros(1, m_num_classes, CV_32SC1);
	if (this->has_trained()) {
		// Loop through each row of the feature vector and predict which class it belongs to
		cv::Mat fv = image->get_feature()->get_feature_vector();
		for (int i = 0; i < fv.rows; i++) {
			cv::Mat descriptor = fv.row(i);
			int label = m_rt->predict(descriptor);
			pred_hist.at<long>(label)++;
		}
		
		
		//cout << "Label: " << label << endl;
		// Find the bin with highest frequency
		int max_bin = 0;
		for (int i = 1; i < pred_hist.cols; i++) {
			if (pred_hist.at<long>(i) > pred_hist.at<long>(max_bin)) {
				max_bin = i;
			}
		}

		return m_label_to_class[max_bin];
	}
	else {
		return 0;
	}
}