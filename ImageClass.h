#pragma once
#include "Image.h"
#include "Feature.h"
#include "Graph.h"
#include "FeatureManager.h"
#include <opencv\cv.h>
#include <vector>

/**
 * @class	ImageClass
 *
 * @brief	Represents a class of images. 
 * 			Stores an array of images belonging to this particular
 * 			class. Allows the addition of new images to the class.
 *
 */

class ImageClass
{
public:

	/**
	 * @fn	ImageClass::ImageClass(vector<Image*> images, FeatureType type);
	 *
	 * @brief	Creates a new class from a set of existing images.
	 *
	 * @param [in,out]	images	A list of images belonging to this class.
	 * @param	type		  	The type of feature that each image uses.
	 */

	ImageClass(vector<Image*> images, FeatureType type);
	~ImageClass();

	/**
	 * @fn	bool ImageClass::add_image(Image* image);
	 *
	 * @brief	Adds a new image to the class.
	 *
	 * @param [in,out]	image	The image to be added to the class.
	 *
	 * @return	true if the image was not already within the class.
	 */

	bool add_image(Image* image);

	/**
	 * @fn	void ImageClass::remove_image(Image* image);
	 *
	 * @brief	Removes an image from the ImageClass.
	 *
	 * @param [in,out]	image	The image to be removed.
	 */

	void remove_image(Image* image);

	/**
	 * @fn	vector<Image*>& ImageClass::get_images()
	 *
	 * @brief	Returns a list of all images that belong to this class.
	 *
	 * @return	A list of images that belong to this class.
	 */

	vector<Image*>& get_images() { return m_images; };

	/**
	 * @fn	int ImageClass::get_image_count()
	 *
	 * @brief	Gets the number of images within this class.
	 *
	 * @return	The image count for this class.
	 */

	int get_image_count() { return m_images.size(); };

	/**
	 * @fn	Graph* ImageClass::get_graph()
	 *
	 * @brief	Gets the class' feature graph.
	 *
	 * @return	The class' feeature graph.
	 */

	Graph* get_graph() { return m_graph; };

	/**
	 * @fn	Image* ImageClass::get_icon();
	 *
	 * @brief	Gets the icon that represents this class.
	 * 			If the icon does not exist, it will be calculated
	 *
	 * @return	null if it fails, else the icon.
	 */

	Image* get_icon();

	/**
	 * @fn	void ImageClass::calculate_icon();
	 *
	 * @brief	Calculates the icon for this class.
	 * 			This is taken to be the image nearest to the center of this class' feature space.
	 */

	void calculate_icon();
private:
	/** @brief	Stores all images belonging to this class. */
	vector<Image*> m_images;
	/** @brief	Stores this class' icon. */
	Image* m_icon;

	/** @brief	Manages the adjacency matrix between nodes of this class. */
	Graph* m_graph;

	/** @brief	Stores the type of feature used for each image. */
	FeatureType m_type;
};

