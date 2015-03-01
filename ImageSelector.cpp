#include "ImageSelector.h"
#include "imageclassifierwindow.h"
#include "LoadingScreen.h"
#include "ClassifierMananger.h"
#include <QFileDialog>
#include <QErrorMessage>
#include <QtConcurrent/QtConcurrentRun>
#include <QSplashScreen>
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

	connect(ui.lstImages, SIGNAL(incorrectFormat()), this, SLOT(incorrectFormat()));
	connect(ui.lstImages, SIGNAL(duplicateImage()), this, SLOT(duplicateImage()));
}

ImageSelector::~ImageSelector()
{
}

// Open up the file loader
void ImageSelector::addImageClicked() {
	addingStarted();

	QStringList image_files = QFileDialog::getOpenFileNames(this, "Select images for clustering process", "C:/data/ProjectImages/FlickrDownloads/", "Images(*.png *.jpg *.gif)");
	QtConcurrent::run(ui.lstImages, &QImageLister::add_files, image_files);
	//ui.lstImages->add_files(image_files);

	addingFinished();
}

void ImageSelector::runClicked() {
	ui.btnRun->setDisabled(true);
	ui.btnRun->setText("Clustering Images..");
	QApplication::processEvents();

	if (ui.lstImages->get_image_files().size() > 0) {
		// Convert the QStringList to a vector<string>, as the image library
		// has no Qt dependency
		vector<string> std_file_paths;
		
		for (QString qs_file_path : ui.lstImages->get_image_files()) {
			std_file_paths.push_back(qs_file_path.toStdString());
		}

		ClassifierMananger* manager = new ClassifierMananger(std_file_paths);
		QPixmap splash_image = QPixmap("C:/Users/Nick/Desktop/b3.png");
		QSplashScreen* loading_screen = new QSplashScreen(splash_image);
		this->hide();
		loading_screen->show();
		loading_screen->showMessage("Loading Images..", Qt::AlignCenter, Qt::white);
		QApplication::processEvents();
		manager->load_images();
		loading_screen->showMessage("Clustering Images..", Qt::AlignCenter, Qt::white);
		QApplication::processEvents();
		manager->cluster_images(6);
		loading_screen->showMessage("Training Classifier..", Qt::AlignCenter, Qt::white);
		QApplication::processEvents();
		manager->train_classifier();
		ImageClassifierWindow* win = new ImageClassifierWindow(manager);
		loading_screen->finish(win);
		//
		win->showMaximized();
		//
	}
	else {

	}
	
}

void ImageSelector::addingStarted() {
	// Temporarily disable the add image button
	// And rename it to let the user know images are being added
	ui.btnAdd->setText("Adding images..");
	ui.btnAdd->setDisabled(true);
	ui.btnRun->setDisabled(true);
}

void ImageSelector::listChanged() {
	// Update the image counter
	ui.lblImageCount->setText("Image Count: " + QString::number(ui.lstImages->get_image_files().size()));
	//QApplication::processEvents();
}

void ImageSelector::addingFinished() {
	// Re-enable the button
	ui.btnAdd->setText("Add Images");
	ui.btnAdd->setDisabled(false);
	ui.btnRun->setDisabled(false);
}

void ImageSelector::incorrectFormat() {
	QErrorMessage* exist_error = new QErrorMessage(this);
	exist_error->showMessage("At least one of the dropped files was of the wrong format");
}

void ImageSelector::duplicateImage() {
	QErrorMessage* exist_error = new QErrorMessage(this);
	exist_error->showMessage("At least one of the dropped files was already on the list");
}