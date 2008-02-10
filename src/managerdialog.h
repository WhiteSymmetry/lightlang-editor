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
#ifndef MANAGERDIALOG_H
#define MANAGERDIALOG_H

#include <QDialog>

class QPushButton;
class QTableWidget;
class QHBoxLayout;

class ManagerDialog : public QDialog
{
	Q_OBJECT
	signals:
		void buttonClicked(int action, QString name);
	private slots:
		void addToSl();
		void removeDict();
		void openDict();
		void askChoice();
		void createNew();
		void sort(int);
	public slots:
		void updateDicts();
	private:
		QHBoxLayout *bottomLayout;
		QTableWidget *tableWidget;
		QPushButton *openButton;
		QPushButton *removeButton;
		QPushButton *addButton;
		QPushButton *closeButton;
		QPushButton *askChoiceButton;
		QPushButton *createNewButton;
		
		QString getSelectedName();
		int sortings[3];
		void setAllButtonEnable(bool);
	public:
		ManagerDialog(QWidget *parent = 0);
		void setMode(int mode);
		enum ActionMode { Open = 0, Remove, Add, AskChoice, CreateNew };
};

#endif
