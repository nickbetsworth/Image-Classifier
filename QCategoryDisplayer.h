#pragma once
#include "ImageClass.h"
#include "QImageDisplayer.h"
#include <QDragEnterEvent>

/**
 * @class	QCategoryDisplayer
 *
 * @brief	Displays a cluster.
 */

class QCategoryDisplayer : public QImageDisplayer
{
	Q_OBJECT
public:

	/**
	 * @fn	QCategoryDisplayer::QCategoryDisplayer(ImageClass* image_class);
	 *
	 * @brief	Constructor.
	 *
	 * @author	Nick
	 * @date	21/02/2015
	 *
	 * @param [in,out]	image_class	If non-null, the image class.
	 */

	QCategoryDisplayer(ImageClass* image_class);
	QCategoryDisplayer(ImageClass* image_class, int diameter);
	~QCategoryDisplayer();

	/**
	 * @fn	ImageClass* QCategoryDisplayer::get_image_class()
	 *
	 * @brief	Gets the image class which this display component represents.
	 *
	 * @return	The image class which this display component represents.
	 */

	ImageClass* get_image_class() { return m_image_class; };

	QRectF boundingRect() const;
protected:
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:
	void categoryClicked(ImageClass*);
private:
	/** @brief	The image class which this display component represents. */
	ImageClass* m_image_class;
};
