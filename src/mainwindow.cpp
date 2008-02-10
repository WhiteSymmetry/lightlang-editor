//LightLang Editor - editor for SL dictionaries
//Copyright (C) 2007-2016 Tikhonov Sergey
//
//This file is part of LightLang Editor
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#include <QtGui>
#include <QtSql>
#include <iostream>
#include "mainwindow.h"
#include "centralwidget.h"
#include "aboutdialog.h"
#include "ifa.h"
#include "manualdialog.h"
#include "settingsdialog.h"
#include "managerdialog.h"
#include "getdictnamedialog.h"
#include "global.h"
#include "const.h"

MainWindow::MainWindow(QString& dictToOpen)
{
	translator.load(TransDir+"lileditor_" + lang + ".qm");
	qApp->installTranslator(&translator);
	
	checkBlockFile();
	createBlockFile();
	
	init();
	if (!dictToOpen.isEmpty())
	{
		clearRecentFile();
		openDict(dictToOpen);
	}
		
	readSettings();
}

void MainWindow::init()
{
	wasCanceled = false;
	setWindowIcon(QIcon(MAIN_ICON));
	setWindowTitle(PROGRAM_NAME_WITH_VERSION);
	
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.beginGroup("General");
	splashScreen = 0;
	
	progressDialog = new QProgressDialog(tr("Please wait for the data to load into SQL database"), tr("Cancel"),0,1, this);
	progressDialog->setWindowIcon(QIcon(MAIN_ICON));
	progressDialog->setAutoClose(true);
	progressDialog->setWindowModality(Qt::WindowModal);
	connect(progressDialog,SIGNAL(canceled()),this,SLOT(cancelLoading()));
	
	showSplashScreen = settings.value("ShowSplashScreen",true).toBool();
	if (showSplashScreen)
	{
		splashScreen = new QSplashScreen;
		splashScreen->setPixmap(QPixmap(PICTURES_PATH + "splash.png"));
		splashScreen->show();
	}		
	settings.endGroup();
	
	createAndSetDirs();
	
	bookmarksStream = new QTextStream(&bookmarksFile);
	historyStream = new QTextStream(&historyFile);
	aboutDictStream = new QTextStream(&aboutDictFile);
	
	statBar = statusBar();
	progressDialog = new QProgressDialog(tr("Please wait for the data to load into SQL database"),tr("Cancel"),0,1,this);
	showSplashMessage(tr("The system of help is loading..."));
	manualDialog = new ManualDialog;
	aboutDialog = new AboutDialog;
	getDictName = new GetDictName(this);
	
	showSplashMessage(tr("Create central widget..."));
	centralWidget = new CentralWidget;
	setCentralWidget(centralWidget);
	
	showSplashMessage(tr("Create settings dialog..."));
	settingsDialog = new SettingsDialog(this);
	connect(settingsDialog,SIGNAL(settingsChanged()),this,SLOT(updateProgram()));
	
	showSplashMessage(tr("Create manager of dictionaries..."));
	managerDialog = new ManagerDialog(this);
	connect(managerDialog,SIGNAL(buttonClicked(int,QString)),this,SLOT(actionOnManagerSignal(int,QString)));
	
	
	showSplashMessage(tr("Create other widgets..."));			
	settingsAction = new QAction(this);
	settingsAction->setText(tr("Settings"));
	settingsAction->setStatusTip(tr("Change behaviour of this program"));
	settingsAction->setIcon(QIcon(SETTINGS_ICONS_PATH + "settings.png"));
	settingsAction->setShortcut(QKeySequence("F10"));
	connect(settingsAction,SIGNAL(triggered()),settingsDialog,SLOT(show()));	
		
	managerAction = new QAction(this);
	managerAction->setText(tr("Manager of dictionaries"));
	managerAction->setStatusTip(tr("Manage by loaded dictionaries"));
	managerAction->setIcon(QIcon(ICONS_PATH + "manager.png"));
	managerAction->setShortcut(QKeySequence("F9"));
	connect(managerAction,SIGNAL(triggered()),this,SLOT(showManagerDialog()));
		
	newDictAction = new QAction(this);
	newDictAction->setText(tr("&New"));
	newDictAction->setShortcut(QKeySequence("Ctrl+N"));
	newDictAction->setIcon(QIcon(ICONS_PATH + "new_dict.png"));
	newDictAction->setStatusTip(tr("Create new dictionary"));
	connect(newDictAction,SIGNAL(triggered()),this,SLOT(newDict()));
	
	exitAction = new QAction(this);
	exitAction->setText(tr("&Exit"));		
	exitAction->setShortcut(QKeySequence("Ctrl+Q"));
	exitAction->setIcon(QIcon(ICONS_PATH + "exit.png"));
	exitAction->setStatusTip(tr("Exit from program"));
	connect(exitAction,SIGNAL(triggered()),this,SLOT(exitFromProgram()));
	
	saveDictAction = new QAction(this);
	saveDictAction->setText(tr("Save"));
	saveDictAction->setIcon(QIcon(ICONS_PATH + "save_dict.png"));
	connect(saveDictAction,SIGNAL(triggered()),this,SLOT(saveDict()));
	saveDictAction->setStatusTip(tr("Save the dictionary"));
	saveDictAction->setEnabled(false);
	
	openDictAction = new QAction(this);
	openDictAction->setText(tr("Open"));
	openDictAction->setShortcut(QKeySequence("Ctrl+O"));
	openDictAction->setIcon(QIcon(ICONS_PATH + "open_dict.png"));
	connect(openDictAction,SIGNAL(triggered()),this,SLOT(openDict()));
	openDictAction->setStatusTip(tr("Open some dictionary"));
	    
	manAction = new QAction(this);
	manAction->setText(tr("Manual"));
	manAction->setIcon(QIcon(ICONS_PATH + "manual.png"));
	manAction->setStatusTip(tr("Show manual of LightLang Editor. If you don't understand something in this program, you should watch it"));
	manAction->setShortcut(QKeySequence("F1"));
	connect(manAction,SIGNAL(triggered()),manualDialog,SLOT(show()));
	 	
	aboutAction = new QAction(this);
	aboutAction->setText(tr("About Editor"));
	aboutAction->setIcon(QIcon(MAIN_ICON));
	connect(aboutAction,SIGNAL(triggered()),aboutDialog,SLOT(show()));
	aboutAction->setStatusTip(tr("Show information about program"));
	
	aboutQtAction = new QAction(this);
	aboutQtAction->setText(tr("About Qt4"));
	aboutQtAction->setIcon(QIcon(ICONS_PATH + "about.png"));
	connect(aboutQtAction,SIGNAL(triggered()),this,SLOT(showAboutQt()));
	aboutQtAction->setStatusTip(tr("Show information about Qt4 (LightLang Editor is written on C++ with Qt4)"));
	
	dictMenu = new QMenu(tr("&Dictionary"));
	dictMenu->addAction(newDictAction);
	dictMenu->addAction(openDictAction);
	dictMenu->addSeparator();
	dictMenu->addAction(saveDictAction);
	dictMenu->addSeparator();
	dictMenu->addAction(exitAction);
	                       		
	helpMenu = new QMenu(tr("&Help"));
	helpMenu->addAction(manAction);
	helpMenu->addSeparator();
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
	
	recordMenu = new QMenu(tr("&Record"));
	recordMenu->addAction(centralWidget->getAction(CentralWidget::SearchAction));
	recordMenu->addSeparator();
	recordMenu->addAction(centralWidget->getAction(CentralWidget::AddAction));
	recordMenu->addAction(centralWidget->getAction(CentralWidget::RemoveAction));
	recordMenu->addAction(centralWidget->getAction(CentralWidget::EditAction));
	recordMenu->addSeparator();
	recordMenu->addAction(centralWidget->getAction(CentralWidget::AboutDictAction));
	
	toolsMenu = new QMenu(tr("&Tools"));
	toolsMenu->addAction(settingsAction);
	toolsMenu->addAction(managerAction);
	
	mainBar = menuBar();	
	mainBar->addMenu(dictMenu);	
	mainBar->addMenu(toolsMenu);
	mainBar->addMenu(recordMenu);
	mainBar->addMenu(helpMenu);
	
	showSplashMessage(tr("Integratable friend application is loading..."));
	ifa = new Ifa;
	ifaMenu = new QMenu;
	ifaMenu->setTitle(tr("Applications"));
	ifaMenu->setIcon(QIcon(SETTINGS_ICONS_PATH + "ifa.png"));
	connect(ifaMenu,SIGNAL(triggered(QAction*)),this,SLOT(startIfApplication(QAction*)));
	bool areThereIfas = false;
	
	QDir defaultIfaDir(PROGRAM_PATH + "ifa","*.xml");
	QDir homeIfaDir(IFA_PATH,"*.xml");
	
	for ( unsigned int i = 0; i < defaultIfaDir.count(); i++ )
	{
		if (QFile(PROGRAM_PATH + "ifa/" + defaultIfaDir[i]).exists())
		{
			areThereIfas = true;
			ifa->addAction(PROGRAM_PATH + "ifa/" + defaultIfaDir[i],ifaMenu);
		}
	}
	for ( unsigned int i = 0; i < homeIfaDir.count(); i++ )
	{
		if (QFile(IFA_PATH + homeIfaDir[i]).exists())
		{
			areThereIfas = true;
			ifa->addAction(IFA_PATH + homeIfaDir[i],ifaMenu);
		}
	}
	if (areThereIfas)
		toolsMenu->addMenu(ifaMenu);
		
	if (showSplashScreen)
	{
		splashScreen->finish(this);
 		delete splashScreen;
	}
}

void MainWindow::showSplashMessage(QString message)
{
	if (showSplashScreen)
	{
		Qt::Alignment topLeft = Qt::AlignLeft | Qt::AlignTop;
		if (splashScreen)
			splashScreen->showMessage(message,topLeft);
	}
}


void MainWindow::createAndSetDirs()
{
	QDir dir;
	if ( !dir.exists(HOME_PATH))
		dir.mkdir(HOME_PATH);
	
	if ( !dir.exists(BOOKMARKS_PATH) )
		dir.mkdir(BOOKMARKS_PATH);
	
	if ( !dir.exists(DATABASES_PATH) )
		dir.mkdir(DATABASES_PATH);
		
	if ( !dir.exists(HISTORIES_PATH) )
		dir.mkdir(HISTORIES_PATH);
		
	if ( !dir.exists(IFA_PATH) )
		dir.mkdir(IFA_PATH);
		
	if ( !dir.exists(ABOUTS_PATH) )
		dir.mkdir(ABOUTS_PATH);
		
	dir.setCurrent(DATABASES_PATH);
	
}

void MainWindow::removeBlockFile()
{
	QFile::remove("/tmp/lileditor.block");
}

void MainWindow::checkBlockFile()
{
	if (QFile::exists("/tmp/lileditor.block"))
	{
		QMessageBox *messageBox = new QMessageBox(this);
		messageBox->setWindowTitle(tr(PROGRAM_NAME));
		messageBox->setWindowIcon(QIcon(MAIN_ICON));
		messageBox->setText(tr(  "The program is already running - close old process and start new.\n"
					"If the program was fallen, press \"Start new\""));
		messageBox->addButton(tr("Start new"),QMessageBox::ActionRole);
		QPushButton *ok = messageBox->addButton(tr("Ok"),QMessageBox::ActionRole);
		messageBox->exec();
		if (messageBox->clickedButton() == ok)
		{
 			qApp->quit();
 			exit(0);
 		}
	}
}

void MainWindow::createBlockFile()
{
	QFile file("/tmp/lileditor.block");
	file.open(QIODevice::WriteOnly);
}

void MainWindow::clearCache(QString name)
{
	if (name == databaseName)
	{
		QMessageBox::warning(this,tr("Warning"),tr("You want to remove opened dictionary.\nOpen other dictionary and remove it"));
		return;
	}
	int r = QMessageBox::warning(this,tr("Warning"),
				tr("Are you sure, that you want to remove all files\nof the dictionary") + " \"" + name + "\"?",
				QMessageBox::No | QMessageBox::Default,QMessageBox::Yes);
	if (r == QMessageBox::No)
		return;
	
	if (QFile::remove(DATABASES_PATH + name))
		std::cout << qPrintable(tr("Database file was removed")) << '\n';
	else
		std::cout << qPrintable(tr("Database file wasn't removed - don't exist (ignored)")) << '\n';
		
	
	if (QFile::remove(BOOKMARKS_PATH + name + "bookmarks"))
		std::cout << qPrintable(tr("Bookmarks file was removed")) << '\n';
	else
		std::cout << qPrintable(tr("Bookmarks file wasn't removed - don't exist  (ignored)")) << '\n';
	
	if (QFile::remove(ABOUTS_PATH + name + "about"))
		std::cout << qPrintable(tr("About dictionary file was removed")) << '\n';
	else
		std::cout << qPrintable(tr("About dictionary file wasn't removed - don't exist  (ignored)")) << '\n';
	
	
	if (QFile::remove(HISTORIES_PATH + name + "history"))
		std::cout << qPrintable(tr("History file was removed")) << '\n';
	else
		std::cout << qPrintable(tr("History file wasn't removed - don't exist  (ignored)")) << '\n';
	
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.beginGroup("General");
	if (settings.value("RecentFile").toString() == name)
	{
		settings.setValue("RecentFile","");
		std::cout << qPrintable(tr("Recent file was changed")) << '\n';
	}
	settings.endGroup();
	centralWidget->showMessage(CentralWidget::Good,tr("The dicionary was removed"));
	managerDialog->updateDicts();
}

void MainWindow::closeEvent(QCloseEvent* /*event*/ )
{
	exitFromProgram();
}

void MainWindow::setRecentFile(QString name)
{
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.beginGroup("General");
	settings.setValue("RecentFile",name);
	settings.endGroup();
}

void MainWindow::clearRecentFile()
{
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.beginGroup("General");
	settings.setValue("RecentFile","");
	settings.endGroup();
}

void MainWindow::readSettings()
{
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	
	settings.beginGroup("General");
	QRect rect = settings.value("geometry",QRect(100,40,900,600)).toRect();
	move(rect.topLeft());
	resize(rect.size());
	
	settings.endGroup();		
			
	settings.beginGroup("Boolean");
	boolSets.append(settings.value("UpdateTransDuringEntering",false).toBool());
     boolSets.append(settings.value("UpdatePreviewDuringEntering",true).toBool());
	boolSets.append(settings.value("OpenRecentFile",true).toBool());
	boolSets.append(settings.value("OpenWordsInNewTabs",true).toBool());
	boolSets.append(settings.value("ShowSplashScreen",true).toBool());
	boolSets.append(settings.value("ShowAutoSearch",false).toBool());
	boolSets.append(settings.value("ShowBookmarks",false).toBool());
	boolSets.append(settings.value("ShowHistory",false).toBool());
	boolSets.append(settings.value("ShowPreviewApart",false).toBool());
	boolSets.append(settings.value("HighLightTrans",true).toBool());
	boolSets.append(settings.value("SearchWordsByBegining",true).toBool());
	boolSets.append(settings.value("MoveBySingleClick",true).toBool() );
	boolSets.append(settings.value("ShowMarksInAutoSearch",true).toBool() );
	boolSets.append(settings.value("ShowMarksInTabs",true).toBool() );
	settings.endGroup();
	
	settings.beginGroup("Integer");
	intSets.append(settings.value("MinimumRecords",100).toInt());
	settings.endGroup();

 	settingsDialog->setSettings(boolSets,intSets);
 	centralWidget->setSettings(boolSets,intSets);
 	centralWidget->showWelcomePage();
	
	settings.beginGroup("General");
	if (boolSets[OpenRecentFile])
	{
		QString recentFileName = settings.value("RecentFile","").toString();
		if (!recentFileName.isEmpty())
		{
			QFile recentFile(recentFileName);
			if (recentFile.exists())
				openDict(recentFileName);
			else
			{
				std::cout << "Recent file was not found, that's why editor clear leaved rubbish:\n";
				clearCache(recentFileName);
				askChoice(false);
			}
			
		}
		else
			askChoice(false);
	}
	else
		askChoice(false);
	settings.endGroup();
}

void MainWindow::writeSettings()
{
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	
	boolSets = settingsDialog->getBoolSettings();
	intSets = settingsDialog->getIntSettings();
	settings.beginGroup("Boolean");
	settings.setValue("UpdateTransDuringEntering",boolSets[UpdateTransDuringEntering]);
	settings.setValue("UpdatePreviewDuringEntering",boolSets[UpdatePreviewDuringEntering]);
	settings.setValue("OpenRecentFile",boolSets[OpenRecentFile]);
	settings.setValue("OpenWordsInNewTabs",boolSets[OpenWordsInNewTabs]);
	settings.setValue("ShowSplashScreen",boolSets[ShowSplashScreen]);
	settings.setValue("ShowAutoSearch",boolSets[ShowAutoSearch]);
	settings.setValue("ShowBookmarks",boolSets[ShowBookmarks]);
	settings.setValue("ShowHistory",boolSets[ShowHistory]);
	settings.setValue("ShowPreviewApart",boolSets[ShowPreviewApart]);
	settings.setValue("HighLightTrans",boolSets[HighLightTrans]);
	settings.setValue("SearchWordsByBegining",boolSets[SearchWordsByBegining]);
	settings.setValue("MoveBySingleClick",boolSets[MoveBySingleClick]);
	settings.setValue("ShowMarksInAutoSearch",boolSets[ShowMarksInAutoSearch]);
	settings.setValue("ShowMarksInTabs",boolSets[ShowMarksInTabs]);
	settings.endGroup();
	settings.beginGroup("Integer");
	settings.setValue("MinimumRecords",intSets[MinimumRecords]);
	settings.endGroup();
		
	settings.beginGroup("General");
	settings.setValue("geometry",geometry()); 	
	settings.setValue("ExtendedSearch",centralWidget->getExtendedSearchStatus());
	settings.setValue("TextEditPartState",centralWidget->getPreviewGeometry());
	settings.setValue("CentralPartState",centralWidget->getLocalMainWidgetState());
	settings.setValue("AutoSearchState",centralWidget->getAutoSearchState());
	settings.setValue("HelpState",manualDialog->getState());
	
	if (!databaseName.isEmpty() && !wasCanceled)
	{			
		settings.setValue("RecentFile",databaseName);
		
		bookmarksFile.remove();
		if (!centralWidget->getBookmarks().isEmpty())
		{
			bookmarksFile.open(QIODevice::WriteOnly);
			QTextStream stream1(&bookmarksFile);
			stream1 << centralWidget->getBookmarks();
		}
				
		historyFile.remove();
		if (!centralWidget->getHistory().isEmpty())
		{
			historyFile.open(QIODevice::WriteOnly);
			QTextStream stream2(&historyFile);
			stream2 << centralWidget->getHistory();
		}
			
		aboutDictFile.remove();
		if (!centralWidget->getAboutDict().isEmpty())
		{
			aboutDictFile.open(QIODevice::WriteOnly);
			QTextStream stream3(&aboutDictFile);
			stream3 << centralWidget->getAboutDict();
		}
	}	
	settings.endGroup();
}

int MainWindow::askChoiceDialog(bool isShowCancel)
{
	QMessageBox messageBox(this);
	messageBox.setText(tr("Choose some action:"));
	messageBox.setIconPixmap(QPixmap(MAIN_ICON).scaled(50,50));
	QPushButton *managerButton = messageBox.addButton(tr("Show manager"), QMessageBox::ActionRole);
	managerButton->setIcon(QIcon(ICONS_PATH + "manager.png"));
	QPushButton *quitButton = 0;
	QPushButton *cancelButton = 0;
 	if ( isShowCancel )
	 	cancelButton = messageBox.addButton(tr("Cancel"),QMessageBox::ActionRole);
	else
	{
		quitButton = messageBox.addButton(tr("Quit"),QMessageBox::ActionRole);
	 	quitButton->setIcon(QIcon(ICONS_PATH + "exit.png"));
 	}
 	QPushButton *openNewDictButton = messageBox.addButton(tr("Open"),QMessageBox::ActionRole);
 	openNewDictButton->setIcon(QIcon(ICONS_PATH + "open_dict.png"));
 	messageBox.setWindowTitle(tr("Choose action"));
	messageBox.exec();
	
	if ( messageBox.clickedButton() == openNewDictButton )
		return OpenNewDictionary;
	else
	if ( messageBox.clickedButton() == managerButton )
		return ShowManager;
	else
	if ( messageBox.clickedButton() == quitButton )
		return Quit;
	else
	if ( messageBox.clickedButton() == cancelButton )
		return WasCanceled;
	return Quit;
}

void MainWindow::askChoice(bool isCancelAvaible)
{
	switch (askChoiceDialog(isCancelAvaible))
	{
		case OpenNewDictionary:
			openDict();
			break;
		case ShowManager:
			showManagerDialog(isCancelAvaible ?  CloseManagerAvaible : DontCloseManagerAvaible);
			break;
		case Quit:
			exitFromProgram();
			break;
	}
}

void MainWindow::openDict(QString name)
{
	name = name.trimmed();
	if (name.isEmpty())
	{
		QString openFileName = QFileDialog::getOpenFileName(this,tr("Open a dictionary"),QDir::homePath(),"Sl dictionaries (*.*-*)");
		if (openFileName.isEmpty())
		{
			if (databaseName.isEmpty())
				askChoice(false);
			return;
		}
		if (QFileInfo(openFileName).fileName() == databaseName)
			return;
		centralWidget->clearAll();
		QString fileName = QFileInfo(openFileName).fileName();
		QFile file(DATABASES_PATH + fileName);
		if (file.exists())
			createLocalConnection(fileName);
		else
		{
			createLocalConnection(fileName);
			fillDatabase(openFileName);
		}
		setTitle();
	}
	else
	if (name[0] == '/')
	{
		QString fileName = QFileInfo(name).fileName();
		if (fileName == databaseName)
			return;
		centralWidget->clearAll();
		QFile file(DATABASES_PATH + fileName);
		if (file.exists())
			createLocalConnection(fileName);
		else
		{
			fillDatabase(name);
			createLocalConnection(fileName);
		}
		setTitle();
	}
	else
	{
		if (name == databaseName)
			return;
		centralWidget->clearAll();
		createLocalConnection(name);
		setTitle();
	}
}

void MainWindow::createLocalConnection(QString localName)
{
	if (db.isOpen())
		QSqlDatabase::removeDatabase("sqlite");
	db = QSqlDatabase::addDatabase("QSQLITE","sqlite");
	databaseName = localName;
	db.setHostName("localhost");
	db.setDatabaseName(databaseName);
	db.setUserName("LightLang");
	db.setPassword("LIGHTLANG"); 
	if (!db.open())
	{
		centralWidget->showMessage(CentralWidget::Bad,tr("Database error:") + db.lastError().text());
		askChoice(false);
		return;
	}
	query = QSqlQuery(db);
	
	if (bookmarksFile.isOpen())
		bookmarksFile.close();
	bookmarksFile.setFileName(BOOKMARKS_PATH + databaseName + "bookmarks");
	bookmarksFile.open(QIODevice::ReadWrite);
	if (!bookmarksStream->atEnd())
		centralWidget->setBookmarks(bookmarksStream->readAll());
	
	if (historyFile.isOpen())
		historyFile.close();	
	historyFile.setFileName(HISTORIES_PATH + databaseName + "history");
	historyFile.open(QIODevice::ReadWrite);
	if (!historyStream->atEnd())
		centralWidget->setHistory(historyStream->readAll());
	
	if (aboutDictFile.isOpen())
		aboutDictFile.close();		
	aboutDictFile.setFileName(ABOUTS_PATH + databaseName + "about");
	aboutDictFile.open(QIODevice::ReadWrite);
	if (!aboutDictStream->atEnd())
		centralWidget->setAboutDict(aboutDictStream->readAll());
	
	query.exec(QString("CREATE TABLE IF NOT EXISTS main(`word` TEXT NOT NULL,`trans` TEXT NOT NULL,`stat` INTEGER(1) NOT NULL)"));
			
	if ( !query.isActive() )
		centralWidget->showMessage(CentralWidget::Bad,tr("Database error:") + query.lastError().text());
	else
		centralWidget->setDatabase(db,query);	
	
	saveDictAction->setEnabled(true);
}

void MainWindow::fillDatabase(QString path_to_dict)
{
	QFile file(path_to_dict);
	if (!file.open(QIODevice::ReadOnly))
	{
			centralWidget->showMessage(CentralWidget::Bad,tr("File Error: It's immposable to open file."));
			if (databaseName.isEmpty())
				askChoice(false);
			return;
	}
	
	QTextStream dataStream(&file);
	
	QString fileName = QFileInfo(path_to_dict).fileName();
	QString tempString;
	QString aboutString;
	QStringList list;
		
	int rows = 0;
	for (rows = 0; !dataStream.atEnd(); rows++)
		dataStream.readLine(); 
		
	progressBarValue = 0;
	progressDialog->setWindowTitle(tr("Loading") + " " + fileName);
	progressDialog->setRange(0,rows);
	progressDialog->show();
	dataStream.seek(0);
	QCoreApplication::processEvents();
	wasCanceled = false;
	while (!dataStream.atEnd())
	{
		tempString = dataStream.readLine().trimmed();
		progressBarValue++;
		progressDialog->setValue(progressBarValue);
		if (tempString.isEmpty())
			continue;
		if (tempString[0] == '#' )
		{
			aboutString += tempString.remove(0,1) + '\n';	
			continue;
		}
		list = tempString.split("  ");
		if (list.count() == 2)
			query.exec(QString("INSERT INTO main(word,trans,stat) VALUES(\"%1\",\"%2\",\'0\')")
								.arg(list[0].simplified().toLower()).arg(list[1].simplified()));					
		if (wasCanceled)
		{
			QFile::remove(DATABASES_PATH + fileName);
			progressDialog->hide();
			show();	
			if (databaseName.isEmpty())
				askChoice(false);
			return;
		}
	}
	progressDialog->hide();
	if (!aboutString.isEmpty())
	{
		aboutDictFile.setFileName(ABOUTS_PATH + fileName + "about");
		aboutDictFile.open(QIODevice::WriteOnly);
		*aboutDictStream << aboutString;
		centralWidget->setAboutDict(aboutString);
	}       
	managerDialog->updateDicts();
}

void MainWindow::newDict()
{
	getDictName->exec();
	
	if (getDictName->getName().isEmpty())
	{
		if (databaseName.isEmpty())
			askChoice(false);
		return;
	}
	
	QString name = getDictName->getName();
	if (QFile::exists(DATABASES_PATH + name))
	{
		QMessageBox::warning(this,tr("Error"),tr("Dictionary with the same name is\nalready exists."));
		return;
	}
	QFile file(DATABASES_PATH + name);
	file.open(QIODevice::WriteOnly);
	managerDialog->updateDicts();
	openDict(name);
}

void MainWindow::saveDict()
{
	QString saveFileName;
	while (true)
	{
		saveFileName = QFileDialog::getSaveFileName(this,tr("Save"),QDir::homePath(),"Sl dictionaries (*.*-*)");
		if (saveFileName.isEmpty())
			return;
		if (saveFileName.contains(QRegExp(".*\\..*\\-.*")))
			break;
		else
			QMessageBox::warning(this,tr("Warning"),tr("Format: <name>.<from_language>-<to_language>"));
	}
	
	QFile saveFile(saveFileName);
	if (saveFile.open(QIODevice::WriteOnly))
	{
		QTextStream dataStream(&saveFile);
		if (!centralWidget->getAboutDict().isEmpty())
			dataStream << '#' << centralWidget->getAboutDict() << '\n';
		query.exec("SELECT * FROM main");
		while (query.next())
			dataStream << query.record().value(0).toString().simplified() << "  " << query.record().value(1).toString().simplified() << '\n';
	}
	else
	{
		centralWidget->showMessage(CentralWidget::Bad,tr("File error: it's immpossible to save file (access denied)"));
		return;
	}
	centralWidget->showMessage(CentralWidget::Good,tr("The dictionary was saved"));
}

void MainWindow::addDictToSlDatabase(QString name)
{
	if (!QDir().exists("/usr/share/sl/"))
	{
		centralWidget->showMessage(CentralWidget::Bad,tr("Sl isn't installed on your computer"));
		return;
	}
	
	tempString = name;
	
	QFile oldFile("/usr/share/sl/dicts/" + tempString);
	if (oldFile.exists())
	{
		QMessageBox messageBox(this);
		messageBox.setText(tr("The dictionary with the same name is already exists."));
		messageBox.setWindowTitle(tr("Warning"));
		messageBox.addButton(tr("Replace"),QMessageBox::ActionRole);
		QPushButton *cancelButton = messageBox.addButton(tr("Cancel"),QMessageBox::ActionRole);
		messageBox.exec();
		if (messageBox.clickedButton() == cancelButton)
			return;
	}
	
	QFile tempFile("/tmp/" + tempString);
	tempFile.open(QIODevice::WriteOnly);
	QTextStream dataStream(&tempFile);
	if (!centralWidget->getAboutDict().isEmpty())
		dataStream << '#' << centralWidget->getAboutDict() << '\n';
	query.exec("SELECT * FROM main");
	while (query.next())
		dataStream << query.record().value(0).toString().simplified() << "  " << query.record().value(1).toString().simplified() << '\n';
	
 	connect(&process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(startAccomOfDict(int,QProcess::ExitStatus)));
	process.start(QString("/bin/sh -c \"/usr/bin/sl --print-index /tmp/%1 > /tmp/tempdict1\"").arg(tempString));
	countOfProcesses = 1;
}

void MainWindow::startAccomOfDict(int /*code*/, QProcess::ExitStatus exitStatus)
{
	if ( exitStatus == QProcess::CrashExit )
	{
		centralWidget->showMessage(CentralWidget::Bad,tr("The dictionary wasn't added, as was error"));
		tempString.clear();
		return;
	}
	switch ( countOfProcesses )
	{
		case 1:
			process.start(QString("/bin/sh -c \"/bin/cat /tmp/%1 >> /tmp/tempdict1\"").arg(tempString)); 
			break;
		case 2:
 			process.start("/bin/sh -c \"/usr/bin/sl --print-index /tmp/tempdict1 > /tmp/tempdict2\"");
			break;
		case 3:
			process.start(QString("/bin/sh -c \"/bin/cat /tmp/%1 >> /tmp/tempdict2\"").arg(tempString));
			break;
		case 4:
			process.start(QString("mv /tmp/tempdict2 /usr/share/sl/dicts/%1").arg(tempString));
			break;	
		case 5:
			process.start(QString("sl --connect %1").arg(tempString));
			break;
	} 
	if ( ++countOfProcesses == 7 )
	{
		centralWidget->showMessage(CentralWidget::Good,tr("The dictionary was added into sl database"));
		process.close();
		tempString.clear();
	}
}

void MainWindow::exitFromProgram()
{
	writeSettings();
	removeBlockFile();
 	qApp->quit();
 	exit(0);
}

void MainWindow::startIfApplication(QAction* chosenAction)
{
	std::cout << "IFA: run \"" << qPrintable(chosenAction->data().toString()) <<  "\"\n";
	QProcess::startDetached(chosenAction->data().toString());
}

void MainWindow::cancelLoading()
{
	int r = QMessageBox::warning(this,tr("Warning"),tr("Are you sure?"),QMessageBox::Yes | QMessageBox::Default,QMessageBox::No | QMessageBox::Escape);
	if ( r == QMessageBox::No )
	{
		progressDialog->setValue(progressBarValue);
		wasCanceled = false;
	}
	else
		wasCanceled = true;
}

void MainWindow::showAboutQt()
{
	QMessageBox::aboutQt(this,tr("About Qt4"));
}

void MainWindow::setTitle()
{
	if (databaseName.isEmpty())
		setWindowTitle(tr(PROGRAM_NAME)+ " " + VERSION);
	setWindowTitle(databaseName + " - " + tr(PROGRAM_NAME) + " " + VERSION);
}

void MainWindow::showManagerDialog(int mode)
{
	managerDialog->setMode(mode);
	managerDialog->exec();
}

void MainWindow::actionOnManagerSignal(int action,QString name)
{
	switch (action)
	{
		case ManagerDialog::AskChoice:
			askChoice(false);
			break;
		case ManagerDialog::Open:
			openDict(name);
			break;
		case ManagerDialog::Remove:
			clearCache(name);
			break;
		case ManagerDialog::Add:
			addDictToSlDatabase(name);
			break;
		case ManagerDialog::CreateNew:
			newDict();
			break;
	}
}

void MainWindow::updateProgram()
{
	boolSets = settingsDialog->getBoolSettings();
	intSets = settingsDialog->getIntSettings();
	centralWidget->setSettings(boolSets,intSets);
}
