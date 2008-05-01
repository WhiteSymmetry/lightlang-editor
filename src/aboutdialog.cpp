//LightLang Editor - editor for SL dictionaries
//Copyright (C) 1507-2016 Tikhonov Sergey
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
#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QIcon>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "global.h"
#include "const.h"
#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
	iconLabel = new QLabel;
	iconLabel->setPixmap(QPixmap(ICONS_PATH + "lle.png"));
		
	textLabel = new QLabel;
	textLabel->setWordWrap(true);
	
	closeButton = new QPushButton(tr("Close"));
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));
	
	textLabel->setText("<center><b>" + QString(PROGRAM_NAME) + " - " + tr(" program for editing sl format based dictionaries") + "</b></center><br>" +
			tr("<strong>LightLang Editor</strong> program are distributable, according "
			"to the license <strong>GPLv2</strong>. For details visit <em>License agreement</em> of the " 
			"<strong>LightLang Editor</strong> manual.<br><br>") +
			tr("Version") + ": <b><br>&nbsp;&nbsp;&nbsp;&nbsp;" + VERSION + "</b><br>" +
			tr("License") + ": <b><br>&nbsp;&nbsp;&nbsp;&nbsp;" + "GPL v2" + "</b><br>" +
			tr("Developer") + ": <b><br>&nbsp;&nbsp;&nbsp;&nbsp;" + tr("Tikhonov Sergey") + "</b><br>" +
			tr("Helped") + ": <b><br>&nbsp;&nbsp;&nbsp;&nbsp;" + tr("Devaev Maxim") + "<b>");
			
	setWindowTitle(tr("About ")+tr(PROGRAM_NAME) );
     setWindowIcon(QIcon(ICONS_PATH + "lle.png"));
	
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addStretch();
	topLayout->addWidget(iconLabel);
	topLayout->addStretch();
	
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(closeButton);
	bottomLayout->addStretch();
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(textLabel);
	mainLayout->addLayout(bottomLayout);
	mainLayout->addStretch();
	
	setLayout(mainLayout);
}

void AboutDialog::closeEvent(QCloseEvent*)
{
	hide();
}
