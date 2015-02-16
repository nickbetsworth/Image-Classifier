#ifndef H_IMAGE_CONVERSION
#define H_IMAGE_CONVERSION

#include <opencv\cv.h>
#include <QImage>
#include <QPixmap>

//http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap/

namespace Conv {
	QImage cvMatToQImage(const cv::Mat &inMat);
	QPixmap cvMatToQPixmap(const cv::Mat &inMat);
}

#endif