#include <QtGui/QToolButton>
#include <QtGui/QAction>
#include "Menu.h"
#include "TabWidget.h"
#include "TabsWidget.h"

TabsWidget::TabsWidget(DatabaseCenter *dbCenter,QWidget *parent) : QTabWidget(parent) {
	databaseCenter = dbCenter;
	updateTranslationInterval = 0;
	showTips = true;
	
	tipsMenu = new Menu(true);
	tipsMenu->setHeaderText(tr("Tips"));
	tipsMenu->setHeaderIcon(QIcon(":/icons/tip.png"));
	
	hideAllTipsAction = new QAction(tipsMenu);
	hideAllTipsAction->setText(tr("Hide tips in all tabs"));
	connect(hideAllTipsAction,SIGNAL(triggered()),this,SLOT(hideAllTips()));
	
	nextTipAction = new QAction(tipsMenu);
	nextTipAction->setText(tr("Show next tip"));
	nextTipAction->setIcon(QIcon(":/icons/forward.png"));
	
	previousTipAction = new QAction(tipsMenu);
	previousTipAction->setText(tr("show previous tip"));
	previousTipAction->setIcon(QIcon(":/icons/backward.png"));
	
	tipsMenu->addAction(nextTipAction);
	tipsMenu->addAction(previousTipAction);
	tipsMenu->addAction(hideAllTipsAction);
	
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
	delete hideAllTipsAction;
	delete nextTipAction;
	delete previousTipAction;
	delete tipsMenu;
	delete newTabButton;
	delete closeCurrentTabButton;
}

TabWidget* TabsWidget::openNewTab(const QString& tabTitle) {
	TabWidget *newTabWidget = new TabWidget(databaseCenter,tabs.count(),updateTranslationInterval);
	tabs << newTabWidget;
	newTabWidget->setEditorMenu(editorMenu);
	newTabWidget->setTipsMenu(tipsMenu);
	setCurrentIndex(addTab(newTabWidget,tabTitle.isEmpty() ? "(" + tr("Unnamed") + ")" : tabTitle));
	newTabWidget->setTipsHidden(!showTips);
	
	connect(newTabWidget,SIGNAL(renameTab(int,const QString&)),this,SLOT(renameTab(int,const QString&)));
	connect(newTabWidget,SIGNAL(showStatusMessage(const QString&)),this,SIGNAL(showStatusMessage(const QString&)));
	
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

void TabsWidget::setAllTipsHidden(bool toHide) {
	foreach (TabWidget *tab,tabs)
		tab->setTipsHidden(toHide);
	showTips = !toHide;
}

void TabsWidget::hideAllTips() {
	setAllTipsHidden(true);
}
