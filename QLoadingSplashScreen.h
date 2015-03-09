#pragma once
#include <QSplashScreen>
#include <QMovie>
#include <QTimer>

/**
 * @class	QLoadingSplashScreen
 *
 * @brief	This component is useful when performing a computationally
 * 			expensive task. The background of the splash screen contains
 * 			an animation, so the user knows the program is still responsive.
 * 			You can display a message to the user, to let them know what is
 * 			being performed by calling showMessage(QString).
 * 			When computation has finished, you may close the splash screen
 * 			by calling the finished() slot.
 */

class QLoadingSplashScreen : public QSplashScreen
{
	Q_OBJECT
public:
	QLoadingSplashScreen();
	virtual ~QLoadingSplashScreen();
protected:
	/**
	 * @fn	void QLoadingSplashScreen::goto_next_frame();
	 *
	 * @brief	Moves the gif animation to the next frame
	 */

	void goto_next_frame();

	/**
	 * @fn	virtual void QLoadingSplashScreen::drawContents(QPainter* painter);
	 *
	 * @brief	Renders the splash screen.
	 *
	 * @param [in,out]	painter	If non-null, the painter.
	 */

	virtual void drawContents(QPainter* painter);

	static const int FRAME_WIDTH = 360;
	static const int FRAME_HEIGHT = 200;
	
protected slots:

	/**
	 * @fn	void QLoadingSplashScreen::animate();
	 *
	 * @brief	Moves the animation to the next frame, and re-renders the window.
	 */

	void animate();

	/**
	 * @fn	void QLoadingSplashScreen::finished();
	 *
	 * @brief	Ends the current animation timer, and closes the splash screen.
	 */

	void finished();
private:
	/** @brief	In charge of reading in the gif animation. */
	QMovie* m_frame_reader;
	/** @brief	In charge of telling the splash screen when  
	/** to move to the next frame in the animation. */
	QTimer* m_anim_timer;

	QColor m_background_color;
	QPoint m_spinner_pos;
};

