#include "QImageDisplayer.h"
#include "ImageConversion.h"
#include <QPainter>
#include <QGraphicsSceneEvent>

QImageDisplayer::QImageDisplayer(Image* image) : QImageDisplayer(image, DEFAULT_DIAMETER) {

}

QImageDisplayer::QImageDisplayer(Image* image, int diameter) {
	m_diameter = diameter;
	m_image = image;
	QSize size = QSize(diameter, diameter);
	// Scale the pixmap to the specified size
	QPixmap* pm = new QPixmap(Conv::cvMatToQPixmap(image->get_image_data()));
	m_pixmap = new QPixmap(pm->scaled(size, Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	// Free up the memory from the unscaled pixmap
	delete pm;

	// The user is not currently hovering over this icon
	m_hovering = false;
	// Enable hover events to be sent when a user hovers over the category
	setAcceptHoverEvents(true);
}


QImageDisplayer::~QImageDisplayer()
{
}

QRectF QImageDisplayer::boundingRect() const {
	return QRectF(-m_diameter / 2, -m_diameter / 2, m_diameter, m_diameter);
}

QPainterPath QImageDisplayer::shape() const {
	QPainterPath path;
	path.addEllipse(boundingRect());
	return path;
}

void QImageDisplayer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	// Turn AA on to smooth the edges
	painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);

	// If the user is hovering over this icon
	// Ensure it is as the front of the foreground
	if (m_hovering) {
		setZValue(10);
	}
	else {
		setZValue(0);
	}


	QPainterPath path;

	path.addEllipse(boundingRect());
	painter->setClipPath(path);

	painter->drawPixmap(boundingRect().toRect(), *m_pixmap);
}

void QImageDisplayer::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	//QGraphicsObject::mouseReleaseEvent(event);
	bool rightClick = false;
	if (event->button() == Qt::RightButton)
		rightClick = true;

	emit(imageClicked(m_image, rightClick));
}

void QImageDisplayer::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
	set_hovering(true);
}

void QImageDisplayer::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
	set_hovering(false);
}

void QImageDisplayer::set_hovering(bool hovering) {
	m_hovering = hovering;

	if (hovering) {
		this->setZValue(10);
		emit(mouseEntered());
	}
	else {
		this->setZValue(1);
		emit(mouseLeft());
	}

	this->prepareGeometryChange();
}