#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtSql/QSqlDatabase>
#include "MainWindow.h"

int main(int argc,char *argv[]) {
	QApplication app(argc,argv);

	if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
		QMessageBox installDriverBeforeUsingDialog;
		installDriverBeforeUsingDialog.setWindowTitle("LightLang Editor");
		installDriverBeforeUsingDialog.setWindowIcon(QIcon(":/icons/lle.png"));
		installDriverBeforeUsingDialog.setText("<b>" + QObject::tr("Cannot load sqlite driver for qt") + "</b><br>" + QObject::tr("LightLang Editor cannot work with SQLite without this driver, that's why you have to install it to run this application."));
		installDriverBeforeUsingDialog.setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
		installDriverBeforeUsingDialog.exec();
		return 1;
	}
	
	MainWindow mainWindow;
	mainWindow.connect(&mainWindow,SIGNAL(toQuit()),&app,SLOT(quit()));
	
	return app.exec();
}
