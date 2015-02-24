#include "ImageSelector.h"
#include "imageclassifierwindow.h"
#include "LoadingScreen.h"
#include <QFileDialog>
#include <QErrorMessage>
#include <QtConcurrent/QtConcurrentRun>
#include <iostream>

using namespace std;

ImageSelector::ImageSelector(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Image Selection");

	ui.lstImages->setIconSize(QSize(60, 60));
	ui.lstImages->setViewMode(QListView::ViewMode::ListMode);
	
	//ui.lstImages->addAction(deleteAction);
	// Connect the add button
	connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addImageClicked()));
	// Connect the run button
	connect(ui.btnRun, SIGNAL(clicked()), this, SLOT(runClicked()));
	// Connect the add start and finish event of the image lister
	connect(ui.lstImages, SIGNAL(addingStarted()), this, SLOT(addingStarted()));
	connect(ui.lstImages, SIGNAL(addingFinished()), this, SLOT(addingFinished()));
	connect(ui.lstImages, SIGNAL(listChanged()), this, SLOT(listChanged()));
}

ImageSelector::~ImageSelector()
{
}

// Open up the file loader
void ImageSelector::addImageClicked() {
	addingStarted();

	QStringList image_files = QFileDialog::getOpenFileNames(this, "Select images for clustering process", "C:/data/ProjectImages/FlickrDownloads/", "Images(*.png *.jpg *.gif)");
	QtConcurrent::run(ui.lstImages, &QImageLister::add_files, image_files);
	//ui.lstImages->add_files(image_files)

	addingFinished();
}

void ImageSelector::runClicked() {
	ui.btnRun->setDisabled(true);
	ui.btnRun->setText("Clustering Images..");
	ui.btnRun->update();

	if (ui.lstImages->get_image_files().size() > 0) {
		ImageClassifierWindow* win = new ImageClassifierWindow(ui.lstImages->get_image_files());
		win->showMaximized();
		this->hide();
	}
	else {

	}
	
}

void ImageSelector::addingStarted() {
	// Temporarily disable the add image button
	// And rename it to let the user know images are being added
	ui.btnAdd->setText("Adding images..");
	ui.btnAdd->setDisabled(true);
}

void ImageSelector::listChanged() {
	// Update the image counter
	ui.lblImageCount->setText("Image Count: " + QString::number(ui.lstImages->get_image_files().size()));
}

void ImageSelector::addingFinished() {
	// Re-enable the button
	ui.btnAdd->setText("Add Images");
	ui.btnAdd->setDisabled(false);
}