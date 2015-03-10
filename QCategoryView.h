#pragma once
#include <QGraphicsView>

/**
 * @class	QCategoryView
 *
 * @brief	QCategoryView represents the main graphics component which the user sees on screen.
 * 			This component is in charge of rendering all of the necessary graphics.
 */

class QCategoryView : public QGraphicsView
{
	Q_OBJECT
signals:
	void filesDropped(const QStringList& files);
public:
	QCategoryView(QWidget* parent = 0);
	QCategoryView(QGraphicsScene* scene, QWidget* parent = 0);
	~QCategoryView();
protected:
	virtual void wheelEvent(QWheelEvent* e);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void dragEnterEvent(QDragEnterEvent *e);
	virtual void dragMoveEvent(QDragMoveEvent *e);
	virtual void dropEvent(QDropEvent *e);
private:
	// Performs any necessary initialization
	void initialize();
};

