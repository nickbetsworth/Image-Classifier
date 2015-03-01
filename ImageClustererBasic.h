#pragma once
#include "ImageClusterer.h"
#include <map>

/**
 * @class	ImageClustererBasic
 *
 * @brief	A basic image classifier which classifies images using file names.
 * 			This classifier will group images through the use of their filename.
 * 			For example: if we have 3 files named "Bike1.jpg", "Bike2.jpg", "Car1.jpg"
 * 			"Bike1.jpg" and "Bike2.jpg" will form one class, and "Car1.jpg" will form another class.
 * 			It removes any digits, and uses the remaining string as a class identifier.
 * 			This class' main purpose is to test the user-interface in the development stages.
 *
 * @author	Nick
 * @date	26/10/2014
 */

class ImageClustererBasic :
	public ImageClusterer
{
public:
	ImageClustererBasic(vector<Image*> images);
	~ImageClustererBasic();

	// Classifies the existing set of images
	virtual void cluster_images();
protected:

	/**
	 * @fn	virtual ImageClass* ImageClustererBasic::get_image_class(Image* image);
	 *
	 * @brief	Works out an ImageClass for a given Image through the use of the Image's filepath.
	 *
	 * @author	Nick
	 * @date	26/10/2014
	 *
	 * @param	image	The image to be classified.
	 *
	 * @return	The ImageClass this Image belongs to,
	 */

	virtual ImageClass* get_image_class(Image* image);
private:
	/** @brief	Stores a mapping from filenames (Such as "bike.jpg", "car.jpg") to an ImageClass. */
	map<string, ImageClass*> m_file_to_class_map;
};

