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
#include "bookmarkspanel.h"
#include "global.h"
#include "const.h"

BookmarksPanel::BookmarksPanel(QString title, QWidget *parent) : QDockWidget(title,parent)
{
	setObjectName("BookmarksPanel");
	
	
	listWidget = new QListWidget;
	listWidget->setMovement(QListView::Static);
	connect(listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(setRemoveActionEnable()));
	connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(itemWasClicked(QListWidgetItem*)));
 
	menu = new QMenu(this);
	addBookMarkAction = new QAction(this);
	addBookMarkAction->setText(tr("Add bookmark"));
	addBookMarkAction->setIcon(QIcon(ICONS_PATH + "addbookmark.png"));
	addBookMarkAction->setShortcut(QKeySequence("Ctrl+A"));
	addBookMarkAction->setStatusTip(tr("Add bookmark"));
	connect(addBookMarkAction,SIGNAL(triggered()),this,SIGNAL(addBookMarkSignal()));
	
	removeBookMarkAction = new QAction(this);
	removeBookMarkAction->setText(tr("Remove bookmark"));
	removeBookMarkAction->setIcon(QIcon(ICONS_PATH + "removebookmark.png"));
	removeBookMarkAction->setStatusTip(tr("Remove bookmark"));
	connect(removeBookMarkAction,SIGNAL(triggered()),this,SLOT(removeBookMark()));
  	removeBookMarkAction->setEnabled(false);
	                            
	menu->addAction(addBookMarkAction);
	menu->addAction(removeBookMarkAction);
	
	// Main window:
	mainBookmarksWidget = new QMainWindow;
	mainBookmarksToolBar = new QToolBar;
	mainBookmarksToolBar->addAction(addBookMarkAction);
	mainBookmarksToolBar->addAction(removeBookMarkAction);
	mainBookmarksWidget->addToolBar(mainBookmarksToolBar);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(listWidget);
	tempWidget = new QWidget;
	tempWidget->setLayout(mainLayout);
	mainBookmarksWidget->setCentralWidget(tempWidget);
	//=============
	
	
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

	setWidget(mainBookmarksWidget);
}

void BookmarksPanel::contextMenuEvent(QContextMenuEvent* event)
{
	if ( !listWidget->itemAt(event->x(),event->y()) )
		removeBookMarkAction->setEnabled(false);
	else
		removeBookMarkAction->setEnabled(true);
	menu->move(event->globalX(),event->globalY());
	menu->show();
}

void BookmarksPanel::removeBookMark()
{
	QList<QListWidgetItem*> list = listWidget->selectedItems();
	if ( list.isEmpty() )
		return;
	listWidget->setRowHidden(listWidget->currentRow(),true);
	removeBookMarkAction->setEnabled(false);
}

void BookmarksPanel::addItem(const QString &str)
{
	int isThereSuchBm = -1;
	int i = 0;
	if ( listWidget->count() > 0 )
		while ( isThereSuchBm == -1 )
		{
			if ( i == listWidget->count() )
				break;
			if ( listWidget->item(i)->text() == str && !listWidget->isRowHidden(i) ) 
				isThereSuchBm = i;
			i++;
		}
	if ( isThereSuchBm == -1)
		listWidget->insertItem(0,str);
	else
		listWidget->setCurrentRow(isThereSuchBm);
}

void BookmarksPanel::setAddActionEnable(bool b)
{
	addBookMarkAction->setEnabled(b);
}

void BookmarksPanel::setRemoveActionEnable(bool b)
{
	removeBookMarkAction->setEnabled(b);
}                                

void BookmarksPanel::clear()
{
	listWidget->clear();
}

void BookmarksPanel::itemWasClicked(QListWidgetItem* item)
{
	QString str = item->text();
	emit itemClicked(str,FromBookmarks);
}

QString BookmarksPanel::getBookmarks() 
{
	QString mainString;
	if ( listWidget->count() == 0 )
		return QString(""); 	
	for ( int i = 0; i < listWidget->count(); i++ )
	{
		if ( !listWidget->item(i)->isHidden() )
			mainString+=listWidget->item(i)->text() + "\n";
	}
	return mainString.trimmed();
}

void BookmarksPanel::setBookmarks(const QString bm)
{
	QStringList list = bm.split("\n");
	for ( int i = 0; i < list.count(); i++ )
		listWidget->addItem(list[i]);
}

int BookmarksPanel::count()
{
	return listWidget->count();
}

QString BookmarksPanel::getText(int row)
{
	return listWidget->item(row)->text();
}

void BookmarksPanel::setRowHidden(int row,bool b)
{
	listWidget->setRowHidden(row,b);
}