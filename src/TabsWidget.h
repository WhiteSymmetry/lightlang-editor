#ifndef TABSWIDGET_H
#define TABSWIDGET_H

#include <QtGui/QTabWidget>

class TabWidget;
class QPushButton;
class DatabaseCenter;

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
	private:
		DatabaseCenter *databaseCenter;
	
		QList<TabWidget *> tabs;
	
		QPushButton *newTabButton;
		QPushButton *closeCurrentTabButton;
};

#endif
