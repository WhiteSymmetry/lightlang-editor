#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
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
	
	closeOrNoIfLoadingDialog = new QMessageBox(mainWindowCommunicater);
	closeOrNoIfLoadingDialog->setIconPixmap(QPixmap(":/icons/main.png"));
	closeOrNoIfLoadingDialog->setWindowTitle(tr("Notification"));
	closeOrNoIfLoadingDialog->setText("<b>" + tr("Dictionary is loading...") + "</b><br>" + tr("If you close LightLang Editor, loading will be canceled."));
	
	continueOrRestartLoadingDialog = new QMessageBox(mainWindowCommunicater);
	continueOrRestartLoadingDialog->setIconPixmap(QPixmap(":/icons/main.png"));
	continueOrRestartLoadingDialog->setWindowTitle(tr("Notification"));
	continueOrRestartLoadingDialog->setText("<b>" + tr("You have already loaded dictionary with this name") + "</b><br>" + tr("But the dictionary wasn't full loaded. You can continue loading or restart it."));
	
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
	connect(loadDictionaryThread,SIGNAL(loadingFinished()),this,SLOT(loadingFinished()));
	connect(loadDictionaryWidget,SIGNAL(paused()),loadDictionaryThread,SLOT(stopLoading()));
	connect(loadDictionaryWidget,SIGNAL(continued()),loadDictionaryThread,SLOT(continueLoading()));
	
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
	delete closeOrNoIfLoadingDialog;
	delete continueOrRestartLoadingDialog;
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
	if (databaseCenter->doesDictionaryExist(dictPath)) {
		continueOrRestartLoadingDialog->exec();
	}
	
	currentLoadingDictAbout = aboutDictionaryString;
	loadDictionaryWidget->reset();
	loadDictionaryWidget->show();
	if (loadDictionaryThread->startLoading(dictPath)) {
		currentLoadingDictName = QFileInfo(dictPath).fileName();
		loadDictionaryThread->start();
	} else {
		qDebug() << "[CentralWidget] Cannot load dictionary" << dictPath;
		loadDictionaryWidget->hide();
	}
}

void CentralWidget::cancelLoading() {
	loadDictionaryThread->cancelLoading();
	loadDictionaryWidget->hide();
	removeDatabaseWithName(currentLoadingDictName);
}

void CentralWidget::removeDatabaseWithName(const QString& dbName) {
	databaseCenter->removeDatabaseWithName(dbName);
}

void CentralWidget::loadingFinished() {
	loadDictionaryWidget->hide();
	setCurrentDatabase(currentLoadingDictName);
	*currentLoadingDictAbout = loadDictionaryThread->getAboutDict();
	emit (loadingCompleted(true));
}

void CentralWidget::setStartPageText(const QString& text) {
	startPageViewer->setHtml(text);
}

void CentralWidget::saveSettings() {
	if (loadDictionaryThread->isRunning()) {
		loadDictionaryThread->stopLoading();
		closeOrNoIfLoadingDialog->exec();
	}
}
