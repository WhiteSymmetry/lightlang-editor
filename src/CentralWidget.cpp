#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
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
	connect(databaseCenter,SIGNAL(databaseNameChanged(const QString&)),mainWindowCommunicater,SLOT(updateWindowTitle(const QString&)));
	
	openDictBorderButton = new QToolButton;
	openDictBorderButton->setCursor(Qt::ArrowCursor);
	openDictBorderButton->setAutoRaise(true);
	openDictBorderButton->setIcon(QIcon(":/icons/open.png"));
	openDictBorderButton->setIconSize(QSize(22,22));
	connect(openDictBorderButton,SIGNAL(clicked()),mainWindowCommunicater,SLOT(openDictionary()));
	
	createNewDictBorderButton = new QToolButton;
	createNewDictBorderButton->setCursor(Qt::ArrowCursor);
	createNewDictBorderButton->setAutoRaise(true);
	createNewDictBorderButton->setIcon(QIcon(":/icons/new.png"));
	createNewDictBorderButton->setIconSize(QSize(22,22));
	connect(createNewDictBorderButton,SIGNAL(clicked()),this,SLOT(showNewDictWidget()));
	
	showDictsManagerButton = new QToolButton;
	showDictsManagerButton->setCursor(Qt::ArrowCursor);
	showDictsManagerButton->setAutoRaise(true);
	showDictsManagerButton->setIcon(QIcon(":/icons/dicts_manager.png"));
	showDictsManagerButton->setIconSize(QSize(22,22));
	connect(showDictsManagerButton,SIGNAL(clicked()),mainWindowCommunicater,SLOT(showDictionariesManager()));
	
	startPageViewer = new BrowserWithWidgets(this);
	startPageViewer->addWidget(createNewDictBorderButton);
	startPageViewer->addWidget(openDictBorderButton);
	startPageViewer->addWidget(showDictsManagerButton);
	connect(startPageViewer,SIGNAL(linkWasClicked(const QString&)),this,SLOT(setCurrentDatabase(const QString&)));

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
	connect(loadDictionaryWidget,SIGNAL(stopped()),loadDictionaryThread,SLOT(stop()));
	
	newDictWidget = new NewDictWidget;
	newDictWidget->hide();
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(newDictWidget);
	mainLayout->addWidget(stackedWidget,1);
	mainLayout->addWidget(loadDictionaryWidget);
	mainLayout->setContentsMargins(0,0,0,0);
	setLayout(mainLayout);
	
	connect(newDictWidget,SIGNAL(createDictionary(const QString&)),mainWindowCommunicater,SLOT(createNewDictionary(const QString&)));
	connect(newDictWidget,SIGNAL(createDictionary(const QString&)),this,SLOT(showTabsWidget()));
}

CentralWidget::~CentralWidget() {
	delete loadDictionaryWidget;
    delete tabsWidget;
	delete createNewDictBorderButton;
	delete openDictBorderButton;
	delete showDictsManagerButton;
	delete newDictWidget;
	delete startPageViewer;
	delete stackedWidget;
	delete databaseCenter;
	delete loadDictionaryThread;
}

void CentralWidget::showNewDictWidget() {
	newDictWidget->show();
}

void CentralWidget::openNewTab() {
	stackedWidget->setCurrentIndex(1);
	emit(startPageShown(false));
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
	loadDictionaryThread->cancel();
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

void CentralWidget::setStartPageText(const QString& text) {
	startPageViewer->setHtml(text);
}
