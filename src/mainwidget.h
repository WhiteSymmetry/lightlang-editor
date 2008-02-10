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
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QtSql>
#include <QProcess>
#include "global.h"

class CentralWidget;
class QListBox;
class QRadioButton;
class AboutDialog;
class QCheckBox;
class IFA;
class QProgressDialog;
class GuideDialog;
class SettingsDialog;
class ManagerDialog;

class MainWidget : public QMainWindow
{
	Q_OBJECT
	protected:
		void closeEvent(QCloseEvent*);
		void keyPressEvent(QKeyEvent*);
	public:
		MainWidget(bool isCreateNewDict,QString&);
		enum Modes  { OpenRecentFileMode = 0,DontOpenRecentFileMode, AskSaveFileNameMode, 
					DontAskSaveFileNameMode, AskOpenFileNameMode, DontAskOpenFileNameMode, ToTemp };
		enum Actions {OpenNewDictionary = 0, ShowManager, Quit, WasCanceled };
	private:
		//INIT
		void init();
		// All actions start
		// file menu's actions:
		QAction *newAction;
		QAction *openAction;
		QAction *saveAction;
		QAction *exitAction;
		
		// help menu's actions:
		QAction *guideAction;
		QAction *aboutAction;
		QAction *aboutQtAction;
		
		// settings menu's actions:
		QAction *settingsAction;
		QAction *managerAction;
	
		// record menu's actions:
		QAction*	editAction;
		QAction*	removeAction;
		QAction*	addAction;
		QAction* startAction;
		
		// Menus start:
		QMenuBar* mainBar;
		QMenu* fileMenu;
		QMenu* recordMenu;
		QMenu* settingsMenu;
		QMenu* helpMenu;
		QMenu* ifaMenu;
		// Menus end.
		
		// Diffirent widgets 
		CentralWidget *centrWidget;
		IFA *ifa;
		GuideDialog *guideDialog;
          AboutDialog *aboutDialog;
		SettingsDialog *settingsDialog;
		ManagerDialog *managerDialog;
			
		//Database vars
		QSqlDatabase db;
		QSqlQuery *query;
		// Vars of dictionary's information start:
		QString fullFileName;
		QString fileName;	
		QString statusFile;
		QString databaseName;
		QString saveFileName;
		// Vars of dictionary's information end
		
		QStatusBar *sb;
		
		bool isShowInformation;  
		bool wasCanceled; 
		
		// Settings:
		QList<bool> boolSets;
		QList<int> intSets;
		
		// IFA:
		QStringList programPathes;
		
		// ProgressBar
		QProgressDialog *pb;
		int pbValue;
		
		// for translation
		QTranslator translator;
		
		//Add a dictionary to XSL
		bool isThereXsl;
		QProcess process;
		int countOfProcesses;
		bool wasAddedPrefixToStartOfDict;
		
		// Private functions start:
		void readSettings(int mode = OpenRecentFileMode);
		void writeSettings();
		void createAndSetDirectories();
		void showInformation();
		int showAskChoiceDialog(bool isCancel);
		void askChoice(bool isCancel = true);
		void enableActions(bool);
		void clearRecentFile();
		//Database functions:
		void setMainConnection();
		void fillDatabase();
		void deleteDatabase();
		//Block functions:
		void createBlockFile();
		void removeBlockFile();
		void checkBlockFile();
		// Private functions end;
	private slots:
		// File menu's slots:
		void newDictionary();  
		void open(int mode = AskOpenFileNameMode);
		bool save(int mode = AskSaveFileNameMode);
		void cleanCache();
		void addDictionaryToXsl();
		void startAccomOfDict(int,QProcess::ExitStatus); 
		void exitFromProgram();
		// IFA  slots:
		void startIfaApplication(QAction*);
		// Progress bar  slots:
		void cancelLoading();
		// Help menu's slots:
		void showHelp();
		void showAboutQt();
		void showAbout();
		// Other slosts:
		void setInformationCheckBoxStatus(bool);
		void setTitle();        
		// Settings slots:
		void showSettings();
		void showManager(int mode = CloseManagerAvaible);
		void actionOnManagerSignal(int,QString);
		// Process slots:
		void updateProgram();
		
		void showMessageInStatusBar(const QString str);
	
};


#endif
