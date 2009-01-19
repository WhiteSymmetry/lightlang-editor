#include <QtGui/QToolButton>
#include "TabWidget.h"
#include "Menu.h"
#include "TabsWidget.h"

TabsWidget::TabsWidget(DatabaseCenter *dbCenter,QWidget *parent) : QTabWidget(parent) {
	databaseCenter = dbCenter;
	updateTranslationInterval = 0;
	showTips = true;
	
	newTabButton = new QToolButton;
	newTabButton->setAutoRaise(true);
	newTabButton->setIcon(QIcon(":/icons/add.png"));
	newTabButton->setToolTip(tr("Open new tab"));
	connect(newTabButton,SIGNAL(clicked()),this,SLOT(openNewTab()));
	
	closeCurrentTabButton = new QToolButton;
	closeCurrentTabButton->setAutoRaise(true);
	closeCurrentTabButton->setIcon(QIcon(":/icons/close_tab.png"));
	closeCurrentTabButton->setToolTip(tr("Close tab"));
	connect(closeCurrentTabButton,SIGNAL(clicked()),this,SIGNAL(closeTabButtonClicked()));
	
	setCornerWidget(newTabButton,Qt::TopLeftCorner);
	setCornerWidget(closeCurrentTabButton,Qt::TopRightCorner);
	
	connect(this,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
}

TabsWidget::~TabsWidget() {
	blockSignals(true);
	foreach (TabWidget *tab,tabs)
		delete tab;
	delete newTabButton;
	delete closeCurrentTabButton;
}

TabWidget* TabsWidget::openNewTab(const QString& tabTitle) {
	TabWidget *newTabWidget = new TabWidget(databaseCenter,tabs.count(),updateTranslationInterval);
	tabs << newTabWidget;
	newTabWidget->setEditorMenu(editorMenu);
	setCurrentIndex(addTab(newTabWidget,tabTitle.isEmpty() ? "(" + tr("Unnamed") + ")" : tabTitle));
	if (!showTips)
		newTabWidget->hideTips();
	
	connect(newTabWidget,SIGNAL(renameTab(int,const QString&)),this,SLOT(renameTab(int,const QString&)));
	connect(newTabWidget,SIGNAL(showStatusMessage(const QString&)),this,SIGNAL(showStatusMessage(const QString&)));
	connect(newTabWidget,SIGNAL(hideAllTips()),this,SLOT(hideAllTips()));
	
	return newTabWidget;
}

void TabsWidget::closeCurrentTab() {
	tabs.removeAt(currentIndex());
	removeTab(currentIndex());
}

void TabsWidget::renameTab(int index,const QString& name) {
	setTabText(index,name.isEmpty() ? "(" + tr("Unnamed") + ")" : name);
}

void TabsWidget::currentTabChanged(int index) {
	if (index >= 0)
		tabs[index]->setFocusAtThisTab();
}


void TabsWidget::setEditorMenu(Menu *menu) {
	editorMenu = menu;
}

void TabsWidget::setFocusOnCurrentTab() {
	tabs[currentIndex()]->setFocusAtThisTab();
}

void TabsWidget::setUpdateTranslationInterval(int interval) {
	updateTranslationInterval = interval;
	foreach (TabWidget *tab,tabs)
		tab->setUpdateTranslationInterval(updateTranslationInterval);
}

void TabsWidget::hideAllTips() {
	foreach (TabWidget *tab,tabs)
		tab->hideTips();
	showTips = false;
}
