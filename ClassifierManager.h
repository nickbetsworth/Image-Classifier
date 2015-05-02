#pragma once
#include "Image.h"
#include "ImageClustererGMM.h"
#include "ImageClassifierRF.h"
#include "FeatureManager.h"
#include <vector>

using namespace std;
/**
 * @class	ClassifierManager
 *
 * @brief	ClassifierManager ties together all components of the image
 * 			clustering and classification processes.
 * 			The methods should be called in the following order:
 * 				load_images
 * 				cluster_images
 * 				train_classifier
 * 			
 * 			New data may be added/classified via classify_image
 */

class ClassifierManager
{
public:
	ClassifierManager(FeatureType type);
	virtual ~ClassifierManager();

	/**
	 * @fn	void ClassifierManager::load_images(vector<string> file_paths);
	 *
	 * @brief	Loads in the specified images.
	 *
	 * @param	file_paths	The file paths of the images.
	 */

	void load_images(vector<string> file_paths);

	/**
	 * @fn	void ClassifierManager::cluster_images(const int n_clusters);
	 *
	 * @brief	Clusters the images which have been loaded in.
	 *
	 * @param	n_clusters	The desired number of clusters.
	 */

	void cluster_images(const int n_clusters);

	/**
	 * @fn	void ClassifierManager::train_classifier();
	 *
	 * @brief	Trains the classifier using the existing clustered data.
	 */

	void train_classifier();

	/**
	 * @fn	ImageClass* ClassifierManager::classify_image(Image* image);
	 *
	 * @brief	Classify a new image. Upon classification, the image is added
	 * 			to the predicted class.
	 *
	 * @param [in,out]	image	The image to be classified.
	 *
	 * @return	0 if classification fails, else a pointer to the predicted class.
	 */

	ImageClass* classify_image(Image* image);

	/**
	 * @fn	void ClassifierManager::remove_class(ImageClass* image_class);
	 *
	 * @brief	Removes the class described by image_class.
	 *
	 * @param [in,out]	image_class	If non-null, the image class.
	 */

	void remove_class(ImageClass* image_class);

	vector<Image*>& get_images() { return m_images; };
	vector<ImageClass*>& get_image_classes() { return m_image_classes; };
	const vector<ImageClass*>& get_image_classes() const { return m_image_classes; };
	ImageClassifier* get_classifier() { return m_classifier; };
	FeatureType get_feature_type() { return m_feature_type; };
private:
	/**
	* @fn	Image* ClassifierManager::load_image(string file_path);
	*
	* @brief	Loads in the image from the specified file path.
	*
	* @param	file_path	Full pathname of the file.
	*
	* @return	0 if the loading fails, the image otherwise.
	*/

	Image* load_image(string file_path);

	vector<Image*> m_images;
	vector<ImageClass*> m_image_classes;
	ImageClassifier* m_classifier;
	FeatureType m_feature_type;
};

