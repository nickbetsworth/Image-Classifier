#pragma once
#include "ImageClass.h"

class RFTest
{
public:
	RFTest(vector<ImageClass*> image_classes);
	~RFTest();
	vector<ImageClass*> get_image_classes() { return m_image_classes; };
	void perform_test(int n_tests);
private:
	vector<ImageClass*> m_image_classes;
};

