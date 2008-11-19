#include <QtGui/QStackedWidget>
#include <QtGui/QPushButton>
#include <QtGui/QMenu>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtCore/QDir>
#include <QDebug>
#include "TabsWidget.h"
#include "NewDictWidget.h"
#include "LoadDictionaryWidget.h"
#include "BrowserWithWidgets.h"
#include "DatabaseCenter.h"
#include "LoadDictionaryThread.h"
#include "CentralWidget.h"

CentralWidget::CentralWidget(QWidget *mainWindowCommunicater) {
	
	databaseCenter = new DatabaseCenter;
	
	recentDictsMenu = new QMenu;
	
	openDictBorderButton = new QPushButton;
	openDictBorderButton->setFlat(true);
	openDictBorderButton->setIcon(QIcon(":/icons/open.png"));
	connect(openDictBorderButton,SIGNAL(clicked()),mainWindowCommunicater,SLOT(openDictionary()));
	
	createNewDictBorderButton = new QPushButton;
	createNewDictBorderButton->setFlat(true);
	createNewDictBorderButton->setIcon(QIcon(":/icons/new.png"));
	connect(createNewDictBorderButton,SIGNAL(clicked()),this,SLOT(showNewDictWidget()));
	
	recentDictsBorderButton = new QPushButton;
	recentDictsBorderButton->setFlat(true);
	recentDictsBorderButton->setMenu(recentDictsMenu);
	recentDictsBorderButton->setIcon(QIcon(":/icons/open_recent.png"));
	
	startPageViewer = new BrowserWithWidgets(this);
	startPageViewer->setPosition(BrowserWithWidgets::RightTopCorner);
	startPageViewer->setHtml
	(
		"<hr><table border=\"0\" width=\"100%\"><tr><td bgcolor=\"#DFEDFF\"><h2 align=\"center\"><em>" + 
		tr("Start page") + 	
		"</em></h2></td></tr></table><hr>" +
		tr("Hello, thank you for LightLang Editor using! With editor help you can edit existed dictionaries, create new dictionaries and add dictionaries to SL database.")
	);
	startPageViewer->addWidget(createNewDictBorderButton);
	startPageViewer->addWidget(openDictBorderButton);
	startPageViewer->addWidget(recentDictsBorderButton);
	
	stackedWidget = new QStackedWidget;
	
	tabsWidget = new TabsWidget(databaseCenter);
	connect(tabsWidget,SIGNAL(closeTabButtonClicked()),this,SLOT(closeCurrentTab()));
	
	stackedWidget->addWidget(startPageViewer);
	stackedWidget->addWidget(tabsWidget);
	stackedWidget->setCurrentIndex(0);
	
	loadDictionaryWidget = new LoadDictionaryWidget;
	loadDictionaryWidget->hide();
	connect(loadDictionaryWidget,SIGNAL(canceled()),this,SLOT(cancelLoading()));
	
	loadDictionaryThread = new LoadDictionaryThread;
	connect(loadDictionaryThread,SIGNAL(rowsCounted(int)),loadDictionaryWidget,SLOT(setMaximum(int)));
	connect(loadDictionaryThread,SIGNAL(rowChanged(int)),loadDictionaryWidget,SLOT(addValue()));
	connect(loadDictionaryThread,SIGNAL(finished()),this,SLOT(loadingFinished()));
	
	newDictWidget = new NewDictWidget;
	newDictWidget->hide();
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(newDictWidget);
	mainLayout->addWidget(stackedWidget,1);
	mainLayout->addWidget(loadDictionaryWidget);
	mainLayout->setMargin(0);
	setLayout(mainLayout);
	
	connect(newDictWidget,SIGNAL(createDictionary(const QString&)),mainWindowCommunicater,SLOT(createNewDictionary(const QString&)));
	connect(newDictWidget,SIGNAL(createDictionary(const QString&)),this,SLOT(showTabsWidget()));
}

CentralWidget::~CentralWidget() {
	delete loadDictionaryWidget;
    delete tabsWidget;
	delete createNewDictBorderButton;
	delete openDictBorderButton;
	delete recentDictsBorderButton;
	delete newDictWidget;
	delete startPageViewer;
	delete stackedWidget;
	delete recentDictsMenu;
	delete databaseCenter;
	delete loadDictionaryThread;
}

void CentralWidget::showNewDictWidget() {
	newDictWidget->show();
}

void CentralWidget::openNewTab() {
	tabsWidget->openNewTab();
}

void CentralWidget::closeCurrentTab() {
	tabsWidget->closeCurrentTab();
	if (tabsWidget->count() == 0) {
		stackedWidget->setCurrentIndex(0);
		emit (startPageShown(true));
	}
}

void CentralWidget::resizeEvent(QResizeEvent *) {
	emit (resized(width(),height()));
}

void CentralWidget::showTabsWidget() {
	stackedWidget->setCurrentIndex(1);
	if (tabsWidget->count() == 0)
		tabsWidget->openNewTab();
	emit (startPageShown(false));
}

void CentralWidget::setCurrentDatabase(const QString& dbName) {
	databaseCenter->setDatabaseName(dbName);
	stackedWidget->setCurrentIndex(1);
	if (tabsWidget->count() == 0)
		tabsWidget->openNewTab();
	emit (startPageShown(false));
}

void CentralWidget::setExistingDictionaries(const QStringList& list) {
	newDictWidget->setInvalidNames(list);
}

void CentralWidget::loadDictionary(const QString& dictPath,QString *aboutDictionaryString) {
	currentLoadingDictAbout = aboutDictionaryString;
	loadDictionaryWidget->reset();
	loadDictionaryWidget->show();
	loadDictionaryThread->setDictionaryPath(dictPath);
	currentLoadingDictName = QFileInfo(dictPath).fileName();
	loadDictionaryThread->start();
}

void CentralWidget::cancelLoading() {
	loadDictionaryThread->stop();
	loadDictionaryWidget->hide();
	removeDatabaseWithName(currentLoadingDictName);
}

void CentralWidget::removeDatabaseWithName(const QString& dbName) {
	databaseCenter->removeDatabaseWithName(dbName);
}

void CentralWidget::loadingFinished() {
	loadDictionaryWidget->hide();
	if (loadDictionaryThread->isSuccessful()) {
		setCurrentDatabase(currentLoadingDictName);
		*currentLoadingDictAbout = loadDictionaryThread->getAboutDict();
	}
	emit (loadingCompleted(loadDictionaryThread->isSuccessful()));
}
