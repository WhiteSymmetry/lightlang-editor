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
#include "settingsdialog.h"
#include "global.h"
#include "const.h"

SettingsDialog::SettingsDialog(QWidget* parent ) : QDialog(parent)
{
	
	// Create list widget items:
	generalItem = new QListWidgetItem;
	generalItem->setText(tr("General"));
	generalItem->setIcon(QIcon(ICONS_PATH + "settings/general.png"));
	generalItem->setTextAlignment(Qt::AlignHCenter);
     generalItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	viewItem = new QListWidgetItem;
	viewItem->setText(tr("View"));
	viewItem->setIcon(QIcon(ICONS_PATH + "settings/view.png"));
	viewItem->setTextAlignment(Qt::AlignHCenter);
     viewItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	autosearchItem = new QListWidgetItem;
	autosearchItem->setText(tr("Auto-search"));
	autosearchItem->setIcon(QIcon(ICONS_PATH + "search.png"));
	autosearchItem->setTextAlignment(Qt::AlignHCenter);
     autosearchItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	ifaItem = new QListWidgetItem;
	ifaItem->setText("IFA");
	ifaItem->setIcon(QIcon(ICONS_PATH + "settings/ifa.png"));
	ifaItem->setTextAlignment(Qt::AlignHCenter);
     ifaItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	// Creation of list widget start:
	settingsItems = new QListWidget; 
	settingsItems->setFlow(QListView::TopToBottom);
	settingsItems->setMovement(QListView::Static);
	settingsItems->addItem(generalItem);
	settingsItems->addItem(viewItem);
	settingsItems->addItem(autosearchItem);
	settingsItems->addItem(ifaItem);
	connect(settingsItems,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
				this,SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
	// Creation of list widget end;
	
	// Create labels:
	generalLabel = new QLabel;
	generalLabel->setText("<center><b>" + tr("General settings") + "</b></center>");
	viewLabel = new QLabel;
	viewLabel->setText("<center><b>" + tr("View settings") + "</b></center>");
	autosearchLabel = new QLabel;
	autosearchLabel->setText("<center><b>" + tr("Auto-search's settings") + "</b></center>");
	//================
	
	// Create widgets:
	pagesWidget = new QStackedWidget;
	generalWidget = new QWidget;
	viewWidget = new QWidget;
	autosearchWidget = new QWidget;
	ifaWidget = new QTabWidget;	

	//Add widgets into stack
	pagesWidget->addWidget(generalWidget);
	pagesWidget->addWidget(viewWidget);
	pagesWidget->addWidget(autosearchWidget);
	pagesWidget->addWidget(ifaWidget);
	
	QPushButton *closeButton = new QPushButton(QIcon(ICONS_PATH + "settings/close.png"),tr("Close"));
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));
	
	QPushButton *applyButton = new QPushButton(QIcon(ICONS_PATH + "settings/apply.png"),tr("Apply"));
	connect(applyButton,SIGNAL(clicked()),this,SIGNAL(settingsChanged()));
	
	QPushButton *okButton = new QPushButton(QIcon(ICONS_PATH + "settings/ok.png"),"Ok");
	connect(okButton,SIGNAL(clicked()),this,SLOT(hide()));
	connect(okButton,SIGNAL(clicked()),this,SIGNAL(settingsChanged()));
	//===========================
	
	
	// Creation of general settigns start:
	isUpdateTransDuringEntering = new QCheckBox(tr("Update translation during entering"));
	isUpdatePreviewDuringEntering = new QCheckBox(tr("Update preview during entering"));
	isOpenRecentFile = new QCheckBox(tr("Open recent file"));
	isOpenWordsInNewTabs = new QCheckBox(tr("Open words in new tabs"));
	
	// group boxes:
	behaviourBox = new QGroupBox(tr("Behaviour:"));
	QVBoxLayout *behaviourBoxLayout = new QVBoxLayout;
	behaviourBoxLayout->addWidget(isUpdateTransDuringEntering);
	behaviourBoxLayout->addWidget(isUpdatePreviewDuringEntering);
	behaviourBox->setLayout(behaviourBoxLayout);
	//--------------
	startOptionsBox = new QGroupBox(tr("Start options:"));
	QVBoxLayout *startOptionsBoxLayout = new QVBoxLayout;
	startOptionsBoxLayout->addWidget(isOpenRecentFile);
	startOptionsBox->setLayout(startOptionsBoxLayout);
	//--------------
	tabsBox = new QGroupBox(tr("Tab's options:"));
	QVBoxLayout *tabsBoxLayout = new QVBoxLayout;
	tabsBoxLayout->addWidget(isOpenWordsInNewTabs);
	tabsBox->setLayout(tabsBoxLayout);
	
	QVBoxLayout *mainGeneralLayout = new QVBoxLayout;
	mainGeneralLayout->addWidget(generalLabel);
	mainGeneralLayout->addWidget(behaviourBox);
	mainGeneralLayout->addWidget(startOptionsBox);
	mainGeneralLayout->addWidget(tabsBox);
	mainGeneralLayout->addStretch();
	generalWidget->setLayout(mainGeneralLayout);
	// Creation of general settings end;
	
	// Creation of view settings start:
	isHighLightTrans = new QCheckBox(tr("Highlight syntax in translation"));
	isShowMarksInAutoSearch = new QCheckBox(tr("Show marks in auto-search panel"));
	isShowMarksInTabs = new QCheckBox(tr("Show marks in tabs"));

	// Group boxes:
	highlightBox = new QGroupBox(tr("Highlight:"));
	QVBoxLayout *highlightBoxLayout = new QVBoxLayout;
	highlightBoxLayout->addWidget(isHighLightTrans);
	highlightBox->setLayout(highlightBoxLayout);
	//---------------	
	marksBox = new QGroupBox(tr("Marks:"));
	QVBoxLayout *marksBoxLayout = new QVBoxLayout;
	marksBoxLayout->addWidget(isShowMarksInAutoSearch);
	marksBoxLayout->addWidget(isShowMarksInTabs);
	marksBox->setLayout(marksBoxLayout);
	
	//----------------
	QVBoxLayout *mainViewLayout = new QVBoxLayout;
	mainViewLayout->addWidget(viewLabel);
	mainViewLayout->addWidget(highlightBox);
	mainViewLayout->addWidget(marksBox);
	mainViewLayout->addStretch();
	viewWidget->setLayout(mainViewLayout);
	// Creation of view settings end;
	
	// Creation of autosearch settings start:
	isSearchWordsByBegining = new QCheckBox(tr("Search words by begining of main line's word"));
	minimumRecords = new QSpinBox;
	minimumRecords->setMaximum(500);
	minimumRecordsLabel = new QLabel(tr("Minimum number of records when extended search is off"));
	isShowWordBySingleClick = new QRadioButton(tr("Single click"));
	isShowWordByDoubleClick = new QRadioButton(tr("Double click"));
		
	// Group boxes:
	commonSettingsBox = new QGroupBox(tr("Common settings:"));
	QVBoxLayout *commonSettingsBoxLayout = new QVBoxLayout;
	commonSettingsBoxLayout->addWidget(isSearchWordsByBegining);
	QHBoxLayout *minimumRecodsLayout = new QHBoxLayout;
	minimumRecodsLayout->addWidget(minimumRecordsLabel);
	minimumRecodsLayout->addWidget(minimumRecords);
	commonSettingsBoxLayout->addLayout(minimumRecodsLayout);
	commonSettingsBox->setLayout(commonSettingsBoxLayout);
	//-------------
	showWordByBox = new QGroupBox(tr("Show record by:"));
	QVBoxLayout *showWordByBoxLayout = new QVBoxLayout;
	showWordByBoxLayout->addWidget(isShowWordBySingleClick);
	showWordByBoxLayout->addWidget(isShowWordByDoubleClick);
	showWordByBox->setLayout(showWordByBoxLayout);
	
	QVBoxLayout *mainAutosearchLayout = new QVBoxLayout;
	mainAutosearchLayout->addWidget(autosearchLabel);
	mainAutosearchLayout->addWidget(commonSettingsBox);
	mainAutosearchLayout->addWidget(showWordByBox);
	mainAutosearchLayout->addStretch();
	
	autosearchWidget->setLayout(mainAutosearchLayout);
	// Creation of autosearch settigns end;
	
	// Creation of ifa settings start:
	nameOnEnglish = new QLineEdit;
	connect(nameOnEnglish,SIGNAL(textChanged(const QString&)),this,SLOT(checkIfaContent()));
	nameOnRussian = new QLineEdit;
	connect(nameOnRussian,SIGNAL(textChanged(const QString&)),this,SLOT(checkIfaContent()));
	descrOnEnglish = new QTextEdit;
	connect(descrOnEnglish,SIGNAL(textChanged()),this,SLOT(checkIfaContent()));
	descrOnEnglish->setMaximumHeight(100);
	descrOnRussian = new QTextEdit;
	connect(descrOnRussian,SIGNAL(textChanged()),this,SLOT(checkIfaContent()));
	descrOnRussian->setMaximumHeight(100);
	programPath = new QLineEdit;
	connect(programPath,SIGNAL(textChanged(const QString&)),this,SLOT(checkIfaContent()));
	iconPath = new QLineEdit;
	programPathBrowser = new QPushButton;
	programPathBrowser->setIcon(QIcon(ICONS_PATH + "open.png"));
	connect(programPathBrowser,SIGNAL(clicked()),this,SLOT(getProgramPath()));
	iconPathBrowser = new QPushButton;
	iconPathBrowser->setIcon(QIcon(ICONS_PATH + "open.png"));
	connect(iconPathBrowser,SIGNAL(clicked()),this,SLOT(getIconPath()));
	addIfa = new QPushButton(tr("Add"));
	connect(addIfa,SIGNAL(clicked()),this,SLOT(add()));
	addIfa->setEnabled(false);
	ifaManager = new QListWidget;
	connect(ifaManager,SIGNAL(currentRowChanged(int)),this,SLOT(setEnableOfRemoveIfa()));
	removeIfa = new QPushButton(tr("Remove"));
	removeIfa->setEnabled(false);
	connect(removeIfa,SIGNAL(clicked()),this,SLOT(removeIFA()));
	nameOnEnglishLabel = new QLabel(tr("Program name (English):"));
	nameOnRussianLabel = new QLabel(tr("Program name (Russian):"));
	descrOnEnglishLabel = new QLabel(tr("Describing (English):"));
	descrOnRussianLabel = new QLabel(tr("Describing (Russian):"));
	programPathLabel = new QLabel(tr("Program path:"));
	iconPathLabel = new QLabel(tr("Icon path:"));
	
	// Group boxes:
	QWidget *addApplicationBox = new QWidget;
	QGridLayout *addApplicationBoxLayout = new QGridLayout;
	addApplicationBoxLayout->addWidget(nameOnEnglishLabel,0,0);
	addApplicationBoxLayout->addWidget(nameOnEnglish,0,1);
	addApplicationBoxLayout->addWidget(nameOnRussianLabel,1,0);
	addApplicationBoxLayout->addWidget(nameOnRussian,1,1);
	addApplicationBoxLayout->addWidget(descrOnEnglishLabel,2,0);
	addApplicationBoxLayout->addWidget(descrOnEnglish,2,1);
	addApplicationBoxLayout->addWidget(descrOnRussianLabel,3,0);
	addApplicationBoxLayout->addWidget(descrOnRussian,3,1);
	addApplicationBoxLayout->addWidget(programPathLabel,4,0);
	addApplicationBoxLayout->addWidget(programPath,4,1);
	addApplicationBoxLayout->addWidget(programPathBrowser,4,2);
	addApplicationBoxLayout->addWidget(iconPathLabel,5,0);
	addApplicationBoxLayout->addWidget(iconPath,5,1);
	addApplicationBoxLayout->addWidget(iconPathBrowser,5,2);
	addApplicationBoxLayout->addWidget(addIfa,6,1);
	addApplicationBox->setLayout(addApplicationBoxLayout);
	ifaWidget->addTab(addApplicationBox,tr("Add IF application"));
	//--------------
	QWidget *manageIfaBox = new QWidget;
	QVBoxLayout *manageIfaBoxLayout = new QVBoxLayout;
	manageIfaBoxLayout->addWidget(ifaManager);
	QHBoxLayout *bottomIfaLayout = new QHBoxLayout;
	bottomIfaLayout->addStretch();
	bottomIfaLayout->addWidget(removeIfa);
	manageIfaBoxLayout->addLayout(bottomIfaLayout);
	manageIfaBox->setLayout(manageIfaBoxLayout);
	ifaWidget->addTab(manageIfaBox,tr("Manage IF applications")); 
	// Creation of ifa settings end;


 	settingsItems->setFixedWidth(settingsItems->sizeHint().width()-100);
	QHBoxLayout *horLayout = new QHBoxLayout;
	horLayout->addWidget(settingsItems);
	horLayout->addWidget(pagesWidget,1);
	                                        
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(applyButton);
	bottomLayout->addWidget(closeButton);
	bottomLayout->addWidget(okButton);                                
	                                    
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(horLayout);
	mainLayout->addLayout(bottomLayout);
	
	setLayout(mainLayout);
	setMinimumSize(510,sizeHint().height());
	setWindowTitle(tr("Settings"));
}

void SettingsDialog::changePage(QListWidgetItem *current,QListWidgetItem *previous)
{
	if ( !current )
		current = previous;
	pagesWidget->setCurrentIndex(settingsItems->row(current));
}

QList<bool> SettingsDialog::getBoolSettings()
{	
	return QList<bool>() 
	<< isUpdateTransDuringEntering->isChecked() 
	<< isUpdatePreviewDuringEntering->isChecked()
	<< isOpenRecentFile->isChecked() 
	<< isOpenWordsInNewTabs->isChecked() 
	<< isHighLightTrans->isChecked() 
	<< isSearchWordsByBegining->isChecked()
	<< isShowWordBySingleClick->isChecked()
	<< isShowMarksInAutoSearch->isChecked()
	<< isShowMarksInTabs->isChecked();
}

QList<int> SettingsDialog::getIntSettings()
{
	return QList<int>() << minimumRecords->value();
}

void SettingsDialog::setSettings(QList<bool>& boolList,QList<int>& intList)
{			
 	isUpdateTransDuringEntering->setChecked(boolList.at(UpdateTransDuringEntering));
	isUpdatePreviewDuringEntering->setChecked(boolList.at(UpdatePreviewDuringEntering));
	isOpenRecentFile->setChecked(boolList.at(OpenRecentFile));
	isOpenWordsInNewTabs->setChecked(boolList.at(OpenWordsInNewTabs));
	isHighLightTrans->setChecked(boolList.at(HighLightTrans));
	isSearchWordsByBegining->setChecked(boolList.at(SearchWordsByBegining));
	isShowMarksInAutoSearch->setChecked(boolList.at(ShowMarksInAutoSearch));
	isShowMarksInTabs->setChecked(boolList.at(ShowMarksInTabs));
	if ( boolList.at(MoveBySingleClick) )
		isShowWordBySingleClick->setChecked(true);
	else
		isShowWordByDoubleClick->setChecked(true);
	
 	minimumRecords->setValue(intList.at(MinimumRecords));
 	
 	// Create ifa list widget
 	QDir ifaDir1,ifaDir2;
 	ifaDir1 = HOME_PATH + "ifa";
 	ifaDir2 = PROGRAM_PATH + "ifa";
 	int count = 0;
 	for ( unsigned int i = 2; i < ifaDir1.count(); i++ )
 	{
 		count++;
 		ifaManager->addItem(ifaDir1[i]);
 	}
 	for ( unsigned int i = 2; i < ifaDir2.count(); i++ )
 	{
 		count++;
 		ifaManager->addItem(ifaDir2[i]);
 	}
 	if ( count == 0 )
 		removeIfa->setEnabled(false);
}

void SettingsDialog::checkIfaContent()
{
	if ( 
		(!nameOnEnglish->text().trimmed().isEmpty() || !nameOnRussian->text().trimmed().isEmpty() ) &&
		(!descrOnEnglish->toPlainText().trimmed().isEmpty() || !descrOnRussian->toPlainText().trimmed().isEmpty() ) &&
		!programPath->text().trimmed().isEmpty()
	)
		addIfa->setEnabled(true);
	else
		addIfa->setEnabled(false);

}

void SettingsDialog::getProgramPath()
{
	QString path = QFileDialog::getOpenFileName(this,tr("Choose a program"),QDir::homePath());
	if ( !path.isEmpty() )
		programPath->setText(path);
}

void SettingsDialog::getIconPath()
{
	QString path = QFileDialog::getOpenFileName(this,tr("Choose a icon"),QDir::homePath());
	if ( !path.isEmpty() )
		iconPath->setText(path);
}

void SettingsDialog::add()
{
	QString fileContent;
	fileContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<app>";
	if ( !nameOnEnglish->text().isEmpty() )
		fileContent += "\n\t<title>" + nameOnEnglish->text() + "</title>";
	if ( !nameOnRussian->text().isEmpty() )
		fileContent += "\n\t<title lang=\"ru\">" + nameOnRussian->text() + "</title>";
	if ( !descrOnEnglish->toPlainText().isEmpty() )
		fileContent += "\n\t<description>" + descrOnEnglish->toPlainText() + "</description>";
	if ( !descrOnRussian->toPlainText().isEmpty() )
		fileContent += "\n\t<description lang=\"ru\">" + descrOnRussian->toPlainText() + "</description>";
	fileContent += "\n\t<path>" + programPath->text() + "</path>";
	if ( !iconPath->text().isEmpty() )
		fileContent += "\n\t<icon>" + iconPath->text() + "</icon>";
	fileContent += "\n</app>"; 
	
	QFile file(programPath->text());
	if ( file.exists() )
	{
		if ( !iconPath->text().isEmpty() ) 
		{
			QFile iconFile(iconPath->text());
			if ( !iconFile.exists() )
			{
				QMessageBox::warning(this,tr("Warning"),tr("The icon with such name don't exists"));
				iconPath->setFocus();
				return;
			}
		}
		QFile xmlFile(HOME_PATH + QString("ifa/%1").arg(QFileInfo(programPath->text()).fileName()) + ".xml");
		
		if ( xmlFile.open(QIODevice::WriteOnly) )
		{
			QTextStream stream(&xmlFile);
			stream << fileContent;
			QMessageBox::warning(this,tr(PROGRAM_NAME),tr("The IF application was added. Next\nstart of the program this\napplication will be in menu."));
			programPath->clear();
			descrOnRussian->clear();
			descrOnEnglish->clear();
			nameOnRussian->clear();
			nameOnEnglish->clear();
			iconPath->clear();
		}
		else
			QMessageBox::warning(this,tr("Warning"),tr("It's imposible to write file"));
		
	}
	else
	{
		QMessageBox::warning(this,tr("Warning"),tr("The program with such name don't exists"));
		programPath->setFocus();
		return;
	}
}

void SettingsDialog::removeIFA()
{
	int r = QMessageBox::warning(this,tr(PROGRAM_NAME),tr("Are you sure?"),QMessageBox::No | QMessageBox::Default,QMessageBox::Yes);
	if ( r == QMessageBox::Yes )
	{
		QString ifaString = ifaManager->currentItem()->text();
		QFile file(PROGRAM_PATH + "ifa/" + ifaString);
		if ( file.exists() )
			file.remove();
		else
			QFile::remove(HOME_PATH + "ifa/" + ifaString);
		ifaManager->setRowHidden(ifaManager->currentRow(),true);
		QMessageBox::information(this,tr(PROGRAM_NAME),tr("The IF application was removed. Next start of program\nthis application won't be in menu"));
	}
	if ( ifaManager->count() == 0 )
		removeIfa->setEnabled(false);
}

void SettingsDialog::setEnableOfRemoveIfa()
{
	removeIfa->setEnabled(true);
}
