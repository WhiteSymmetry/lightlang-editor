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
#include "spin.h"

Spin::Spin()
{
	currentPosition=0.0F;
	timer.setInterval(5);
	connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
}

Spin::Spin(int w,int h)
{
	currentPosition=0.0F;
	timer.setInterval(5);
	connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
	show();
	start();
	setFixedSize(QSize(w,h));
}

void Spin::paintEvent(QPaintEvent* /*event*/)
{
	QPainter painter(this);
	currentPosition+=0.5F;
	if ( currentPosition == 360.0 )
		currentPosition = 0;
	QConicalGradient radialGrad(QPointF(int(width/2), int(height/2)), currentPosition);
     radialGrad.setColorAt(0,Qt::white);
     radialGrad.setColorAt(0.6, Qt::black);
     
     painter.setBrush(radialGrad);
	painter.drawEllipse(0,0,20,20);
}

void Spin::setSize(int w, int h)
{
	width = w;
	height = h;
	setFixedSize(QSize(width,height));
}

void Spin::start()
{
	timer.start();
}

void Spin::stop()
{
	timer.stop();
}
