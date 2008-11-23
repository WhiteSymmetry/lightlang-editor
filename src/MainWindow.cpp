#include <QtGui/QMenu>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMoveEvent>
#include <QtGui/QFileDialog>
#include <QtCore/QDir>	
#include <QtCore/QSettings>
#include "Manual.h"
#include "DictionariesManager.h"
#include "About.h"
#include "CentralWidget.h"
#include "const.h"
#include "MainWindow.h"

MainWindow::MainWindow() {
	createDirs();
	
	centralWidget = new CentralWidget(this);
	connect(centralWidget,SIGNAL(loadingCompleted(bool)),this,SLOT(loadingCompleted(bool)));
	
	about = new About(this);
	manual = new Manual;
	
	dictionariesManager = new DictionariesManager(this);
	connect(dictionariesManager,SIGNAL(openDatabaseWithName(const QString&)),centralWidget,SLOT(setCurrentDatabase(const QString&)));
	connect(dictionariesManager,SIGNAL(removeDatabaseWithName(const QString&)),centralWidget,SLOT(removeDatabaseWithName(const QString&)));
	
	centralWidget->setExistingDictionaries(dictionariesManager->getExistingDictionaries());
	
	createActions();
	
	connect(centralWidget,SIGNAL(startPageShown(bool)),editionToolBar,SLOT(setHidden(bool)));
	
	setCentralWidget(centralWidget);
	
	setWindowTitle("LightLang Editor");
	setWindowIcon(QIcon(":/icons/lle.png"));
	setMinimumHeight(400);
	resize(600,400);
	
	loadSettings();
}

MainWindow::~MainWindow() {
	
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

    delete manualAction;
    delete aboutProgramAction;
	
    delete pasteBoldAction;
    delete pasteItalicAction;
    delete pasteUnderlineAction;
    delete pasteLinkAction;
    delete pasteBlockAction;
    delete pasteSoundAction;
    delete pasteSpecialAction;
	
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
	
	openRecentDictsAction = new QAction(this);
	openRecentDictsAction->setText(tr("Recently opened"));
	openRecentDictsAction->setIcon(QIcon(":/icons/open_recent.png"));
	openRecentDictsAction->setMenu(recentDictsMenu);
	
	saveDictAction = new QAction(this);
	saveDictAction->setText(tr("Save dictionary"));
	saveDictAction->setIcon(QIcon(":/icons/save.png"));
	saveDictAction->setShortcut(QKeySequence("Ctrl+S"));
	
	saveDictAsAction = new QAction(this);
	saveDictAsAction->setText(tr("Save dictionary as"));
	saveDictAsAction->setIcon(QIcon(":/icons/saveas.png"));
	saveDictAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
	
	openTabAction = new QAction(this);
	openTabAction->setText(tr("New tab"));
	openTabAction->setIcon(QIcon(":/icons/new_tab.png"));
	openTabAction->setShortcut(QKeySequence("Ctrl+T"));
	connect(openTabAction,SIGNAL(triggered()),centralWidget,SLOT(openNewTab()));
	
	closeTabAction = new QAction(this);
	closeTabAction->setText(tr("Close tab"));
	closeTabAction->setIcon(QIcon(":/icons/close_tab.png"));
	closeTabAction->setShortcut(QKeySequence("Ctrl+W"));
	connect(closeTabAction,SIGNAL(triggered()),centralWidget,SLOT(closeCurrentTab()));
	
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
	
	QMenu *toolsMenu = menuBar()->addMenu("&" + tr("Tools"));
	toolsMenu->addAction(pluginsManagerAction);
	toolsMenu->addAction(dictsManagerAction);

	pasteBlockAction = new QAction(this);
	pasteBlockAction->setText(tr("Paste block"));       
	pasteBlockAction->setShortcut(QKeySequence("Alt+S"));
	pasteBlockAction->setIcon(QIcon(":/icons/text_block.png"));
	
	pasteBoldAction = new QAction(this);
	pasteBoldAction->setText(tr("Paste bold"));              
	pasteBoldAction->setShortcut(QKeySequence("Alt+B"));
	pasteBoldAction->setIcon(QIcon(":/icons/text_bold.png"));
	
	pasteItalicAction = new QAction(this);
	pasteItalicAction->setText(tr("Paste italic"));
	pasteItalicAction->setIcon(QIcon(":/icons/text_italic.png"));
	pasteItalicAction->setShortcut(QKeySequence("Alt+I"));
	
	pasteUnderlineAction = new QAction(this);
	pasteUnderlineAction->setText(tr("Paste underline"));
	pasteUnderlineAction->setIcon(QIcon(":/icons/text_underline.png"));
	pasteUnderlineAction->setShortcut(QKeySequence("Alt+U"));
	
	pasteSpecialAction = new QAction(this);
	pasteSpecialAction->setText(tr("Paste office word"));
	pasteSpecialAction->setIcon(QIcon(":/icons/text_officeword.png"));
	pasteSpecialAction->setShortcut(QKeySequence("Alt+O"));
	
	pasteLinkAction = new QAction(this);
	pasteLinkAction->setText(tr("Paste link"));
	pasteLinkAction->setIcon(QIcon(":/icons/text_link.png"));
	pasteLinkAction->setShortcut(QKeySequence("Alt+L"));
	
	pasteSoundAction = new QAction(this);
	pasteSoundAction->setText(tr("Paste sound"));
	pasteSoundAction->setStatusTip(tr("Paste teg \"sound of some words\""));
	pasteSoundAction->setShortcut(QKeySequence("Alt+S"));
	pasteSoundAction->setIcon(QIcon(":/icons/text_sound.png"));
	
	QMenu *editMenu = menuBar()->addMenu("&" + tr("Edit"));
	editMenu->addAction(pasteBlockAction);
	editMenu->addAction(pasteBoldAction);
	editMenu->addAction(pasteItalicAction);
	editMenu->addAction(pasteUnderlineAction);
	editMenu->addAction(pasteSpecialAction);
	editMenu->addAction(pasteLinkAction);
	editMenu->addAction(pasteSoundAction);
	
	editionToolBar = addToolBar(tr("Edition tool bar"));
	editionToolBar->setObjectName("EditionToolBar");
	editionToolBar->addAction(pasteBlockAction);
	editionToolBar->addAction(pasteBoldAction);
	editionToolBar->addAction(pasteItalicAction);
	editionToolBar->addAction(pasteUnderlineAction);
	editionToolBar->addAction(pasteSpecialAction);
	editionToolBar->addAction(pasteLinkAction);
	editionToolBar->addAction(pasteSoundAction);
	editionToolBar->hide();
	
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
	centralWidget->setCurrentDatabase(dictName);
	dictionariesManager->addDictionary(dictName);
	editionToolBar->show();
	QDir::setCurrent(homePath);
	recentOpenedDictionaries << dictName;
	updateRecentDictsMenu();
}

void MainWindow::moveEvent(QMoveEvent *event) {
	emit (moved(event->pos().x(),event->pos().y()));
}

void MainWindow::openDictionary() {
	QString dictionaryPath = QFileDialog::getOpenFileName(this,tr("Open dictionary"),QDir::homePath(),tr("SL dictionaries(*.*-*)"));
	if (dictionaryPath.isEmpty())
		return;
	currentLoadingDictPath = dictionaryPath;
	currentLoadingDictAbout.clear();
	centralWidget->loadDictionary(currentLoadingDictPath,&currentLoadingDictAbout);
}

void MainWindow::loadingCompleted(bool isSuccessful) {
	if (isSuccessful) {
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
}

void MainWindow::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	updateRecentDictsMenu();
	settings.setValue("General/RecentOpenedDictionaries",recentOpenedDictionaries);
	settings.setValue("MainWindow/Position",pos());
	settings.setValue("MainWindow/Size",size());
	settings.setValue("MainWindow/State",saveState());
}

void MainWindow::quit() {
	saveSettings();
	emit (toQuit());
}

void MainWindow::closeEvent(QCloseEvent *) {
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
		recentDictsMenu->addAction(recentOpenedDictionaries[i]);
	
	QString startPageText = 
		"<hr><table border=\"0\" width=\"100%\"><tr><td bgcolor=\"#DFEDFF\"><h2 align=\"center\"><em>" + 
		tr("Start page") + 	
		"</em></h2></td></tr></table><hr>" +
		tr("Hello, thank you for LightLang Editor using! With editor help you can edit existed dictionaries, create new dictionaries and add dictionaries to SL database.") +
		"<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>" + tr("Recent opened dictionaries") + ":</b><ul>";
	for (int i = recentOpenedDictionaries.count() - 1; i >= 0; i--)
		startPageText += "<li><a href=\"" + recentOpenedDictionaries[i] + "\">" + recentOpenedDictionaries[i] + "</a></li>";
	startPageText += "</ul>";
	centralWidget->setStartPageText(startPageText);
}

void MainWindow::updateWindowTitle(const QString& addToTitle) {
		setWindowTitle("LightLang Editor - " + addToTitle);
}
