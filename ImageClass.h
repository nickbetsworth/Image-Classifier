#pragma once
#include "Image.h"
#include "NodeProperties.h"
#include "NodePropertiesGraph.h"
#include <opencv\cv.h>
#include <vector>
/**
 * @class	ImageClass
 *
 * @brief	Represents a class of images.
 * 			Stores an array of images belonging to this particular class.
 * 			Allows the addition of new images to the class.
 *
 * @author	Nick
 * @date	25/10/2014
 */

class ImageClass : public NodeProperties
{
public:

	/**
	 * @fn	ImageClass::ImageClass();
	 *
	 * @brief	Generates a new empty image class.
	 *
	 * @author	Nick
	 * @date	25/10/2014
	 */
	ImageClass();

	/**
	 * @fn	ImageClass::ImageClass(vector<Image> images);
	 *
	 * @brief	Creates a new class from a set of existing images.
	 *
	 * @param	images	A list of images belonging to this class
	 */
	ImageClass(vector<Image*> images);
	~ImageClass();

	/**
	 * @fn	bool ImageClass::add_image(Image image);
	 *
	 * @brief	Adds a new image to the class.
	 *
	 * @param	image	The image to be added to the class.
	 *
	 * @return	true if the image is added, false if it fails.
	 */
	bool add_image(Image* image);

	void remove_image(Image* image);

	/**
	 * @fn	vector<Image*> ImageClass::get_images()
	 *
	 * @brief	Returns a list of all images that belong to this class.
	 *
	 * @return	A list of images that belong to this class.
	 */

	vector<Image*>& get_images() { return m_images; };
	int get_image_count() { return m_images.size(); };

	NodePropertiesGraph* get_graph() { return m_graph; };
	
	Image* get_icon();
	void calculate_icon();
private:
	/** @brief	Stores all images belonging to this class. */
	vector<Image*> m_images;
	Image* m_icon;

	// Manages the adjacency matrix between nodes of this class
	NodePropertiesGraph* m_graph;

	
};

