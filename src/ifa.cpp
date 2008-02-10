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
#include <QMenu>
#include <iostream>
#include "ifa.h"
#include "ifareader.h"

Ifa::Ifa()
{
	ifaReader = new IfaReader;
}

void Ifa::addAction(QString path,QMenu *menu)
{
	QFile file(path);
	QXmlInputSource inputSource(&file);
	QXmlSimpleReader reader;
	reader.setContentHandler(ifaReader);
	reader.setErrorHandler(ifaReader);
	reader.parse(inputSource);
	
	if (ifaReader->isValid())
	{
		if (ifaReader->getTitle().isEmpty())
		{
			std::cout << "Ifa: " << qPrintable(tr("there is not title in ")) << qPrintable(path) << '\n';
			return;
		}
		if (ifaReader->getProgramPath().isEmpty())
			std::cout << "Ifa: " << qPrintable(tr("there is not program path in ")) << qPrintable(path) << " - action is disabled\n";
		action = new QAction(menu);
		action->setText(ifaReader->getTitle());
		action->setData(ifaReader->getProgramPath() + " " + ifaReader->getOptions().simplified());
		action->setIcon(QIcon(ifaReader->getIconPath()));
		action->setEnabled(QFile(ifaReader->getProgramPath()).exists());
		action->setStatusTip(ifaReader->getDescription()); 
		menu->addAction(action);
		ifaReader->clear();
	}
	else
		std::cout << "Ifa: " << qPrintable(tr("it's immpossible to parse the xml file: ")) << qPrintable(path) << '\n';
}
