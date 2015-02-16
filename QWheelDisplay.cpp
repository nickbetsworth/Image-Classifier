#include "QWheelDisplay.h"
#include <QtMath>
#include <QPainter>
#include "ImageConversion.h"
#include <QPropertyAnimation>

QWheelDisplay::QWheelDisplay(QCategoryDisplayer* category_displayer) : QGraphicsObject()
{
	// Set the size of each image in the wheel
	m_diameter = category_displayer->get_diameter();

	// Load in the images for this category
	m_category_displayer = category_displayer;

	m_displaying = false;
	m_rad_scale = 1.0;

	ImageClass* image_class = m_category_displayer->get_image_class();
	for (Image* image : image_class->get_images()) {

		QPixmap* pm = new QPixmap(Conv::cvMatToQPixmap(image->get_image_data()));
		QPixmap* pm_resized = new QPixmap(pm->scaled(QSize(m_diameter, m_diameter), Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
		delete pm;

		m_pixmaps.push_back(pm_resized);
	}
}


QWheelDisplay::~QWheelDisplay()
{
	for (QPixmap* pm : m_pixmaps) {
		delete pm;
	}
}

QRectF QWheelDisplay::boundingRect() const {
	double r = get_wheel_radius();
	double s = get_rad_scale();
	// Wheel has radius of SCALING * ImageDiameter + PADDING
	return QRectF(-r * s,
		-r * s,
		r * 2 * s,
		r * 2 * s);
}

void QWheelDisplay::set_displaying(bool displaying) {
	m_displaying = displaying;
	//update();
}

void QWheelDisplay::show() {
	set_displaying(true);
	QPropertyAnimation* show_anim = new QPropertyAnimation(this, "rad_scale");
	show_anim->setDuration(400);
	show_anim->setStartValue(0.0);
	show_anim->setEndValue(1.0);
	show_anim->setEasingCurve(QEasingCurve::OutQuart);
	show_anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void QWheelDisplay::hide() {
	set_displaying(false);
}

void QWheelDisplay::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	// If the user is hovering over this category, display a wheel of all images
	if (is_displaying()) {
		painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);

		// Draw a background underlay which is on top of any of the category images
		setZValue(3);
		// Clip out the overlay from displaying where the category icon is
		QPainterPath category_path;
		QPainterPath wheel_path;
		QPainterPath category_overlay_path;

		QRectF icon_rect = m_category_displayer->boundingRect();
		icon_rect.adjust(4, 4, -4, -4);
		category_path.addEllipse(icon_rect);
		wheel_path.addEllipse(boundingRect());
		category_overlay_path = wheel_path.subtracted(category_path);

		painter->setClipPath(category_overlay_path);

		QBrush brush(QColor(0, 0, 0, 225));
		painter->setBrush(brush);
		// Draw the wheel background
		painter->drawEllipse(boundingRect());
		//cout << "Updating QWheelDisplay with scale: " << get_rad_scale() << endl;
		// Only draw the images in the wheel if the wheel is sufficiently sized
		// (Prevent the images being drawn in the early stages of the animation)
		
		//if (get_icon_wheel_radius() >= m_category_displayer->get_diameter() / 2) {
			// The radius each image will be displayed along
			double rad = get_icon_wheel_radius() * get_rad_scale();

			int max_images = get_max_images();
			// Now bound the number of images to the maximum we calculated
			// Subtract -1 in this calculation as the central category icon is one of our images, we do not wish to display it twice
			int num_images = m_pixmaps.size() - 1;
			if (num_images > max_images) {
				num_images = max_images;
			}
			// If there are no extra images other than the central icon to draw, quit out to avoid divison by zero
			else if (num_images <= 0) {
				return;
			}

			double inc = 360.0 / num_images;
			QPainterPath circle_clip;

			// Set the clip path so each image in the wheel is clipped to be circular
			for (int i = 0; i < num_images; i++) {
				double deg = qDegreesToRadians(i * inc);
				QPointF pos = QPointF(rad * sin(deg) - (m_diameter / 2), rad * cos(deg) - (m_diameter / 2));
				circle_clip.addEllipse(QRectF(pos.x(), pos.y(), m_diameter, m_diameter));
			}
			painter->setClipPath(circle_clip);

			// Draw each of the images
			// The images will be on top of everything else
			setZValue(4);
			auto it = m_pixmaps.begin() + 1;
			for (int i = 0; i < num_images; ++i, ++it) {
				QPixmap* pm = *it;
				double deg = qDegreesToRadians(i * inc);
				QPointF pos = QPointF(rad * sin(deg) - (m_diameter / 2), rad * cos(deg) - (m_diameter / 2));

				painter->drawPixmap(pos, *pm);
			}
		//}
	}
}

double QWheelDisplay::get_wheel_radius() const {
	double rad = m_diameter + PADDING + (m_category_displayer->get_diameter());

	return rad;
}

double QWheelDisplay::get_icon_wheel_radius() const {
	// The radius each image will be displayed along (The border of the wheel - PADDING)
	double rad = get_wheel_radius() - (m_diameter / 2) - (PADDING / 2);

	return rad;
}

int QWheelDisplay::get_max_images() {
	// Calculate the maximum number of images we can display
	// First calculate the circumference of the circle which is displaying our images
	double circumference = 2 * M_PI * get_icon_wheel_radius();

	// Estimate the maximum by treating the path of the circle as the diameter of each image
	int max_images = floor(circumference / (m_diameter + SPACE_BETWEEN_IMAGES));

	return max_images;
}