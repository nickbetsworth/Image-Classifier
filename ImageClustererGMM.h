#pragma once
#include "ImageClusterer.h"

#include <opencv\ml.h>

class ImageClustererGMM :
	public ImageClusterer
{
public:
	ImageClustererGMM(vector<Image*> images, int n_clusters);
	~ImageClustererGMM();
	virtual void cluster_images();

private:
	EM* m_em;
	int m_n_clusters;
};

