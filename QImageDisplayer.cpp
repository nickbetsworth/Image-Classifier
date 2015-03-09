#include "QImageDisplayer.h"
#include "ImageConversion.h"
#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QVector2D>

QImageDisplayer::QImageDisplayer(Image* image, QGraphicsItem* parent) : QImageDisplayer(image, DEFAULT_DIAMETER, parent) {

}

QImageDisplayer::QImageDisplayer(Image* image, int diameter, QGraphicsItem* parent) : QGraphicsObject(parent) {
	m_diameter = diameter;
	m_pixmap = 0;
	set_image(image);

	// The user is not currently hovering over this icon
	m_hovering = false;
	m_highlighted = false;
	
	// Enable hover events to be sent when a user hovers over the category
	setAcceptHoverEvents(true);
}


QImageDisplayer::~QImageDisplayer()
{
	if (m_pixmap != 0) {
		delete m_pixmap;
	}
}

QRectF QImageDisplayer::boundingRect() const {
	if (is_highlighted()) {
		return QRectF(-(m_diameter / 2 + HIGHLIGHT_SIZE), -(m_diameter / 2 + HIGHLIGHT_SIZE), m_diameter + HIGHLIGHT_SIZE * 2, m_diameter + HIGHLIGHT_SIZE * 2);
	}
	else {
		return QRectF(-m_diameter / 2, -m_diameter / 2, m_diameter, m_diameter);
	}
}

QPainterPath QImageDisplayer::shape() const {
	QPainterPath path;
	path.addEllipse(boundingRect());
	return path;
}

void QImageDisplayer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	// Turn AA on to smooth the edges
	painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);

	QRectF pixmapRect = QImageDisplayer::boundingRect();

	// Draw the highlighted ring
	if (is_highlighted()) {
		QBrush brush = QBrush(Qt::SolidPattern);
		brush.setColor(QColor(255, 60, 60));
		painter->setBrush(brush);
		painter->setPen(Qt::PenStyle::NoPen);
		painter->drawEllipse(pixmapRect.toRect());

		// Make the pixmap smaller accordingly
		pixmapRect.adjust(HIGHLIGHT_SIZE, HIGHLIGHT_SIZE, -HIGHLIGHT_SIZE, -HIGHLIGHT_SIZE);
	}

	// Clip the image to be a circle
	QPainterPath path;
	path.addEllipse(pixmapRect);
	painter->setClipPath(path);


	painter->drawPixmap(pixmapRect.toRect(), *m_pixmap);
}

void QImageDisplayer::set_image(Image* image) {
	// If we are told to load the image we already have,
	// do nothing
	if (image == m_image) {
		return;
	}

	if (m_pixmap != 0) {
		delete m_pixmap;
	}

	QSize size = QSize(get_diameter(), get_diameter());
	// Scale the pixmap to the specified size
	QPixmap* pm = new QPixmap(Conv::cvMatToQPixmap(image->get_image_data()));
	m_pixmap = new QPixmap(pm->scaled(size, Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	// Free up the memory from the unscaled pixmap
	delete pm;

	// Need to load in full res first if diameter > thumbnail size
	// f
	// f
	// f
	// c
	// c
	// c
	
	
	m_image = image;
}

void QImageDisplayer::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	m_pos_clicked = event->pos().toPoint();
	event->accept();
}

void QImageDisplayer::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	QVector2D v1 = QVector2D(m_pos_clicked);
	QVector2D v2 = QVector2D(event->pos());

	// Only send out a signal if the mouse was released within the image
	if (v1.distanceToPoint(v2) < QImageDisplayer::get_diameter() / 2) {
		bool rightClick = false;
		if (event->button() == Qt::RightButton)
			rightClick = true;

		emit(imageClicked(m_image, rightClick));
	}
}

void QImageDisplayer::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
	set_hovering(true);
}

void QImageDisplayer::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
	set_hovering(false);
}

void QImageDisplayer::set_hovering(bool hovering) {
	this->prepareGeometryChange();
	m_hovering = hovering;

	// If the user is hovering over the image, bring it to the front
	if (hovering) {
		this->setZValue(10);
		emit(mouseEntered());
	}
	else {
		this->setZValue(1);
		emit(mouseLeft());
	}

	// Let the renderer know the size of the image is about to change
	
}

void QImageDisplayer::set_highlighted(bool highlighted) {
	this->prepareGeometryChange();
	m_highlighted = highlighted;
	// Let the renderer know the size of the image is about to change
	
}