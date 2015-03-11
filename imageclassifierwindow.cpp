#include "imageclassifierwindow.h"
#include "QCategoryDisplayer.h"
#include "QCategoryView.h"
#include "QWheelDisplay.h"
#include "ImageClustererBasic.h"
#include "ImageClustererGMM.h"
#include "ImageClassifierRF.h"
#include "ImageConversion.h"
#include "RFTest.h"
#include "Histograms.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMenuBar>
#include <QDir>
#include <QFileDialog>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QScrollBar>
#include <QtConcurrent/QtConcurrentRun>


ImageClassifierWindow::ImageClassifierWindow(ClassifierManager* mananger, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Image Classifier");

	m_manager = mananger;
	m_current_class = 0;
	m_status_checker = new QTimer(this);
	m_current_task = ProgramTask::IDLE;
	m_loading_screen = new QLoadingSplashScreen();

	connect(this, SIGNAL(updateStatus(QString)), m_loading_screen, SLOT(showMessage(const QString&)));

	// Set up the event handler for the menu bar being clicked
	connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(menuBarClicked(QAction*)));
	// Set up the event handler for the status checker
	connect(m_status_checker, SIGNAL(timeout()), this, SLOT(checkStatus()));
	// Set up the event hander for when a user drag & drops files to be classified
	std::cout << "filesDropped->addImages connection status: " << connect(ui.view, SIGNAL(filesDropped(const QStringList&)), this, SLOT(addImages(const QStringList&))) << std::endl;

	m_scene_classes = new QGraphicsScene(this);
	m_scene_class = new QGraphicsScene(this);
	m_scene_image = new QGraphicsScene(this);

	srand(time(NULL));
	
	m_state = BrowseState::CLASSES;
	
	setup_classes();

	ui.view->show();
}

ImageClassifierWindow::~ImageClassifierWindow()
{
	
}

void ImageClassifierWindow::setup_classes() {
	m_scene_classes->clear();

	NodePropertiesGraph* graph = new NodePropertiesGraph();

	// For each image category, create a component to display itself
	for (ImageClass* image_class : get_image_classes()) {
		image_class->calculate_icon();
		graph->add_node(image_class);
		// Create the icon for the category
		QCategoryDisplayer* cluster = new QCategoryDisplayer(image_class);
		// If this category contains a new image

		// Create the wheel that is displayed on mouse over
		//QWheelDisplay* wheel = new QWheelDisplay(cat);
		//m_wheels.push_back(wheel);
		m_clusters.push_back(cluster);

		// Add a mapping from the QCategoryDisplayer to ImageClass, for later use when drawing edges
		m_class_to_displayer[image_class] = cluster;
		// Add both components to the scene
		//m_scene_classes->addItem(wheel);
		m_scene_classes->addItem(cluster);

		// Create event handlers for when a user mouses in and out of a category
		//connect(cat, SIGNAL(mouseEntered()), wheel, SLOT(show()));
		//connect(cat, SIGNAL(mouseLeft()), wheel, SLOT(hide()));

		// Create an event handler for when a category is clicked on screen
		connect(cluster, SIGNAL(classClicked(ImageClass*)), this, SLOT(classClicked(ImageClass*)));
	}

	// Calculate the positions of each of the image classes
	NodePositioner* positioner = new NodePositioner(graph);
	// Use a force based layout
	// We add some padding to the total radius, so it doesn't come so close to
	// other categories
	int category_radius = QCategoryDisplayer::get_total_diameter() + 200;
	map<NodeProperties*, cv::Point> positions = positioner->get_node_positions_fmmm(category_radius, category_radius);

	// Position each category and category wheel
	for (QCategoryDisplayer* cluster : m_clusters) {
		cv::Point cv_pos = positions[cluster->get_image_class()];
		QPointF pos = QPointF(cv_pos.x, cv_pos.y);
		cluster->setPos(pos);
	}

	setState(BrowseState::CLASSES);

	delete positioner;
}

void ImageClassifierWindow::render_class() {
	NodePositioner* np = m_positioner.result();
	NodePositions positions = np->get_previous_node_positions();
	NodeEdges edges = np->get_edges();

	// Determines whether or not edges will be drawn between the nodes
	const bool edges_enabled = true;

	// Remove the hover event from the class that was clicked
	// So that the wheel is no longer displayed on class exit
	m_class_to_displayer[m_current_class]->set_hovering(false);
	// Clean up all existing sub categories
	m_scene_class->clear();
	// Reset the bounding box of the scene
	m_scene_class->setSceneRect(QRectF());

	m_image_displayers.clear();
	m_image_to_displayer.clear();

	// Loop through each of the images in the category that was clicked
	for (Image* img : m_current_class->get_images()) {
		// Create the component that will display the image
		QImageDisplayer* displayer = new QImageDisplayer(img);
		// If this image is to be highlighted
		if (m_new_image_map[m_current_class].contains(img)) {
			displayer->set_highlighted(true);
		}

		// Set up the event handler for when the image is clicked
		connect(displayer, SIGNAL(imageClicked(Image*, bool)), this, SLOT(imageClicked(Image*, bool)));

		// Create a map from the image to its display component
		// so we can easily obtain the respective objects later
		m_image_to_displayer[img] = displayer;

		// Add this display component to our list of display components
		m_image_displayers.push_back(displayer);
	}

	for (QImageDisplayer* displayer : m_image_displayers) {
		// Obtain the position for this image
		cv::Point cv_pos = positions[displayer->get_image()];
		// Convert the Point we have been given to a Qt friendly Point
		QPointF pos = QPointF(cv_pos.x, cv_pos.y);
		// Set the position of the image to the given point
		displayer->setPos(pos);
		// Add the image to the scene
		m_scene_class->addItem(displayer);
	}


	// If we are drawing edges
	if (edges_enabled) {
		// Create the pen that will be used to draw our edges
		QPen pen = QPen(Qt::PenStyle::SolidLine);
		pen.setWidth(1);
		pen.setColor(QColor(255, 255, 255));
		pen.setCosmetic(true);

		// Draw all new edge lines
		// Turn on anti-aliasing for the lines
		ui.view->setRenderHint(QPainter::Antialiasing);
		for (NodePositioner::Edge e : edges) {
			// Cast each node to an Image
			Image* image1 = static_cast<Image*>(e.node1);
			Image* image2 = static_cast<Image*>(e.node2);
			// Work out which display component each of the Images is represented by
			QImageDisplayer* class1 = m_image_to_displayer[image1];
			QImageDisplayer* class2 = m_image_to_displayer[image2];

			// Create the line from image1 to image2
			QGraphicsLineItem* line = new QGraphicsLineItem(class1->x(), class1->y(), class2->x(), class2->y());
			line->setPen(pen);
			// Draw the line behind all other graphics items
			line->setZValue(-1);

			// Add the line to the scene
			m_scene_class->addItem(line);
		}
	}

	QParallelAnimationGroup* anim_group = new QParallelAnimationGroup();

	// Animate each of the image displayers to fade in
	for (QImageDisplayer* displayer : m_image_displayers) {
		QPropertyAnimation* anim = new QPropertyAnimation(displayer, "opacity");
		anim->setDuration(500);
		anim->setStartValue(0.2);
		anim->setEndValue(1.0);
		anim->setEasingCurve(QEasingCurve::OutCubic);

		anim_group->addAnimation(anim);
	}

	// Start the animations
	anim_group->start(QAbstractAnimation::DeleteWhenStopped);

	setState(BrowseState::CLASS);

	// Get the graphics item for the root node and center the view on it
	QGraphicsItem* root_node_gfx = m_image_to_displayer[m_current_class->get_icon()];
	ui.view->centerOn(root_node_gfx);
}

void ImageClassifierWindow::start_task(ProgramTask task) {
	// If we're doing anything other than idling
	if (task != ProgramTask::IDLE) {
		// Disable the view while we attempt to render the class
		ui.view->setEnabled(false);

		// Open up a splash screen to report to the user what is being done
		m_loading_screen->show();
	}

	m_current_task = task;

	// Create a timer that will poll the status of the task
	m_status_checker->start(100);
}

void ImageClassifierWindow::checkStatus() {
	// Stores whether the job being undertaken has finished
	bool has_finished = false;

	if (m_current_task == ProgramTask::POSITIONING) {
		// Check if we have finished positioning the nodes
		if (m_positioner.isFinished()) {
			// Render the class with the positions
			this->render_class();
	
			has_finished = true;
		}
	}
	else if (m_current_task == ProgramTask::CLASSIFYING) {
		has_finished = m_classifierProcess.isFinished();

		if (has_finished) {
			// Re-evaluate the icon/neighbours of each class that has received new images
			for (ImageClass* image_class : m_manager->get_image_classes()) {
				if (m_new_image_map[image_class].size() > 0) {
					this->update_class(image_class);
				}
			}
		}
	}
	
	if (has_finished) {
		// Stop the timer which periodically runs this method
		m_status_checker->stop();
		// Re-enable the view
		ui.view->setEnabled(true);
		// Close the splash screen
		m_loading_screen->close();
		m_current_task = ProgramTask::IDLE;
	}
}

NodePositioner* ImageClassifierWindow::calculate_image_positions(ImageClass* image_class) {
	// Update the splash screen to say what we are doing
	emit updateStatus("Positioning Nodes");
	
	NodePositioner* positioner = new NodePositioner(image_class->get_graph());
	// Get the positions and edges, and make a copy of them
	NodePositions positions = NodePositions(positioner->get_node_positions_tree(image_class->get_icon(), 100, 100));
	NodeEdges edges = NodeEdges(positioner->get_edges());

	return positioner;
}

void ImageClassifierWindow::classClicked(ImageClass* class_clicked) {
	m_current_class = class_clicked;
	m_positioner = QtConcurrent::run(this, &ImageClassifierWindow::calculate_image_positions, class_clicked);
	start_task(ProgramTask::POSITIONING);
}


void ImageClassifierWindow::imageClicked(Image* image, bool rightClick) {
	if (rightClick) {
		m_image_removed = true;
		// Remove this image from the category
		m_current_class->remove_image(image);
		// Remove the image from the scene
		m_scene_class->removeItem(m_image_to_displayer[image]);
		
		// Update the scene
		m_scene_class->update();
		// If there are no images left in the class, then remove the class
		if (m_current_class->get_images().size() == 0) {
			// Remove the class from our list of classes
			m_manager->remove_class(m_current_class);
			// Remove the displayer for this class
			QCategoryDisplayer* displayer = m_class_to_displayer[m_current_class];

			m_scene_classes->removeItem(displayer);

			// Move up a level as the class we are inside is now empty
			setState(BrowseState::CLASSES);
		}
		// If the node that was removed was the root node
		else if (image == m_current_class->get_icon()) {
			// Re-calculate this classes icon
			m_current_class->calculate_icon();
		}
	}
	else {
		m_scene_image->clear();

		// DEBUGGING MESSAGES
		/*cout << image->get_filepath() << endl;
		Mat hist = get_1d_histogram(image->get_image_data(), Image::HIST_BINS);
		Mat hist_t = hist.t();
		cout << "Hist: " << hist_t << endl;*/
		// END OF DEBUGGING MESSAGES

		QPixmap* pm = new QPixmap(Conv::cvMatToQPixmap(image->get_fullres_image()));
		QGraphicsItem* item = new QGraphicsPixmapItem(*pm);
		// Center the image in the co-ordinate system
		item->setPos(QPointF(-pm->width() / 2, -pm->height() / 2));

		// Set the state here so we store the current position of the class view
		setState(BrowseState::IMAGE);

		ui.view->resetMatrix();
		m_scene_image->addItem(item);
		ui.view->centerOn(item);;
	}
}

void ImageClassifierWindow::keyPressEvent(QKeyEvent* e) {
	// When the escape key is pressed, move up one level, if possible
	if (e->key() == Qt::Key::Key_Escape) {
		if (m_state == BrowseState::CLASS) {
			setState(BrowseState::CLASSES);
		}
		else if(m_state == BrowseState::IMAGE) {
			setState(BrowseState::CLASS);
		}
		
		// Tell the graphics view to update
		ui.view->update();
	}
	else if (e->key() == Qt::Key::Key_T) {
		cout << "Re-training Random Forest" << endl;
		get_classifier()->train(get_image_classes());
		cout << "Training Finished" << endl;
	}
	else if (e->key() == Qt::Key::Key_R) {
		//ui.view->update();
	}
}

void ImageClassifierWindow::setState(BrowseState state) {
	QGraphicsScene* new_scene = 0;
	QPointF new_position;

	// If we're going deeper, from viewing all classes, inside to view a single class
	if (m_state == BrowseState::CLASSES && state == BrowseState::CLASS) {
		new_scene = m_scene_class;
		// Store the position the scene is currently centered on
		m_scene_classes_pos = ui.view->mapToScene(ui.view->viewport()->rect()).boundingRect().center();
	}
	// If we're going deeper, from viewing images in a class, to view a single image
	else if (m_state == BrowseState::CLASS && state == BrowseState::IMAGE) {
		new_scene = m_scene_image;
		// Store the position the scene is currently centered on
		m_scene_class_pos = ui.view->mapToScene(ui.view->viewport()->rect()).boundingRect().center();

	}
	// If we're moving up a level, we need to restore the last position
	else {
		// If the new state is the classes scene 
		if (state == BrowseState::CLASSES) {
			// Remove highlights from all images inside class we were just in
			// And remove the highlight from the class itself
			if (m_current_class != 0) {
				// If any images were removed whilst the user was inside the class
				if (m_image_removed) {
					this->update_class(m_current_class);
				}

				remove_highlight(m_current_class);
			}

			// Reset the flag
			m_image_removed = false;

			ui.view->setSceneRect(QRectF());
			new_scene = m_scene_classes;
			new_position = m_scene_classes_pos;
		}
		// If the new state is inside a class
		else if (state == BrowseState::CLASS) {
			new_scene = m_scene_class;
			new_position = m_scene_class_pos;
		}
	}
	
	if (new_scene != NULL) {
		// Reset the cache as there will most likely be no relevant images
		ui.view->resetCachedContent();
		ui.view->setScene(new_scene);

		// Expand the boundaries of the scene slightly, so the user can browse 
		// outside the bounds of where the images are
		// Get the bounding rect of the scene
		QRectF sceneRect = new_scene->itemsBoundingRect();
		// Enlargen it
		sceneRect.adjust(-1000, -1000, 1000, 1000);
		ui.view->setSceneRect(sceneRect);

		ui.view->update();

		m_state = state;
		
		if (!new_position.isNull())
			ui.view->centerOn(new_position);
	}
}


void ImageClassifierWindow::menuBarClicked(QAction* action) {
	// If the add image button is clicked
	if (action == ui.actionAddImage) {
		// Query the user for the images they wish to add 
		QStringList image_files = QFileDialog::getOpenFileNames(this, "Select images to add", "", "Images (*.png *.jpg *.gif)");
		this->addImages(image_files);
	}
	else if (action == ui.actionTrainClassifier) {
		m_manager->train_classifier();
	}
}

void ImageClassifierWindow::addImages(const QStringList& image_files) {
	std::cout << "Signal received" << std::endl;
	m_classifierProcess = QtConcurrent::run(this, &ImageClassifierWindow::classify_images, image_files);
	start_task(ProgramTask::CLASSIFYING);
}

void ImageClassifierWindow::classify_images(QStringList image_files) {
	updateStatus("Adding new images");

	for (QString image_file : image_files) {
		// Ensure that this is a valid image file
		if (image_file.endsWith(".jpg", Qt::CaseInsensitive) ||
			image_file.endsWith(".png", Qt::CaseInsensitive) ||
			image_file.endsWith(".gif", Qt::CaseInsensitive)) {
			// Load in the image
			Image* image = new Image(image_file.toStdString());

			// Check that the image has successfully loaded
			if (image->has_loaded()) {
				ImageClass* predicted_class = m_manager->classify_image(image);
				if (predicted_class != 0)
					m_new_image_map[predicted_class].push_back(image);
			}
		}
	}

	// Highlight all classes necessary
	highlight_classes();

	cout << image_files.size() << " new images were added" << endl;
}

void ImageClassifierWindow::highlight_classes() {
	for (ImageClass* image_class : get_image_classes()) {
		if (!m_new_image_map[image_class].isEmpty()) {
			m_class_to_displayer[image_class]->set_highlighted(true);
		}
	}
}

void ImageClassifierWindow::remove_highlight(ImageClass* image_class) {
	m_new_image_map[image_class].clear();
	m_class_to_displayer[image_class]->set_highlighted(false);
}

void ImageClassifierWindow::update_class(ImageClass* image_class) {
	if (image_class->get_image_count() > 0) {
		image_class->calculate_icon();
		m_class_to_displayer[image_class]->update_images();
	}
}
const vector<ImageClass*>& ImageClassifierWindow::get_image_classes() const {
	return m_manager->get_image_classes();
}

vector<ImageClass*>& ImageClassifierWindow::get_image_classes() {
	return m_manager->get_image_classes();
}

ImageClassifier* ImageClassifierWindow::get_classifier() {
	return m_manager->get_classifier();
}