#pragma once
#include "ImageClass.h"
#include "QImageDisplayer.h"
#include <QDragEnterEvent>

/**
 * @class	QCategoryDisplayer
 *
 * @brief	Displays a cluster and its nearest neighbours.
 */

class QCategoryDisplayer : public QImageDisplayer
{
	Q_OBJECT
public:

	/**
	 * @fn	QCategoryDisplayer::QCategoryDisplayer(ImageClass* image_class);
	 *
	 * @brief	Constructs a new display component for the specified image class.
	 *
	 * @param [in,out]	image_class	If non-null, the image class.
	 */

	QCategoryDisplayer(ImageClass* image_class);
	~QCategoryDisplayer();

	/**
	 * @fn	ImageClass* QCategoryDisplayer::get_image_class()
	 *
	 * @brief	Gets the image class which this display component is rendering.
	 *
	 * @return	The image class which this display component is rendering.
	 */

	ImageClass* get_image_class() { return m_image_class; };

	/**
	 * @fn	static int QCategoryDisplayer::get_total_diameter();
	 *
	 * @brief	Gets total diameter of the component (including neighbours).
	 *
	 * @return	The total diameter.
	 */

	static int get_total_diameter();
	virtual QRectF boundingRect() const;

	/**
	 * @fn	void QCategoryDisplayer::update_images();
	 *
	 * @brief	Updates the pixmaps of the class icon and its neighbours.
	 * 			This should be called following any call to ImageClass::calculate_icon.
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

	/**
	 * @fn	void QCategoryDisplayer::classClicked(ImageClass* image_class);
	 *
	 * @brief	Sent out whenever a user clicks on the class icon.
	 *
	 * @param [in,out]	image_class A reference to the class this component is drawing.
	 */

	void classClicked(ImageClass* image_class);
private:
	/** @brief	The image class which this display component represents. */
	ImageClass* m_image_class;
	/** @brief	The nearest neighbours found within the class with respect to the class icon. */
	QVector<QImageDisplayer*> m_neighbours;
};
