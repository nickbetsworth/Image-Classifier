#pragma once
#include "ui_LoadingScreen.h"
#include <QWidget>

/**
 * @class	LoadingScreen
 *
 * @brief	Provides the user with a loading screen, which will update them on
 * 			the current progress of the system.
 */

class LoadingScreen : public QWidget
{
	Q_OBJECT
public:
	LoadingScreen(QWidget *parent = 0);
	~LoadingScreen();
	void setupBusyIndicator();
private:
	Ui::LoadingScreen ui;
};

