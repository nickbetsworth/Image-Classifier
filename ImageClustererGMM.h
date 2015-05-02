#pragma once
#include "ImageClusterer.h"
#include <opencv\ml.h>

/**
 * @class	ImageClustererGMM
 *
 * @brief	Clusters Features via Gaussian Mixture Modelling.
 * 			If performance required, recommended use of ImageClustererKMeans instead.
 */

class ImageClustererGMM :
	public ImageClusterer
{
public:
	ImageClustererGMM(vector<Image*> images, int n_clusters, FeatureType type);
	~ImageClustererGMM();

	virtual void cluster_images();
private:
	cv::EM* m_em;
	int m_n_clusters;
};

