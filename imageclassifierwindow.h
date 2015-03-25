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
#include "ImageClass.h"
#include "ImageClassifier.h"
#include "ImageClassifierRF.h"
#include "NodePositioner.h"
#include "ClassifierManager.h"
#include "QLoadingSplashScreen.h"
#include "QCategoryDisplayer.h"

/**
 * @enum	BrowseState
 *
 * @brief	Values that represent browse states.
 * 			CLASSES - A user is browsing an overview of all classes
 * 			CLASS   - A user is browsing images within a class
 * 			IMAGE   - A user is viewing a single image
 * 			There is a browsing hierarchy of CLASSES -> CLASS -> IMAGE
 */

enum class BrowseState {
	CLASSES, CLASS, IMAGE
};

/**
 * @enum	ProgramTask
 *
 * @brief	Values that represent lengthy program tasks.
 * 			POSITIONING - The program is calculating the positions of image nodes within a class
 * 			CLASSIFYING - The program is loading in and classifying new image data
 * 			IDLE        - No lengthy operation is being performed
 */

enum class ProgramTask {
	POSITIONING, CLASSIFYING, IDLE
};

typedef map<Node, cv::Point> NodePositions;
typedef vector<NodePositioner::Edge> NodeEdges;

/**
 * @class	ImageClassifierWindow
 *
 * @brief	The main form that is in charge of managing and displaying all graphics components.
 */

class ImageClassifierWindow : public QMainWindow
{
	Q_OBJECT

public:
	ImageClassifierWindow(ClassifierManager* mananger, QWidget *parent = 0);
	~ImageClassifierWindow();
signals:

	/**
	 * @fn	void ImageClassifierWindow::updateStatus(QString status);
	 *
	 * @brief	Sends out a status update when a lengthy task is being performed.
	 * 			This is received by QLoadingSplashScreen which displays the message.
	 *
	 * @param	status	The status.
	 */

	void updateStatus(QString status);
public slots:

	/**
	 * @fn	void ImageClassifierWindow::imageClicked(Image* image, bool rightClick);
	 *
	 * @brief	Executed when a user clicks on an individual image from the class view.
	 *
	 * @param [in,out]	image	If non-null, the image.
	 * @param	rightClick   	true to right click.
	 */

	void imageClicked(Image* image, bool rightClick);

	/**
	 * @fn	void ImageClassifierWindow::classClicked(ImageClass* class_clicked);
	 *
	 * @brief	Executed when a user clicks on a class from the classes view.
	 *
	 * @param [in,out]	class_clicked	If non-null, the root class.
	 */

	void classClicked(ImageClass* class_clicked);

	/**
	 * @fn	void ImageClassifierWindow::menuBarClicked(QAction* action);
	 *
	 * @brief	Executed when a user clicks on an item in the menu bar.
	 *
	 * @param [in,out]	action	If non-null, the action.
	 */

	void menuBarClicked(QAction* action);

	/**
	 * @fn	void ImageClassifierWindow::checkStatus();
	 *
	 * @brief	Executed when a lengthy operation is taking place, to determine
	 * 			whether or not it has been complete yet.
	 */

	void checkStatus();

	/**
	 * @fn	void ImageClassifierWindow::addImages(QStringList& image_files);
	 *
	 * @brief	Called when we are given a list of file paths to be classified (via drag &amp; drop
	 * 			or file -> add new images)
	 *
	 * @param [in,out]	image_files	The files to be classified.
	 */

	void addImages(const QStringList& image_files);
protected:

	/**
	 * @fn	void ImageClassifierWindow::keyPressEvent(QKeyEvent* e);
	 *
	 * @brief	Executed when the user presses a key
	 *
	 * @param [in,out]	e	If non-null, the QKeyEvent* to process.
	 */

	void keyPressEvent(QKeyEvent* e);

	/**
	 * @fn	void ImageClassifierWindow::setState(BrowseState state);
	 *
	 * @brief	Sets the current state of the program, with respect to BrowseState.
	 * 			This will swap the graphics view to the correct scene, and also
	 * 			perform additional operations such as moving the view point to the
	 * 			position it was at whilst last in this BrowseState. 
	 *
	 * @param	state	The state.
	 */

	void setState(BrowseState state);
private:

	/**
	 * @fn	void ImageClassifierWindow::setup_classes();
	 *
	 * @brief	Sets up and positions each of the classes/clusters.
	 */

	void setup_classes();

	/**
	 * @fn	void ImageClassifierWindow::render_class();
	 *
	 * @brief	Renders the class that was last clicked by the user.
	 * 			This is typically called upon the completion of calculate_image_positions,
	 * 			to prevent the GUI from locking up whilst the calculations are performed.
	 */

	void render_class();

	/**
	 * @fn	NodePositioner* ImageClassifierWindow::calculate_image_positions(ImageClass* image_class);
	 *
	 * @brief	Calculates the positions of each image for the specified image class.
	 * 			This method should be called in a separate thread to the GUI, to prevent
	 * 			the interface from becoming unresponsive.
	 *
	 * @param [in,out]	image_class	If non-null, the image class to calculate the positions for.
	 *
	 * @return	A NodePositioner containing the edges and positions of the class.
	 */

	NodePositioner* calculate_image_positions(ImageClass* image_class);

	/**
	 * @fn	void ImageClassifierWindow::classify_images(QStringList image_files);
	 *
	 * @brief	Loads and inserts images from the specified file paths via classification.
	 *
	 * @param	image_files	File paths to the image files to be added.
	 */

	void classify_images(QStringList image_files);

	/**
	 * @fn	void ImageClassifierWindow::highlight_classes();
	 *
	 * @brief	Generates a ring around any classes that contain new images.
	 */

	void highlight_classes();

	/**
	 * @fn	void ImageClassifierWindow::remove_highlight(ImageClass* image_class);
	 *
	 * @brief	Removes the highlight around the specified image class.
	 *
	 */

	void remove_highlight(ImageClass* image_class);

	/**
	 * @fn	void ImageClassifierWindow::update_class(ImageClass* image_class);
	 *
	 * @brief	Updates the class icon and neighbours for the given class.
	 *
	 */

	void update_class(ImageClass* image_class);

	vector<ImageClass*>& get_image_classes();
	const vector<ImageClass*>& get_image_classes() const;

	ImageClassifier* get_classifier();

	/**
	 * @fn	void ImageClassifierWindow::start_task(ProgramTask task);
	 *
	 * @brief	Starts the specified ProgramTask.
	 * 			This involves disabling interaction with the program, and opening
	 * 			up a splash screen which informs the user that the program
	 * 			is loading/calculating something.
	 *
	 * @param	task	The task being performed.
	 */

	void start_task(ProgramTask task);

	/** @brief	The main user interface. */
	Ui::ImageClassifierWindowClass ui;

	/** @brief	The manager ties together all of the library components.  
	/** This provides us a convenient interface to clustering and classification. */
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

	/** @brief	Stores the current browsing state of the program.  
	/** This is essentially how deep the user is in the hierarchy of CLASSES -> CLASS -> IMAGE */
	BrowseState m_state;
	/** @brief	Stores whether or not an image has been removed whilst   
	/** the user was browsing inside the current class. */
	bool m_image_removed;

	QVector<QImageDisplayer*> m_image_displayers;
	QMap<Image*, QImageDisplayer*> m_image_to_displayer;

	/** @brief	Stores a map from each class to the list of new images. */
	QMap<ImageClass*, QList<Image*>> m_new_image_map;

	/**
	 * @brief	Store the previous positions of the graphics view when the user was last in these
	 * 			states.
	 */
	QPointF m_scene_classes_pos;
	QPointF m_scene_class_pos;

	/** @brief	Periodically checks the status of the program,  
	/** when a lengthy operation is being performed. */
	QTimer* m_status_checker;
	/** @brief	The splash screen that is shown when a lengthy operation is being performed. */
	QLoadingSplashScreen* m_loading_screen;
	/** @brief	Objects that store the status and result of lengthy tasks. */
	QFuture<NodePositioner*> m_positioner;
	QFuture<void> m_classifierProcess;
	/** @brief	Stores the current task that is being performed. */
	ProgramTask m_current_task;
};

#endif // IMAGECLASSIFIERWINDOW_H
