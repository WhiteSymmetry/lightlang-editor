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
#include "historypanel.h"
#include "global.h"

HistoryPanel::HistoryPanel(QString title,QWidget *parent) : QDockWidget(title,parent)
{
	setObjectName("HistoryPanel");
	
	filter = new QLineEdit;
	filter->setEnabled(false);
	connect(filter,SIGNAL(textChanged(const QString&)),this,SLOT(setFilter(const QString&)));
	listWidget = new QListWidget;
	connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(emitMoveSignal(QListWidgetItem*)));
	clearHistory = new QPushButton(tr("Clear history"));
	connect(clearHistory,SIGNAL(clicked()),this,SLOT(disableButton()));
	connect(clearHistory,SIGNAL(clicked()),listWidget,SLOT(clear()));
	
	clearButton = new QPushButton;
	clearButton->setIcon(QIcon(ICONS_PATH + "clear.png"));
	clearButton->setFlat(true);
	clearButton->setEnabled(false);
	connect(clearButton,SIGNAL(clicked()),filter,SLOT(clear()));
	connect(clearButton,SIGNAL(clicked()),filter,SLOT(setFocus()));
	
	QHBoxLayout *topLayout  = new QHBoxLayout;
	topLayout->addWidget(filter);
	topLayout->addWidget(clearButton);
	
	QWidget *mainHistoryWidget = new QWidget;
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(listWidget);
	mainLayout->addWidget(clearHistory);
	mainHistoryWidget->setLayout(mainLayout);
	
	setWidget(mainHistoryWidget);
 	setVisible(false);
}

void HistoryPanel::addItem(QString itemTitle)
{
	listWidget->insertItem(0,itemTitle);
	clearHistory->setEnabled(true);
	filter->setEnabled(true);
	clearButton->setEnabled(true);
}

void HistoryPanel::emitMoveSignal(QListWidgetItem* item)
{
	QString str = item->text();
	emit itemClicked(str,FromHistory);
}

void HistoryPanel::setHistory(const QString& his)
{
	QStringList list = his.split("\n");
	for ( int i = 0; i < list.count(); i++ )
		listWidget->addItem(list[i]);
	if ( list.count() == 0 )
		clearHistory->setEnabled(false);
}

QString HistoryPanel::getHistory()
{
	if ( listWidget->count() == 0 )
		return QString("");
	QString mainString;
	for ( int i = 0; i < listWidget->count(); i++ )
		mainString += listWidget->item(i)->text() + "\n";	
	return mainString.trimmed();
}

void HistoryPanel::clear()
{
	listWidget->clear();
	filter->setEnabled(false);
	clearButton->setEnabled(false);
}

void HistoryPanel::disableButton()
{
	clearHistory->setEnabled(false);
	filter->setEnabled(false);
	clearButton->setEnabled(false);
}

void HistoryPanel::setFilter( const QString& s)
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