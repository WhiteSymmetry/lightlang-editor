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
#include <iostream>
#include "mainwidget.h"
#include "centralwidget.h"
#include "aboutdialog.h"
#include "ifa.h"
#include "guidedialog.h"
#include "settingsdialog.h"
#include "global.h"
#include "const.h"
#include "managerdialog.h"

			
MainWidget::MainWidget(bool isCreateNewDictionary,QString& fileName)
{	
	init();

	if ( isCreateNewDictionary )
	{
		clearRecentFile();
		readSettings(DontOpenRecentFileMode);
		newDictionary();
	}
	else
	if ( !fileName.isEmpty() )
	{
		fullFileName = fileName;
		fileName = QFileInfo(fullFileName).fileName();
		open(DontAskOpenFileNameMode);
	}
	
	if ( !isCreateNewDictionary )
		readSettings();
}

void MainWidget::init()
{
	// Check and create block file start
	checkBlockFile();
	createBlockFile();
	//Check and create block file end
	
	// Set options of MainWindow   
	setWindowIcon(QIcon(ICONS_PATH + "lle.png")); 
	setWindowTitle(NAME_WITH_VERSION);
	
	// For translation on dif. languages
	translator.load(TrDir+"lileditor_" + lang + ".qm");
	qApp->installTranslator(&translator);
	
	QSettings settings(HOME_PATH + "settings.conf",QSettings::NativeFormat,0);
	settings.beginGroup("BoolSettings");
	bool showSplash = settings.value("ShowSplashScreen",true).toBool();
	settings.endGroup();
	
	QSplashScreen *splash = 0;
	Qt::Alignment topLeft = Qt::AlignLeft | Qt::AlignTop;
	if ( showSplash )
	{
		splash = new QSplashScreen;
		splash->setPixmap(QPixmap(PROGRAM_PATH + "pictures/splash.png"));
		splash->show();
	}
	
	// Create and set Directories:
	createAndSetDirectories();
	
	
	wasCanceled = false;
	if ( showSplash )
	{
		splash->showMessage(tr("Loading IFA..."),topLeft);
	}
	// Integretable Friend Applications
	isThereXsl = false;
	ifa = 0;
	QDir ifaDir(HOME_PATH +"ifa","*.xml");
	QDir ifaDir2(PROGRAM_PATH + "ifa","*.xml");
	unsigned int numFiles = 0;
	numFiles = ifaDir.count() + ifaDir2.count();
	ifa = new  IFA [numFiles];
	for ( unsigned int pos = 0; pos < ifaDir2.count(); pos++ )
	{
		QFile file(PROGRAM_PATH + "ifa/" + ifaDir2[pos]);
		if ( file.exists() )
		{
			QXmlInputSource inputSource(&file);
			QXmlSimpleReader reader;
			reader.setContentHandler(&ifa[pos]);
			reader.setErrorHandler(&ifa[pos]);
			reader.parse(inputSource);
		}
	}
	for ( unsigned int pos = 0; pos < ifaDir.count(); pos++ )
	{
		QFile file(HOME_PATH + "ifa/" + ifaDir[pos]);
		if ( file.exists() )
		{
			QXmlInputSource inputSource(&file);
			QXmlSimpleReader reader;
			reader.setContentHandler(&ifa[ifaDir2.count() + pos]);
			reader.setErrorHandler(&ifa[ifaDir2.count() + pos]);
			reader.parse(inputSource);
		}
	}
	//===============================
	
	// init widgets:
	pb = 0;     
	sb = statusBar();
	guideDialog = 0;
	aboutDialog = 0;
	query = 0;
	//==============
	
	
	if ( showSplash )
	{
		splash->showMessage(tr("Creating central widget..."),topLeft);
	}
	// Creation Central part of MainWidget	
	centrWidget = new CentralWidget;		
	centrWidget->setModified(CentralWidget::NotModified);
	connect(centrWidget,SIGNAL(somethingChanged()),this,SLOT(setTitle()));
	connect(centrWidget,SIGNAL(showMessage(const QString)),this,SLOT(showMessageInStatusBar(const QString)));
	setCentralWidget(centrWidget);
	
	if ( showSplash )
	{
		splash->showMessage(tr("Creating help system.."),topLeft);
	}
	guideDialog = new GuideDialog;
	
	if ( showSplash )
	{
		splash->showMessage(tr("Creating settings dialog..."),topLeft);
	}
	// Creation of Settings Widget
	settingsDialog = new SettingsDialog(this);
	connect(settingsDialog,SIGNAL(settingsChanged()),this,SLOT(updateProgram()));
	
	if ( showSplash )
	{
		splash->showMessage(tr("Creating manager of loaded dictionaries..."),topLeft);
	}
	managerDialog = new ManagerDialog(this);
	connect(managerDialog,SIGNAL(buttonClicked(int,QString)),this,SLOT(actionOnManagerSignal(int,QString)));
	
	if ( showSplash )
	{
		splash->showMessage(tr("Creating actions..."),topLeft);
	}
	
	// Settings actions:
			
	settingsAction = new QAction(this);
	settingsAction->setText(tr("Settings"));
	settingsAction->setStatusTip(tr("Change behaviour of this program"));
	settingsAction->setIcon(QIcon(ICONS_PATH + "settings/settings.png"));
	settingsAction->setShortcut(QKeySequence("F10"));
	connect(settingsAction,SIGNAL(triggered()),this,SLOT(showSettings()));	
		
	managerAction = new QAction(this);
	managerAction->setText(tr("Manager of dictionaries"));
	managerAction->setStatusTip(tr("Managing of loaded dictionaries"));
	managerAction->setIcon(QIcon(ICONS_PATH + "manager.png"));
	managerAction->setShortcut(QKeySequence("F9"));
	connect(managerAction,SIGNAL(triggered()),this,SLOT(showManager()));
		
	// File Actions
	
	newAction = new QAction(this);
	newAction->setText(tr("&New"));
	newAction->setShortcut(QKeySequence("Ctrl+N"));
	newAction->setIcon(QIcon(ICONS_PATH + "new.png"));
	connect(newAction,SIGNAL(triggered()),this,SLOT(newDictionary()));
	newAction->setStatusTip(tr("Create new dictionary"));
	
	exitAction = new QAction(this);
	exitAction->setText(tr("&Exit"));		
	exitAction->setShortcut(QKeySequence("Ctrl+Q"));
	exitAction->setIcon(QIcon(ICONS_PATH + "exit.png"));
	connect(exitAction,SIGNAL(triggered()),this,SLOT(exitFromProgram()));
	exitAction->setStatusTip(tr("Exit from program"));
	
	saveAction = new QAction(this);
	saveAction->setText(tr("Save"));
	saveAction->setIcon(QIcon(ICONS_PATH + "save.png"));
	connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));
	saveAction->setStatusTip(tr("Save dictionary"));
	saveAction->setEnabled(false);
	
	openAction = new QAction(this);
	openAction->setText(tr("Open"));
	openAction->setShortcut(QKeySequence("Ctrl+O"));
	openAction->setIcon(QIcon(ICONS_PATH + "open.png"));
	connect(openAction,SIGNAL(triggered()),this,SLOT(open()));
	openAction->setStatusTip(tr("Open some dictionary"));
	    
	// Help Actions:
	
	guideAction = new QAction(this);
	guideAction->setText(tr("Manual of LilEditor"));
	guideAction->setIcon(QIcon(ICONS_PATH + "guide.png"));
	guideAction->setStatusTip(tr("Show guide of LightLang Editor. If you don't understand something in this program, you should watch it"));
	guideAction->setShortcut(QKeySequence("F1"));
	connect(guideAction,SIGNAL(triggered()),this,SLOT(showHelp()));
	 	
	aboutAction = new QAction(this);
	aboutAction->setText(tr("About LilEditor"));
	aboutAction->setIcon(QIcon(ICONS_PATH + "lle.png"));
	connect(aboutAction,SIGNAL(triggered()),this,SLOT(showAbout()));
	aboutAction->setStatusTip(tr("Show information about program"));
	
	aboutQtAction = new QAction(this);
	aboutQtAction->setText(tr("About Qt4"));
	aboutQtAction->setIcon(QIcon(ICONS_PATH + "about.png"));
	connect(aboutQtAction,SIGNAL(triggered()),this,SLOT(showAboutQt()));
	aboutQtAction->setStatusTip(tr("Show information about Qt 4"));
	
	// Create actions end
	
	// Creation file Menu
			
	if ( showSplash )
	{
		splash->showMessage(tr("Creating menus..."),topLeft);
	}
	fileMenu = new QMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addSeparator();
	fileMenu->addAction(saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	                                              
	// Creation help Menu		
	helpMenu = new QMenu(tr("&Help"));
	helpMenu->addAction(guideAction);
	helpMenu->addSeparator();
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
	
	// Create record menu:	
	recordMenu = new QMenu(tr("&Record"));
	recordMenu->addAction(centrWidget->searchAction);
	recordMenu->addSeparator();
	recordMenu->addAction(centrWidget->addAction);
	recordMenu->addAction(centrWidget->removeAction);
	recordMenu->addAction(centrWidget->editAction);
	recordMenu->addSeparator();
	recordMenu->addAction(centrWidget->aboutDictAction);
	
	// Create settings menu:
	settingsMenu = new QMenu(tr("&Tools"));
	settingsMenu->addAction(settingsAction);
	settingsMenu->addAction(managerAction);
	
	// Creation main Bar start		
	mainBar = menuBar();	
	mainBar->addMenu(fileMenu);	
	mainBar->addMenu(settingsMenu);
	// Integratable Friend Applications
	ifaMenu = new QMenu(tr("&Applications"));
	ifaMenu->setIcon(QIcon(ICONS_PATH + "settings/ifa.png"));
	unsigned int pos;
	for ( pos = 0; pos < numFiles; pos++ )
	{
		startAction = new QAction(this);
		if ( ifa[pos].addMenu(startAction) )
			ifaMenu->addAction(startAction);
	}
	if ( pos != 0 )
	{
		connect(ifaMenu,SIGNAL(triggered(QAction*)),this,SLOT(startIfaApplication(QAction*)));
		settingsMenu->addMenu(ifaMenu);
	}
	//==================================
	mainBar->addMenu(recordMenu);
	mainBar->addMenu(helpMenu);
	// Creation of main Bar end
	
	if ( showSplash )
		splash->finish(this);
}

//===========================================//
//================= SLOTS: ==================//
//===========================================//

//--------------Private start----------------//
// File menu's slots start:
void MainWidget::newDictionary()
{
	QString tempFullFileName = HOME_PATH + "databases/temp";
	QString tempName = "temp";
	QFile tempFile(tempFullFileName);
	if ( tempFile.exists() )
	{	
		int r = QMessageBox::warning(this, tr("Warning - %1").arg(PROGRAM_NAME),
					    tr("Old temporary file will be removed. Save it?"),
					    QMessageBox::Cancel | QMessageBox::Escape,
					    QMessageBox::No,
					    QMessageBox::Yes | QMessageBox::Default);
		if ( r == QMessageBox::Yes )
		{
			fullFileName = tempFullFileName;
			fileName = tempName;
			setMainConnection();
			save();
// 			writeSettings();
		}
		else 
		if ( r == QMessageBox::Cancel )
			return;	
		fullFileName = tempFullFileName;
		fileName = tempName;
		deleteDatabase();
	} 
	fullFileName = tempFullFileName;
	fileName = tempName;
	centrWidget->clearAll();
	centrWidget->setModified(CentralWidget::TempWasNotSaved);
	open(DontAskOpenFileNameMode);
	enableActions(true);
}

void MainWidget::open(int mode)
{ 
	QString openFileName;
	if ( mode == AskOpenFileNameMode )
	{
		openFileName = QFileDialog::getOpenFileName(this,tr("Open - %1").arg(PROGRAM_NAME),QDir::homePath(),"SL dictonaries ( *.*-* )");
		if (! fullFileName.isEmpty() && openFileName == fullFileName && centrWidget->getModified() == CentralWidget::NotModified )
			return;	
		if ( openFileName.isEmpty() )
		{
			if ( fullFileName.isEmpty() )
				askChoice(false);
			return;
		}
		// Check file start:
		QStringList expList = openFileName.split(".");
		if (  expList.count() != 0 )
		{
			QRegExp openFileNameSuffixRegExp("+-.+");
			if ( openFileNameSuffixRegExp.exactMatch(expList[expList.count()-1]) )
			{
				QMessageBox::warning(this,tr("%1").arg(PROGRAM_NAME),tr("Such expansion is not supported."));
				return;
			}
		} else
		{	
			QMessageBox::warning(this,tr("%1").arg(PROGRAM_NAME),tr("Such expansion is not supported."));
			return;
		}
		// Check file end;
		fullFileName = openFileName;
		fileName = QFileInfo(fullFileName).fileName();
	}	
	centrWidget->clearAll();	
	setMainConnection();
	fillDatabase();
	if ( !centrWidget->getMainLineString().isEmpty() )
		centrWidget->checkMainLineContent();
	wasCanceled = false;
	setTitle();
}

bool MainWidget::save(int mode)
{
	if ( mode == AskSaveFileNameMode && fileName != "temp" )
	{
		saveFileName = QFileDialog::getSaveFileName(this,tr("Save - %1").arg(PROGRAM_NAME),QDir::homePath(),"SL dictionaries ( *.* )");
		if ( saveFileName.isEmpty() )
			return false;			
	}
	else
	if ( mode == ToTemp )
		saveFileName = "/tmp/" + fileName;
	else
		saveFileName = fullFileName;
	
	QFile file(saveFileName);
	if ( !file.open(QIODevice::WriteOnly))
	{
		centrWidget->showWarning(CentralWidget::Warning,tr("File Error: It's immposable to open file."));
		return false;                          
	}
	else
	{
		fileName = QFileInfo(saveFileName).fileName();
		QTextStream stream(&file);
		if ( !centrWidget->getInformationDict().isEmpty() )
			stream << "#" << centrWidget->getInformationDict() << '\n';
		stream << centrWidget->getSaveString();
		centrWidget->setModified(CentralWidget::WasSaved);
		setTitle();
	}
	return true;
}

void MainWidget::cleanCache()
{
	int k = QMessageBox::warning(this,tr(PROGRAM_NAME),tr("Are you sure that you want to clear cache?"),QMessageBox::Yes, QMessageBox::No | QMessageBox::Default,QMessageBox::Cancel);
	if ( k == QMessageBox::Yes )
	{
		deleteDatabase();
		databaseName = "";
		fileName = "";
		fullFileName = "";
		statusFile = "";
		centrWidget->clearAll();
		centrWidget->setModified(CentralWidget::NotModified);
		clearRecentFile();
		enableActions(false);
		setTitle();
		askChoice();
	}
}

void MainWidget::clearRecentFile()
{
	QSettings settings(HOME_PATH + "settings.conf",QSettings::NativeFormat,0);
	settings.beginGroup("GeneralSettings");
	settings.setValue("recentfile","");
	settings.endGroup();
}

void MainWidget::addDictionaryToXsl()
{
	if ( fileName == "temp" )
	{
		centrWidget->showWarning(CentralWidget::Warning,tr("Before adding, you have to save your dictionary"));
		return;
	}
	QFile oldFile("/usr/share/sl/dicts/" + fileName);
	if ( oldFile.exists() )
	{
		QMessageBox messageBox(this);
		messageBox.setText(tr("The dictionary with the same name is already exists. "));
		messageBox.setWindowTitle(tr("Warning - %1").arg(PROGRAM_NAME));
		QPushButton *replaceButton = messageBox.addButton(tr("Replace"),QMessageBox::ActionRole);
		QPushButton *giveNameButton = messageBox.addButton(tr("Give another name"),QMessageBox::ActionRole);
		QPushButton *cancelButton = messageBox.addButton(tr("Cancel"),QMessageBox::ActionRole);
		messageBox.exec();
		if ( messageBox.clickedButton() == cancelButton )
			return;
		if ( messageBox.clickedButton() == giveNameButton )
		{
			wasAddedPrefixToStartOfDict = true;
			fileName = "New_" + fileName;
		}
		else
		if ( messageBox.clickedButton() == replaceButton )
			wasAddedPrefixToStartOfDict = false;
	}
	
	if ( !save(ToTemp) )
		return;
	
	
 	connect(&process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(startAccomOfDict(int,QProcess::ExitStatus)));
	process.start(QString("/bin/sh -c \"/usr/bin/sl --print-index %1 > /tmp/tempdict1\"").arg(fullFileName));
	countOfProcesses = 1;
	
}

void MainWidget::startAccomOfDict(int /*code*/, QProcess::ExitStatus exitStatus)
{
	if ( exitStatus == QProcess::CrashExit )
	{
		centrWidget->showWarning(CentralWidget::Warning,tr("The dictionary was not added, as was error"));
		if ( wasAddedPrefixToStartOfDict )
			fileName.remove(0,4);
		return;
	}
	switch ( countOfProcesses )
	{
		case 1:
			process.start(QString("/bin/sh -c \"/bin/cat %1 >> /tmp/tempdict1\"").arg("/tmp/" + fileName)); 
			break;
		case 2:
 			process.start("/bin/sh -c \"/usr/bin/sl --print-index /tmp/tempdict1 > /tmp/tempdict2\"");
			break;
		case 3:
			process.start(QString("/bin/sh -c \"/bin/cat %1 >> /tmp/tempdict2\"").arg("/tmp/" + fileName));
			break;
		case 4:
			process.start(QString("mv /tmp/tempdict2 /usr/share/sl/dicts/%1").arg(fileName));
			break;	
		case 5:
			process.start(QString("sl --connect %1").arg(fileName));
			break;
	} 
	if ( ++countOfProcesses == 7 )
	{
		centrWidget->showWarning(CentralWidget::AllGood,tr("The dictionary was added into sl database"));
		if ( wasAddedPrefixToStartOfDict )
			fileName.remove(0,4);
		process.close();
	}
}

void MainWidget::exitFromProgram()
{
	if ( !fullFileName.isEmpty() &&  centrWidget->getModified() == CentralWidget::Modified && isShowInformation)
 		showInformation();
	writeSettings();
	removeBlockFile();
	if ( !isShowInformation || fullFileName.isEmpty() || centrWidget->getModified() != CentralWidget::Modified )
	{
 		qApp->quit();
 		exit(0);
 	}
}
// File menu's slots end;
// IFA  slots start:
void MainWidget::startIfaApplication(QAction *action)
{
	std::cout << "IFA: run \"" << qPrintable(action->data().toString()) <<  "\"\n";
	QProcess::startDetached(action->data().toString());
}
// IFA  slots end;
// Progress bar  slots start:
void MainWidget::cancelLoading()
{
	int r = QMessageBox::warning(this,tr("Warning - %1").arg(PROGRAM_NAME),tr("Are you sure that you want to stop the loading?"),QMessageBox::Yes | QMessageBox::Default,QMessageBox::No | QMessageBox::Escape);
	if ( r == QMessageBox::No )
	{
		pb->setValue(pbValue);
		wasCanceled = false;
	}
	else
		wasCanceled = true;
}
// Progress bar  slots end;
// Help menu's slots start:
void MainWidget::showHelp()
{
	guideDialog->show();
}

void MainWidget::showAboutQt()
{
	QMessageBox::aboutQt(this);
}

void MainWidget::showAbout()
{
	if ( !aboutDialog )
		aboutDialog = new AboutDialog;
	aboutDialog->show();
}
// Help menu's slots end;
// Other slosts start:
void MainWidget::setInformationCheckBoxStatus(bool b)
{
	isShowInformation = b == true ? false : true;
	QSettings settings(HOME_PATH + "settings.conf",QSettings::NativeFormat,0);
	settings.beginGroup("GeneralSettings");
	settings.setValue("ShowInformation",isShowInformation);
	settings.endGroup();
}

void MainWidget::setTitle()
{
	QString titleName;
	if ( fileName == "temp" )
		titleName = "Untitled";
	else
		titleName = fileName;
	if ( centrWidget->getModified() == CentralWidget::TempWasNotSaved || centrWidget->getModified() == CentralWidget::ModifiedInTemp)
		setWindowTitle(QString("Untitled* - %1 %2").arg(PROGRAM_NAME).arg(VERSION));
	else
	if ( !titleName.isEmpty() && ( centrWidget->getModified()== CentralWidget::NotModified ||centrWidget->getModified() == CentralWidget::WasSaved))
		setWindowTitle(titleName + QString(" - %1 %2").arg(PROGRAM_NAME).arg(VERSION));
	else
	if ( !titleName.isEmpty() && ( centrWidget->getModified()== CentralWidget::NotModified ||centrWidget->getModified() == CentralWidget::WasSaved))
		setWindowTitle(titleName + QString(" - %1 %2").arg(PROGRAM_NAME).arg(VERSION));
	else
	if ( !titleName.isEmpty() && centrWidget->getModified()== CentralWidget::Modified )
		setWindowTitle(titleName + QString("* - %1 %2").arg(PROGRAM_NAME).arg(VERSION));
	else
		setWindowTitle(QString("%1 %2").arg(PROGRAM_NAME).arg(VERSION));
}

// Other slosts end;
// Settings slots start:

void MainWidget::showSettings()
{
	settingsDialog->show();
}

void MainWidget::showManager(int mode)
{
	managerDialog->setMode(mode);
	managerDialog->show();
}

void MainWidget::actionOnManagerSignal(int action,QString name)
{
	switch (action)
	{
		case ManagerDialog::AskChoice:
			askChoice(false);
			break;
		case ManagerDialog::Open:
			fileName = name;
			fullFileName = HOME_PATH + "databases/" + name;
			open(DontAskOpenFileNameMode);
			managerDialog->updateDicts();
		break;
		case ManagerDialog::Remove:
			fileName = name;
			fullFileName = HOME_PATH + "databases/" + name;
			open(DontAskOpenFileNameMode);
			cleanCache();
			managerDialog->updateDicts();
		break;
		case ManagerDialog::Add:
			QString wasFileName = fileName;
			QString wasFullFileName = fullFileName;
			fileName = name;
			fullFileName = HOME_PATH + "databases/" + name;
			open(DontAskOpenFileNameMode);
			addDictionaryToXsl();
			fileName = wasFileName;
			fullFileName = wasFullFileName;
			open(DontAskOpenFileNameMode);
			break;
	}
}

void MainWidget::updateProgram()
{
	boolSets = settingsDialog->getBoolSettings();
	intSets = settingsDialog->getIntSettings();
	centrWidget->setSettings(boolSets,intSets);
}

void MainWidget::showMessageInStatusBar(const QString str)
{
	sb->showMessage(str);
}


//Settigns slots end;                   

//--------------Private end------------------//

//===========================================//
//============ Private functions: ===========//
//===========================================//

void MainWidget::readSettings(int mode)
{
	QSettings settings(HOME_PATH + "settings.conf",QSettings::NativeFormat,0);
	
	settings.beginGroup("GeneralSettings");
	QRect rect = settings.value("geometry",QRect(100,40,900,600)).toRect();
	move(rect.topLeft());
	resize(rect.size());
	
	isShowInformation = settings.value("ShowInformation",true).toBool();
	settings.endGroup();		
			
	settings.beginGroup("BoolSettings");
	// Bool Settings start:
	boolSets.append( settings.value("UpdateTransDuringEntering",false).toBool());
     boolSets.append( settings.value("UpdatePreviewDuringEntering",true).toBool());
	boolSets.append( settings.value("OpenRecentFile",true).toBool());
	boolSets.append( settings.value("OpenWordsInNewTabs",true).toBool());
	boolSets.append( settings.value("ShowSplashScreen",true).toBool());
	boolSets.append( settings.value("ShowAutoSearch",false).toBool());
	boolSets.append( settings.value("ShowBookmarks",false).toBool());
	boolSets.append( settings.value("ShowHistory",false).toBool());
	boolSets.append( settings.value("ShowPreviewApart",false).toBool());
	boolSets.append( settings.value("HighLightTrans",true).toBool());
	boolSets.append( settings.value("SearchWordsByBegining",true).toBool());
	boolSets.append( settings.value("MoveBySingleClick",true).toBool() );
	boolSets.append( settings.value("ShowMarksInAutoSearch",true).toBool() );
	boolSets.append( settings.value("ShowMarksInTabs",true).toBool() );
	// Bool Settings end;
	settings.endGroup();
	settings.beginGroup("IntSettings");
	// Int Settigns start:
	intSets << settings.value("MinimumRecords",100).toInt();
	// Int Settigns end;
	settings.endGroup();

	settingsDialog->setSettings(boolSets,intSets);
	centrWidget->setSettings(boolSets,intSets);
	
	settings.beginGroup("GeneralSettings");
	if ( mode == OpenRecentFileMode && boolSets[OpenRecentFile] )
	{
		QString recentFileName = settings.value("recentfile",QString("")).toString();
		if ( !recentFileName.isEmpty() )
		{
			QFile recentFile(recentFileName);
			if ( recentFile.exists() )
			{
				fullFileName = recentFileName;
				fileName = QFileInfo(fullFileName).fileName();
				setMainConnection();
			}
			else
			{
				settings.setValue("recentfile","");
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

void MainWidget::writeSettings()
{	
	QSettings settings(HOME_PATH + "settings.conf",QSettings::NativeFormat,0);
	
	boolSets = settingsDialog->getBoolSettings();
	intSets = settingsDialog->getIntSettings();
	settings.beginGroup("BoolSettings");
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
	settings.beginGroup("IntSettings");
	settings.setValue("MinimumRecords",intSets[MinimumRecords]);
	settings.endGroup();
		
	settings.beginGroup("GeneralSettings");
	settings.setValue("geometry",geometry()); 	
	settings.setValue("ExtendedSearch",centrWidget->getExtendedSearchStatus());
	settings.setValue("TextEditPartState",centrWidget->getPreviewGeometry());
	settings.setValue("CentralPartState",centrWidget->getLocalMainWidgetState());
	settings.setValue("HelpState",guideDialog->getState());
	// Update status,bookmark files and set recent file start
	if ( !fullFileName.isEmpty() && !wasCanceled )
	{			
		settings.setValue("recentfile",fullFileName);
		
		// Put status of database to status file start
		QFile statusF(statusFile);
		statusF.open(QIODevice::WriteOnly);
		QTextStream stream(&statusF);
		stream << centrWidget->getModified();
		// Put status of database to status file end
		// Bookmarks start:
		QString bookMarksFileName = HOME_PATH + "bookmarks/" + fileName + "bookmarks";
		QFile bookMarkFile(bookMarksFileName);
		if ( !centrWidget->getBookMarks().isEmpty() )
		{	
			bookMarkFile.open(QIODevice::WriteOnly);
			QTextStream streamBookMark(&bookMarkFile);
			streamBookMark << centrWidget->getBookMarks();
		}
		else
			bookMarkFile.remove();		
		// Bookmarks end;
		// history start:
		QFile historyFile(HOME_PATH + "histories/" + fileName + "history");
		if ( centrWidget->getHistory() != "0" )
		{
			historyFile.open(QIODevice::WriteOnly);
			QTextStream stream(&historyFile);
			stream << centrWidget->getHistory();
		}
		else
			historyFile.remove();
		// history end;	
		// about start:
		QFile aboutFile(HOME_PATH + "abouts/" + fileName + "about");
		if ( !centrWidget->getInformationDict().isEmpty() )
		{
			aboutFile.open(QIODevice::WriteOnly);
			QTextStream stream(&aboutFile);
			stream << centrWidget->getInformationDict();
		}
		else
			aboutFile.remove();
		// about end;
	}	
	settings.endGroup();
	// Update status,bookmark files and set recent file end;
}

void MainWidget::createAndSetDirectories()
{
	QDir dir;
	if ( !dir.exists(HOME_PATH))
		dir.mkdir(HOME_PATH);
	
	if ( !dir.exists(HOME_PATH + "bookmarks/") )
		dir.mkdir(HOME_PATH + "bookmarks/");
	
	if ( !dir.exists(HOME_PATH + "statuses/") )
		dir.mkdir(HOME_PATH + "statuses/");
	
	if ( !dir.exists(HOME_PATH + "databases/") )
		dir.mkdir(HOME_PATH + "databases/");
		
	if ( !dir.exists(HOME_PATH + "histories/") )
		dir.mkdir(HOME_PATH + "histories/");
		
	if ( !dir.exists(HOME_PATH + "ifa/") )
		dir.mkdir(HOME_PATH + "ifa/");
		
	if ( !dir.exists(HOME_PATH + "abouts/") )
		dir.mkdir(HOME_PATH + "abouts/");
		
	dir.setCurrent(HOME_PATH + "databases/");
	
}

void MainWidget::showInformation()
{
	QDialog *infDialog = new QDialog(this);
	infDialog->setModal(true);
	QCheckBox *isShowAgain = new QCheckBox(tr("Dont' show this message again?"));
	connect(isShowAgain,SIGNAL(toggled(bool)),this,SLOT(setInformationCheckBoxStatus(bool)));
	QLabel *text = new QLabel;
	text->setText(tr("Data, which you have edited or created is saved in temp file.\n"
												"If you want to save data in special file, choose point \"Save\" in menu"));
	QPushButton *closeButton = new QPushButton(tr("Close"));
	connect(closeButton,SIGNAL(clicked()),infDialog,SLOT(close()));
	connect(closeButton,SIGNAL(clicked()),qApp,SLOT(quit()));
			
	QHBoxLayout* bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addStretch();
	bottomLayout->addWidget(closeButton);
	bottomLayout->addStretch();
	bottomLayout->addStretch();
			
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(text);
	mainLayout->addWidget(isShowAgain);
	mainLayout->addLayout(bottomLayout);
			
	infDialog->setLayout(mainLayout);
	infDialog->setFixedSize(infDialog->sizeHint());
	infDialog->setWindowTitle(tr("Information - %1").arg(PROGRAM_NAME));
	infDialog->show();
}

int MainWidget::showAskChoiceDialog(bool isCancel)
{
	QMessageBox messageBox(this);
	messageBox.setText(tr("What do you want to do?"));
	messageBox.setIconPixmap(QPixmap(ICONS_PATH + "lle.png").scaled(50,50));
	QPushButton *managerButton = messageBox.addButton(tr("Show manager"), QMessageBox::ActionRole);
	managerButton->setIcon(QIcon(ICONS_PATH + "manager.png"));
	QPushButton *quitButton = 0;
	QPushButton *cancelButton = 0;
 	if ( isCancel )
 	{
	 	cancelButton = messageBox.addButton(tr("Cancel"),QMessageBox::ActionRole);
	}
	else
	{		
		quitButton = messageBox.addButton(tr("Quit"),QMessageBox::ActionRole);
	 	quitButton->setIcon(QIcon(ICONS_PATH + "exit.png"));
 	}
 	QPushButton *openNewDictButton = messageBox.addButton(tr("Open"),QMessageBox::ActionRole);
 	openNewDictButton->setIcon(QIcon(ICONS_PATH + "open.png"));
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

void MainWidget::askChoice(bool isCancelAvaible)
{
	switch (showAskChoiceDialog(isCancelAvaible))
	{
		case OpenNewDictionary:
			open();
			break;
		case ShowManager:
			showManager(isCancelAvaible ?  CloseManagerAvaible : DontCloseManagerAvaible);
			break;
		case Quit:
			exitFromProgram();
			break;
	}
}

void MainWidget::enableActions(bool b)
{
	saveAction->setEnabled(b);
}
// Database's functions start:
void MainWidget::setMainConnection()
{
	QSqlDatabase::removeDatabase("sqlite");
	db = QSqlDatabase::addDatabase("QSQLITE","sqlite");
	databaseName = fileName;
	db.setHostName("localhost");
	db.setDatabaseName(databaseName);
	db.setUserName("user");
	db.setPassword("");
	if ( !db.open() )
	{
		centrWidget->showWarning(CentralWidget::Warning,tr("Database Error: ") + db.lastError().text());
		return;	
	}
	query = new QSqlQuery(db);
	statusFile = HOME_PATH + "statuses/" + fileName + "status";
	QFile statusF(statusFile);
	if ( !statusF.exists() )
	{	
		statusF.open(QIODevice::WriteOnly);
		centrWidget->setModified(CentralWidget::FillDatabase);
		QTextStream streamStatus(&statusF);
		streamStatus << CentralWidget::FillDatabase;
	}
	else                                                                                          
	{
		statusF.open(QIODevice::ReadOnly);
		QTextStream streamStatus(&statusF);
		int tempWasModified;
		streamStatus >> tempWasModified;
		centrWidget->setModified(tempWasModified);
	}
		
	QFile bookMarkFile(HOME_PATH + "bookmarks/" + databaseName + "bookmarks");
	if ( bookMarkFile.exists() )
	{
		bookMarkFile.open(QIODevice::ReadOnly);
		QTextStream streamBookMark(&bookMarkFile);
		QString bookMarksString = streamBookMark.readAll();
		centrWidget->setBookMarks(bookMarksString);
	}
			
	QFile historyFile(HOME_PATH + "histories/" + databaseName + "history");
	if ( historyFile.exists() )
	{
		historyFile.open(QIODevice::ReadOnly);
		QTextStream streamHistory(&historyFile);
		QString historyString = streamHistory.readAll();
		centrWidget->setHistory(historyString);
	}
			
	QFile aboutFile(HOME_PATH + "abouts/" + databaseName + "about");
	if ( aboutFile.exists() )
	{
		aboutFile.open(QIODevice::ReadOnly);
		QTextStream streamAbout(&aboutFile);
		QString aboutString = streamAbout.readAll();
		centrWidget->setInformationDict(aboutString);
	}
	
	query->exec(QString("CREATE TABLE IF NOT EXISTS main(`word` TEXT NOT NULL,`trans` TEXT NOT NULL,`stat` TEXT NOT NULL)"));
			
	if ( !query->isActive() )
		centrWidget->showWarning(CentralWidget::Warning,tr("Database Error:") + query->lastError().text());
	else
		centrWidget->setDatabase(db,query);	
	setTitle();
	enableActions(true);
}

void MainWidget::fillDatabase()
{
	if ( centrWidget->getModified() == CentralWidget::FillDatabase )
	{
		QFile file(fullFileName);
		if ( !file.open(QIODevice::ReadOnly) )
		{
				centrWidget->showWarning(CentralWidget::Warning,tr("File Error: It's immposable to open file."));
				return;
		}
		QTextStream streamData(&file);
		
		QString tempString;
		QString aboutString;
		QStringList list;
			
 		int rows = 0;
 		for ( rows = 0; !streamData.atEnd(); rows++ )
 			streamData.readLine(); 
 			
 		if ( !pb )
 		{
 			pb = new QProgressDialog(tr("Please wait for the data to load into SQL database"), tr("Cancel"),0,1, this);
 			connect(pb,SIGNAL(canceled()),this,SLOT(cancelLoading()));
 			pb->setWindowTitle(tr("Loading - %1").arg(PROGRAM_NAME));
 			pb->setWindowIcon(QIcon(ICONS_PATH + "lle.png"));
 			pb->setAutoClose(true);
 			pb->setWindowModality(Qt::WindowModal);
 		}
 		pbValue = 0;
 		pb->setRange(0,rows);
 		streamData.seek(0);
 		QCoreApplication::processEvents();
		wasCanceled = false;
		while ( !streamData.atEnd() )
		{
			tempString = streamData.readLine();
			pbValue++;
			pb->setValue(pbValue);
			if ( tempString[0] == '#' || tempString.isEmpty() )
			{
				aboutString += tempString;	
				continue;
			}
			list = tempString.split("  ");
			if ( list.count() == 2 )
			{
				query->exec(QString("INSERT INTO main(word,trans,stat) VALUES(\"%1\",\"%2\",\"0\")")
									.arg(list[0].simplified().toLower()).arg(list[1].simplified()));					
			}
			if ( wasCanceled )
			{
				streamData.seek(pb->maximum());
				QFile databaseFile(HOME_PATH + "databases/" + fileName);
				databaseFile.remove();
				QFile statusFile(HOME_PATH + "statuses/" + fileName + "status");
				statusFile.remove();
				pb->hide();
				show();	
				askChoice();
				return;
			}
		}
		pb->hide();
		if ( !aboutString.isEmpty() )
		{
			QFile file(HOME_PATH + "abouts" + fileName + "about");
			file.open(QIODevice::WriteOnly);
			QTextStream stream(&file);
			stream << aboutString;
			centrWidget->setInformationDict(aboutString);
		}
		centrWidget->setModified(CentralWidget::NotModified);          
		managerDialog->updateDicts();
	}
}

void MainWidget::deleteDatabase()
{                                                     
	if ( fileName != "temp" )
	{
		QFile fileDatabase(HOME_PATH + "databases/" + databaseName);
		if ( fileDatabase.exists() )
			fileDatabase.remove();
	}
	else
		query->exec("DELETE FROM main");
	QFile fileStatus(statusFile);
	if ( fileStatus.exists() )
		fileStatus.remove();
	QFile bookMarkFile(HOME_PATH + "bookmarks/" + databaseName + "bookmarks");
	if ( bookMarkFile.exists() )
		bookMarkFile.remove();
	QFile historyFile(HOME_PATH + "histories/" + databaseName + "history");
	if ( historyFile.exists() )
		historyFile.remove();
	QFile aboutFile(HOME_PATH + "abouts/" + databaseName + "about");
	if ( aboutFile.exists() )
		 aboutFile.remove();
}
// Database's functions end;
//Block functions start:
void MainWidget::checkBlockFile()
{
	QFile blockFile("/tmp/lileditor.block");
	if ( blockFile.exists() )
	{
		QMessageBox::warning(this,tr("Error - %1").arg(PROGRAM_NAME),tr("The program is already running - close old process and start new.\n"
															"If the program was fallen, run \"lileditor -r\" to remove block file\n"
															"and run \"lileditor\" again."));
 		qApp->quit();
 		exit(1);
	}
}

void MainWidget::createBlockFile()
{
	QFile blockFile("/tmp/lileditor.block");
	if ( !blockFile.exists() )
		blockFile.open(QIODevice::WriteOnly);
}

void MainWidget::removeBlockFile()
{
	QFile blockFile("/tmp/lileditor.block");
	if ( blockFile.exists() )
		blockFile.remove();
}
//Block functions end;


//===========================================//
//========== Protected functions: ===========//
//===========================================//

void MainWidget::closeEvent(QCloseEvent*)
{
 	exitFromProgram();
}

void MainWidget::keyPressEvent(QKeyEvent* event)
{
	if ( Qt::Key_Space == event->key() )
		centrWidget->setFocusOnMainLine();
}
