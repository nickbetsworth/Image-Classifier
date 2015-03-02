#include "ImageSelector.h"
#include "imageclassifierwindow.h"
#include "LoadingScreen.h"
#include "ClassifierManager.h"
#include <QFileDialog>
#include <QErrorMessage>
#include <QtConcurrent/QtConcurrentRun>
#include <QSplashScreen>
#include "QLoadingSplashScreen.h"
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
	if (ui.lstImages->get_image_files().size() > 0) {
		// Create the loading screen, and set up communication between this class and the splash screen
		m_loading_screen = new QLoadingSplashScreen();
		connect(this, SIGNAL(statusUpdate(QString)), m_loading_screen, SLOT(showMessage(const QString&)));
		connect(this, SIGNAL(finishedLoading()), m_loading_screen, SLOT(finished()));
		// Let us know when the loading has finished, 
		// so we do not block the splash screen's draw thread
		connect(this, SIGNAL(finishedLoading()), this, SLOT(managerLoaded()));

		// Hide the current image selection window and show the splash screen
		this->hide();
		m_loading_screen->show();

		// Perform all of the loading in a separate thread
		m_loader = QtConcurrent::run(this, &ImageSelector::load_classifier_manager);
	}
	else {
		QErrorMessage* error = new QErrorMessage(this);
		error->showMessage("You must specify at least one image");
	}
}

ClassifierManager* ImageSelector::load_classifier_manager() {
	emit statusUpdate("Loading Images");
	// Convert the QStringList to a vector<string>, as the image library
	// has no Qt dependency
	vector<string> std_file_paths;

	for (QString qs_file_path : ui.lstImages->get_image_files()) {
		std_file_paths.push_back(qs_file_path.toStdString());
	}

	ClassifierManager* manager = new ClassifierManager();
	manager->load_images(std_file_paths);
	emit statusUpdate("Clustering Images");
	manager->cluster_images(6);
	emit statusUpdate("Training Classifier");
	manager->train_classifier();

	emit finishedLoading();
	return manager;
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
}

void ImageSelector::addingFinished() {
	// Re-enable the button
	ui.btnAdd->setText("Add Images");
	ui.btnAdd->setDisabled(false);
	ui.btnRun->setDisabled(false);
}

void ImageSelector::incorrectFormat() {
	QErrorMessage* format_error = new QErrorMessage(this);
	format_error->showMessage("At least one of the dropped files was of the wrong format");
	delete format_error;
}

void ImageSelector::duplicateImage() {
	QErrorMessage* duplicate_error = new QErrorMessage(this);
	duplicate_error->showMessage("At least one of the dropped files was already on the list");
	delete duplicate_error;
}

void ImageSelector::managerLoaded() {
	ClassifierManager* manager = m_loader.result();
	ImageClassifierWindow* win = new ImageClassifierWindow(manager);
	
	win->showMaximized();
}