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
	 */
	ImageClassifier();
	virtual ~ImageClassifier();

	/**
	 * @fn	virtual void ImageClassifier::train(vector<ImageClass*> training_data) = 0;
	 *
	 * @brief	Trains the classifier using the given training data.
	 *
	 * @param [in,out]	training_data	The data to train the classifier with.
	 */

	virtual void train(const vector<ImageClass*> training_data) = 0;

	/**
	 * @fn	virtual ImageClass* ImageClassifier::predict(Image* image) = 0;
	 *
	 * @brief	Predicts which class an image belongs to.
	 *
	 * @param [in,out]	image	The image we are predicting the class for.
	 *
	 * @return	The class that the model has predicted the image belongs to.
	 */

	virtual ImageClass* predict(const Image* image) = 0;

	/**
	 * @fn	bool ImageClassifier::has_trained() const
	 *
	 * @brief	Query if this classifier has been trained yet.
	 *
	 * @return	true if trained, false if not.
	 */

	bool has_trained() const { return m_has_trained; };
protected:

	/**
	 * @fn	void ImageClassifier::set_trained_state(const bool has_trained)
	 *
	 * @brief	Sets whether or not the classifier has been trained.
	 *
	 */

	void set_trained_state(const bool has_trained) { m_has_trained = has_trained; };
private:
	/** @brief	Stores whether or not this classifier has been trained. */
	bool m_has_trained;
};

