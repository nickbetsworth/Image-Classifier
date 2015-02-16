#include "QCategoryView.h"
#include <QWheelEvent>

QCategoryView::QCategoryView(QWidget* parent) : QGraphicsView(parent) { initialize(); }
QCategoryView::QCategoryView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) { initialize(); }

QCategoryView::~QCategoryView()
{
}

// When a user scrolls, zoom in or out of the scene
void QCategoryView::wheelEvent(QWheelEvent* e) {
	// How quickly the scale will change with respect to mouse scroling
	const double zoom_speed = 0.15;

	int mult = 0;
	// Determine which way the user wishes to scroll
	if (e->angleDelta().y() > 0)
		mult = 1;
	else if (e->angleDelta().y() < 0)
		mult = -1;

	// Determine how much we will scale by
	double s = 1 + (mult * zoom_speed);
	scale(s, s);

	e->accept();
}

void QCategoryView::mousePressEvent(QMouseEvent* event) {
	QGraphicsView::mousePressEvent(event);
}

void QCategoryView::initialize() {
	scale(1.0, 1.0);
}