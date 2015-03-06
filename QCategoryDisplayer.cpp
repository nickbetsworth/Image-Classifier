#include "QCategoryDisplayer.h"
#include "ImageConversion.h"
#include <QGraphicsSceneEvent>
#include <QApplication>
#include <QDrag>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

QCategoryDisplayer::QCategoryDisplayer(ImageClass* image_class) : QImageDisplayer(image_class->get_icon(), ICON_DIAMETER) {
	m_image_class = image_class;

	// Get the nearest neighbours of the icon
	update_images();
}

QCategoryDisplayer::~QCategoryDisplayer()
{
	delete m_image_class;
}

QRectF QCategoryDisplayer::boundingRect() const {
	// Work out the total diameter, including icon, edge, and neighbour
	int total_diameter = get_total_diameter();
	return QRectF(-total_diameter / 2, -total_diameter / 2, total_diameter, total_diameter);
}

int QCategoryDisplayer::get_total_diameter() {
	int total_diameter = ICON_DIAMETER +
						(NEIGHBOUR_EDGE_LENGTH - ICON_DIAMETER / 2 - NEIGHBOUR_DIAMETER / 2) * 2 +
						NEIGHBOUR_DIAMETER * 2;

	return total_diameter;
}

void QCategoryDisplayer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->setRenderHint(QPainter::RenderHint::Antialiasing);
	// Set up the pen for drawing the edges
	QPen pen = QPen(Qt::PenStyle::SolidLine);
	pen.setWidth(1);
	pen.setColor(QColor(240, 240, 240));
	pen.setCosmetic(true);
	painter->setPen(pen);
	// Draw the main central icon image
	

	// Draw the edges first, and then the neighbours on top of the edges
	for (QImageDisplayer* neighbour : m_neighbours) {
		QPoint neighbour_pos = neighbour->pos().toPoint();
		
		// Draw the edge from the central icon to the center of this neighbour
		QLine edge = QLine(neighbour_pos, QPoint(0, 0));
		
		painter->drawLine(edge);
		
	}

	QImageDisplayer::paint(painter, option, widget);
}

void QCategoryDisplayer::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	// Check if the mouse click was within the central icon
	cout << "Here!" << endl;
	QVector2D mouse_point = QVector2D(event->pos());
	QVector2D icon_center = QVector2D(QPointF(0, 0));
	if (mouse_point.distanceToPoint(icon_center) <= (ICON_DIAMETER / 2)) {
		emit(classClicked(m_image_class));
		event->accept();
	}
	else {
		event->ignore();
	}
}

void QCategoryDisplayer::update_images() {
	// Release existing images if necessary
	for (QImageDisplayer* neighbour : m_neighbours) {
		delete neighbour;
	}

	// Set the central icon (QImageDisplayer::set_image)
	this->set_image(get_image_class()->get_icon());

	// Work out the maximum number of images we can display (-1 as we don't count root node)
	int class_image_count = get_image_class()->get_images().size() - 1;
	int num_neighbours = min((int)class_image_count, MAX_NEIGHBOURS);

	NodeList nodes = get_image_class()->get_graph()->get_n_nearest_nodes(get_image_class()->get_icon(), num_neighbours);
	double multiplier = 360.0 / num_neighbours;
	int i = 0;
	// Create a QImageDisplayer for each neighbour
	for (Node node : nodes) {
		Image* image = static_cast<Image*>(node);
		
		QImageDisplayer* neighbour = new QImageDisplayer(image, NEIGHBOUR_DIAMETER, this);
		double ang = qDegreesToRadians(i * multiplier);
		// Work out the central position of this neighbour
		int pos_x = sin(ang) * (NEIGHBOUR_EDGE_LENGTH);
		int pos_y = cos(ang) * (NEIGHBOUR_EDGE_LENGTH);
		QPoint neighbour_pos = QPoint(pos_x, pos_y);
		neighbour->setPos(neighbour_pos);

		m_neighbours.push_back(neighbour);

		i++;
	}
}