#pragma once
#include "Image.h"
#include "ImageClassifierGMM.h"
#include "ImageClassifierRF.h"
#include <vector>

using namespace std;
/**
 * @class	ClassifierMananger
 *
 * @brief	ClassifierManager ties together all components of the image
 * 			clustering and classification processes.
 */

class ClassifierMananger
{
public:
	ClassifierMananger(vector<string> file_paths);
	virtual ~ClassifierMananger();
	void load_images();
	void cluster_images(int n_clusters);
	void train_classifier();
	void classify_images(vector<string> file_paths);

	vector<Image*>& get_images() { return m_images; };
	vector<ImageClass*>& get_image_classes() { return m_image_classes; };
	ImageClassifier* get_classifier() { return m_classifier; };
private:
	vector<string> m_file_paths;
	vector<Image*> m_images;
	vector<ImageClass*> m_image_classes;
	ImageClassifierRF* m_classifier;
};

