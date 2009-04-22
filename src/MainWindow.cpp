#include <QtGui/QMenu>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMoveEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QPushButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QMessageBox>
#include <QtGui/QCheckBox>
#include <QtCore/QDir>	
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>
#include "Manual.h"
#include "DictionariesManager.h"
#include "About.h"
#include "CentralWidget.h"
#include "const.h"
#include "Menu.h"
#include "StatusBarLabel.h"
#include "PopupWindow.h"
#include "MainWindow.h"

MainWindow::MainWindow() {
	createDirs();

	editDictInfoPopupWindow = new PopupWindow(this);
	editDictInfoPopupWindow->setHeaderText(tr("Dictionary information"));
	editDictInfoPopupWindow->setReadOnly(false);
	editDictInfoPopupWindow->resize(400,300);
	connect(editDictInfoPopupWindow,SIGNAL(closedAfterChanges()),this,SLOT(saveDictionaryInformation()));
	
	newDictionaryCreatedDialog = new QMessageBox(this);
	newDictionaryCreatedDialog->setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
	newDictionaryCreatedDialog->setText("<b>" + tr("You have created the new dictionary") + "</b><br>" + tr("You can edit it now by clicking on special button, or open created dictionary in dictionaries manager later."));
	openDictionaryNowButton = newDictionaryCreatedDialog->addButton(tr("Open dictionary Now"),QMessageBox::ActionRole);
	openDictionaryLaterButton = newDictionaryCreatedDialog->addButton(tr("Open dictionary Later"),QMessageBox::ActionRole);
	
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
	connect(centralWidget,SIGNAL(openDatabaseWithName(const QString&)),this,SLOT(openDatabaseWithName(const QString&)));
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

	delete openDictionaryNowButton;
	delete openDictionaryLaterButton;
	delete newDictionaryCreatedDialog;

	delete createDictAction;
	delete openDictAction;
	delete recentDictsMenu;
	delete openRecentDictsAction;
	delete saveDictAction;
	delete saveDictAsAction;
	delete addToSlAction;
	delete editDictInformationAction;
	delete openTabAction;
	delete closeTabAction;
	delete quitAction;

	delete dictsManagerAction;
	delete settingsAction;
	delete startPageAction;
	delete dictionarySearchAction;

	delete manualAction;
	delete aboutProgramAction;
	  
	delete previewAction;
	delete findAction;
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
	delete editorMenu;
	
	delete editDictInfoPopupWindow;
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
	connect(openDictAction,SIGNAL(triggered()),this,SLOT(openDictionaryFile()));
	
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
	connect(saveDictAction,SIGNAL(triggered()),this,SLOT(saveDictionaryFile()));
	
	saveDictAsAction = new QAction(this);
	saveDictAsAction->setText(tr("Save dictionary as"));
	saveDictAsAction->setIcon(QIcon(":/icons/saveas.png"));
	saveDictAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
	saveDictAsAction->setEnabled(false);
	connect(saveDictAsAction,SIGNAL(triggered()),this,SLOT(saveDictionaryFileAs()));
	
	addToSlAction = new QAction(this);
	addToSlAction->setText(tr("Add dictionary to SL"));
	addToSlAction->setIcon(QIcon(":/icons/xsl.png"));
	connect(addToSlAction,SIGNAL(triggered()),this,SLOT(addDictionaryToSl()));
	
	editDictInformationAction = new QAction(this);
	editDictInformationAction->setText(tr("Edit information about dictionary"));
	editDictInformationAction->setIcon(QIcon(":/icons/edit_dict.png"));
	connect(editDictInformationAction,SIGNAL(triggered()),editDictInfoPopupWindow,SLOT(showPopup()));
	
	connect(centralWidget,SIGNAL(startPageShown(bool)),saveDictAction,SLOT(setDisabled(bool)));
	connect(centralWidget,SIGNAL(startPageShown(bool)),saveDictAsAction,SLOT(setDisabled(bool)));
	
	openTabAction = new QAction(this);
	openTabAction->setText(tr("New tab"));
	openTabAction->setIcon(QIcon(":/icons/tab-new.png"));
	openTabAction->setShortcut(QKeySequence("Ctrl+T"));
	openTabAction->setEnabled(false);
	connect(openTabAction,SIGNAL(triggered()),centralWidget,SLOT(openNewTab()));
	
	closeTabAction = new QAction(this);
	closeTabAction->setText(tr("Close tab"));
	closeTabAction->setIcon(QIcon(":/icons/tab-close.png"));
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
	dictionaryMenu->addAction(addToSlAction);
	dictionaryMenu->addAction(editDictInformationAction);
	dictionaryMenu->addSeparator();
	dictionaryMenu->addAction(openTabAction);
	dictionaryMenu->addAction(closeTabAction);
	dictionaryMenu->addSeparator();
	dictionaryMenu->addAction(quitAction);

	dictsManagerAction = new QAction(this);
	dictsManagerAction->setText(tr("Dictionaries manager"));
	dictsManagerAction->setIcon(QIcon(":/icons/dicts_manager.png"));
	connect(dictsManagerAction,SIGNAL(triggered()),dictionariesManager,SLOT(exec()));
	
	settingsAction = new QAction(this);
	settingsAction->setText(tr("Preferences"));
	settingsAction->setIcon(QIcon(":/icons/settings.png"));
	connect(settingsAction,SIGNAL(triggered()),centralWidget,SLOT(showSettings()));
	
	dictionarySearchAction = new QAction(this);
	dictionarySearchAction->setText(tr("Dictionary searching"));
	dictionarySearchAction->setIcon(QIcon(":/icons/search.png"));
	dictionarySearchAction->setShortcut(QKeySequence("Ctrl+F"));
	connect(dictionarySearchAction,SIGNAL(triggered()),centralWidget,SLOT(setFocusOnSearchPanel()));
	
	startPageAction = new QAction(this);
	startPageAction->setText(tr("Show start page"));
	startPageAction->setIcon(QIcon(":/icons/start_page.png"));
	connect(startPageAction,SIGNAL(triggered()),centralWidget,SLOT(showStartPage()));
	
	QMenu *toolsMenu = menuBar()->addMenu("&" + tr("Tools"));
	toolsMenu->addAction(dictsManagerAction);
	toolsMenu->addAction(settingsAction);
	toolsMenu->addAction(dictionarySearchAction);
	toolsMenu->addAction(startPageAction);

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
	connect(undoAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(undoActionInCurrentTab()));
	
	redoAction = new QAction(this);
	redoAction->setText(tr("Redo action"));
	redoAction->setIcon(QIcon(":/icons/redo.png"));
	redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));
	connect(redoAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(redoActionInCurrentTab()));
	
	cutAction = new QAction(this);
	cutAction->setText(tr("Cut"));
	cutAction->setIcon(QIcon(":/icons/cut.png"));
	cutAction->setShortcut(QKeySequence("Ctrl+X"));
	connect(cutAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(cutInCurrentTab()));
	
	copyAction = new QAction(this);
	copyAction->setText(tr("Copy"));
	copyAction->setIcon(QIcon(":/icons/copy.png"));
	copyAction->setShortcut(QKeySequence("Ctrl+C"));
	connect(copyAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(copyInCurrentTab()));
	
	pasteAction = new QAction(this);
	pasteAction->setText(tr("Paste"));
	pasteAction->setIcon(QIcon(":/icons/paste.png"));
	pasteAction->setShortcut(QKeySequence("Ctrl+V"));
	connect(pasteAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(pasteInCurrentTab()));
	
	findAction = new QAction(this);
	findAction->setText(tr("Find in translation..."));
	findAction->setIcon(QIcon(":/icons/find_in_text.png"));
	findAction->setShortcut(QKeySequence("/"));
	connect(findAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(showFindWidgetInCurrentTab()));

	previewAction = new QAction(this);
	previewAction->setText(tr("Preview translation"));
	previewAction->setIcon(QIcon(":/icons/preview.png"));
	previewAction->setShortcut(QKeySequence("Ctrl+P"));
	connect(previewAction,SIGNAL(triggered()),centralWidget->getTabsWidget(),SLOT(previewCurrentTranslation()));
	
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
	editMenu->addAction(previewAction);
	
	editionToolBar = new QToolBar(tr("Edition tool bar"));
	editionToolBar->setIconSize(QSize(16,16));
	addToolBar(Qt::LeftToolBarArea,editionToolBar);
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
	editionToolBar->addAction(previewAction);
	editionToolBar->hide();
	
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
	homePath = QDir::toNativeSeparators(QDir::homePath() + "/.LilEditorData/");
	QDir dir;
	if (!dir.exists(homePath))
		dir.mkpath(homePath);
	
#ifdef Q_OS_UNIX
	QFile scriptFile(QDir::homePath() + "/.LilEditorData/addtoslscript");
	if (!scriptFile.exists()) {
		scriptFile.open(QIODevice::WriteOnly);
		QTextStream stream(&scriptFile);
		stream << "#!/bin/bash\nsl --print-index dict > dict1\ncat dict >> dict1\n"
				"sl --print-index dict1 > dict2\ncat dict >> dict2\n"
				"rm -f dict dict1\nPREFIX=\"pkg-config lightlang --variable=prefix\"\n"
				"mv dict2 $PREFIX/share/sl/dicts";
		scriptFile.setPermissions(QFile::ExeOther | QFile::ExeUser | QFile::ExeOwner | QFile::ReadOwner 
								| QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther | QFile::ExeGroup);
	}
#endif	

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

	newDictionaryCreatedDialog->exec();
	if (newDictionaryCreatedDialog->clickedButton() == openDictionaryNowButton)
		openDatabaseWithName(dictName);
}

void MainWindow::moveEvent(QMoveEvent *event) {
	emit (moved(event->pos().x(),event->pos().y()));
}

void MainWindow::openDictionaryFile() {
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
	currentLoadingDictInformation.clear();
	centralWidget->loadDictionary(currentLoadingDictPath);
}

void MainWindow::loadingCompleted(bool isSuccessful) {
	if (isSuccessful) {
		currentLoadingDictInformation = centralWidget->getLoadedDictionaryInformation();
		dictionariesManager->addDictionary(QFileInfo(currentLoadingDictPath).fileName(),currentLoadingDictPath,currentLoadingDictInformation);
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
	if (!settings.value("MainWindow/State").toByteArray().isEmpty())
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

void MainWindow::openDatabaseWithName(const QString& databaseName,bool addToRecentOpenedDicts) {
	if (addToRecentOpenedDicts) {
		recentOpenedDictionaries << databaseName;
		updateRecentDictsMenu();
	}
	editDictInfoPopupWindow->setText(dictionariesManager->getDictionaryInformationWithName(databaseName));
	centralWidget->setCurrentDatabase(databaseName);
	centralWidget->showTabsWidget();
	
	openedDictionaryName = databaseName;
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
	dictionarySearchAction->setEnabled(!isDisabled);
	startPageAction->setEnabled(!isDisabled);
	findAction->setEnabled(!isDisabled);
	previewAction->setEnabled(!isDisabled);
	addToSlAction->setEnabled(!isDisabled);
	editDictInformationAction->setEnabled(!isDisabled);
}

void MainWindow::openDictionaryOfAction(QAction *chosenAction) {
	// Open database to edit without addition to recent opened dictionaries menu
	openDatabaseWithName(chosenAction->text(),false);
}

void MainWindow::saveDictionaryFile() {
	saveDictionaryFileAs(dictionariesManager->getPathForDictionaryWithName(openedDictionaryName));
}

void MainWindow::saveDictionaryFileAs(const QString& path) {
	QString filePath;
	if (path.isEmpty())
		filePath = QFileDialog::getSaveFileName(this,tr("Save dictionary as..."),QDir::homePath(),tr("SL dictionaries(*.*-*)"));
	else
		filePath = path;
	if (filePath.isEmpty())
		return;
	if (centralWidget->saveDictionaryAs(filePath,dictionariesManager->getDictionaryInformationWithName(openedDictionaryName)) == 0)
		showStatusMessage(tr("Dictionary was saved successfully"));
	else
		showStatusMessage(tr("Cannot save dictionary(check your rights in directory)"));
}

void MainWindow::showStatusMessage(const QString& message) {
        statusbar->show();
        statusBarLabel->setMessage(message);
        hideStatusBarTimer->start(10000);
}

void MainWindow::addDictionaryToSl() {
#ifdef Q_OS_WIN32
	QMessageBox::warning(this,tr("Notification"),tr("Windows version doesn't support this function"));
#endif

#ifdef Q_OS_UNIX
	QDir currentDir = QDir::current();
	currentDir.cdUp();
	saveDictionaryFileAs(currentDir.path() + "/dict");
	QProcess::startDetached(currentDir.path() + "/addtoslscript");
#endif
}

void MainWindow::saveDictionaryInformation() {
	dictionariesManager->setDictionaryInformation(openedDictionaryName,editDictInfoPopupWindow->getText());
	showStatusMessage(tr("Dictionary information was saved"));
}
