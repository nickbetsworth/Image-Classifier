#pragma once
#include <QGraphicsItem>
#include <QPixmap>
#include "Image.h"


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

	Image* get_image() { return m_image; };
	QPixmap* get_pixmap() { return m_pixmap; };
	int get_diameter() const { return m_diameter; };
	void set_diameter(int diameter) { m_diameter = diameter; };
	bool is_hovering() const { return m_hovering; };
	void set_hovering(bool hovering);
protected:
	static const int DEFAULT_DIAMETER = 100;

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
	Image* m_image;
	QPixmap* m_pixmap;
	int m_diameter;
	bool m_hovering;
};

