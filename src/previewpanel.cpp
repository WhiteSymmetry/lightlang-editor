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
#include "previewpanel.h"
#include "global.h"
#include "const.h"

PreviewPanel::PreviewPanel(QString title,QWidget *parent) : QDockWidget(title,parent) 
{
	browser = new QTextBrowser;
	
	QWidget *mainPreviewWidget = new QWidget;	
	QVBoxLayout *mainLayout = new QVBoxLayout;	
	mainLayout->addWidget(browser);
	mainPreviewWidget->setLayout(mainLayout);
	
	setWidget(mainPreviewWidget);
	setWindowTitle(tr("Preview - %1").arg(PROGRAM_NAME));
     setWindowIcon(QIcon(ICONS_PATH + "lle.png"));
 	
	setObjectName("previewdockwidget");
	setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);	
	setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void PreviewPanel::setText(QString str)
{
	str.replace("\\[","<b>").replace("\\]","</b>").replace("\\(","<i>")
	    .replace("\\)","</i>").replace("\\<","<font color=green>").replace("\\>","</font>")
	    .replace("\\{","<br>").replace("\n\\}","<br>").replace(QRegExp("\\\\s.*\\\\s"),QString("<img src=\"%1\">").arg(ICONS_PATH + "sound.png"));
	str.replace(QRegExp("\\_(.*)\\_"),"<u>\\1</u>");
	str.replace(QRegExp("\\@(.*)\\@"),"<u><font color='#0000FF'>\\1</font></u>");
	str.replace("\\<u","<u");
	str.replace("\\</u>","</u>");
	str.replace("\\</font>","</font>");
	browser->setHtml(str);
}

void PreviewPanel::clear()
{
	browser->clear();
}
                                                                          
void PreviewPanel::closeEvent(QCloseEvent*)
{
	hide();
}
