#pragma once
#include "Image.h"
#include "ImageClass.h"
#include "FeatureManager.h"
#include <vector>

/**
 * @class	ImageClusterer
 *
 * @brief	This class is in charge of clustering together an arbitrarily sized set of Features.
 * 			Each cluster is represented by an ImageClass.
 */

class ImageClusterer
{
public:
	ImageClusterer(vector<Image*> images, FeatureType type);
	virtual ~ImageClusterer();

	/**
	 * @fn	int ImageClusterer::get_cluster_count();
	 *
	 * @brief	Gets the number of clusters that have been generated.
	 *
	 * @return	The number of image clusters.
	 */

	int get_cluster_count();

	/**
	 * @fn	const vector<ImageClass*>& ImageClusterer::get_clusters() const;
	 *
	 * @brief	Gets the clusters that have been generated.
	 *
	 * @return	The clusters that have been generated.
	 */

	const vector<ImageClass*>& get_clusters() const;

	/**
	 * @fn	const vector<Image*>& ImageClusterer::get_images() const;
	 *
	 * @brief	Gets the images that will form the clusters.
	 *
	 * @return	The images that will form the clusters.
	 */

	const vector<Image*>& get_images() const;

	/**
	 * @fn	virtual void ImageClusterer::cluster_images() = 0;
	 *
	 * @brief	This method is in charge of performing the clustering of the images.
	 */

	virtual void cluster_images() = 0;

protected:

	/**
	 * @fn	void ImageClusterer::create_cluster(ImageClass* cluster);
	 *
	 * @brief	Creates a new cluster.
	 *
	 * @param [in,out]	cluster	The new cluster to be added.
	 */

	void create_cluster(ImageClass* cluster);

	/**
	 * @fn	void ImageClusterer::add_image_to_cluster(Image* image, ImageClass* cluster);
	 *
	 * @brief	Adds an image to the specified cluster.
	 *
	 * @param [in,out]	image  	The image to be added.
	 * @param [in,out]	cluster	The cluster to add the image to.
	 */

	void add_image_to_cluster(Image* image, ImageClass* cluster);

	/**
	 * @fn	FeatureType ImageClusterer::get_feature_type()
	 *
	 * @brief	Get the type of feature we are clustering by.
	 *
	 * @return	The feature type.
	 */

	FeatureType get_feature_type() { return m_type; };
private:
	/** @brief	The images to be clustered. */
	vector<Image*> m_images;
	/** @brief	The list of produced clusters. */
	vector<ImageClass*> m_clusters;
	/** @brief	The feature type by which we are clustering. */
	FeatureType m_type;
};

