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
#ifndef HISTORYPANEL_H
#define HISTORYPANEL_H

#include <QDockWidget>

class QListWidget;
class QPushButton;
class QListWidgetItem;
class QLineEdit;

class HistoryPanel : public QDockWidget
{
	Q_OBJECT
	signals:
		void itemClicked(QString& str,int);
	private slots:
		void emitMoveSignal(QListWidgetItem*);
		void disableButton();
		void setFilter(const QString&);
	private:
		QListWidget *listWidget;
		QPushButton *clearHistory;
		QLineEdit *filter;
		QPushButton *clearButton;
	public:
		HistoryPanel(QString title,QWidget *parent = 0);
		void addItem(QString itemTitle);
		void setHistory(const QString& his);
		QString getHistory();
		void clear();
};

#endif
