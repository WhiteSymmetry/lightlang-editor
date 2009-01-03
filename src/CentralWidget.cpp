#include <QtGui/QToolButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QDebug>
#include "TabsWidget.h"
#include "NewDictWidget.h"
#include "LoadDictionaryWidget.h"
#include "BrowserWithWidgets.h"
#include "DatabaseCenter.h"
#include "LoadDictionaryThread.h"
#include "StackedWidget.h"
#include "SettingsWidget.h"
#include "const.h"
#include "CentralWidget.h"

CentralWidget::CentralWidget(QWidget *mainWindowCommunicater) {
	
	continueLoadingOfLastLoadedOrNotDialog = new QMessageBox(mainWindowCommunicater);
	continueLoadingOfLastLoadedOrNotDialog->setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
	continueLoadingOfLastLoadedOrNotDialog->setWindowTitle(tr("Offer"));
	continueLoadingLastLoadedButton = continueLoadingOfLastLoadedOrNotDialog->addButton(tr("Continue loading"),QMessageBox::ActionRole);
	ignoreLoadingLastLoadedButton = continueLoadingOfLastLoadedOrNotDialog->addButton(tr("Ignore"),QMessageBox::ActionRole);
	continueLoadingOfLastLoadedDictionary = false;
	
	cancelOrContinueCurrentLoadingDialog = new QMessageBox(mainWindowCommunicater);
	cancelOrContinueCurrentLoadingDialog->setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
	cancelOrContinueCurrentLoadingDialog->setWindowTitle(tr("Notification"));
	cancelOrContinueCurrentLoadingDialog->setText("<b>" + tr("Another dictionary is already loading...") + "</b><br>" + tr("If you continue, current loading will be canceled, but you will be able to resume it later."));
	continueStartOfNewLoadingButton = cancelOrContinueCurrentLoadingDialog->addButton(tr("Continue action"),QMessageBox::ActionRole);
	cancelStartOfNewLoadingButton = cancelOrContinueCurrentLoadingDialog->addButton(tr("Cancel action"),QMessageBox::ActionRole);
	
	closeOrNoIfLoadingDialog = new QMessageBox(mainWindowCommunicater);
	closeOrNoIfLoadingDialog->setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
	closeOrNoIfLoadingDialog->setWindowTitle(tr("Notification"));
	closeOrNoIfLoadingDialog->setText("<b>" + tr("Dictionary is loading...") + "</b><br>" + tr("If you close LightLang Editor, dictionary loading will be canceled."));
	continueIfLoadingButton = closeOrNoIfLoadingDialog->addButton(tr("Continue"),QMessageBox::ActionRole);
	closeIfLoadingButton = closeOrNoIfLoadingDialog->addButton(tr("Close"),QMessageBox::ActionRole);
	
	continueOrRestartLoadingDialog = new QMessageBox(mainWindowCommunicater);
	continueOrRestartLoadingDialog->setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
	continueOrRestartLoadingDialog->setWindowTitle(tr("Notification"));
	continueOrRestartLoadingDialog->setText("<b>" + tr("You have already loaded dictionary with this name") + "</b><br>" + tr("But the dictionary wasn't full loaded. You can continue loading or restart it."));
	continueLoadingButton = continueOrRestartLoadingDialog->addButton(tr("Continue"),QMessageBox::ActionRole);
	restartLoadingButton = continueOrRestartLoadingDialog->addButton(tr("Restart"),QMessageBox::ActionRole);
	ignoreLoadingButton = continueOrRestartLoadingDialog->addButton(tr("Ignore"),QMessageBox::ActionRole);
	
	databaseCenter = new DatabaseCenter;
	connect(databaseCenter,SIGNAL(databaseNameChanged(const QString&)),mainWindowCommunicater,SLOT(updateWindowTitle(const QString&)));
	
	openDictBorderButton = new QToolButton;
	openDictBorderButton->setCursor(Qt::ArrowCursor);
	openDictBorderButton->setAutoRaise(true);
	openDictBorderButton->setIcon(QIcon(":/icons/open.png"));
	openDictBorderButton->setIconSize(QSize(22,22));
	openDictBorderButton->setToolTip(tr("Open a dictionary"));
	connect(openDictBorderButton,SIGNAL(clicked()),mainWindowCommunicater,SLOT(openDictionary()));
	
	createNewDictBorderButton = new QToolButton;
	createNewDictBorderButton->setCursor(Qt::ArrowCursor);
	createNewDictBorderButton->setAutoRaise(true);
	createNewDictBorderButton->setIcon(QIcon(":/icons/new.png"));
	createNewDictBorderButton->setIconSize(QSize(22,22));
	createNewDictBorderButton->setToolTip(tr("Create a new dictionary"));
	connect(createNewDictBorderButton,SIGNAL(clicked()),this,SLOT(showNewDictWidget()));
	
	showDictsManagerButton = new QToolButton;
	showDictsManagerButton->setCursor(Qt::ArrowCursor);
	showDictsManagerButton->setAutoRaise(true);
	showDictsManagerButton->setIcon(QIcon(":/icons/dicts_manager.png"));
	showDictsManagerButton->setIconSize(QSize(22,22));
	showDictsManagerButton->setToolTip(tr("Show loaded dictionaries"));
	connect(showDictsManagerButton,SIGNAL(clicked()),mainWindowCommunicater,SLOT(showDictionariesManager()));
	
	startPageViewer = new BrowserWithWidgets(this);
	startPageViewer->addWidget(createNewDictBorderButton);
	startPageViewer->addWidget(openDictBorderButton);
	startPageViewer->addWidget(showDictsManagerButton);
	connect(startPageViewer,SIGNAL(linkWasClicked(const QString&)),this,SLOT(setCurrentDatabase(const QString&)));
	connect(startPageViewer,SIGNAL(linkWasClicked(const QString&)),this,SLOT(showTabsWidget()));

	stackedWidget = new StackedWidget;
	
	tabsWidget = new TabsWidget(databaseCenter);
	connect(tabsWidget,SIGNAL(closeTabButtonClicked()),this,SLOT(closeCurrentTab()));
	
	settingsWidget = new SettingsWidget;
	connect(settingsWidget,SIGNAL(closed()),this,SLOT(closeSettings()));
	
	stackedWidget->addNewWidget(startPageViewer);
	stackedWidget->addNewWidget(tabsWidget);
	stackedWidget->addNewWidget(settingsWidget);
	stackedWidget->setCurrentIndex(0);
	connect(stackedWidget,SIGNAL(currentIndexChanged(int)),this,SLOT(currentWidgetChanged(int)));
	
	loadDictionaryWidget = new LoadDictionaryWidget;
	loadDictionaryWidget->setMaximumHeight(0);
	connect(loadDictionaryWidget,SIGNAL(canceled()),this,SLOT(cancelLoading()));
	connect(loadDictionaryWidget,SIGNAL(openLastLoadedDictionary()),this,SLOT(openLastLoadedDictionary()));
	
	loadDictionaryThread = new LoadDictionaryThread;
	connect(loadDictionaryThread,SIGNAL(rowsCounted(int)),loadDictionaryWidget,SLOT(setMaximum(int)));
	connect(loadDictionaryThread,SIGNAL(rowChanged(int)),loadDictionaryWidget,SLOT(addValue()));
	connect(loadDictionaryThread,SIGNAL(loadingFinished()),this,SLOT(loadingFinished()));
	connect(loadDictionaryWidget,SIGNAL(paused()),loadDictionaryThread,SLOT(stopLoading()));
	connect(loadDictionaryWidget,SIGNAL(continued()),loadDictionaryThread,SLOT(continueLoading()));
	
	newDictWidget = new NewDictWidget;
	newDictWidget->setMaximumHeight(0);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(newDictWidget);
	mainLayout->addWidget(stackedWidget,1);
	mainLayout->addWidget(loadDictionaryWidget);
	mainLayout->setContentsMargins(0,0,0,0);
	setLayout(mainLayout);
	
	connect(newDictWidget,SIGNAL(createDictionary(const QString&)),mainWindowCommunicater,SLOT(createNewDictionary(const QString&)));
}

CentralWidget::~CentralWidget() {
	delete continueLoadingLastLoadedButton;
	delete ignoreLoadingLastLoadedButton;
	delete cancelStartOfNewLoadingButton;
	delete continueStartOfNewLoadingButton;
	delete continueIfLoadingButton;
	delete closeIfLoadingButton;
	delete continueLoadingButton;
	delete restartLoadingButton;
	delete ignoreLoadingButton;
	
	delete closeOrNoIfLoadingDialog;
	delete continueOrRestartLoadingDialog;
	delete continueLoadingOfLastLoadedOrNotDialog;
	delete cancelOrContinueCurrentLoadingDialog;
	
	delete settingsWidget;
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
	newDictWidget->showWithRolling();
}

void CentralWidget::openNewTab() {
	tabsWidget->openNewTab();
}

void CentralWidget::showTabsWidget() {
	stackedWidget->setCurrentIndex(1);
	if (tabsWidget->count() == 0)
		tabsWidget->openNewTab();
}

void CentralWidget::closeCurrentTab() {
	if (tabsWidget->count() == 1)
		stackedWidget->setCurrentIndex(0);
	tabsWidget->closeCurrentTab();
}

void CentralWidget::resizeEvent(QResizeEvent *) {
	emit (resized(width(),height()));
}

void CentralWidget::setCurrentDatabase(const QString& dbName) {
	databaseCenter->setDatabaseName(dbName);
	emit (databaseWasOpened(dbName));
}

void CentralWidget::setExistingDictionaries(const QStringList& list) {
	newDictWidget->setInvalidNames(list);
	existingDictionaries = list;
}

void CentralWidget::loadDictionary(const QString& dictPath) {
	if (existingDictionaries.contains(QFileInfo(dictPath).fileName())) {
		setCurrentDatabase(QFileInfo(dictPath).fileName());
		showTabsWidget();
		return;
	}
	
	if (loadDictionaryThread->isRunning()) {
		if (dictPath != loadDictionaryThread->getDictionaryPath()) {
			cancelOrContinueCurrentLoadingDialog->exec();
			if (cancelOrContinueCurrentLoadingDialog->clickedButton() == continueStartOfNewLoadingButton)
				loadDictionaryThread->stopLoading();
			else
				return;
		} else {
			loadDictionaryWidget->showWithRolling();
			return;
		}
	}
	
	if (databaseCenter->doesDictionaryExist(dictPath) && !continueLoadingOfLastLoadedDictionary) {
		continueOrRestartLoadingDialog->exec();
		if (continueOrRestartLoadingDialog->clickedButton() == ignoreLoadingButton)
			return;
		else if (continueOrRestartLoadingDialog->clickedButton() == restartLoadingButton)
			databaseCenter->removeDatabaseWithName(QFileInfo(dictPath).fileName());
	}
	
	currentLoadingDictAbout.clear();
	loadDictionaryWidget->reset();
	loadDictionaryWidget->showWithRolling();
	if (loadDictionaryThread->startLoading(dictPath)) {
		currentLoadingDictName = QFileInfo(dictPath).fileName();
		loadDictionaryThread->start();
	} else {
		qDebug() << "[CentralWidget] Cannot load dictionary" << dictPath;
		loadDictionaryWidget->hideWithRolling();
	}
	continueLoadingOfLastLoadedDictionary = false;
}

void CentralWidget::cancelLoading() {
	loadDictionaryThread->cancelLoading();
	loadDictionaryWidget->hideWithRolling();
	removeDatabaseWithName(currentLoadingDictName);
}

void CentralWidget::removeDatabaseWithName(const QString& dbName) {
	if (dbName == databaseCenter->getCurrentDatabaseName()) {
		stackedWidget->setCurrentIndex(0);
		emit (changeWindowTitle(""));
	}
	existingDictionaries.removeAll(dbName);
	databaseCenter->removeDatabaseWithName(dbName);
}

void CentralWidget::loadingFinished() {
	existingDictionaries << currentLoadingDictName;
	currentLoadingDictAbout = loadDictionaryThread->getAboutDict();
	emit (loadingCompleted(true));
}

void CentralWidget::setStartPageText(const QString& text) {
	startPageViewer->setHtml(text);
}

void CentralWidget::loadSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	QString lastLoadedDictionary = settings.value("CentralWidget/LastLoadedDictionary").toString();
	if (!lastLoadedDictionary.isEmpty()) {
		continueLoadingOfLastLoadedOrNotDialog->setText("<b>" + tr("Dictionary was loading at last quit...") + "</b><br>" + tr("When you quitted last time, the dictionary \'%1\' was loading. You can continue loading now or ignore it.").arg(QFileInfo(lastLoadedDictionary).fileName()));
		continueLoadingOfLastLoadedOrNotDialog->exec();
		if (continueLoadingOfLastLoadedOrNotDialog->clickedButton() == continueLoadingLastLoadedButton) {
			continueLoadingOfLastLoadedDictionary = true;
			loadDictionary(lastLoadedDictionary);
		}
	}
}

bool CentralWidget::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	
	if (loadDictionaryThread->isRunning()) {
		closeOrNoIfLoadingDialog->exec();
		if (closeOrNoIfLoadingDialog->clickedButton() == continueIfLoadingButton)
			return false;
		loadDictionaryThread->stopLoading();
	} else if (loadDictionaryThread->isStopped()) {
		settings.setValue("CentralWidget/LastLoadedDictionary",loadDictionaryThread->getDictionaryPath());
	} else
		settings.setValue("CentralWidget/LastLoadedDictionary","");
	return true;
}

QString CentralWidget::getLoadedDictAbout() const {
	return currentLoadingDictAbout;
}

void CentralWidget::openLastLoadedDictionary() {
	loadDictionaryWidget->hideWithRolling();
	setCurrentDatabase(currentLoadingDictName);
	showTabsWidget();
}

void CentralWidget::currentWidgetChanged(int widgetIndex) {
	emit (startPageShown(widgetIndex != 1));
	if (widgetIndex == 1)
		tabsWidget->setFocusOnCurrentTab();
}

void CentralWidget::setEditorMenu(Menu *menu) {
	tabsWidget->setEditorMenu(menu);
}

bool CentralWidget::saveDictionary() {
	if (currentOpenedDictPath.isEmpty())
		return false;
	saveDictionaryAs(currentOpenedDictPath);
	return true;
}

void CentralWidget::saveDictionaryAs(const QString& path) {
	databaseCenter->saveCurrentDatabaseAs(path,currentOpenedDictAbout);
}

void CentralWidget::setPathForOpenedDictionary(const QString& path,const QString& about) {
	currentOpenedDictPath = path;
	currentOpenedDictAbout = about;
}

void CentralWidget::showSettings() {
	stackedWidget->setCurrentIndex(2);
}

void CentralWidget::closeSettings() {
	if (tabsWidget->count() > 0)
		stackedWidget->setCurrentIndex(1);
	else
		stackedWidget->setCurrentIndex(0);
}
