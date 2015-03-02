#include "QLoadingSplashScreen.h"
#include <QPainter>
#include <QTextItem>
#include <QApplication>
#include <QDesktopWidget>
#include <iostream>

QLoadingSplashScreen::QLoadingSplashScreen() : QSplashScreen()
{
	// Load in the animated background gif
	m_frame_reader = new QMovie(":/splash.gif");

	// Read in the first frame
	this->goto_next_frame();
	// Set the size of the splash screen with respect to the size of
	// the image we just loaded in
	this->setFixedSize(m_frame_reader->currentPixmap().size());
	// Center the splash screen on the user's screen
	this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
	// Ensure this window is always on top of the main window
	this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
	// Make sure the user can not click this window to close it
	this->setEnabled(false);

	// Create a timer which will run the animation
	m_anim_timer = new QTimer(this);
	connect(m_anim_timer, SIGNAL(timeout()), this, SLOT(animate()));
	m_anim_timer->start(m_frame_reader->nextFrameDelay());
	
}


QLoadingSplashScreen::~QLoadingSplashScreen()
{
	delete m_frame_reader;
	delete m_anim_timer;
}

void QLoadingSplashScreen::drawContents(QPainter* painter) {
	QSplashScreen::drawContents(painter);

	// Load the message that is to be written (if any)
	QString message = this->message();

	// Render the background
	painter->drawPixmap(QPoint(0, 0), m_frame_reader->currentPixmap());

	// Create the font that will be used to write the message
	QFont font = QFont("Sans-serif", 11);
	painter->setFont(font);
	

	// Work out how big our text is, so we can center it
	QFontMetrics fm = QFontMetrics(font);
	int text_width = fm.width(message);
	int text_height = fm.height();
	// Calculate the position of the text
	int x_pos = (this->width() - text_width) / 2;
	int y_pos = (this->height() / 2) + text_height + 20;
	QPoint pos = QPoint(x_pos, y_pos);

	// Set the color of the text
	QPen font_pen = QPen(QColor(190, 190, 190));
	painter->setPen(font_pen);
	
	// Render the text
	painter->drawText(pos, message);
}

void QLoadingSplashScreen::goto_next_frame() {
	m_frame_reader->jumpToNextFrame();
}

void QLoadingSplashScreen::animate() {
	goto_next_frame();
	this->repaint();
}

void QLoadingSplashScreen::finished() {
	// Stop the timer and close the window
	m_anim_timer->stop();
	this->close();
}