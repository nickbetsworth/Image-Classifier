#include "ImageClusterer.h"


ImageClusterer::ImageClusterer(vector<Image*> images, FeatureType type)
{
	m_images = images;
	m_type = type;
}


ImageClusterer::~ImageClusterer()
{
}

int ImageClusterer::get_cluster_count() {
	return m_clusters.size();
}

const vector<ImageClass*>& ImageClusterer::get_clusters() const {
	return m_clusters;
}

const vector<Image*>& ImageClusterer::get_images() const {
	return m_images;
}

void ImageClusterer::create_cluster(ImageClass* cluster) {
	m_clusters.push_back(cluster);
}

void ImageClusterer::add_image_to_cluster(Image* image, ImageClass* cluster) {
	cluster->add_image(image);
}