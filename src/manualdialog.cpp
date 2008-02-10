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
#include "manualdialog.h"
#include "global.h"
#include "const.h"

const QString PATH_TO_DOC = PROGRAM_PATH + "doc/" + lang + "/html/";

ManualDialog::ManualDialog(QWidget *parent) : QDialog(parent)
{
	headerLabel = new QLabel;
	headerLabel->setText("<center><b><font size=\'4\'>" + tr("LightLang Editor\'s manual") + "</font></b></center>");
	// Create push buttons
	// backward button
	backwardButton = new QPushButton;
	backwardButton->setIcon(QIcon(ICONS_PATH + "backward.png"));
	backwardButton->setIconSize(QSize(22,22));
	backwardButton->setFlat(true);
	backwardButton->setEnabled(false);
	backwardButton->adjustSize();
	backwardButton->setFixedSize(backwardButton->size());
	// Next button
	forwardButton = new QPushButton;
	forwardButton->setIcon(QIcon(ICONS_PATH + "forward.png"));
	forwardButton->setIconSize(QSize(22,22));
	forwardButton->setFlat(true);
	forwardButton->setEnabled(false);
	forwardButton->adjustSize();
	forwardButton->setFixedSize(forwardButton->size());
	// Close button
	close = new QPushButton;
	close->setShortcut(QKeySequence("Esc"));
	close->setText(tr("Close"));
	//=========================
	// Create list widget
	listWidget = new QListWidget;
	listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	connect(listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(changePage(int)));
	// Create text browser
	browser = new QTextBrowser;
	
	splitter = new QSplitter;
	splitter->insertWidget(0,listWidget);
	splitter->insertWidget(1,browser);
	splitter->setChildrenCollapsible(false);
	splitter->setStretchFactor(1,1);
	
	QSettings settings(HOME_PATH + "settings.conf",QSettings::NativeFormat,0);
	settings.beginGroup("GeneralSettings");
	splitter->restoreState(settings.value("HelpState").toByteArray());
	settings.endGroup();
	
	addItem(tr("About program LightLang Editor"),"about.html");
	addItem(tr("Tags and dictionary's format"),"tags.html");
	addItem(tr("How to use the program"),"howtouse.html");
	addItem(tr("Interaction of Editor and SL"),"interaction.html");
	addItem(tr("Integradable friend applications"),"ifa.html");
	addItem(tr("Bugs and offers"),"bugs.html");
	addItem(tr("Authors and thanks"),"authors.html");
	addItem(tr("Internet links"),"links.html");
	addItem(tr("List of changes"),"changelog.html");
	addItem(tr("License"),"license.html");
	listWidget->setCurrentRow(0);
	
	// Create connections 
	connect(close,SIGNAL(clicked()),this,SLOT(hide()));
	connect(backwardButton,SIGNAL(clicked()),this,SLOT(backward()));
	connect(forwardButton,SIGNAL(clicked()),this,SLOT(forward()));
	connect(browser,SIGNAL(backwardAvailable(bool)),backwardButton,SLOT(setEnabled(bool)));
	connect(browser,SIGNAL(forwardAvailable(bool)),forwardButton,SLOT(setEnabled(bool)));
	connect(browser,SIGNAL(anchorClicked(const QUrl&)),this,SLOT(changePage(const QUrl&)));
	
	// Create top Layout with home, backward and forward buttons
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(backwardButton);
	topLayout->addWidget(forwardButton);
	topLayout->addWidget(headerLabel,1);
	
	// Create bottom layout with close button
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch(1);
	bottomLayout->addWidget(close);
	
	//Create Main Layout
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(splitter,1);
	mainLayout->addLayout(bottomLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Manual of %1").arg(PROGRAM_NAME));
	setWindowIcon(QIcon(ICONS_PATH + "lle.png"));
	resize(900,700);
}

QByteArray ManualDialog::getState()
{
	return splitter->saveState();
}

void ManualDialog::addItem(const QString title,const QString url)
{
	QListWidgetItem *item = new QListWidgetItem;
	item->setData(1,url);
	item->setText(title);
	listWidget->addItem(item);
}

void ManualDialog::changePage(int index)
{
	browser->setSource(QUrl(PATH_TO_DOC + listWidget->item(index)->data(1).toString()));
}

void ManualDialog::changePage(const QUrl& url)
{
	QString name = QFileInfo(url.toString()).fileName();
	for (int i = 0; i < listWidget->count(); i++)
		if (name == listWidget->item(i)->data(1).toString())
		{
			listWidget->blockSignals(true);
			listWidget->setCurrentRow(i);
			listWidget->blockSignals(false);
		}
}

void ManualDialog::backward()
{
	browser->backward();
	QString name = QFileInfo(browser->source().toString()).fileName();
	for ( int i = 0; i < listWidget->count(); i++ )
		if ( name == listWidget->item(i)->data(1).toString() )
		{
			listWidget->blockSignals(true);
			listWidget->setCurrentRow(i);
			listWidget->blockSignals(false);
		}
}

void ManualDialog::forward()
{
	browser->forward();
	QString name = QFileInfo(browser->source().toString()).fileName();
	for ( int i = 0; i < listWidget->count(); i++ )
		if ( name == listWidget->item(i)->data(1).toString() )
		{
			listWidget->blockSignals(true);
			listWidget->setCurrentRow(i);
			listWidget->blockSignals(false);
		}
}
