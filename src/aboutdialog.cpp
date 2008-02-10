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
#include <QtGui>
#include <iostream>
#include "global.h"
#include "const.h"
#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{		
	// Create Background
	QPalette palette;
	pixmap = new QPixmap(PROGRAM_PATH + "pictures/splash.png");
	palette.setBrush(QPalette::Text,QBrush(QColor(Qt::white)));
	palette.setBrush(QPalette::Base,QBrush(QColor("#487BBA")));
	palette.setBrush(backgroundRole(),QBrush(*pixmap));
	
 	setPalette(palette); 
	//==================
	
     setFixedSize(pixmap->size());
     setWindowTitle(tr("About ")+tr(PROGRAM_NAME) );
     setWindowIcon(QIcon(ICONS_PATH + "lle.png"));
}

void AboutDialog::paintEvent(QPaintEvent* /*event*/)
{
	QPainter painter(this);	
	//PaintMainText	
	painter.setPen(QColor("#4d84e8"));
	if ( lang == "ru" )
		painter.setFont(QFont("Sans Serif",10,QFont::Bold,true));
	else
		painter.setFont(QFont("Sans Serif",12,QFont::Bold,true));
	painter.drawText(90,100,tr(PROGRAM_NAME) + tr(" - the program for editing and"));	
	painter.drawText(100,120,tr("creation of special dictionaries for LightLang"));
	
	// Paint word "License"	
	painter.setPen(QColor("#f1b84e"));
	painter.setFont(QFont("Sans Serif",14,QFont::Bold,true));
	painter.drawText(150,50,tr("License: GPL v2"));
	
	// Paint author
	painter.setPen(QColor("#4d84e8"));
	painter.setFont(QFont("Sans Serif",12,QFont::Bold,true));
	painter.drawText(150,150,tr("Developer: Tikhonov Sergey"));
	
	// Paint Helped
	painter.setPen(QColor("#4d84e8"));
	painter.setFont(QFont("Sans Serif",12,QFont::Bold,true));
	painter.drawText(150,190,tr("Helper: Devaev Maxim"));

}

void AboutDialog::closeEvent(QCloseEvent*)
{
	hide();
}

void AboutDialog::mousePressEvent(QMouseEvent*)
{
// 	hide();
}

