#include "LoadingScreen.h"


LoadingScreen::LoadingScreen(QStringList file_paths, QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Loading..");
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::setupBusyIndicator() {
	ui.progressBar->setRange(0, 0);
}