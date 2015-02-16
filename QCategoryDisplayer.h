#pragma once
#include "ImageClass.h"
#include "QImageDisplayer.h"
#include <QDragEnterEvent>
/**
 * @class	QCategoryDisplayer
 *
 * @brief	A category displayer.
 *
 * @author	Nick
 * @date	10/11/2014
 */

class QCategoryDisplayer : public QImageDisplayer
{
	Q_OBJECT
public:
	QCategoryDisplayer(ImageClass* image_class);
	QCategoryDisplayer(ImageClass* image_class, int diameter);
	~QCategoryDisplayer();
	ImageClass* get_image_class() { return m_image_class; };
	QRectF boundingRect() const;
protected:
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
	virtual void dragEnterEvent(QDragEnterEvent* event);
	//virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
signals:
	void categoryClicked(ImageClass*);
private:
	ImageClass* m_image_class;
};
