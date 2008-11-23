#include <QtGui/QApplication>
#include "MainWindow.h"


int main(int argc,char *argv[]) {
	QApplication app(argc,argv);
	
	MainWindow mainWindow;
	mainWindow.show();
	mainWindow.connect(&mainWindow,SIGNAL(toQuit()),&app,SLOT(quit()));
	
	return app.exec();
}
