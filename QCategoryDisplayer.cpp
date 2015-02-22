#include "QCategoryDisplayer.h"
#include <QGraphicsSceneEvent>
#include <QApplication>
#include <QDrag>
QCategoryDisplayer::QCategoryDisplayer(ImageClass* image_class) : QCategoryDisplayer(image_class, DEFAULT_DIAMETER)
{
	
}

QCategoryDisplayer::QCategoryDisplayer(ImageClass* image_class, int diameter) : QImageDisplayer(image_class->get_icon(), diameter) {
	m_image_class = image_class;
}

QCategoryDisplayer::~QCategoryDisplayer()
{
	delete m_image_class;
}

QRectF QCategoryDisplayer::boundingRect() const {
	int diameter = get_diameter();

	if (is_hovering()) {
		return QRectF(-diameter, -diameter, diameter * 2, diameter * 2);
	}
	else {
		return QImageDisplayer::boundingRect();
	}
	
}

void QCategoryDisplayer::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	emit(categoryClicked(m_image_class));
}
