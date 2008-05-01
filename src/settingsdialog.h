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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QWidget;
class QLabel;
class QCheckBox;
class QGroupBox;
class QStackedWidget;
class QSpinBox;
class QRadioButton;
class QLineEdit;
class QTabWidget;
class QTextEdit;

class SettingsDialog : public QDialog
{
	Q_OBJECT
	signals:
		void settingsChanged();
	private slots:
		void changePage(QListWidgetItem*,QListWidgetItem*);
		void checkIfaContent();
		void getProgramPath();
		void getIconPath();
		void add();
		void removeIFA();
		void setEnableOfRemoveIfa();
	private:
		QListWidget *settingsItems;
		QListWidgetItem *generalItem;
		QListWidgetItem *viewItem;
		QListWidgetItem *autosearchItem;
		QListWidgetItem *ifaItem;
		QWidget *generalWidget;
		QWidget *viewWidget;
		QWidget *autosearchWidget;
		QTabWidget *ifaWidget;
		QStackedWidget *pagesWidget;
		QLabel *generalLabel;
		QLabel *viewLabel;
		QLabel *autosearchLabel;
		
		// Group boxes start:
		// -----General:-----//
		QGroupBox *behaviourBox;
		QGroupBox *startOptionsBox;
		QGroupBox *tabsBox;
		//------View:--------//
		QGroupBox *viewBox;
		QGroupBox *highlightBox;	
		QGroupBox *marksBox;
		//-----Auto search:--//
		QGroupBox *commonSettingsBox;
		QGroupBox *showWordByBox;	
		// Group boxes end;
		
		// General settings start:
		QCheckBox *isUpdateTransDuringEntering;
		QCheckBox *isUpdatePreviewDuringEntering;
		QCheckBox *isOpenRecentFile;
		QCheckBox *isOpenWordsInNewTabs;
		// General settings end;
		// View settigns start:
		QCheckBox *isHighLightTrans;
		QCheckBox *isShowMarksInAutoSearch;
		QCheckBox *isShowMarksInTabs;
		// View settings end;
		// Auto search settings start:
		QCheckBox *isSearchWordsByBegining;
		QSpinBox *minimumRecords;
		QLabel *minimumRecordsLabel;
		QRadioButton *isShowWordBySingleClick;
		QRadioButton *isShowWordByDoubleClick;		 
		// Auto search settings end;
		// Ifa settings start:
		QLineEdit *nameOnEnglish;
		QLineEdit *nameOnRussian;
		QTextEdit *descrOnEnglish;
		QTextEdit *descrOnRussian;
		QLineEdit *programPath;
		QLineEdit *iconPath;
		QPushButton *programPathBrowser;
		QPushButton *iconPathBrowser;
		QPushButton *addIfa;
		QListWidget *ifaManager;
		QPushButton *removeIfa;
		QLabel *nameOnEnglishLabel;
		QLabel *nameOnRussianLabel;
		QLabel *descrOnEnglishLabel;
		QLabel *descrOnRussianLabel;
		QLabel *programPathLabel;
		QLabel *iconPathLabel;
		// Ifa settigns end;
	public:
		SettingsDialog(QWidget *parent = 0);
		QList<bool> getBoolSettings();
		QList<int>  getIntSettings();
		void setSettings(QList<bool>&,QList<int>&);
};

#endif
