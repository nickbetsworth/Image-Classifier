#include "SiftTest.h"
#include "Image.h"
#include "ImageClustererKMeans.h"

void test_sift() {

	vector<std::string> files = { "C:/data/ProjectImages/SIFTTest/a.png",
		"C:/data/ProjectImages/SIFTTest/b.png",
		"C:/data/ProjectImages/SIFTTest/c.png",
		"C:/data/ProjectImages/SIFTTest/d.png",
		"C:/data/ProjectImages/SIFTTest/e.png",
		"C:/data/ProjectImages/SIFTTest/f.png",
		"C:/data/ProjectImages/SIFTTest/g.png",
		"C:/data/ProjectImages/SIFTTest/h.png",
		"C:/data/ProjectImages/SIFTTest/i.png",
		"C:/data/ProjectImages/SIFTTest/j.png",
		"C:/data/ProjectImages/SIFTTest/k.png" };

	vector<Image*> images;
	for (std::string file : files) {
		Image* image = new Image(file);
		images.push_back(image);
	}

	ImageClustererKMeans* clusterer = new ImageClustererKMeans(images, 6);
	clusterer->cluster_images();
}