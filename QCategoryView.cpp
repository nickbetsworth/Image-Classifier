#include "QCategoryView.h"
#include <QWheelEvent>
#include <iostream>
QCategoryView::QCategoryView(QWidget* parent) : QGraphicsView(parent) { initialize(); }
QCategoryView::QCategoryView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) { initialize(); }

QCategoryView::~QCategoryView()
{
}

// When a user scrolls, zoom in or out of the scene
void QCategoryView::wheelEvent(QWheelEvent* e) {
	const double max_zoom = 1.0;
	const double min_zoom = 0.2;
	// How quickly the scale will change with respect to mouse scroling
	const double zoom_speed = 0.15;

	int mult = 0;
	// Determine which way the user wishes to scroll
	if (e->angleDelta().y() > 0)
		mult = 1;
	else if (e->angleDelta().y() < 0)
		mult = -1;

	double s = 1 + (mult * zoom_speed);

	QTransform t = this->transform();
	t.scale(s, s);

	// Bound the zoom by min_zoom and max_zoom
	// We will only run on scaling factor of one axis, as they should always be equivalent
	if (t.m11() > max_zoom || t.m11() < min_zoom) {
		double new_zoom = qMax(qMin(max_zoom, t.m11()), min_zoom);
		t.setMatrix(new_zoom, t.m12(), t.m13(),
			t.m21(), new_zoom, t.m23(),
			t.m31(), t.m32(), t.m33());
	}

	//std::cout << t.m11() << ", " << t.m22() << std::endl;

	this->setTransform(t);

	e->accept();
}

void QCategoryView::mousePressEvent(QMouseEvent* event) {
	QGraphicsView::mousePressEvent(event);
}

void QCategoryView::initialize() {
	//scale(1.0, 1.0);
}