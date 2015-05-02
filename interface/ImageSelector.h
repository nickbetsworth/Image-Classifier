#pragma once
#include <QWidget>
#include "ui_ImageSelector.h"
#include "Image.h"
#include "ClassifierManager.h"
#include "QLoadingSplashScreen.h"
#include <QFuture>

/**
 * @class	ImageSelector
 *
 * @brief	Provides a user interface for the user to select which images they wish to cluster.
 */

class ImageSelector : public QWidget
{
	Q_OBJECT
public:
	ImageSelector(QWidget *parent = 0);
	~ImageSelector();
signals:

	/**
	 * @fn	void ImageSelector::statusUpdate(QString status);
	 *
	 * @brief	Sends out a signal to the splash screen to update its message.
	 *
	 * @param	status	The new status message.
	 */

	void statusUpdate(QString status);

	/**
	 * @fn	void ImageSelector::finishedLoading();
	 *
	 * @brief	Signals to the splash screen that loading of images has finished.
	 */

	void finishedLoading();
public slots:

	/**
	 * @fn	void ImageSelector::updateClusterCount(int count);
	 *
	 * @brief	Updates the cluster count label with the new number of clusters.
	 *
	 * @param	count	New number of clusters.
	 */

	void updateClusterCount(int count);

	/**
	 * @fn	void ImageSelector::addingStarted();
	 *
	 * @brief	Gets called when the user is attempting to add images.
	 * 			Disables add buttons on user interface.
	 */

	void addingStarted();

	/**
	 * @fn	void ImageSelector::listChanged();
	 *
	 * @brief	Gets called when the number of elements in the list has changed.
	 * 			Updates the counter in the Image Count label.
	 */

	void listChanged();

	/**
	 * @fn	void ImageSelector::addingFinished();
	 *
	 * @brief	Gets called when all images have finished loading in.
	 * 			Re-enables the disabled buttons.
	 */

	void addingFinished();

	/**
	 * @fn	void ImageSelector::duplicateImage();
	 *
	 * @brief	Gets called when a duplicate image (already on the list) is provided.
	 */

	void duplicateImage();

	/**
	 * @fn	void ImageSelector::incorrectFormat();
	 *
	 * @brief	Gets called when an image of incorrect format is drag and dropped.
	 */

	void incorrectFormat();
protected slots:

	/**
	 * @fn	void ImageSelector::addImageClicked();
	 *
	 * @brief	Gets called when the user has clicked the add image button.
	 */

	void addImageClicked();

	/**
	 * @fn	void ImageSelector::runClicked();
	 *
	 * @brief	Gets called when the user clicks the run button.
	 */

	void runClicked();

	/**
	 * @fn	void ImageSelector::managerLoaded();
	 *
	 * @brief	Called when the classifier manager has finished its processing.
	 */

	void managerLoaded();
private:
	ClassifierManager* load_classifier_manager();

	Ui::ImageSelector ui;
	QLoadingSplashScreen* m_loading_screen;
	QFuture<ClassifierManager*> m_loader;
};

