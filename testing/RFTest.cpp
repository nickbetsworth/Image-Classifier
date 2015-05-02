#include "RFTest.h"
#include "ImageClassifierRF.h"

RFTest::RFTest(vector<ImageClass*> image_classes)
{
	m_image_classes = image_classes;
}

void RFTest::perform_test(int n_tests) {
	// Totals across all tests
	/*double test_total_positive = 0;
	double test_total_negative = 0;

	for (int i = 0; i < n_tests; i++) {
		vector<ImageClass*> training_classes;
		vector<ImageClass*> testing_classes;
		// Maps from a testing class to the respective training class it was taken from
		map<ImageClass*, ImageClass*> testing_class_map;

		int n = 0;
		srand(time(0));

		cout << "Splitting up " << m_image_classes.size() << " image classes in to training and testing data" << endl;
		// Randomly split up each image class, half training data, half testing data
		for (ImageClass* image_class : m_image_classes) {
			ImageClass* training_class = new ImageClass();
			// Make a copy of the image vector
			vector<Image*> images(image_class->get_images());
			// Store the initial size of the class
			int size = images.size();
			for (int j = 0; j < size / 2; j++) {
				// Pick a random image
				int n = rand() % images.size();
				Image* img = images.at(n);
				// Remove the random image from the array
				images.erase(images.begin() + n);
				// Add it to the set of training data
				training_class->add_image(img);
			}

			// Create the class of testing data from the remaining images
			ImageClass* testing_class = new ImageClass(images);
			testing_class_map[testing_class] = training_class;
			// Add both training and testing classes to the global array
			training_classes.push_back(training_class);
			testing_classes.push_back(testing_class);
			cout << "Class " << n << " created with " << training_class->get_images().size() << " training images";
			cout << " and " << testing_class->get_images().size() << " testing images" << endl;
			n++;
		}

		// Train with new image classes
		ImageClassifierRF rf = ImageClassifierRF();
		rf.train(training_classes);

		// Use testing data with image classes
		// Stores the ID of the current class we are predicting data for
		int n_class = 0;
		// Total number of correct and incorrect true positive predictions, across all classes 
		int tp_correct = 0;
		int tp_incorrect = 0;
		int tn_correct = 0;
		int tn_incorrect = 0;
		for (ImageClass* image_class : testing_classes) {
			cout << "Using training data for class " << n_class << endl;

			// Total up true positive rate for this class
			int n_correct = 0;
			int n_incorrect = 0;
			for (Image* img : image_class->get_images()) {
				ImageClass* predicted_class = rf.predict(img);
				if (predicted_class == testing_class_map[image_class])
					n_correct++;
				else
					n_incorrect++;
			}

			// Add the class true positive totals up to the global total
			tp_correct += n_correct;
			tp_incorrect += n_incorrect;

			// Calculate true negative rate for this class
			n_correct = 0;
			n_incorrect = 0;
			// Choose a random class that is different to this class
			/*ImageClass* negative_class = image_class;
			while (negative_class == image_class) {
				negative_class = testing_classes.at(rand() % testing_classes.size());
			}

			for (ImageClass* negative_class : testing_classes) {
				// If the negative_class == image_class then it does not contain negative samples
				if (negative_class != image_class) {
					for (Image* img : negative_class->get_images()) {
						ImageClass* predicted_class = rf.predict(img);
						if (predicted_class != testing_class_map[image_class])
							n_correct++;
						else
							n_incorrect++;
					}
				}
			}
			// Total up true negative rate for this class
			

			// Add the class true negative totals up to the global total
			tn_correct += n_correct;
			tn_incorrect += n_incorrect;

			n_class++;
		}

		// Calculate true positive & true negative rates for this set of classes
		int class_total_positive = tp_correct + tp_incorrect;
		int class_total_negative = tn_correct + tn_incorrect;;
		double positive_percent = (static_cast<double>(tp_correct) / static_cast<double>(class_total_positive)) * 100.0;
		double negative_percent = (static_cast<double>(tn_correct) / static_cast<double>(class_total_negative)) * 100.0;
		test_total_positive += positive_percent;
		test_total_negative += negative_percent;

		cout << setprecision(3);
		cout << "True Positive: " << tp_correct << " of " << class_total_positive << " (" << positive_percent << "%)" << endl;
		cout << "True Negative: " << tn_correct << " of " << class_total_negative << " (" << negative_percent << "%)" << endl;
		// Free up memory

	}

	// Calculate the final averages across all of the tests
	double avg_true_positive = test_total_positive / n_tests;
	double avg_true_negative = test_total_negative / n_tests;

	cout << "Testing has finished " << n_tests << " runs" << endl;
	cout << "Average True Positive: " << avg_true_positive << "%" << endl;
	cout << "Average True Negative: " << avg_true_negative << "%" << endl;*/
}

RFTest::~RFTest()
{
}
