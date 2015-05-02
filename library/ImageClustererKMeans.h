#pragma once
#include "ImageClusterer.h"
#include <opencv/cv.h>

/**
 * @class	ImageClustererKMeans
 *
 * @brief	Clusters a set of Features with k-means algorithm.
 * 			This is best suited to sets of local feature descriptors - fast implementation.
 */

class ImageClustererKMeans :
	public ImageClusterer
{
public:
	ImageClustererKMeans(vector<Image*> images, int n_clusters, FeatureType type);
	virtual ~ImageClustererKMeans();

	virtual void cluster_images();
private:
	/** @brief	How many clusters. */
	int m_n_clusters;
};

