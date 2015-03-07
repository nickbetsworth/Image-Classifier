#ifndef IMAGECLASSIFIERWINDOW_H
#define IMAGECLASSIFIERWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QVector>
#include <QMap>
#include <QGraphicsScene>
#include <QAction>
#include <QTimer>
#include <QFuture>
#include "ui_imageclassifierwindow.h"
#include "QWheelDisplay.h"
#include "ImageClass.h"
#include "ImageClassifier.h"
#include "ImageClassifierRF.h"
#include "NodePositioner.h"
#include "ClassifierManager.h"
#include "QLoadingSplashScreen.h"

enum class BrowseState {
	CLASSES, CLASS, IMAGE
};

enum class ProgramTask {
	POSITIONING, CLASSIFYING, IDLE
};
typedef map<Node, Point> NodePositions;
typedef vector<NodePositioner::Edge> NodeEdges;
class ImageClassifierWindow : public QMainWindow
{
	Q_OBJECT

public:
	ImageClassifierWindow(ClassifierManager* mananger, QWidget *parent = 0);
	~ImageClassifierWindow();
signals:
	void updateStatus(QString status);
	void testSignal();
	void finished();
public slots:
	void imageClicked(Image* image, bool rightClick);
	void classClicked(ImageClass* root_class);
	void menuBarClicked(QAction* action);
	void checkStatus();
protected:
	void keyPressEvent(QKeyEvent* e);
	void setState(BrowseState state);
private slots:
	
private:
	void setup_classes();
	void render_class();
	NodePositioner* calculate_image_positions();

	void classify_images(QStringList image_files);
	void highlight_classes();

	vector<ImageClass*>& get_image_classes();
	const vector<ImageClass*>& get_image_classes() const;
	ImageClassifier* get_classifier();

	void start_task(ProgramTask task);

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
	QVector<QCategoryDisplayer*> m_clusters;
	/** @brief	Maps from ImageClass to its respective UI element. */
	QMap<ImageClass*, QCategoryDisplayer*> m_class_to_displayer;

	BrowseState m_state;

	QVector<QImageDisplayer*> m_image_displayers;
	QMap<Image*, QImageDisplayer*> m_image_to_displayer;

	// Stores a map from each class to the list of new images
	QMap<ImageClass*, QList<Image*>> m_new_image_map;

	QPointF m_scene_classes_pos;
	QPointF m_scene_class_pos;

	QFuture<NodePositioner*> m_positioner;
	QTimer* m_status_checker;
	QLoadingSplashScreen* m_loading_screen;

	QFuture<void> m_classifierProcess;

	ProgramTask m_current_task;
};

#endif // IMAGECLASSIFIERWINDOW_H
