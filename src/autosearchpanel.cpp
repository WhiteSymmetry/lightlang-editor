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
#include "autosearchpanel.h"
#include "global.h"
#include <iostream>
#include "const.h"


AutoSearchPanel::AutoSearchPanel(QString title,QWidget* parent) : QDockWidget(title,parent) 
{
	setObjectName("AutoSearchPanel");
	listWidget = new QListWidget;
	
	mainAutoSearchWidget = new QMainWindow;
	toolBar = new QToolBar;		
	toolBar->setObjectName("AutoSearchToolBar");
		
	autoSearchFilter = new QLineEdit;
	connect(autoSearchFilter,SIGNAL(textChanged(const QString&)),this,SLOT(setAutoSearchFilter(const QString&)));
	autoSearchFilter->setEnabled(false);
	
	clearButton = new QPushButton;
	clearButton->setIcon(QIcon(ICONS_PATH + "clear.png"));
	clearButton->setFlat(true);
	connect(clearButton,SIGNAL(clicked()),autoSearchFilter,SLOT(clear()));
	connect(clearButton,SIGNAL(clicked()),autoSearchFilter,SLOT(setFocus()));
		
	extendedSearch = new QCheckBox(tr("Extended search"));	
	
	QHBoxLayout *topAutoSearchLayout = new QHBoxLayout;
	topAutoSearchLayout->addWidget(autoSearchFilter);
	topAutoSearchLayout->addWidget(clearButton);
	
	QVBoxLayout *autoSearchLayout = new QVBoxLayout;
	autoSearchLayout->addLayout(topAutoSearchLayout);
	autoSearchLayout->addWidget(extendedSearch);
	autoSearchLayout->addWidget(listWidget);
	
	sortUpAction = new QAction(this);
	sortUpAction->setText(tr("Sorting up"));
	sortUpAction->setIcon(QIcon(ICONS_PATH + "sortup.png"));
	connect(sortUpAction,SIGNAL(triggered()),this,SLOT(autoSearchSortUp()));
	sortUpAction->setStatusTip(tr("Sort all items up"));
	sortUpAction->setEnabled(false);
	
	sortDownAction = new QAction(this);
	sortDownAction->setText(tr("Sorting down"));
	sortDownAction->setIcon(QIcon(ICONS_PATH + "sortdown.png"));
	connect(sortDownAction,SIGNAL(triggered()),this,SLOT(autoSearchSortDown()));
	sortDownAction->setStatusTip(tr("Sort all items down"));
	sortDownAction->setEnabled(false);
	
	toolBar->addAction(sortUpAction);
	toolBar->addAction(sortDownAction);
	mainAutoSearchWidget->addToolBar(toolBar);
	
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.beginGroup("General");
	extendedSearch->setChecked(settings.value("ExtendedSearch",false).toBool());
	mainAutoSearchWidget->restoreState(settings.value("AutoSearchState").toByteArray()); 
	settings.endGroup();
	
     setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea); 
     QWidget *tempWidget = new QWidget;
     tempWidget->setLayout(autoSearchLayout);
	mainAutoSearchWidget->setCentralWidget(tempWidget);	
	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	setWidget(mainAutoSearchWidget);
	
}

QByteArray AutoSearchPanel::getMainWidgetState()
{
	return mainAutoSearchWidget->saveState();
}

void AutoSearchPanel::toEmitMoveSignal()
{
	QListWidgetItem* selectedItem = listWidget->selectedItems().first();
	QString str = selectedItem->text();
	emit signalToMove(str,FromAutoSearch);
}

void AutoSearchPanel::autoSearchSortDown()
{
	listWidget->sortItems(Qt::DescendingOrder);
	sortDownAction->setEnabled(false);
	sortUpAction->setEnabled(true);
}

void AutoSearchPanel::autoSearchSortUp()
{
	listWidget->sortItems(Qt::AscendingOrder);
	sortDownAction->setEnabled(true);
	sortUpAction->setEnabled(false);
}

void AutoSearchPanel::setAutoSearchFilter(const QString &s)
{
	QString str = s;
	str = str.simplified().trimmed();
	if ( !str.isEmpty() )
		for ( int i = 0; i < listWidget->count(); i++ )
			if ( listWidget->item(i)->text().startsWith(str,Qt::CaseInsensitive) )
			{
				listWidget->setCurrentRow(i);
				return;
			}
	else
		listWidget->setCurrentRow(0);
}

bool AutoSearchPanel::getExtendedSearchStatus()
{
	return extendedSearch->isChecked();
}

void AutoSearchPanel::setEnableOfFilter(bool b)
{
	autoSearchFilter->setEnabled(b);
	clearButton->setEnabled(b);
}

void AutoSearchPanel::setEnableActions(bool b)
{
	sortUpAction->setEnabled(false);
	sortDownAction->setEnabled(b);
}

void AutoSearchPanel::clear()
{
	listWidget->clear();
	autoSearchFilter->clear();
	setEnableOfFilter(false);
	sortUpAction->setEnabled(false);
	sortDownAction->setEnabled(false);
}

void AutoSearchPanel::addItem(QSqlRecord *record,bool showStatus)
{
	QListWidgetItem *item;
	if ( showStatus ) 
		item = new QListWidgetItem(QIcon(ICONS_PATH + (record->value(2).toInt() == 1 ? "was_edited" : "was_not_edited") + ".png"),record->value(0).toString());
	else
		item = new QListWidgetItem(record->value(0).toString());
	listWidget->addItem(item);
	setEnableOfFilter(true);
	sortUpAction->setEnabled(false);
	sortDownAction->setEnabled(true);
}

void AutoSearchPanel::activeSettings()
{
	if ( isMoveBySingleClick )
	{
		connect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(toEmitMoveSignal()));
		disconnect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(toEmitMoveSignal()));
	}
	else
	{
		disconnect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(toEmitMoveSignal()));
		connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(toEmitMoveSignal()));
	}
}
