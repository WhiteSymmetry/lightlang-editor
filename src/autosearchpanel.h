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
#ifndef AUTOSEARCHPANEL_H
#define AUTOSEARCHPANEL_H

#include <QDockWidget>
#include <QSqlRecord>

class QMenu;
class QAction;
class QListWidget;
class QLineEdit;
class QLabel;
class QCheckBox;
class QPushButton;
class QToolBar;
class QMainWindow;

class AutoSearchPanel : public QDockWidget
{
	Q_OBJECT
	private slots:
		void toEmitMoveSignal();
		void autoSearchSortDown();
		void autoSearchSortUp();
		void setAutoSearchFilter(const QString&);
	signals:
		void signalToMove(QString&,int);
	private:
		QListWidget *listWidget;
		QLineEdit *autoSearchFilter;
		QCheckBox *extendedSearch;
		QPushButton *clearButton;
		QAction *sortUpAction;
		QAction *sortDownAction;
		QToolBar *toolBar;
		QMainWindow *mainAutoSearchWidget;
		bool isMoveBySingleClick;
	public:
		AutoSearchPanel(QString title,QWidget* parent = 0);
		QByteArray getMainWidgetState();
		bool getExtendedSearchStatus();
		void setEnableOfFilter(bool);
		void setEnableActions(bool);
		void addItem(QSqlRecord *record,bool);
		void clear();
		void setMoveBySingleClick(bool b)
		{
			isMoveBySingleClick = b;
		}
		void activeSettings();
};

#endif
