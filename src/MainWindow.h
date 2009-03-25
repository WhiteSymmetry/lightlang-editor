#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QAction;
class Manual;
class About;
class DictionariesManager;
class CentralWidget;
class QToolBar;
class StatusBarLabel;
class QStatusBar;
class QToolButton;
class Menu;
class QMessageBox;
class QPushButton;
class PopupWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	signals:
		void moved(int x,int y);
		void toQuit();
	public slots:
		void createNewDictionary(const QString& dictName);
		void openDictionaryFile();
		void updateWindowTitle(const QString &addToTitle);
		void removeDatabaseWithName(const QString& dbName);
		void showDictionariesManager();
		void showStatusMessage(const QString& message);
	public:
		MainWindow();
		~MainWindow();
	private slots:
		void loadingCompleted(bool isSuccessful);
		void quit();
		void openDatabaseWithName(const QString& databaseName,bool addToRecentOpenedDicts = true);
		void disableEditionActions(bool isDisabled);
		void openDictionaryOfAction(QAction *chosenAction);
		void saveDictionaryFile();
		void saveDictionaryFileAs(const QString& path = QString());
		void addDictionaryToSl();
		void saveDictionaryInformation();
	private:
		void saveSettings();
		void loadSettings();
		void createActions();
		void createDirs();
		void updateRecentDictsMenu();
	
		QStringList recentOpenedDictionaries;
	
		QMessageBox *formatIsNotSuitableDialog;
		
		QMessageBox *newDictionaryCreatedDialog;
		QPushButton *openDictionaryNowButton;
		QPushButton *openDictionaryLaterButton;

		About *about;
		Manual *manual;
		DictionariesManager *dictionariesManager;
		CentralWidget *centralWidget;
		PopupWindow *editDictInfoPopupWindow;
		
		StatusBarLabel *statusBarLabel;
		QStatusBar *statusbar;
		QTimer *hideStatusBarTimer;
	
		QToolBar *editionToolBar;
		Menu *editorMenu;

		QAction *createDictAction;
		QAction *openDictAction;
		QAction *openRecentDictsAction;
		QMenu *recentDictsMenu;
		QAction *saveDictAction;
		QAction *saveDictAsAction;
		QAction *addToSlAction;
		QAction *editDictInformationAction;
		QAction *openTabAction;
		QAction *closeTabAction;
		QAction *quitAction;

		QAction *dictsManagerAction;
		QAction *settingsAction;
		QAction *dictionarySearchAction;
		QAction *startPageAction;

		QAction *manualAction;
		QAction *aboutProgramAction;
	
		QAction *pasteBoldAction;
		QAction *pasteItalicAction;
		QAction *pasteUnderlineAction;
		QAction *pasteLinkAction;
		QAction *pasteBlockAction;
		QAction *pasteSoundAction;
		QAction *pasteSpecialAction;
		QAction *undoAction;
		QAction *redoAction;
		QAction *cutAction;
		QAction *copyAction;
		QAction *pasteAction;
		QAction *findAction;
		QAction *previewAction;
		
		QString openedDictionaryName;
		QString currentLoadingDictPath;
		QString currentLoadingDictInformation;
		QString homePath;
		QString databasesPath;
		QString controlPath;
	protected:
		void moveEvent(QMoveEvent *event);
		void closeEvent(QCloseEvent *event);
};

#endif
