#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QAction;
class Manual;
class About;
class DictionariesManager;
class CentralWidget;
class QToolBar;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	signals:
		void moved(int x,int y);
	public slots:
		void createNewDictionary(const QString& dictName);
		void openDictionary();
	public:
		MainWindow();
		~MainWindow();
	private slots:
		void loadingCompleted(bool isSuccessful);
	private:
		void createActions();
		void createDirs();
	
		About *about;
		Manual *manual;
		DictionariesManager *dictionariesManager;
		CentralWidget *centralWidget;
	
		QToolBar *editionToolBar;

		QAction *createDictAction;
		QAction *openDictAction;
		QAction *saveDictAction;
		QAction *saveDictAsAction;
		QAction *openTabAction;
		QAction *closeTabAction;
		QAction *quitAction;

		QAction *pluginsManagerAction;
		QAction *dictsManagerAction;

		QAction *manualAction;
		QAction *aboutProgramAction;
	
		QAction *pasteBoldAction;
		QAction *pasteItalicAction;
		QAction *pasteUnderlineAction;
		QAction *pasteLinkAction;
		QAction *pasteBlockAction;
		QAction *pasteSoundAction;
		QAction *pasteSpecialAction;
		
		QString currentLoadingDictPath;
		QString currentLoadingDictAbout;
		QString homePath;
		QString databasesPath;
		QString controlPath;
	protected:
		void moveEvent(QMoveEvent *event);
};

#endif
