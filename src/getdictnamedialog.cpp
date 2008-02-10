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
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QRegExp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include "getdictnamedialog.h"

GetDictName::GetDictName(QWidget *parent) : QDialog(parent)
{
	lineEdit = new QLineEdit;
	connect(lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(checkLineContent(const QString&)));
	label = new QLabel(tr("Enter the name of new dictionary.\nFormat: <name>.<language_from>-<language_to>"));
	okButton = new QPushButton(tr("Ok"));
	okButton->setEnabled(false);
	cancelButton = new QPushButton(tr("Cancel"));
	connect(cancelButton,SIGNAL(clicked()),lineEdit,SLOT(clear()));
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
	connect(okButton,SIGNAL(clicked()),this,SLOT(close()));
	
	QHBoxLayout *boxLayout = new QHBoxLayout;
	boxLayout->addStretch();
	boxLayout->addWidget(okButton);
	boxLayout->addWidget(cancelButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(label);
	mainLayout->addWidget(lineEdit);
	mainLayout->addLayout(boxLayout);
	
	setLayout(mainLayout);
	setWindowTitle(tr("Create new dictionary"));
}

void GetDictName::checkLineContent(const QString& content)
{
	if (content.indexOf(" ") == -1)
		okButton->setEnabled(content.contains(QRegExp(".*\\..*\\-.*")));
}

QString GetDictName::getName()
{
	if (okButton->isEnabled())
		return lineEdit->text().trimmed();
	else
		return QString("");
}
