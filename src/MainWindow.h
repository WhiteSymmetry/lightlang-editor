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
		void toQuit();
	public slots:
		void createNewDictionary(const QString& dictName);
		void openDictionary();
		void updateWindowTitle(const QString &addToTitle);
		void removeDatabaseWithName(const QString& dbName);
		void showDictionariesManager();
	public:
		MainWindow();
		~MainWindow();
	private slots:
		void loadingCompleted(bool isSuccessful);
		void quit();
	private:
		void saveSettings();
		void loadSettings();
		void createActions();
		void createDirs();
		void updateRecentDictsMenu();
	
		QStringList recentOpenedDictionaries;
	
		About *about;
		Manual *manual;
		DictionariesManager *dictionariesManager;
		CentralWidget *centralWidget;
	
		QToolBar *editionToolBar;

		QAction *createDictAction;
		QAction *openDictAction;
		QAction *openRecentDictsAction;
		QMenu *recentDictsMenu;
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
		void closeEvent(QCloseEvent *event);
};

#endif
