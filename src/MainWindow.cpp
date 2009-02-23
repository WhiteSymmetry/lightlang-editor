#include <QtGui/QMenu>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMoveEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QCloseEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QMessageBox>
#include <QtCore/QDir>	
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include "Manual.h"
#include "DictionariesManager.h"
#include "About.h"
#include "CentralWidget.h"
#include "const.h"
#include "Menu.h"
#include "StatusBarLabel.h"
#include "MainWindow.h"

MainWindow::MainWindow() {
	createDirs();
	
	formatIsNotSuitableDialog = new QMessageBox(this);
	formatIsNotSuitableDialog->setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
	formatIsNotSuitableDialog->setWindowTitle(tr("Notification"));
	formatIsNotSuitableDialog->setText("<b>" + tr("You have chosen file with unsupported format") + "</b><br>" + tr("Format must be: &lt;dictionary name&gt;.&lt;from language&gt;-&lt;to language&gt;. Check one more time the file format, please."));
	
	statusBarLabel = new StatusBarLabel(this);
	
	statusbar = statusBar();
	statusbar->hide();
	statusbar->setMaximumHeight(20);
	
	hideStatusBarTimer = new QTimer;
	
	statusbar->addWidget(statusBarLabel,1);
	connect(hideStatusBarTimer,SIGNAL(timeout()),statusbar,SLOT(hide()));
	
	centralWidget = new CentralWidget(this);
	
	about = new About(this);
	manual = new Manual;
	
	dictionariesManager = new DictionariesManager(this);
	connect(dictionariesManager,SIGNAL(openDatabaseWithName(const QString&)),this,SLOT(openDatabaseWithName(const QString&)));
	connect(dictionariesManager,SIGNAL(removeDatabaseWithName(const QString&)),this,SLOT(removeDatabaseWithName(const QString&)));
	
	centralWidget->setExistingDictionaries(dictionariesManager->getExistingDictionaries());
	
	createActions();
	
	connect(centralWidget,SIGNAL(loadingCompleted(bool)),this,SLOT(loadingCompleted(bool)));
	connect(centralWidget,SIGNAL(changeWindowTitle(const QString&)),this,SLOT(updateWindowTitle(const QString&)));
	connect(centralWidget,SIGNAL(databaseWasOpened(const QString&)),this,SLOT(setPathForOpenedDictionary(const QString&)));
	connect(centralWidget,SIGNAL(showProgramAbout()),about,SLOT(exec()));
	connect(centralWidget,SIGNAL(showProgramDocumentation()),manual,SLOT(show()));
	
	setCentralWidget(centralWidget);
	
	setWindowTitle("LightLang Editor");
	setWindowIcon(QIcon(":/icons/lle.png"));
	setMinimumHeight(400);
	resize(600,400);
	
	show();
	loadSettings();
}

MainWindow::~MainWindow() {
	delete formatIsNotSuitableDialog;
	
    delete createDictAction;
    delete openDictAction;
	delete recentDictsMenu;
	delete openRecentDictsAction;
	delete saveDictAction;
    delete saveDictAsAction;
    delete openTabAction;
    delete closeTabAction;
    delete quitAction;

    delete pluginsManagerAction;
    delete dictsManagerAction;
	delete settingsAction;

    delete manualAction;
    delete aboutProgramAction;
	
    delete pasteBoldAction;
    delete pasteItalicAction;
    delete pasteUnderlineAction;
    delete pasteLinkAction;
    delete pasteBlockAction;
    delete pasteSoundAction;
    delete pasteSpecialAction;
	delete undoAction;
	delete redoAction;
	delete cutAction;
	delete copyAction;
	delete pasteAction;
	delete findAction;
	delete editorMenu;
	
	delete hideStatusBarTimer;
	delete statusBarLabel;
	delete dictionariesManager;
	delete about;
	delete manual;
	delete centralWidget;
}


void MainWindow::createActions() {
	createDictAction = new QAction(this);
	createDictAction->setText(tr("Create new dictionary"));
	createDictAction->setIcon(QIcon(":/icons/new.png"));
	createDictAction->setShortcut(QKeySequence("Ctrl+N"));
	connect(createDictAction,SIGNAL(triggered()),centralWidget,SLOT(showNewDictWidget()));
	
	openDictAction = new QAction(this);
	openDictAction->setText(tr("Open dictionary"));
	openDictAction->setIcon(QIcon(":/icons/open.png"));
	openDictAction->setShortcut(QKeySequence("Ctrl+O"));
	connect(openDictAction,SIGNAL(triggered()),this,SLOT(openDictionary()));
	
	recentDictsMenu = new QMenu;
	connect(recentDictsMenu,SIGNAL(triggered(QAction *)),this,SLOT(openDictionaryOfAction(QAction *)));
	
	openRecentDictsAction = new QAction(this);
	openRecentDictsAction->setText(tr("Recently opened"));
	openRecentDictsAction->setIcon(QIcon(":/icons/open_recent.png"));
	openRecentDictsAction->setMenu(recentDictsMenu);
	
	saveDictAction = new QAction(this);
	saveDictAction->setText(tr("Save dictionary"));
	saveDictAction->setIcon(QIcon(":/icons/save.png"));
	saveDictAction->setShortcut(QKeySequence("Ctrl+Alt+S"));
	saveDictAction->setEnabled(false);
	connect(saveDictAction,SIGNAL(triggered()),this,SLOT(saveDictionary()));
	
	saveDictAsAction = new QAction(this);
	saveDictAsAction->setText(tr("Save dictionary as"));
	saveDictAsAction->setIcon(QIcon(":/icons/saveas.png"));
	saveDictAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
	saveDictAsAction->setEnabled(false);
	connect(saveDictAsAction,SIGNAL(triggered()),this,SLOT(saveDictionaryAs()));
	
	connect(centralWidget,SIGNAL(startPageShown(bool)),saveDictAction,SLOT(setDisabled(bool)));
	connect(centralWidget,SIGNAL(startPageShown(bool)),saveDictAsAction,SLOT(setDisabled(bool)));
	
	openTabAction = new QAction(this);
	openTabAction->setText(tr("New tab"));
	openTabAction->setIcon(QIcon(":/icons/add.png"));
	openTabAction->setShortcut(QKeySequence("Ctrl+T"));
	openTabAction->setEnabled(false);
	connect(openTabAction,SIGNAL(triggered()),centralWidget,SLOT(openNewTab()));
	
	closeTabAction = new QAction(this);
	closeTabAction->setText(tr("Close tab"));
	closeTabAction->setIcon(QIcon(":/icons/close_tab.png"));
	closeTabAction->setShortcut(QKeySequence("Ctrl+W"));
	closeTabAction->setEnabled(false);
	connect(closeTabAction,SIGNAL(triggered()),centralWidget,SLOT(closeCurrentTab()));
	
	connect(centralWidget,SIGNAL(startPageShown(bool)),closeTabAction,SLOT(setDisabled(bool)));
	connect(centralWidget,SIGNAL(startPageShown(bool)),openTabAction,SLOT(setDisabled(bool)));
	
	quitAction = new QAction(this);
	quitAction->setText(tr("Quit"));
	quitAction->setShortcut(QKeySequence("Ctrl+Q"));
	quitAction->setIcon(QIcon(":/icons/quit.png"));
	connect(quitAction,SIGNAL(triggered()),this,SLOT(quit()));
	
	QMenu *dictionaryMenu = menuBar()->addMenu("&" + tr("Dictionary"));
	dictionaryMenu->addAction(createDictAction);
	dictionaryMenu->addAction(openDictAction);
	dictionaryMenu->addAction(openRecentDictsAction);
	dictionaryMenu->addAction(saveDictAction);
	dictionaryMenu->addAction(saveDictAsAction);
	dictionaryMenu->addSeparator();
	dictionaryMenu->addAction(openTabAction);
	dictionaryMenu->addAction(closeTabAction);
	dictionaryMenu->addSeparator();
	dictionaryMenu->addAction(quitAction);

	pluginsManagerAction = new QAction(this);
	pluginsManagerAction->setText(tr("Plugins manager"));
	pluginsManagerAction->setIcon(QIcon(":/icons/plugins_manager.png"));
	
	dictsManagerAction = new QAction(this);
	dictsManagerAction->setText(tr("Dictionaries manager"));
	dictsManagerAction->setIcon(QIcon(":/icons/dicts_manager.png"));
	connect(dictsManagerAction,SIGNAL(triggered()),dictionariesManager,SLOT(exec()));
	
	settingsAction = new QAction(this);
	settingsAction->setText(tr("Preferences"));
	settingsAction->setIcon(QIcon(":/icons/settings.png"));
	connect(settingsAction,SIGNAL(triggered()),centralWidget,SLOT(showSettings()));
	
	QMenu *toolsMenu = menuBar()->addMenu("&" + tr("Tools"));
	toolsMenu->addAction(pluginsManagerAction);
	toolsMenu->addAction(dictsManagerAction);
	toolsMenu->addAction(settingsAction);

	pasteBlockAction = new QAction(this);
	pasteBlockAction->setText(tr("Paste indent"));       
	pasteBlockAction->setShortcut(QKeySequence("Ctrl+R"));
	pasteBlockAction->setIcon(QIcon(":/icons/text_block.png"));
	
	pasteBoldAction = new QAction(this);
	pasteBoldAction->setText(tr("Paste bold"));              
	pasteBoldAction->setShortcut(QKeySequence("Ctrl+B"));
	pasteBoldAction->setIcon(QIcon(":/icons/text_bold.png"));
	
	pasteItalicAction = new QAction(this);
	pasteItalicAction->setText(tr("Paste italic"));
	pasteItalicAction->setIcon(QIcon(":/icons/text_italic.png"));
	pasteItalicAction->setShortcut(QKeySequence("Ctrl+I"));
	
	pasteUnderlineAction = new QAction(this);
	pasteUnderlineAction->setText(tr("Paste underline"));
	pasteUnderlineAction->setIcon(QIcon(":/icons/text_underline.png"));
	pasteUnderlineAction->setShortcut(QKeySequence("Ctrl+U"));
	
	pasteSpecialAction = new QAction(this);
	pasteSpecialAction->setText(tr("Paste office word"));
	pasteSpecialAction->setIcon(QIcon(":/icons/text_officeword.png"));
	pasteSpecialAction->setShortcut(QKeySequence("Ctrl+D"));
	
	pasteLinkAction = new QAction(this);
	pasteLinkAction->setText(tr("Paste link"));
	pasteLinkAction->setIcon(QIcon(":/icons/text_link.png"));
	pasteLinkAction->setShortcut(QKeySequence("Ctrl+L"));
	
	pasteSoundAction = new QAction(this);
	pasteSoundAction->setText(tr("Paste sound"));
	pasteSoundAction->setIcon(QIcon(":/icons/text_sound.png"));
	
	undoAction = new QAction(this);
	undoAction->setText(tr("Undo action"));
	undoAction->setIcon(QIcon(":/icons/undo.png"));
	undoAction->setShortcut(QKeySequence("Ctrl+Z"));
	
	redoAction = new QAction(this);
	redoAction->setText(tr("Redo action"));
	redoAction->setIcon(QIcon(":/icons/redo.png"));
	redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));
	
	cutAction = new QAction(this);
	cutAction->setText(tr("Cut"));
	cutAction->setIcon(QIcon(":/icons/cut.png"));
	cutAction->setShortcut(QKeySequence("Ctrl+X"));
	
	copyAction = new QAction(this);
	copyAction->setText(tr("Copy"));
	copyAction->setIcon(QIcon(":/icons/copy.png"));
	copyAction->setShortcut(QKeySequence("Ctrl+C"));
	
	pasteAction = new QAction(this);
	pasteAction->setText(tr("Paste"));
	pasteAction->setIcon(QIcon(":/icons/paste.png"));
	pasteAction->setShortcut(QKeySequence("Ctrl+V"));
	
	findAction = new QAction(this);
	findAction->setText(tr("Find..."));
	findAction->setIcon(QIcon(":/icons/search.png"));
	findAction->setShortcut(QKeySequence("Ctrl+F"));
	connect(findAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(showFindWidgetInCurrentTab()));
	
		
	editorMenu = new Menu;
	editorMenu->setHeaderIcon(QIcon(":/icons/edit.png"));
	editorMenu->setHeaderText("LightLang Editor");
	editorMenu->addAction(pasteBlockAction);
	editorMenu->addAction(pasteBoldAction);
	editorMenu->addAction(pasteItalicAction);
	editorMenu->addAction(pasteUnderlineAction);
	editorMenu->addAction(pasteSpecialAction);
	editorMenu->addAction(pasteSpecialAction);
	editorMenu->addAction(pasteLinkAction);
	editorMenu->addAction(pasteSoundAction);
	editorMenu->addSeparator();
	editorMenu->addAction(cutAction);
	editorMenu->addAction(copyAction);
	editorMenu->addAction(pasteAction);
	editorMenu->addSeparator();
	editorMenu->addAction(undoAction);
	editorMenu->addAction(redoAction);
	editorMenu->addSeparator();
	editorMenu->addAction(findAction);
	
	QMenu *editMenu = menuBar()->addMenu("&" + tr("Edit"));
	editMenu->addAction(pasteBlockAction);
	editMenu->addAction(pasteBoldAction);
	editMenu->addAction(pasteItalicAction);
	editMenu->addAction(pasteUnderlineAction);
	editMenu->addAction(pasteSpecialAction);
	editMenu->addAction(pasteLinkAction);
	editMenu->addAction(pasteSoundAction);
	editMenu->addSeparator();
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addSeparator();
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addSeparator();
	editMenu->addAction(findAction);
	
	editionToolBar = addToolBar(tr("Edition tool bar"));
	editionToolBar->setObjectName("EditionToolBar");
	editionToolBar->addAction(pasteBlockAction);
	editionToolBar->addAction(pasteBoldAction);
	editionToolBar->addAction(pasteItalicAction);
	editionToolBar->addAction(pasteUnderlineAction);
	editionToolBar->addAction(pasteSpecialAction);
	editionToolBar->addAction(pasteLinkAction);
	editionToolBar->addAction(pasteSoundAction);
	editionToolBar->addSeparator();
	editionToolBar->addAction(cutAction);
	editionToolBar->addAction(copyAction);
	editionToolBar->addAction(pasteAction);
	editionToolBar->addSeparator();
	editionToolBar->addAction(undoAction);
	editionToolBar->addAction(redoAction);
	editionToolBar->addSeparator();
	editionToolBar->addAction(findAction);
	editionToolBar->hide();
	editionToolBar->setOrientation(Qt::Vertical);
	
	centralWidget->setEditorMenu(editorMenu);
	
	connect(centralWidget,SIGNAL(startPageShown(bool)),editionToolBar,SLOT(setHidden(bool)));
	connect(centralWidget,SIGNAL(startPageShown(bool)),this,SLOT(disableEditionActions(bool)));
	disableEditionActions(true);
	
	manualAction = new QAction(this);
	manualAction->setText(tr("Manual"));
	manualAction->setIcon(QIcon(":/icons/manual.png"));
	manualAction->setShortcut(QKeySequence("F1"));
	connect(manualAction,SIGNAL(triggered()),manual,SLOT(show()));
	
	aboutProgramAction = new QAction(this);
	aboutProgramAction->setText(tr("About program"));
	aboutProgramAction->setIcon(QIcon(":/icons/lle.png"));
	connect(aboutProgramAction,SIGNAL(triggered()),about,SLOT(exec()));
	
	QMenu *helpMenu = menuBar()->addMenu("&" + tr("Help"));
	helpMenu->addAction(manualAction);
	helpMenu->addAction(aboutProgramAction);
	
	// Different actions to make navigation easier
	QAction *moveNextTabAction = new QAction(this);
	moveNextTabAction->setShortcut(QKeySequence("Alt+Right"));
	connect(moveNextTabAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(moveNextTab()));
	
	QAction *movePreviousTabAction = new QAction(this);
	movePreviousTabAction->setShortcut(QKeySequence("Alt+Left"));
	connect(movePreviousTabAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(movePreviousTab()));
	
	addAction(movePreviousTabAction);
	addAction(moveNextTabAction);
}

void MainWindow::createDirs() {
	homePath = QDir::toNativeSeparators(QDir::currentPath() + "/EditorData/");
	QDir dir;
	if (!dir.exists(homePath))
		dir.mkpath(homePath);
	
	databasesPath = QDir::toNativeSeparators(homePath + "Databases/");
	if (!dir.exists(databasesPath))
		dir.mkpath(databasesPath);
	
	controlPath = QDir::toNativeSeparators(homePath + "Control/");
	if (!dir.exists(controlPath))
		dir.mkpath(controlPath);
}
		
void MainWindow::createNewDictionary(const QString& dictName) {
	dictionariesManager->addDictionary(dictName);
	showStatusMessage(tr("New dictionary with name %1 was created. Choose it in Dictionaries Manager to edit.").arg(dictName));
}

void MainWindow::moveEvent(QMoveEvent *event) {
	emit (moved(event->pos().x(),event->pos().y()));
}

void MainWindow::openDictionary() {
	QString dictionaryPath;
	while(true) {
		dictionaryPath = QFileDialog::getOpenFileName(this,tr("Open a dictionary"),QDir::homePath(),tr("SL dictionaries(*.*-*)"));
		if (dictionaryPath.isEmpty())
			return;
		if (!dictionaryPath.contains(QRegExp("^(.*\\.[a-z][a-z]\\-[a-z][a-z])$")))
			formatIsNotSuitableDialog->exec();
		else
			break;
	}
	currentLoadingDictPath = dictionaryPath;
	currentLoadingDictAbout.clear();
	centralWidget->loadDictionary(currentLoadingDictPath);
}

void MainWindow::loadingCompleted(bool isSuccessful) {
	if (isSuccessful) {
		currentLoadingDictAbout = centralWidget->getLoadedDictAbout();
		dictionariesManager->addDictionary(QFileInfo(currentLoadingDictPath).fileName(),currentLoadingDictPath,currentLoadingDictAbout);
		recentOpenedDictionaries << QFileInfo(currentLoadingDictPath).fileName();
		updateRecentDictsMenu();
	}
}

void MainWindow::showDictionariesManager() {
	dictionariesManager->exec();
}

void MainWindow::loadSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	recentOpenedDictionaries = settings.value("General/RecentOpenedDictionaries").toStringList();
	updateRecentDictsMenu();
	resize(settings.value("MainWindow/Size",QSize(800,400)).toSize());
	move(settings.value("MainWindow/Position",QPoint(0,0)).toPoint());
	restoreState(settings.value("MainWindow/State").toByteArray());
	centralWidget->loadSettings();
	manual->loadSettings();
}

void MainWindow::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	updateRecentDictsMenu();
	settings.setValue("General/RecentOpenedDictionaries",recentOpenedDictionaries);
	settings.setValue("MainWindow/Position",pos());
	settings.setValue("MainWindow/Size",size());
	editionToolBar->hide();
	settings.setValue("MainWindow/State",saveState());
	manual->saveSettings();
}

void MainWindow::quit() {
	if (!centralWidget->saveSettings())
		return;
	saveSettings();
	emit (toQuit());
}

void MainWindow::closeEvent(QCloseEvent *event) {
	event->ignore();
	quit();
}

void MainWindow::updateRecentDictsMenu() {
	recentDictsMenu->clear();
	if (recentOpenedDictionaries.count() > 6)
		for (int i = 0; i < recentOpenedDictionaries.count()-6; i++)
			recentOpenedDictionaries.removeFirst();
	for (int i = 0; i < recentOpenedDictionaries.count(); i++)
		if (recentOpenedDictionaries.count(recentOpenedDictionaries[i]) > 1)
			recentOpenedDictionaries.removeAt(i);
	for (int i = recentOpenedDictionaries.count() - 1; i >= 0; i--)
		if (dictionariesManager->getExistingDictionaries().contains(recentOpenedDictionaries[i]))
			recentDictsMenu->addAction(recentOpenedDictionaries[i]);
	
	QString startPageText = 
		"<hr><table border=\"0\" width=\"100%\"><tr><td bgcolor=\"#DFEDFF\"><h2 align=\"center\"><em>" + 
		tr("Start page") + 	
		"</em></h2></td></tr></table><hr>&nbsp;&nbsp;&nbsp;&nbsp;" +
		tr("Hello, thank you for using LightLang Editor! This program can help you to edit existing dictionaries, create new and add it to SL database. Please, %1read documentation%4 about SL tags before starting edition to learn about formatting text in SL dictionaries. If you started LightLang in first time, you should open existing dictionary to edit or create new. Also you can %2point your preferences%4 to make your working process with editor easier and %3read some extra information%4 about program.").arg("<a href=\"documentation\">").arg("<a href=\"preferences\">").arg("<a href=\"about\">").arg("</a>") +
		"<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>" + tr("Recent opened dictionaries") + ":</b>";
	startPageText += "<ul>";
	for (int i = recentOpenedDictionaries.count() - 1; i >= 0; i--)
		if (dictionariesManager->getExistingDictionaries().contains(recentOpenedDictionaries[i]))
			startPageText += "<li><a href=\"" + recentOpenedDictionaries[i] + "\">" + recentOpenedDictionaries[i] + "</a></li>";
	startPageText += "</ul>";
	
	centralWidget->setStartPageText(startPageText);
}

void MainWindow::updateWindowTitle(const QString& addToTitle) {
	if (addToTitle.isEmpty())
		setWindowTitle("LightLang Editor");
	else
		setWindowTitle("LightLang Editor - " + addToTitle);
}

void MainWindow::removeDatabaseWithName(const QString& dbName) {
	centralWidget->removeDatabaseWithName(dbName);
	if (recentOpenedDictionaries.contains(dbName)) {
		recentOpenedDictionaries.removeAll(dbName);
		updateRecentDictsMenu();
	}
}

void MainWindow::openDatabaseWithName(const QString& databaseName) {
	recentOpenedDictionaries << databaseName;
	updateRecentDictsMenu();
	centralWidget->setCurrentDatabase(databaseName);
	centralWidget->showTabsWidget();
}

void MainWindow::disableEditionActions(bool isDisabled) {
	pasteBoldAction->setEnabled(!isDisabled);
	pasteItalicAction->setEnabled(!isDisabled);
	pasteUnderlineAction->setEnabled(!isDisabled);
	pasteLinkAction->setEnabled(!isDisabled);
	pasteBlockAction->setEnabled(!isDisabled);
	pasteSoundAction->setEnabled(!isDisabled);
	pasteSpecialAction->setEnabled(!isDisabled);
	cutAction->setEnabled(!isDisabled);
	copyAction->setEnabled(!isDisabled);
	pasteAction->setEnabled(!isDisabled);
	undoAction->setEnabled(!isDisabled);
	redoAction->setEnabled(!isDisabled);
}

void MainWindow::openDictionaryOfAction(QAction *chosenAction) {
	centralWidget->setCurrentDatabase(chosenAction->text());
	centralWidget->showTabsWidget();
}

void MainWindow::saveDictionary() {
	if (!centralWidget->saveDictionary())
		saveDictionaryAs();
}

void MainWindow::saveDictionaryAs() {
	QString filePath = QFileDialog::getSaveFileName(this,tr("Save dictionary as..."),QDir::homePath(),tr("SL dictionaries(*.*-*"));
	if (filePath.isEmpty())
		return;
	centralWidget->saveDictionaryAs(filePath);
}

void MainWindow::setPathForOpenedDictionary(const QString& dbName) {
	centralWidget->setPathForOpenedDictionary(dictionariesManager->getPathForDictionaryWithName(dbName),
											dictionariesManager->getDictionaryAboutWithName(dbName));
}

void MainWindow::showStatusMessage(const QString& message) {
	statusbar->show();
	statusBarLabel->setMessage(message);
	hideStatusBarTimer->start(10000);
}
