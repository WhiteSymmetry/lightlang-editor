#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtGui/QWidget>

class NewDictWidget;
class LoadDictionaryWidget;
class TabsWidget;
class QStackedWidget;
class QTextBrowser;
class BrowserWithWidgets;
class QToolButton;
class DatabaseCenter;
class LoadDictionaryThread;
class QMessageBox;
class QPushButton;
class SettingsWidget;
class Menu;
class SearchPanel;
class QAction;

class CentralWidget : public QWidget
{
	Q_OBJECT
	signals:
		void resized(int w,int h);
		void startPageShown(bool);
		void loadingCompleted(bool isSuccessful);
		void changeWindowTitle(const QString& title);
		void openDatabaseWithName(const QString& dbName);
		void showProgramAbout();
		void showProgramDocumentation();
	public slots:
		void showNewDictWidget();
		void openNewTab();
		void closeCurrentTab();
		void showTabsWidget();
		void showSettings();
		void hideOrShowSearchPanel();
		void setFocusOnSearchPanel();
		void showStartPage();
	
		void setCurrentDatabase(const QString& dbName);
		void removeDatabaseWithName(const QString& dbName);
	public:
		CentralWidget(QWidget *mainWindowCommunicater);
		~CentralWidget();
	
		void setStartPageText(const QString& text);
		void setExistingDictionaries(const QStringList& list);
		void loadDictionary(const QString& dictPath,int startLoadingFromRow = 0);
		QString getLoadedDictionaryInformation() const;
	
		void setEditorMenu(Menu *menu);
	
		int saveDictionaryAs(const QString& whereSave,const QString& information);
		
		QWidget *getTabsWidget() const;
	
		// If there is a loading, user will be asked is he sure, that he want to cancel loading and quit from program
		bool saveSettings();
		void loadSettings();
	private slots:
		void cancelLoading();
		void loadingFinished();
		void openLastLoadedDictionary();
		void currentWidgetChanged(int widgetIndex);
		void startPageLinkClicked(const QString& link);
		void updateSettings();
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
		QStackedWidget *stackedWidget;
		TabsWidget *tabsWidget;
		NewDictWidget *newDictWidget;
		LoadDictionaryWidget *loadDictionaryWidget;
		BrowserWithWidgets *startPageViewer;
		LoadDictionaryThread *loadDictionaryThread;
		SettingsWidget *settingsWidget;
		SearchPanel *searchPanel;
		QPushButton *searchPanelButton;
		
		Menu *startPageContextMenu;
		QAction *openDictAction;
		QAction *createNewDictAction;
		QAction *showDictsManagerAction;
	
		QToolButton *createNewDictBorderButton;
		QToolButton *openDictBorderButton;
		QToolButton *showDictsManagerButton;
	
		bool searchPanelWasHidden;
		QString currentLoadingDictName;
		QString currentLoadingDictInformation;
		QStringList existingDictionaries;
	protected:
		void resizeEvent(QResizeEvent *resizeEvent);
};

#endif
