#pragma once
#include <QGraphicsItem>
#include <QPixmap>
#include "Image.h"

/**
 * @class	QImageDisplayer
 *
 * @brief	This component is in charge of rendering an image.
 */

class QImageDisplayer :
	public QGraphicsObject
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
	QImageDisplayer(Image* image);
	QImageDisplayer(Image* image, int diameter);
	~QImageDisplayer();

	QRectF boundingRect() const;

	/**
	 * @fn	Image* QImageDisplayer::get_image()
	 *
	 * @brief	Gets the image which this component is rendering.
	 *
	 * @return	null if it fails, else the image.
	 */

	Image* get_image() { return m_image; };
	QPixmap* get_pixmap() { return m_pixmap; };
	int get_diameter() const { return m_diameter; };
	void set_diameter(int diameter) { m_diameter = diameter; };

	/**
	 * @fn	bool QImageDisplayer::is_hovering() const
	 *
	 * @brief	Query if the user is hovering over this image.
	 *
	 * @return	true if hovering, false if not.
	 */

	bool is_hovering() const { return m_hovering; };
	void set_hovering(bool hovering);

	/**
	 * @fn	bool QImageDisplayer::is_highlighted() const
	 *
	 * @brief	Query if this image is being highlighted.
	 *
	 * @return	true if highlighted, false if not.
	 */

	bool is_highlighted() const { return m_highlighted; };

	/**
	 * @fn	void QImageDisplayer::set_highlighted(bool highlighted);
	 *
	 * @brief	Sets whether or not this image will be highlighted.
	 *
	 * @param	highlighted	true if highlighted.
	 */

	void set_highlighted(bool highlighted);
protected:
	/** @brief	The default diameter of the display component. */
	static const int DEFAULT_DIAMETER = 100;
	/** @brief	Thickness of the highlight. */
	static const int HIGHLIGHT_SIZE = 6;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	virtual QPainterPath shape() const;
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
signals:
	void imageClicked(Image* image, bool rightClick);
	void mouseEntered();
	void mouseLeft();
private:
	/** @brief	Stores the image this component is rendering. */
	Image* m_image;
	/** @brief	Stores the pixmap which is loaded from the image */
	QPixmap* m_pixmap;
	int m_diameter;
	/** @brief	Stores whether or not this image is being hovered over by the user. */
	bool m_hovering;
	/** @brief	Stores whether or not this image is being highlighted. */
	bool m_highlighted;
};

