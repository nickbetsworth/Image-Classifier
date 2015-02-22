#pragma once
#include "Image.h"
#include "NodeProperties.h"
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
	 * @author	Nick
	 * @date	25/10/2014
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
	 * @author	Nick
	 * @date	25/10/2014
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
	 * @author	Nick
	 * @date	30/10/2014
	 *
	 * @return	A list of images that belong to this class.
	 */

	vector<Image*>& get_images() { return m_images; };

	
	Image* get_icon();
	vector<Image*> get_nearest_neighbours(int n);
private:
	/** @brief	Stores all images belonging to this class. */
	vector<Image*> m_images;
	Image* m_icon;

	void calculate_icon();
};

