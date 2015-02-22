#ifndef H_IMAGE_CONVERSION
#define H_IMAGE_CONVERSION

#include <opencv\cv.h>
#include <QImage>
#include <QPixmap>

//http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap/

namespace Conv {

	/**
	 * @fn	QImage cvMatToQImage(const cv::Mat &inMat);
	 *
	 * @brief	Converts an OpenCV matrix to a QImage
	 *
	 * @param	inMat	The in matrix.
	 *
	 * @return	A QImage.
	 */

	QImage cvMatToQImage(const cv::Mat &inMat);

	/**
	 * @fn	QPixmap cvMatToQPixmap(const cv::Mat &inMat);
	 *
	 * @brief	Converts an OpenCV matrix to a QImage
	 *
	 * @param	inMat	The in matrix.
	 *
	 * @return	A QPixmap.
	 */

	QPixmap cvMatToQPixmap(const cv::Mat &inMat);
}

#endif