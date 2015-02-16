#pragma once
#include <QGraphicsView>
class QCategoryView : public QGraphicsView
{
public:
	QCategoryView(QWidget* parent = 0);
	QCategoryView(QGraphicsScene* scene, QWidget* parent = 0);
	~QCategoryView();
protected:
	virtual void wheelEvent(QWheelEvent* e);
	virtual void mousePressEvent(QMouseEvent* event);
private:
	// Performs any necessary initialization
	void initialize();
};

