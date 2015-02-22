#pragma once
#include <QListWidget>
#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QPoint>

/**
 * @class	QImageLister
 *
 * @brief	QImageLister manages a list of images that are to be loaded in.
 * 			It is capable of accepting drag and drop events from explorer windows.
 */

class QImageLister : public QListWidget
{
	Q_OBJECT
public:
	QImageLister(QWidget *parent = 0);
	~QImageLister();
	bool add_file(QString file_path);

	/**
	 * @fn	QStringList QImageLister::get_image_files()
	 *
	 * @brief	Gets the list of image files which the user has provided.
	 *
	 * @return	The image files.
	 */

	QStringList get_image_files() { return m_image_files; };
signals:

	/**
	 * @fn	void QImageLister::addingStarted();
	 *
	 * @brief	A signal sent out when the user is attempting to add images.
	 */

	void addingStarted();

	/**
	 * @fn	void QImageLister::listChanged();
	 *
	 * @brief	A signal sent out when the size of the list changes (added/deleted).
	 */

	void listChanged();

	/**
	 * @fn	void QImageLister::addingFinished();
	 *
	 * @brief	A signal sent out when the list has been populated with the users images.
	 */

	void addingFinished();
protected slots:
	void deleteClicked();
protected:
	virtual void contextMenuEvent(QContextMenuEvent *e);
	virtual void dragEnterEvent(QDragEnterEvent *e);
	virtual void dragMoveEvent(QDragMoveEvent *e);
	virtual void dropEvent(QDropEvent *e);
private:
	/** @brief	Stores a list of image files the user wishes to load in. */
	QStringList m_image_files;
	QMenu* m_delete_menu;
	QAction* m_delete_action;
};

