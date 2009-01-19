#ifndef TABSWIDGET_H
#define TABSWIDGET_H

#include <QtGui/QTabWidget>

class TabWidget;
class QToolButton;
class DatabaseCenter;
class Menu;

class TabsWidget : public QTabWidget
{
	Q_OBJECT
	signals:
		void closeTabButtonClicked();
		void showStatusMessage(const QString& message);
	public slots:
		TabWidget* openNewTab(const QString& tabTitle = QString());
		void closeCurrentTab();
	public:
		TabsWidget(DatabaseCenter *databaseCenter,QWidget *parent = 0);
		~TabsWidget();
	
		void setFocusOnCurrentTab();
		void setEditorMenu(Menu *menu);
		void setUpdateTranslationInterval(int interval);
	private slots:
		void renameTab(int index,const QString& name);
		void currentTabChanged(int index);
		void hideAllTips();
	private:
		DatabaseCenter *databaseCenter;
	
		bool showTips;
		int updateTranslationInterval;
		
		QList<TabWidget *> tabs;
		Menu *editorMenu;
	
		QToolButton *newTabButton;
		QToolButton *closeCurrentTabButton;
};

#endif
