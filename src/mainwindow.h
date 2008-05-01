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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTranslator>
#include <QProcess>
#include <QFile>
#include "global.h"

class CentralWidget;
class AboutDialog;
class Ifa;
class QProgressDialog;
class ManualDialog;
class SettingsDialog;
class ManagerDialog;
class GetDictName;
class QTextStream;
class QSplashScreen;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
	protected:
		void closeEvent(QCloseEvent *event);
	private:
		enum Modes  {   
					AskSaveFileName_Mode = 0, 
					DontAskSaveFileName_Mode, 
					ToTemp_Mode 
				  };
		enum Actions {
					OpenNewDictionary = 0, 
					ShowManager, 
					Quit, 
					WasCanceled 
				   };
		//INIT
		void init();
		// All actions start
		// file menu's actions:
		QAction *newDictAction;
		QAction *openDictAction;
		QAction *saveDictAction;
		QAction *exitAction;
		
		// help menu's actions:
		QAction *manAction;
		QAction *aboutAction;
		QAction *aboutQtAction;
		
		// tools menu's actions:
		QAction *settingsAction;
		QAction *managerAction;
		QAction *previewPanelAction;
		QAction *bookmarksPanelAction;
		QAction *autoSearchPanelAction;
		QAction *historyPanelAction;
	
		// record menu's actions:
		QAction*	editAction;
		QAction*	removeAction;
		QAction*	addAction;
		QAction*  ifaAction;
		
		// Menus start:
		QMenuBar* mainBar;
		QMenu* dictMenu;
		QMenu* recordMenu;
		QMenu* toolsMenu;
		QMenu* helpMenu;
		QMenu* ifaMenu;
		// Menus end.
		
		// Diffirent widgets 
		CentralWidget *centralWidget;
		Ifa *ifa;
		ManualDialog *manualDialog;
          AboutDialog *aboutDialog;
		SettingsDialog *settingsDialog;
		ManagerDialog *managerDialog;
		GetDictName *getDictName;
			
		// Splash screen vars
		bool showSplashScreen;
		QSplashScreen *splashScreen;	
		
		//Database vars
		QSqlDatabase db;
		QSqlQuery query;
		// Vars of dictionary's information start:
		QString databaseName;
		QFile bookmarksFile;
		QFile historyFile;
		QFile aboutDictFile;
		QTextStream *bookmarksStream;
		QTextStream *historyStream;
		QTextStream *aboutDictStream;
		//-----------
		QString tempString; 
		// Vars of dictionary's information end
		
		QStatusBar *statBar;
		
		bool wasCanceled; 
		
		// Settings:
		QList<bool> boolSets;
		QList<int> intSets;
		
		// IFA:
		QStringList programPathes;
		
		// ProgressBar
		QProgressDialog *progressDialog;
		int progressBarValue;
		
		// for translation
		QTranslator translator;
		
		// Sl information
		QProcess process;
		int countOfProcesses;
		
		// Private functions start:
		void readSettings();
		void writeSettings();
		void createAndSetDirs();
		int askChoiceDialog(bool isShowCancel);
		void askChoice(bool isShowCancel = true);
		void enableActions(bool);
		void cleatRecentFile();
		//Database functions:
		void createLocalConnection(QString name);
		void fillDatabase(QString path_to_dict);
		//Block functions:
		void createBlockFile();
		void checkBlockFile();
		void removeBlockFile();
		// Private functions end;
		void clearRecentFile();
		void setRecentFile(QString name);
		void showSplashMessage(const QString text);
	public:
		MainWindow(QString& dictToOpen);
	private slots:
		// File menu's slots:
		void newDict();  
		void openDict(QString dictToOpen = QString(""));
		void saveDict();
		void addDictToSlDatabase(QString name);
		void startAccomOfDict(int,QProcess::ExitStatus); 
		void exitFromProgram();
		// IFA  slots:
		void startIfApplication(QAction* chosenAction);
		// Progress bar  slots:
		void cancelLoading();
		// Help menu's slots:
		void showAboutQt();
		// Other slosts:
		void setTitle();        
		// Settings slots:
		void showManagerDialog(int mode = CloseManagerAvaible);
		void actionOnManagerSignal(int action,QString name);
		// Process slots:
		void updateProgram();
		void clearCache(QString name);
};

#endif
