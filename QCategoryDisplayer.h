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
	 * @param [in,out]	image_class	If non-null, the image class.
	 */

	QCategoryDisplayer(ImageClass* image_class);
	~QCategoryDisplayer();

	/**
	 * @fn	ImageClass* QCategoryDisplayer::get_image_class()
	 *
	 * @brief	Gets the image class which this display component represents.
	 *
	 * @return	The image class which this display component represents.
	 */

	ImageClass* get_image_class() { return m_image_class; };
	static int get_total_diameter();
	virtual QRectF boundingRect() const;

	/**
	 * @fn	void QCategoryDisplayer::update_images();
	 *
	 * @brief	Updates the pixmaps of the class icon and its neighbours
	 * 			This should be called following any call to ImageClass::calculate_icon
	 */

	void update_images();
protected:
	static const int MAX_NEIGHBOURS = 8;
	static const int NEIGHBOUR_EDGE_LENGTH = 200;
	static const int NEIGHBOUR_DIAMETER = 100;
	static const int ICON_DIAMETER = 200;

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:
	void classClicked(ImageClass*);
private:
	/** @brief	The image class which this display component represents. */
	ImageClass* m_image_class;
	QVector<QImageDisplayer*> m_neighbours;
};
