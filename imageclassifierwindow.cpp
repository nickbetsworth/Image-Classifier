#include "imageclassifierwindow.h"
#include "QCategoryDisplayer.h"
#include "QCategoryView.h"
#include "QWheelDisplay.h"
#include "ImageClassifierBasic.h"
#include "ImageClassifierGMM.h"
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


ImageClassifierWindow::ImageClassifierWindow(QStringList image_list, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Image Classifier");

	connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(menuBarClicked(QAction*)));
	m_scene_classes = new QGraphicsScene(this);
	m_scene_class = new QGraphicsScene(this);
	m_scene_image = new QGraphicsScene(this);

	srand(time(NULL));
	
	m_state = BrowseState::CLASSES;

	cout << "Loading images.." << endl;
	vector<Image*> images;
	/*
	//QDir dir = QDir("C:/data/ProjectImages/FlickrDownloads/");
	QDir dir = QDir("C:/data/ProjectImages/Test/");
	//QDir dir = QDir("C:/data/ProjectImages/GGImages/");
	QStringList filters;
	filters << "*.png" << "*.jpg" << "*.gif";

	QFileInfoList list = dir.entryInfoList(filters);

	
	for (QFileInfo info : list) {
		string filepath = info.absoluteFilePath().toStdString();
		Image* image = new Image(filepath);
		if (image->has_loaded())
			images.push_back(image);
		else
			cout << "Unable to load image: " << filepath << endl;
	}*/

	for (QString image_file : image_list) {
		string filepath = image_file.toStdString();
		Image* image = new Image(filepath);
		if (image->has_loaded())
			images.push_back(image);
		else
			cout << "Unable to load image: " << filepath << endl;
	}

	cout << "Loaded in " << images.size() << " images" << endl;
	
	//m_classifier = new ImageClassifierBasic(image_classes);
	int n_clusters = 6;
	cout << "Using GMM with " << n_clusters << " clusters." << endl;
	m_classifier = new ImageClassifierGMM(images, n_clusters);
	m_classifier->classify_images();
	m_classes = m_classifier->get_image_classes();

	// Use our clustered data as training data for a new random forest
	m_rf_classifier = new ImageClassifierRF();
	m_rf_classifier->train(m_classes);
	
	//RFTest rft = RFTest(m_classes);
	//rft.perform_test(20);

	vector<NodeProperties*> node_props;
	for (NodeProperties* node : m_classifier->get_image_classes()) {
		node_props.push_back(node);
	}
	
	this->setup_classes(m_classes.front());

	// Set it so that we can drag through the scene with the mouse
	ui.view->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
	// Remove the scroll bars as they are no longer needed due to the hand drag option
	ui.view->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	ui.view->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	ui.view->setCacheMode(QGraphicsView::CacheBackground);
	setState(BrowseState::CLASSES);
	ui.view->show();
}

ImageClassifierWindow::~ImageClassifierWindow()
{
	delete m_classifier;
}

void ImageClassifierWindow::imageClicked(Image* image, bool rightClick) {
	if (rightClick) {
		// Remove this image from the category
		m_current_class->remove_image(image);
		// Remove the image from the scene
		m_scene_class->removeItem(m_image_to_displayer[image]);
		// Update the scene
		m_scene_class->update();
		// If there are no images left in the class, then remove the class
		if (m_current_class->get_images().size() == 0) {
			m_classes.erase(remove(m_classes.begin(), m_classes.end(), m_current_class), m_classes.end());
			// Remove the displayer for this class
			QCategoryDisplayer* displayer = m_class_to_displayer[m_current_class];
			m_scene_classes->removeItem(displayer);
			
			// Move up a level as the class we are inside is now empty
			setState(BrowseState::CLASSES);
		}
	}
	else {
		if (clicked_img1 == 0) {
			clicked_img1 = image;
		}
		else {
			cout << clicked_img1->get_filepath() << ": " << clicked_img1->get_histogram() << endl;
			cout << image->get_filepath() << ": "  << image->get_histogram() << endl;
			double val = clicked_img1->calculate_distance(image);
			cout << "Distance: " << val << endl;
			clicked_img1 = 0;
		}
	}
	/*
	else {
		m_scene_image->clear();

		// DEBUGGING MESSAGES
		cout << image->get_filepath() << endl;
		Mat hist = get_1d_histogram(image->get_image_data(), Image::HIST_BINS);
		Mat hist_t = hist.t();
		cout << "Hist: " << hist_t << endl;
		// END OF DEBUGGING MESSAGES

		QPixmap* pm = new QPixmap(Conv::cvMatToQPixmap(image->get_fullres_image()));
		QGraphicsItem* item = new QGraphicsPixmapItem(*pm);
		ui.view->resetMatrix();
		m_scene_image->addItem(item);

		setState(BrowseState::IMAGE);
	}*/

	
}

void ImageClassifierWindow::categoryClicked(ImageClass* class_clicked) {
	const bool edges_enabled = true;

	// Remove the hover event from the class that was clicked
	// So that the wheel is no longer displayed on class exit
	m_class_to_displayer[class_clicked]->set_hovering(false);
	// Clean up all existing sub categories
	m_scene_class->clear();
	// Reset the bounding box of the scene
	m_scene_class->setSceneRect(QRectF());

	m_image_displayers.clear();
	m_image_to_displayer.clear();

	// Loop through each of the images in the category that was clicked
	for (Image* img : class_clicked->get_images()) {
		QImageDisplayer* displayer = new QImageDisplayer(img);
		if (m_new_image_map[class_clicked].contains(img)) {
			displayer->set_highlighted(true);
		}

		connect(displayer, SIGNAL(imageClicked(Image*, bool)), this, SLOT(imageClicked(Image*, bool)));
		m_image_to_displayer[img] = displayer;
		m_displayer_to_image[displayer] = img;
		m_image_displayers.push_back(displayer);
	}

	// Calculate the positions via the histogram of each class
	vector<NodeProperties*> node_props;
	for (NodeProperties* node : class_clicked->get_images()) {
		node_props.push_back(node);
	}
	NodePositioner* positioner = new NodePositioner(class_clicked->get_graph());

	map<NodeProperties*, Point> positions = positioner->get_node_positions(class_clicked->get_icon(), 0.6);
	// Position each image displayer
	for (QImageDisplayer* displayer : m_image_displayers) {
		Image* displayer_class = m_displayer_to_image[displayer];

		Point cv_pos = positions[displayer_class];
		QPointF pos = QPointF(cv_pos.x, cv_pos.y);
		//QPointF pos = QPointF(0, 0);
		displayer->setPos(pos);
		m_scene_class->addItem(displayer);
	}

	// Clear up all existing edge lines
	for (QGraphicsLineItem* item : m_image_edges) {
		if (edges_enabled)
			m_scene_class->removeItem(item);
	}

	m_image_edges.clear();

	// Create the pen that will be used to draw our lines
	QPen pen = QPen(Qt::PenStyle::SolidLine);
	pen.setWidth(1);
	pen.setColor(QColor(140, 140, 140));
	pen.setCosmetic(true);

	// Draw all new edge lines
	// Turn on anti-aliasing for the lines
	ui.view->setRenderHints(QPainter::Antialiasing);
	for (NodePositioner::Edge e : positioner->get_edges()) {
		// Cast each node to an ImageClass
		Image* image1 = static_cast<Image*>(e.node1);
		Image* image2 = static_cast<Image*>(e.node2);
		QImageDisplayer* class1 = m_image_to_displayer[image1];
		QImageDisplayer* class2 = m_image_to_displayer[image2];

		QGraphicsLineItem* line = new QGraphicsLineItem(class1->x(), class1->y(), class2->x(), class2->y());
		line->setPen(pen);
		// Draw the line behind all other graphics items
		line->setZValue(-1);
		if (edges_enabled)
			m_scene_class->addItem(line);
		m_edges.push_back(line);
	}

	setState(BrowseState::CLASS);

	// Calculate center point of scene
	QPointF center_point = m_scene_class->itemsBoundingRect().center();
	ui.view->centerOn(center_point);

	QParallelAnimationGroup* anim_group = new QParallelAnimationGroup();

	for (QImageDisplayer* displayer : m_image_displayers) {
		Image* displayer_class = m_displayer_to_image[displayer];
		
		Point cv_pos = positions[displayer_class];
		QPointF end_pos = QPointF(cv_pos.x, cv_pos.y);
		QPropertyAnimation* anim = new QPropertyAnimation(displayer, "opacity");
		anim->setDuration(500);
		anim->setStartValue(0.2);
		anim->setEndValue(1.0);
		//anim->setStartValue(center_point);
		//anim->setEndValue(end_pos);
		anim->setEasingCurve(QEasingCurve::OutCubic);

		anim_group->addAnimation(anim);
	}

	// Start all animations
	anim_group->start(QAbstractAnimation::DeleteWhenStopped);
	
	// Get the graphics item for the root node
	QGraphicsItem* root_node_gfx = m_image_to_displayer[class_clicked->get_images().front()];
	// Center the view on the root node
	ui.view->centerOn(root_node_gfx);
	// Perform some clean up
	delete positioner;

	m_current_class = class_clicked;


	// Get the bounding rect of the scene
	QRectF sceneRect = m_scene_class->itemsBoundingRect();
	sceneRect.adjust(-1000, -1000, 1000, 1000);
	ui.view->setSceneRect(sceneRect);
}

void ImageClassifierWindow::setup_classes(ImageClass* root_class) {
	m_scene_classes->clear();

	NodePropertiesGraph* graph = new NodePropertiesGraph();

	// For each image category, create a component to display itself
	for (ImageClass* image_class : m_classes) {
		image_class->calculate_icon();
		graph->add_node(image_class);
		// Create the icon for the category
		QCategoryDisplayer* cat = new QCategoryDisplayer(image_class);
		// If this category contains a new image
		
		// Create the wheel that is displayed on mouse over
		QWheelDisplay* wheel = new QWheelDisplay(cat);


		m_wheels.push_back(wheel);
		// Add a mapping from the QCategoryDisplayer to ImageClass, for later use when drawing edges
		m_class_to_displayer[image_class] = cat;
		// Add both components to the scene
		m_scene_classes->addItem(wheel);
		m_scene_classes->addItem(cat);

		// Create event handlers for when a user mouses in and out of a category
		connect(cat, SIGNAL(mouseEntered()), wheel, SLOT(show()));
		connect(cat, SIGNAL(mouseLeft()), wheel, SLOT(hide()));

		// Create an event handler for when a category is clicked on screen
		connect(cat, SIGNAL(categoryClicked(ImageClass*)), this, SLOT(categoryClicked(ImageClass*)));
	}

	NodePositioner* positioner = new NodePositioner(graph);
	map<NodeProperties*, Point> positions = positioner->get_node_positions(m_classes.front(), 1);

	// Position each category
	for (QWheelDisplay* wheel : m_wheels) {
		QCategoryDisplayer* displayer = wheel->get_category_displayer();

		Point cv_pos = positions[displayer->get_image_class()];
		QPointF pos = QPointF(cv_pos.x, cv_pos.y);
		displayer->setPos(pos);
		wheel->setPos(pos);
	}

	// Clear up all existing edge lines
	for (QGraphicsLineItem* item : m_edges) {
		m_scene_classes->removeItem(item);
	}

	m_edges.clear();

	// Create the pen that will be used to draw our lines
	QPen pen = QPen(Qt::PenStyle::SolidLine);
	pen.setWidth(1);
	pen.setColor(QColor(140, 140, 140));
	pen.setCosmetic(true);

	// Draw all new edge lines
	// Turn on anti-aliasing for the lines
	ui.view->setRenderHints(QPainter::Antialiasing);
	for (NodePositioner::Edge e : positioner->get_edges()) {
		ImageClass* image_class1 = static_cast<ImageClass*>(e.node1);
		ImageClass* image_class2 = static_cast<ImageClass*>(e.node2);
		QCategoryDisplayer* class1 = m_class_to_displayer[image_class1];
		QCategoryDisplayer* class2 = m_class_to_displayer[image_class2];

		QGraphicsLineItem* line = new QGraphicsLineItem(class1->x(), class1->y(), class2->x(), class2->y());
		line->setPen(pen);
		// Draw the line behind all other graphics items
		line->setZValue(-1);
		m_scene_classes->addItem(line);
		m_edges.push_back(line);
	}

	delete positioner;
}


void ImageClassifierWindow::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key::Key_Escape) {
		if (m_state == BrowseState::CLASS) {
			setState(BrowseState::CLASSES);
			//ui.view->setScene(m_scene_classes);
		}
		else if(m_state == BrowseState::IMAGE) {
			setState(BrowseState::CLASS);
			//ui.view->setScene(m_scene_class);
		}
		
		ui.view->update();
	}
	else if (e->key() == Qt::Key::Key_T) {
		cout << "Re-training Random Forest" << endl;
		m_rf_classifier->train(m_classes);
		cout << "Training Finished" << endl;

		// Re-position the classes again incase there was any slight changes
		//m_cp = new NodePositionerHist(m_classifier->get_image_classes());
		//this->setup_classes(m_classes.front());
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
			if (m_current_class != 0) {
				m_new_image_map[m_current_class].clear();
				m_class_to_displayer[m_current_class]->set_highlighted(false);
			}

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
		ui.view->update();

		m_state = state;
		
		if (!new_position.isNull())
			ui.view->centerOn(new_position);
	}
}


void ImageClassifierWindow::menuBarClicked(QAction* action) {
	// If the add image button is clciked
	if (action == ui.actionAddImage) {
		QStringList image_files = QFileDialog::getOpenFileNames(this, "Select images to add", "", "Images (*.png *.jpg *.gif)");
		// For each of the images selected
		for (QString image_file : image_files) {
			// Load in the image
			Image* image = new Image(image_file.toStdString());

			// Check that the image has successfully loaded
			if (image->has_loaded()) {
				ImageClass* predicted_class = m_rf_classifier->predict(image);

				m_new_image_map[predicted_class].push_back(image);

				predicted_class->add_image(image);
			}
		}

		// Highlight all classes necessary
		highlight_classes();

		cout << image_files.size() << " new images were added" << endl;
	}
}

void ImageClassifierWindow::highlight_classes() {
	for (ImageClass* image_class : m_classes) {
		if (!m_new_image_map[image_class].isEmpty()) {
			m_class_to_displayer[image_class]->set_highlighted(true);
		}
	}
}