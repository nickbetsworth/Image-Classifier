#pragma once
#include <QGraphicsObject>
#include "QCategoryDisplayer.h"
#include "ImageClass.h"

class QWheelDisplay : public QGraphicsObject
{
	Q_OBJECT
	Q_PROPERTY(double rad_scale READ get_rad_scale WRITE set_rad_scale)
public:
	QWheelDisplay(QCategoryDisplayer* category_displayer);
	~QWheelDisplay();
	QRectF boundingRect() const;
	bool is_displaying() { return m_displaying; };
	QCategoryDisplayer* get_category_displayer() { return m_category_displayer; };
	void set_displaying(bool displaying);
	void set_rad_scale(double rad_scale) { this->prepareGeometryChange();  this->m_rad_scale = rad_scale; this->update(); };
	double get_rad_scale() const { return m_rad_scale; };
public slots:
	void show();
	void hide();
protected:
	const int PADDING = 25;
	const int SPACE_BETWEEN_IMAGES = 15;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	// Calculates the maximum number of images that may be displayed in the wheel
	int get_max_images();
	// Gets the radius from the center of the wheel to the center of the icons
	double get_icon_wheel_radius() const;
	// Gets the total radius of the wheel
	double get_wheel_radius() const;
private:
	QCategoryDisplayer* m_category_displayer;
	// Stores a list of all pixmaps in this category
	vector<QPixmap*> m_pixmaps;
	// Stores the size of each image in the wheel
	int m_diameter;

	bool m_displaying;
	// The current scaling of the radius (moved between 0->1 in animation)
	double m_rad_scale;
};

