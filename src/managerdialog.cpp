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
#include "managerdialog.h"
#include "global.h"

ManagerDialog::ManagerDialog(QWidget *parent) : QDialog(parent)
{
	for ( int i = 0; i < 3; i++ )
		sortings[i] = 0;
	tableWidget = new QTableWidget;
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
   	tableWidget->setColumnCount(3);
     tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
     tableWidget->horizontalHeader()->setStretchLastSection(true);
     
     QHeaderView *allHeaders = tableWidget->horizontalHeader();
     connect(allHeaders,SIGNAL(sectionClicked(int)),this,SLOT(sort(int)));

	openButton = new QPushButton(QIcon(ICONS_PATH + "open_dict.png"),tr("Open"));
	connect(openButton,SIGNAL(clicked()),this,SLOT(openDict()));
	removeButton = new QPushButton(QIcon(ICONS_PATH + "remove_dict.png"),tr("Remove"));
	connect(removeButton,SIGNAL(clicked()),this,SLOT(removeDict()));
	addButton = new QPushButton(QIcon(ICONS_PATH + "add_dict.png"),tr("Add to SL"));
	connect(addButton,SIGNAL(clicked()),this,SLOT(addToSl()));
	closeButton = new QPushButton(tr("Close"));
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));
	createNewButton = new QPushButton(QIcon(ICONS_PATH + "new_dict.png"),tr("New"));
	connect(createNewButton,SIGNAL(clicked()),this,SLOT(createNew()));
	askChoiceButton = new QPushButton(tr("Back to dialog of choice"));
	connect(askChoiceButton,SIGNAL(clicked()),this,SLOT(askChoice()));
	askChoiceButton->hide();
	
	bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(createNewButton);
	bottomLayout->addWidget(openButton);
	bottomLayout->addWidget(addButton);
	bottomLayout->addWidget(removeButton);
	bottomLayout->addWidget(closeButton);
	bottomLayout->addWidget(askChoiceButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tableWidget);
	mainLayout->addLayout(bottomLayout);
	
	setWindowTitle(tr("Manager of loaded dictionaries"));
	setLayout(mainLayout);
	setMinimumSize(400,300);
	updateDicts();
}

void ManagerDialog::updateDicts()
{	
	tableWidget->clear();
	tableWidget->setRowCount(0);
	QStringList labels;
	labels << tr("Name") << tr("Size,kb") << tr("Directive");
     tableWidget->setHorizontalHeaderLabels(labels);	
	
	bool areThereLoadedDicts = false;
	QDir databaseDir;
	databaseDir = HOME_PATH + "databases";
	QTableWidgetItem *nameItem;
	QTableWidgetItem *sizeItem;
	QTableWidgetItem *dirItem;
	QStringList list;
	for ( unsigned int i = 2; i < databaseDir.count(); i++ )
	{
		list = databaseDir[i].split(".");
		if (list.count() <= 1)
		{
			QMessageBox::warning(this,tr("Warning"),tr("Some loaded dicionaries has invalid format"));
			break;
		}
		nameItem = new QTableWidgetItem;
		nameItem->setText(list[0]);
		sizeItem = new QTableWidgetItem;
		sizeItem->setText(QString::number(int(QFileInfo(databaseDir[i]).size()*0.58/1000)));
		dirItem = new QTableWidgetItem;
		dirItem->setText(list[1]);
		tableWidget->insertRow(i-2);
		tableWidget->setItem(i-2,0,nameItem);
		tableWidget->setItem(i-2,1,sizeItem);
		tableWidget->setItem(i-2,2,dirItem);
		areThereLoadedDicts = true;
	}
	if ( areThereLoadedDicts )
	{
		tableWidget->setColumnCount(3);
		tableWidget->selectRow(0);
    	 	setAllButtonEnable(true);
	}
	else
	{
		tableWidget->setColumnCount(1);
		tableWidget->setRowCount(1);
		QStringList label;
		label << "Empty";
    	 	tableWidget->setHorizontalHeaderLabels(label);
    	 	QTableWidgetItem *item = new QTableWidgetItem;
    	 	item->setText(tr("There are no loaded dictionaries"));
    	 	tableWidget->setItem(0,0,item);
    	 	setAllButtonEnable(false);
    	}	
    	tableWidget->resizeColumnsToContents();
}

QString ManagerDialog::getSelectedName()
{
	QList<QTableWidgetItem *> list = tableWidget->selectedItems();
	QString name = list[0]->text() + "." + list[2]->text();
	return name;
}

void ManagerDialog::addToSl()
{
	emit buttonClicked(Add,getSelectedName());
}

void ManagerDialog::removeDict()
{
	hide();
	emit buttonClicked(Remove,getSelectedName());
}

void ManagerDialog::openDict()
{	
	hide();
	emit buttonClicked(Open,getSelectedName());
}

void ManagerDialog::createNew()
{
	hide();
	emit buttonClicked(CreateNew,"");
}

void ManagerDialog::askChoice()
{
	hide();
	emit buttonClicked(AskChoice,QString(""));
}

void ManagerDialog::setAllButtonEnable(bool b)
{
	removeButton->setEnabled(b);
	addButton->setEnabled(b);
	openButton->setEnabled(b);
}

void ManagerDialog::setMode(int mode)
{
	if ( mode == DontCloseManagerAvaible )
	{
		closeButton->hide();
		askChoiceButton->show();
	}
	else
	{
		askChoiceButton->hide();
		closeButton->show();
	}
}

void ManagerDialog::sort(int section)
{
	if ( sortings[section] == 1)
	{
		tableWidget->sortItems(section,Qt::AscendingOrder);
		sortings[section] = 0;
	}
	else
	{
		tableWidget->sortItems(section,Qt::DescendingOrder);
		sortings[section] = 1;
	}
}

