#pragma once
#include <QGraphicsItem>
#include <QPixmap>
#include "Image.h"

/**
 * @class	QImageDisplayer
 *
 * @brief	Renders a single Image in to the scene.
 */

class QImageDisplayer :
	public QGraphicsObject
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
	QImageDisplayer(Image* image, QGraphicsItem* parent = 0);
	QImageDisplayer(Image* image, int diameter, QGraphicsItem* parent = 0);
	virtual ~QImageDisplayer();

	virtual QRectF boundingRect() const;

	/**
	 * @fn	Image* QImageDisplayer::get_image()
	 *
	 * @brief	Gets the image which this component is rendering.
	 *
	 * @return	null if it fails, else the image.
	 */

	Image* get_image() { return m_image; };

	/**
	 * @fn	void QImageDisplayer::set_image(Image* image);
	 *
	 * @brief	Sets the image which this component will render.
	 * 			Note: this also re-loads in the pixmap.
	 *
	 * @param [in,out]	image	If non-null, the image.
	 */

	void set_image(Image* image);

	/**
	 * @fn	QPixmap* QImageDisplayer::get_pixmap()
	 *
	 * @brief	Gets the pixmap which is being used to render the image.
	 *
	 * @return	null if it fails, else the pixmap.
	 */

	QPixmap* get_pixmap() { return m_pixmap; };

	/**
	 * @fn	int QImageDisplayer::get_diameter() const
	 *
	 * @brief	Gets the diameter of the rendered image.
	 *
	 * @return	The diameter.
	 */

	int get_diameter() const { return m_diameter; };

	/**
	 * @fn	void QImageDisplayer::set_diameter(int diameter)
	 *
	 * @brief	Sets the diameter of the rendered image.
	 *
	 * @param	diameter	The diameter.
	 */

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

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
protected:
	/** @brief	The default diameter of the display component. */
	static const int DEFAULT_DIAMETER = 100;
	/** @brief	Thickness of the highlight. */
	static const int HIGHLIGHT_SIZE = 6;

	virtual QPainterPath shape() const;
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
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

	/** @brief	Stores the position a user clicked on the image at the last mousePressEvent */
	QPoint m_pos_clicked;
};

