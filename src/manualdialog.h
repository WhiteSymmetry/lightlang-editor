//LightLang Editor - editor for SL dictionaries
//Copyright (C) 2007-2016  Tikhonov Sergey
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
#ifndef MANUALDIALOG_H
#define MANUALDIALOG_H

#include <QDialog>
#include <QUrl>

class QPushButton;
class QTextBrowser;
class QListWidget;
class QSplitter;
class QLabel;

class ManualDialog : public QDialog
{
	Q_OBJECT
	private slots:
		void changePage(int);
		void changePage(const QUrl&);
		void backward();
		void forward();
	private:
		QPushButton *backwardButton;
		QPushButton *forwardButton;
		QTextBrowser *browser;
		QPushButton *close;
		QListWidget *listWidget;
		QSplitter *splitter;
		QLabel *headerLabel;
		
		void addItem(const QString title,const QString url);
	public:
		ManualDialog(QWidget* parent = 0);
		QByteArray getState();
};

#endif
