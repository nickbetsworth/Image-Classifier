#ifndef IMAGECLASSIFIERWINDOW_H
#define IMAGECLASSIFIERWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QVector>
#include <QMap>
#include <QGraphicsScene>
#include <QAction>
#include "ui_imageclassifierwindow.h"
#include "QWheelDisplay.h"
#include "ImageClass.h"
#include "ImageClassifier.h"
#include "ImageClassifierRF.h"
#include "NodePositioner.h"
#include "ClassifierManager.h"

enum class BrowseState {
	CLASSES, CLASS, IMAGE
};

class ImageClassifierWindow : public QMainWindow
{
	Q_OBJECT

public:
	ImageClassifierWindow(ClassifierManager* mananger, QWidget *parent = 0);
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

	ClassifierManager* m_manager;

	/** @brief	Stores the class which is currently being viewed */
	ImageClass* m_current_class;

	/** @brief	Stores all graphics items which display the relationship between classes. */
	QGraphicsScene* m_scene_classes;
	/** @brief	Stores all graphics items which display the relationship between images within a class. */
	QGraphicsScene* m_scene_class;
	/** @brief	This scene is used to display a single image. */
	QGraphicsScene* m_scene_image;

	/** @brief	Stores the UI elements that represent each ImageClass. */
	QVector<QWheelDisplay*> m_wheels;
	/** @brief	Maps from ImageClass to its respective UI element. */
	QMap<ImageClass*, QCategoryDisplayer*> m_class_to_displayer;

	BrowseState m_state;

	QVector<QImageDisplayer*> m_image_displayers;
	QMap<Image*, QImageDisplayer*> m_image_to_displayer;

	// Stores a map from each class to the list of new images
	QMap<ImageClass*, QList<Image*>> m_new_image_map;

	QPointF m_scene_classes_pos;
	QPointF m_scene_class_pos;

	
	void setup_classes();
	void highlight_classes();

	vector<ImageClass*>& get_image_classes();
	const vector<ImageClass*>& get_image_classes() const;
	ImageClassifier* get_classifier();
};

#endif // IMAGECLASSIFIERWINDOW_H
