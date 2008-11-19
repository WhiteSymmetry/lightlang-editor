#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtGui/QWidget>

class NewDictWidget;
class LoadDictionaryWidget;
class TabsWidget;
class QStackedWidget;
class QTextBrowser;
class BrowserWithWidgets;
class QPushButton;
class QMenu;
class DatabaseCenter;
class LoadDictionaryThread;

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
	
		void setExistingDictionaries(const QStringList& list);
		void loadDictionary(const QString& dictPath,QString *aboutDictionaryString);
	private slots:
		void cancelLoading();
		void loadingFinished();
	private:	
		DatabaseCenter *databaseCenter;
		QStackedWidget *stackedWidget;
		TabsWidget *tabsWidget;
		NewDictWidget *newDictWidget;
		LoadDictionaryWidget *loadDictionaryWidget;
		BrowserWithWidgets *startPageViewer;
		LoadDictionaryThread *loadDictionaryThread;
	
		QPushButton *createNewDictBorderButton;
		QPushButton *openDictBorderButton;
		QPushButton *recentDictsBorderButton;
		QMenu *recentDictsMenu;
	
		QString currentLoadingDictName;
		QString *currentLoadingDictAbout;
	protected:
		void resizeEvent(QResizeEvent *resizeEvent);
};

#endif
