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

class CentralWidget : public QWidget
{
	Q_OBJECT
	signals:
		void resized(int w,int h);
		void startPageShown(bool);
		void loadingCompleted(bool isSuccessful);
	public slots:
		void showNewDictWidget();
		void openNewTab();
		void closeCurrentTab();
		void showTabsWidget();
	
		void setCurrentDatabase(const QString& dbName);
		void removeDatabaseWithName(const QString& dbName);
	public:
		CentralWidget(QWidget *mainWindowCommunicater);
		~CentralWidget();
	
		void setStartPageText(const QString& text);
		void setExistingDictionaries(const QStringList& list);
		void loadDictionary(const QString& dictPath,QString *aboutDictionaryString);
	
		void saveSettings();
	private slots:
		void cancelLoading();
		void loadingFinished();
	private:
		QMessageBox *closeOrNoIfLoadingDialog;
		QMessageBox *continueOrRestartLoadingDialog;
	
		DatabaseCenter *databaseCenter;
		QStackedWidget *stackedWidget;
		TabsWidget *tabsWidget;
		NewDictWidget *newDictWidget;
		LoadDictionaryWidget *loadDictionaryWidget;
		BrowserWithWidgets *startPageViewer;
		LoadDictionaryThread *loadDictionaryThread;
	
		QToolButton *createNewDictBorderButton;
		QToolButton *openDictBorderButton;
		QToolButton *showDictsManagerButton;
	
		QString currentLoadingDictName;
		QString *currentLoadingDictAbout;
	protected:
		void resizeEvent(QResizeEvent *resizeEvent);
};

#endif
