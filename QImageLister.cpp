#include "QImageLister.h"
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QStyle>
#include <QApplication>
#include <QMenu>
#include <QErrorMessage>
#include <QtConcurrent/QtConcurrentRun>
#include <iostream>

using namespace std;

QImageLister::QImageLister(QWidget *parent) : QListWidget(parent)
{
	setAcceptDrops(true);

	this->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
	// Create the context menu
	m_delete_menu = new QMenu(this);
	QStyle* style = QApplication::style();
	m_delete_action = new QAction(style->standardIcon(QStyle::StandardPixmap::SP_TrashIcon), "Delete", this);
	m_delete_menu->addAction(m_delete_action);

	// Connect up the delete pressed
	connect(m_delete_action, SIGNAL(triggered()), this, SLOT(deleteClicked()));
}


QImageLister::~QImageLister()
{
	delete m_delete_action;
}

void QImageLister::contextMenuEvent(QContextMenuEvent *e) {
	QModelIndex index = this->indexAt(e->pos());

	if (index.isValid()) {
		// Store the item so we know what is to be deleted later
		m_delete_action->setData(index);
		m_delete_menu->exec(e->globalPos());
	}
	
}

void QImageLister::deleteClicked() {
	// Get the position of the click
	QModelIndex index = m_delete_action->data().toModelIndex();

	if (index.isValid()) {
		// Find the item this index corresponds to
		QListWidgetItem* item = this->item(index.row());
		
		if (item) {
			// Remove it from the array list
			m_image_files.removeAll(item->text());
			// Remove it from the list widget
			this->takeItem(index.row());

			// Let listening classes know that the list has changed
			emit listChanged();
		}
	}

	// Clear the delete actions data
	m_delete_action->setData(QModelIndex());
}

void QImageLister::add_files(QStringList file_paths) {
	emit addingStarted();
	// Stores whether any duplicates are found
	bool duplicates = false;
	// Stores whether a file of the incorrect type was provided
	bool incorrect_type = false;

	for (QString file_path : file_paths) {
		if (file_path.endsWith(".jpg", Qt::CaseInsensitive) ||
			file_path.endsWith(".png", Qt::CaseInsensitive) ||
			file_path.endsWith(".gif", Qt::CaseInsensitive)) {

			// Try to add the file
			// If this fails then the file already exists
			if (!add_file(file_path))
				duplicates = true;

		}
		else {
			incorrect_type = true;
		}
	}

	if (duplicates) {
		emit duplicateImage();
	}
	if (incorrect_type) {
		emit incorrectFormat();
	}

	

	emit addingFinished();
}

bool QImageLister::add_file(QString file_path) {
	const bool load_icon = false;

	// If the file already exists in the list, don't add it
	if (m_image_files.contains(file_path)) {
		return false;
	}
	else {
		m_image_files.push_back(file_path);

		// Load the image's icon
		if (load_icon) {
			QPixmap image = QPixmap(file_path);
			QPixmap thumbnail = image.scaled(QSize(49, 49), Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation);

			QIcon image_icon = QIcon(thumbnail);
			// Create a new list item with the icon and file path
			QListWidgetItem* item = new QListWidgetItem(image_icon, file_path, this);
		}
		else {
			QListWidgetItem* item = new QListWidgetItem(file_path, this);
		}
		

		// Let other classes know that the contents of the list has changed
		emit listChanged();

		return true;
	}
}

void QImageLister::dragEnterEvent(QDragEnterEvent *e) {
	if (e->mimeData()->hasFormat("text/uri-list")) {
		e->acceptProposedAction();

		cout << "User attempting to drag acceptable data" << endl;
	}
	else {
		cout << "User attempting to drag incorrect data type" << endl;
	}
		
}

void QImageLister::dragMoveEvent(QDragMoveEvent *e) {
	if (e->mimeData()->hasFormat("text/uri-list")) {
		e->acceptProposedAction();
	}
}

void QImageLister::dropEvent(QDropEvent *e) {
	if (e->mimeData()->hasUrls()) {
		QList<QUrl> urls = e->mimeData()->urls();
		QStringList file_paths;
		
		for (QUrl url : urls) {
			file_paths.push_back(url.toLocalFile());
		}

		QtConcurrent::run(this, &QImageLister::add_files, file_paths);
		//add_files(file_paths);
		e->acceptProposedAction();
	}
}