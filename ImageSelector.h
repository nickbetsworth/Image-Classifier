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
 * @brief	Provides a user interface for the user to select which images they wish
 * 			to cluster.
 */

class ImageSelector : public QWidget
{
	Q_OBJECT
public:
	ImageSelector(QWidget *parent = 0);
	~ImageSelector();
signals:
	void statusUpdate(QString status);
	void finishedLoading();
public slots:

	/**
	 * @fn	void ImageSelector::addingStarted();
	 *
	 * @brief	Gets called when the user is attempting to add images.
	 */

	void addingStarted();

	/**
	 * @fn	void ImageSelector::listChanged();
	 *
	 * @brief	Gets called when the number of elements in the list has changed.
	 * 			
	 */

	void listChanged();

	/**
	 * @fn	void ImageSelector::addingFinished();
	 *
	 * @brief	Gets called when all images have finished loading in.
	 */

	void addingFinished();

	void duplicateImage();

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

	void managerLoaded();
private:
	ClassifierManager* load_classifier_manager();

	Ui::ImageSelector ui;
	QLoadingSplashScreen* m_loading_screen;
	QFuture<ClassifierManager*> m_loader;
};

