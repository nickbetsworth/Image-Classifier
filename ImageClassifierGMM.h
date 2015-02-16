#pragma once
#include "ImageClassifier.h"

#include <opencv\ml.h>

class ImageClassifierGMM :
	public ImageClassifier
{
public:
	ImageClassifierGMM(vector<Image*> images, int n_clusters);
	~ImageClassifierGMM();
	virtual void classify_images();

private:
	EM* m_em;
	int m_n_clusters;
};

