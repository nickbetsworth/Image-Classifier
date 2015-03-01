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


ImageClassifierWindow::ImageClassifierWindow(ClassifierManager* mananger, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Image Classifier");

	m_manager = mananger;
	m_current_class = 0;

	connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(menuBarClicked(QAction*)));
	m_scene_classes = new QGraphicsScene(this);
	m_scene_class = new QGraphicsScene(this);
	m_scene_image = new QGraphicsScene(this);

	srand(time(NULL));
	
	m_state = BrowseState::CLASSES;
	
	setup_classes();

	// Set it so that we can drag through the scene with the mouse
	ui.view->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
	// Remove the scroll bars as they are no longer needed due to the hand drag option
	ui.view->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	ui.view->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	ui.view->setCacheMode(QGraphicsView::CacheBackground);

	ui.view->show();
}

ImageClassifierWindow::~ImageClassifierWindow()
{
	
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
			// Remove the class from our list of classes
			m_manager->remove_class(m_current_class);
			// Remove the displayer for this class
			QCategoryDisplayer* displayer = m_class_to_displayer[m_current_class];

			m_scene_classes->removeItem(displayer);
			
			// Move up a level as the class we are inside is now empty
			setState(BrowseState::CLASSES);
		}
	}
	/*else {
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
	}*/
	
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
		ui.view->resetMatrix();
		m_scene_image->addItem(item);

		setState(BrowseState::IMAGE);
	}

	
}

void ImageClassifierWindow::setup_classes() {
	m_scene_classes->clear();

	NodePropertiesGraph* graph = new NodePropertiesGraph();

	// For each image category, create a component to display itself
	for (ImageClass* image_class : get_image_classes()) {
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

	// Calculate the positions of each of the image classes
	NodePositioner* positioner = new NodePositioner(graph);
	// Use a force based layout
	map<NodeProperties*, Point> positions = positioner->get_node_positions_fmmm(200, 200);

	// Position each category and category wheel
	for (QWheelDisplay* wheel : m_wheels) {
		QCategoryDisplayer* displayer = wheel->get_category_displayer();

		Point cv_pos = positions[displayer->get_image_class()];
		QPointF pos = QPointF(cv_pos.x, cv_pos.y);
		displayer->setPos(pos);
		wheel->setPos(pos);
	}

	setState(BrowseState::CLASSES);

	delete positioner;
}

void ImageClassifierWindow::categoryClicked(ImageClass* class_clicked) {
	// Determines whether or not edges will be drawn between the nodes
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
		// Create the component that will display the image
		QImageDisplayer* displayer = new QImageDisplayer(img);
		// If this image is to be highlighted
		if (m_new_image_map[class_clicked].contains(img)) {
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

	// Calculate the positions of each of the image elements
	NodePositioner* positioner = new NodePositioner(class_clicked->get_graph());
	// Use a tree layout, the root node is the ImageClass's icon
	map<NodeProperties*, Point> positions = positioner->get_node_positions_tree(class_clicked->get_icon(), 100, 100);

	for (QImageDisplayer* displayer : m_image_displayers) {
		// Obtain the position for this image
		Point cv_pos = positions[displayer->get_image()];
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
		pen.setColor(QColor(140, 140, 140));
		pen.setCosmetic(true);

		// Draw all new edge lines
		// Turn on anti-aliasing for the lines
		ui.view->setRenderHints(QPainter::Antialiasing);
		for (NodePositioner::Edge e : positioner->get_edges()) {
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

	// Expand the boundaries of the scene slightly, so the user can browse 
	// outside the bounds of where the images are
	// Get the bounding rect of the scene
	QRectF sceneRect = m_scene_class->itemsBoundingRect();
	// Enlargen it
	sceneRect.adjust(-1000, -1000, 1000, 1000);
	ui.view->setSceneRect(sceneRect);

	// Get the graphics item for the root node and center the view on it
	QGraphicsItem* root_node_gfx = m_image_to_displayer[class_clicked->get_icon()];
	ui.view->centerOn(root_node_gfx);

	m_current_class = class_clicked;
	setState(BrowseState::CLASS);

	delete positioner;
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
	// If the add image button is clicked
	if (action == ui.actionAddImage) {
		// Query the user for the images they wish to add 
		QStringList image_files = QFileDialog::getOpenFileNames(this, "Select images to add", "", "Images (*.png *.jpg *.gif)");

		for (QString image_file : image_files) {
			// Load in the image
			Image* image = new Image(image_file.toStdString());

			// Check that the image has successfully loaded
			if (image->has_loaded()) {
				ImageClass* predicted_class = m_manager->classify_image(image);
				if (predicted_class != 0)
					m_new_image_map[predicted_class].push_back(image);
			}
		}

		// Highlight all classes necessary
		highlight_classes();

		cout << image_files.size() << " new images were added" << endl;
	}
}

void ImageClassifierWindow::highlight_classes() {
	for (ImageClass* image_class : get_image_classes()) {
		if (!m_new_image_map[image_class].isEmpty()) {
			m_class_to_displayer[image_class]->set_highlighted(true);
		}
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