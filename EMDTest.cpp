#include "EMDTest.h"
#include "Histograms.h"
#include "Image.h"
#include "ImageFactory.h"

void test_EMD() {
	// Load in the two images
	Image* image = ImageFactory::create_image("C:/data/ProjectImages/FlickrDownloads/9755879056.png", Property::Histogram);
	Image* imageDissimilar = ImageFactory::create_image("C:/data/ProjectImages/FlickrDownloads/9692322722.png", Property::Histogram);
	std::cout << image->get_histogram() << std::endl;
	std::cout << imageDissimilar->get_histogram() << std::endl;
	std::cout << "Dissimilar Val: " << image->calculate_distance(imageDissimilar) << std::endl;

	/*float sum = 0;
	for (int i = 0; i < image->get_histogram().rows; i++) {
		sum += image->get_histogram().at<float>(i);
	}

	cout << "Sum: " << sum << endl;

	sum = 0;
	for (int i = 0; i < image->get_histogram().rows; i++) {
		sum += imageDissimilar->get_histogram().at<float>(i);
	}

	cout << "Sum: " << sum << endl;*/

	Image* imageSimilar = ImageFactory::create_image("C:/data/ProjectImages/FlickrDownloads/8929525390.png", Property::Histogram);
	std::cout << "Similar Val: " << image->calculate_distance(imageSimilar) << std::endl;
}