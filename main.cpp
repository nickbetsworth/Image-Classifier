#include "imageclassifierwindow.h"
#include "ImageSelector.h"
#include "LoadingScreen.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QWidget>
#include <opencv\cv.h>
#include <opencv\highgui.h>
// Include Windows.h to temporarily display console
#include <Windows.h>
#include <sstream>
#include <iostream>
#include "Testing.h"

void ShowConsole()
{
	AllocConsole();
	FILE *pFileCon = NULL;
	pFileCon = freopen("CONOUT$", "w", stdout);

	COORD coordInfo;
	coordInfo.X = 130;
	coordInfo.Y = 9000;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
}

int main(int argc, char *argv[])
{
	ShowConsole();
	perform_tests();
	QApplication a(argc, argv);
	ImageSelector selector;
	selector.show();
	
	
	//ImageClassifierWindow w;
	//w.show();
	//w.showMaximized();

	return a.exec();
}

