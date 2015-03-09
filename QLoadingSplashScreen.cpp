#include "QLoadingSplashScreen.h"
#include <QPainter>
#include <QTextItem>
#include <QApplication>
#include <QDesktopWidget>
#include <iostream>

QLoadingSplashScreen::QLoadingSplashScreen() : QSplashScreen()
{
	// Set the size of the splash screen with respect to the size of
	// the image we just loaded in
	this->setFixedSize(FRAME_WIDTH, FRAME_HEIGHT);
	// Center the splash screen on the user's screen
	this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
	// Ensure this window is always on top of the main window
	this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
	// Make sure the user can not click this window to close it
	this->setEnabled(false);

	// Load in the animated spinner
	m_frame_reader = new QMovie(":/splash.gif");

	// Read in the first frame, so we have access to the size of it
	this->goto_next_frame();
	// Pre-calculate the position of the spinner
	int pos_x = (this->width() - m_frame_reader->frameRect().width()) / 2;
	int pos_y = (this->height() - m_frame_reader->frameRect().height()) / 2 - 20;
	m_spinner_pos = QPoint(pos_x, pos_y);

	// Set the background color
	m_background_color = QColor(20, 20, 20);

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

	// Fill the background with the given color
	painter->fillRect(QRectF(0, 0, this->width(), this->height()), QBrush(m_background_color));

	// Load the message that is to be written (if any)
	QString message = this->message();

	// Render the spinner in the center of the image
	
	painter->drawPixmap(m_spinner_pos, m_frame_reader->currentPixmap());

	// Create the font that will be used to write the message
	QFont font = QFont("Sans-serif", 11);
	painter->setFont(font);
	

	// Work out how big our text is, so we can center it
	QFontMetrics fm = QFontMetrics(font);
	int text_width = fm.width(message);
	int text_height = fm.height();
	// Calculate the position of the text
	int x_pos = (this->width() - text_width) / 2;
	int y_pos = (this->height() / 2) + text_height + 25;
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