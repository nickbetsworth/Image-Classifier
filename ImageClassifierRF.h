#pragma once
#include "ImageClassifier.h"
#include <opencv\ml.h>

class ImageClassifierRF :
	public ImageClassifier
{
public:
	ImageClassifierRF();
	~ImageClassifierRF();

	void train(vector<ImageClass*> training_data);
	virtual void classify_images();
	ImageClass* predict(Image* image);
	bool has_trained() { return m_has_trained; };
protected:
	const int MAX_DEPTH = 7;
	const int MIN_SAMPLE_COUNT = 2;
	const float REGRESSION_ACCURACY = 0.0f;
	const bool USE_SURROGATES = false;
	const int MAX_CATEGORIES = 16;
	const bool CALC_VAR_IMPORTANCE = false;
	const int NACTIVE_VARS = 0;
	const int MAX_TREES = 100;
	const float FOREST_ACCURACY = 0.0f;
	const int TERMCRIT_TYPE = CV_TERMCRIT_ITER;

private:
	vector<ImageClass*> m_training_data;

	CvRTrees* m_rt;
	CvRTParams* m_rt_params;
	bool m_has_trained;
	map<int, ImageClass*> m_label_to_class;
};

