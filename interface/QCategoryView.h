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

	/**
	 * @fn	void QCategoryView::filesDropped(const QStringList& files);
	 *
	 * @brief	Signal sent out when a user drag & drops files on to the screen.
	 *
	 * @param	files	A list of filepaths that were dropped.
	 */

	void filesDropped(const QStringList& files);
public:
	QCategoryView(QWidget* parent = 0);
	QCategoryView(QGraphicsScene* scene, QWidget* parent = 0);
	~QCategoryView();
protected:

	/**
	 * @fn	virtual void QCategoryView::wheelEvent(QWheelEvent* e);
	 *
	 * @brief	Enables users to scroll in and out of the scene.
	 *
	 * @param [in,out]	e	If non-null, the QWheelEvent* to process.
	 */

	virtual void wheelEvent(QWheelEvent* e);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void dragEnterEvent(QDragEnterEvent *e);
	virtual void dragMoveEvent(QDragMoveEvent *e);

	/**
	 * @fn	virtual void QCategoryView::dropEvent(QDropEvent *e);
	 *
	 * @brief	Enables users to drag & drop images on to the scene, for them to be classified.
	 *
	 * @param [in,out]	e	If non-null, the QDropEvent * to process.
	 */

	virtual void dropEvent(QDropEvent *e);
private:

	/**
	 * @fn	void QCategoryView::initialize();
	 *
	 * @brief	Initialises the user interface.
	 * 			Sets the scene to be draggable, disables scroll bars, sets background colour etc.
	 */

	void initialize();
};

