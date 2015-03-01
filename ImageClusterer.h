#pragma once
#include "Image.h"
#include "ImageClass.h"

#include <vector>

/**
 * @class	ImageClusterer
 *
 * @brief	This class is in charge of clustering together an arbitrarily sized
 * 			set of images.
 * 			Each cluster is represented by an ImageClass
 */

class ImageClusterer
{
public:
	ImageClusterer(vector<Image*> images);
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
	 * @fn	vector<ImageClass*> ImageClusterer::get_clusters();
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
	 */

	void add_image_to_cluster(Image* image, ImageClass* cluster);
private:
	vector<Image*> m_images;
	vector<ImageClass*> m_clusters;
};

