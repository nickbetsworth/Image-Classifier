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

	/**
	 * @fn	bool QImageLister::add_file(QString file_path);
	 *
	 * @brief	Adds a new file to the list.
	 *
	 * @param	file_path	Full pathname of the file.
	 *
	 * @return	true if it succeeds (is not already in the list)
	 */

	bool add_file(QString file_path);

	/**
	 * @fn	void QImageLister::add_files(QStringList file_paths);
	 *
	 * @brief	Adds multiple files to the image list.
	 *
	 * @param	file_paths	The file paths.
	 */

	void add_files(QStringList file_paths);

	/**
	 * @fn	QStringList QImageLister::get_image_files()
	 *
	 * @brief	Gets the list of image paths from this list.
	 *
	 * @return	The image files.
	 */

	QStringList get_image_files() { return m_image_files; };

	/**
	 * @fn	int QImageLister::get_image_count()
	 *
	 * @brief	Gets the number of images in the list.
	 *
	 * @return	The image count.
	 */

	int get_image_count() { return m_image_files.size(); };
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

	/**
	 * @fn	void QImageLister::incorrectFormat();
	 *
	 * @brief	Sends out a signal when an image is of the wrong format.
	 */

	void incorrectFormat();

	/**
	 * @fn	void QImageLister::duplicateImage();
	 *
	 * @brief	Sends out this signal if a user tries adding the same image more than once.
	 */

	void duplicateImage();
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

