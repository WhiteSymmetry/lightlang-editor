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
#include "aboutdictdialog.h"

AboutDictDialog::AboutDictDialog(QWidget *parent) : QDialog(parent)
{
	// options of window
	setWindowTitle(tr("Edit information"));
	
	// Create widgets
	textEdit = new QTextEdit;
	connect(textEdit,SIGNAL(textChanged()),this,SLOT(actionOnTextChanged()));
	saveButton = new QPushButton(tr("Save"));
	saveButton->setEnabled(false);
	connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));
	closeButton = new QPushButton(tr("Close"));
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));
	
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(saveButton);
	bottomLayout->addWidget(closeButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(textEdit);
	mainLayout->addLayout(bottomLayout);
	
	setLayout(mainLayout);
}

void AboutDictDialog::save()
{
	saveButton->setEnabled(false);
	emit saveButtonClicked();
}

void AboutDictDialog::actionOnTextChanged()
{
	saveButton->setEnabled(true);
}

void AboutDictDialog::setAbout(const QString str)
{
	textEdit->setText(str);
	saveButton->setEnabled(false);
}

QString AboutDictDialog::getAbout()
{
	return textEdit->toPlainText();
}

void AboutDictDialog::closeEvent(QCloseEvent*)
{
	hide();
}

void AboutDictDialog::clear()
{
	textEdit->clear();
}

void AboutDictDialog::showEvent(QShowEvent*)
{
	textEdit->setFocus();	
}
