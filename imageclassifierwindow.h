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
#include "ClassifierMananger.h"

enum class BrowseState {
	CLASSES, CLASS, IMAGE
};

class ImageClassifierWindow : public QMainWindow
{
	Q_OBJECT

public:
	ImageClassifierWindow(ClassifierMananger* mananger, QWidget *parent = 0);
	~ImageClassifierWindow();
public slots:

	void imageClicked(Image* image, bool rightClick);
	void categoryClicked(ImageClass* root_class);
	void menuBarClicked(QAction* action);
protected:
	void keyPressEvent(QKeyEvent* e);
	void setState(BrowseState state);
private:
	/** @brief	The main user interface. */
	Ui::ImageClassifierWindowClass ui;

	ImageClass* m_current_class = 0;

	/** @brief	Stores all graphics items which display the relationship between classes. */
	QGraphicsScene* m_scene_classes;
	/** @brief	Stores all graphics items which display the relationship between images within a class. */
	QGraphicsScene* m_scene_class;

	QGraphicsScene* m_scene_image;

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

	// Stores a map from each class to the list of new images
	QMap<ImageClass*, QList<Image*>> m_new_image_map;

	QPointF m_scene_classes_pos;
	QPointF m_scene_class_pos;

	Image* clicked_img1 = 0;

	ClassifierMananger* m_manager;

	void setup_classes(ImageClass* root_class);
	void highlight_classes();

	vector<ImageClass*>& get_image_classes();
	const vector<ImageClass*>& get_image_classes() const;
	ImageClassifierRF* get_classifier();
};

#endif // IMAGECLASSIFIERWINDOW_H
