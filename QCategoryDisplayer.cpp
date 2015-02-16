#include "QCategoryDisplayer.h"
#include <QGraphicsSceneEvent>
#include <QApplication>
#include <QDrag>
QCategoryDisplayer::QCategoryDisplayer(ImageClass* image_class) : QCategoryDisplayer(image_class, DEFAULT_DIAMETER)
{
	
}

QCategoryDisplayer::QCategoryDisplayer(ImageClass* image_class, int diameter) : QImageDisplayer(image_class->get_icon()) {
	//setFlag(QGraphicsItem::ItemIsMovable);
	setAcceptDrops(true);
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
		return QRectF(-diameter / 2, -diameter / 2, diameter, diameter);
	}
	
}

void QCategoryDisplayer::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	emit(categoryClicked(m_image_class));
}

void QCategoryDisplayer::dropEvent(QGraphicsSceneDragDropEvent* event) {
	cout << "Dropped at " << m_image_class->get_images().front()->get_filepath() << endl;
}
void QCategoryDisplayer::dragEnterEvent(QDragEnterEvent* event) {
	cout << "Here2";
}

/*void QCategoryDisplayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
		.length() < QApplication::startDragDistance()) {
		return;
	}

	//QDrag* drag = new QDrag(this);
	//cout << "Here3";
	//drag->exec();
}*/