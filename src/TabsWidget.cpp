#include <QtGui/QPushButton>
#include "TabWidget.h"
#include "TabsWidget.h"

TabsWidget::TabsWidget(DatabaseCenter *dbCenter,QWidget *parent) : QTabWidget(parent) {
	databaseCenter = dbCenter;
	
	newTabButton = new QPushButton;
	newTabButton->setFlat(true);
	newTabButton->setIcon(QIcon(":/icons/new_tab.png"));
	connect(newTabButton,SIGNAL(clicked()),this,SLOT(openNewTab()));
	
	closeCurrentTabButton = new QPushButton;
	closeCurrentTabButton->setFlat(true);
	closeCurrentTabButton->setIcon(QIcon(":/icons/close_tab.png"));
	connect(closeCurrentTabButton,SIGNAL(clicked()),this,SIGNAL(closeTabButtonClicked()));
	
	setCornerWidget(newTabButton,Qt::TopLeftCorner);
	setCornerWidget(closeCurrentTabButton,Qt::TopRightCorner);
	
	openNewTab();
}

TabsWidget::~TabsWidget() {
	foreach (TabWidget *tab,tabs)
		delete tab;
	delete newTabButton;
	delete closeCurrentTabButton;
}

TabWidget* TabsWidget::openNewTab(const QString& tabTitle) {
	TabWidget *newTabWidget = new TabWidget(databaseCenter);
	setCurrentIndex(addTab(newTabWidget,tabTitle.isEmpty() ? "*" : tabTitle));
	tabs << newTabWidget;
	return newTabWidget;
}

void TabsWidget::closeCurrentTab() {
	removeTab(currentIndex());
}
