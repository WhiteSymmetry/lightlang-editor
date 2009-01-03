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
	public slots:
		TabWidget* openNewTab(const QString& tabTitle = QString());
		void closeCurrentTab();
	public:
		TabsWidget(DatabaseCenter *databaseCenter,QWidget *parent = 0);
		~TabsWidget();
	
		void setFocusOnCurrentTab();
		void setEditorMenu(Menu *menu);
	private slots:
		void renameTab(int index,const QString& name);
		void currentTabChanged(int index);
	private:
		DatabaseCenter *databaseCenter;
	
		QList<TabWidget *> tabs;
		Menu *editorMenu;
	
		QToolButton *newTabButton;
		QToolButton *closeCurrentTabButton;
};

#endif
