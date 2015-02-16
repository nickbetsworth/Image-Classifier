#ifndef IMAGECLASSIFIERWINDOW_H
#define IMAGECLASSIFIERWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QGraphicsScene>
#include <QAction>
#include "ui_imageclassifierwindow.h"
#include "QWheelDisplay.h"
#include "ImageClass.h"
#include "ImageClassifier.h"
#include "ImageClassifierRF.h"
#include "NodePositioner.h"

enum class BrowseState {
	CLASSES, CLASS, IMAGE
};

class ImageClassifierWindow : public QMainWindow
{
	Q_OBJECT

public:
	ImageClassifierWindow(QWidget *parent = 0);
	~ImageClassifierWindow();
public slots:
	/**
	
	 */
	void imageClicked(Image* image, bool rightClick);
	void categoryClicked(ImageClass* root_class);
	void menuBarClicked(QAction* action);
protected:
	void keyPressEvent(QKeyEvent* e);
	void setState(BrowseState state);
private:
	/** @brief	The main user interface. */
	Ui::ImageClassifierWindowClass ui;

	vector<ImageClass*> m_classes;
	ImageClass* m_current_class;

	/** @brief	Stores all graphics items which display the relationship between classes. */
	QGraphicsScene* m_scene_classes;
	/** @brief	Stores all graphics items which display the relationship between images within a class. */
	QGraphicsScene* m_scene_class;

	QGraphicsScene* m_scene_image;

	// Predicts new data coming in
	ImageClassifierRF* m_rf_classifier;
	/** @brief	In charge of classifying images. */
	ImageClassifier* m_classifier;
	/** @brief	In charge of positioning the classes on screen. */
	NodePositioner* m_cp;

	/** @brief	Stores the UI elements that represent each ImageClass. */
	//vector<QCategoryDisplayer*> m_displayers;
	vector<QWheelDisplay*> m_wheels;
	/** @brief	Maps from ImageClass to its respective UI element. */
	map<ImageClass*, QCategoryDisplayer*> m_class_to_displayer;
	/** @brief	Stores the UI lines that represent the edges in the graph. */
	vector<QGraphicsLineItem*> m_edges;

	BrowseState m_state;

	vector<QImageDisplayer*> m_image_displayers;
	vector<QGraphicsLineItem*> m_image_edges;
	map<Image*, QImageDisplayer*> m_image_to_displayer;
	map<QImageDisplayer*, Image*> m_displayer_to_image;

	QPointF m_scene_classes_pos;
	QPointF m_scene_class_pos;

	void setup_classes(ImageClass* root_class);
};

#endif // IMAGECLASSIFIERWINDOW_H
