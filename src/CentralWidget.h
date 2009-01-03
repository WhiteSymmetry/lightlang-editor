#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtGui/QWidget>

class NewDictWidget;
class LoadDictionaryWidget;
class TabsWidget;
class StackedWidget;
class QTextBrowser;
class BrowserWithWidgets;
class QToolButton;
class DatabaseCenter;
class LoadDictionaryThread;
class QMessageBox;
class QPushButton;
class SettingsWidget;
class Menu;

class CentralWidget : public QWidget
{
	Q_OBJECT
	signals:
		void resized(int w,int h);
		void startPageShown(bool);
		void loadingCompleted(bool isSuccessful);
		void changeWindowTitle(const QString& title);
		void databaseWasOpened(const QString& dbName);
	public slots:
		void showNewDictWidget();
		void openNewTab();
		void closeCurrentTab();
		void showTabsWidget();
		void showSettings();
	
		void setCurrentDatabase(const QString& dbName);
		void removeDatabaseWithName(const QString& dbName);
	public:
		CentralWidget(QWidget *mainWindowCommunicater);
		~CentralWidget();
	
		void setStartPageText(const QString& text);
		void setExistingDictionaries(const QStringList& list);
		void loadDictionary(const QString& dictPath);
		QString getLoadedDictAbout() const;
	
		void setEditorMenu(Menu *menu);
	
		bool saveDictionary();
		void saveDictionaryAs(const QString& dictPath);
	
		void setPathForOpenedDictionary(const QString& pathOfOpenedDictionary,const QString& aboutDictionaryText);
	
		// If there is a loading, user will be asked is he sure, that he want to cancel loading and quit from program
		bool saveSettings();
		void loadSettings();
	private slots:
		void cancelLoading();
		void loadingFinished();
		void openLastLoadedDictionary();
		void currentWidgetChanged(int widgetIndex);
		void closeSettings();
	private:
		QMessageBox *continueLoadingOfLastLoadedOrNotDialog;
		QPushButton *continueLoadingLastLoadedButton;
		QPushButton *ignoreLoadingLastLoadedButton;
		bool continueLoadingOfLastLoadedDictionary;
	
		QMessageBox *cancelOrContinueCurrentLoadingDialog;
		QPushButton *cancelStartOfNewLoadingButton;
		QPushButton *continueStartOfNewLoadingButton;
	
		QMessageBox *closeOrNoIfLoadingDialog;
		QPushButton *continueIfLoadingButton;
		QPushButton *closeIfLoadingButton;
	
		QMessageBox *continueOrRestartLoadingDialog;
		QPushButton *continueLoadingButton;
		QPushButton *restartLoadingButton;
		QPushButton *ignoreLoadingButton;
	
		DatabaseCenter *databaseCenter;
		StackedWidget *stackedWidget;
		TabsWidget *tabsWidget;
		NewDictWidget *newDictWidget;
		LoadDictionaryWidget *loadDictionaryWidget;
		BrowserWithWidgets *startPageViewer;
		LoadDictionaryThread *loadDictionaryThread;
		SettingsWidget *settingsWidget;
	
		QToolButton *createNewDictBorderButton;
		QToolButton *openDictBorderButton;
		QToolButton *showDictsManagerButton;
	
		QString currentLoadingDictName;
		QString currentLoadingDictAbout;
		QString currentOpenedDictPath;
		QString currentOpenedDictAbout;
		QStringList existingDictionaries;
	protected:
		void resizeEvent(QResizeEvent *resizeEvent);
};

#endif
