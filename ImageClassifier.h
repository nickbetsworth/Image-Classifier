#pragma once
#include "Image.h"
#include "ImageClass.h"
#include <map>
#include <vector>

/**
 * @class	ImageClassifier
 *
 * @brief	Classifies new images in to an existing class.
 *
 * @author	Nick
 * @date	26/10/2014
 */
using namespace std;
class ImageClassifier
{
public:
	
	/**
	 * @fn	ImageClassifier::ImageClassifier();
	 *
	 * @brief	Creates a new instance of ImageClassifier
	 *
	 * @author	Nick
	 * @date	26/10/2014
	 */
	ImageClassifier(vector<Image*> images);
	ImageClassifier();
	virtual ~ImageClassifier();

	/**
	 * @fn	int ImageClassifier::get_class_count();
	 *
	 * @brief	Returns how many classes belong to this image classifier.
	 *
	 * @author	Nick
	 * @date	26/10/2014
	 *
	 * @return	The number of separate classes.
	 */
	int get_class_count();

	/**
	 * @fn	vector<ImageClass*> ImageClassifier::get_image_classes();
	 *
	 * @brief	Retrieves a list of existing ImageClasses.
	 *
	 * @author	Nick
	 * @date	28/10/2014
	 *
	 * @return	A list of existing ImageClasses.
	 */

	vector<ImageClass*> get_image_classes();

	virtual void classify_images() = 0;

	vector<Image*> get_images();
protected:

	/**
	 * @fn	void ImageClassifier::add_image_class(ImageClass* image_class);
	 *
	 * @brief	Adds a new ImageClass to be considered.
	 *
	 * @author	Nick
	 * @date	28/10/2014
	 *
	 * @param image_class	The new ImageClass.
	 */

	void add_image_class(ImageClass* image_class);

	/**
	* @fn	void ImageClassifier::add_image_to_class(Image* image, ImageClass* image_class);
	*
	* @brief	Adds the provided image to the specified ImageClass.
	*
	* @author	Nick
	* @date	26/10/2014
	*
	* @param image	   		The image to be added.
	* @param image_class	The class the image is to be added to.
	*/

	void add_image_to_class(Image* image, ImageClass* image_class);

	/**
	 * @fn	map<Image*, ImageClass*> ImageClassifier::get_class_map()
	 *
	 * @brief	Returns a mapping from Images to the respective ImageClass it belongs to.
	 *
	 * @author	Nick
	 * @date	28/10/2014
	 *
	 * @return	An Image to ImageClass map.
	 */

	map<Image*, ImageClass*> get_class_map() { return m_class_map; };
private:
	vector<Image*> m_images;
	/** @brief	Stores a list of ImageClasses */
	vector<ImageClass*> m_classes;
	/** @brief	Stores a mapping from Image to ImageClass */
	map<Image*, ImageClass*> m_class_map;
	
};

