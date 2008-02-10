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
#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QDir>
#include <iostream>
#include "mainwindow.h"
using namespace std;

int main(int argc, char** argv)
{
	QString fileName;
	if (argc == 2)
	{
		fileName = argv[1];
		fileName = fileName.trimmed();
		if ( fileName[0] != '/' )
		{
			QRegExp exp(".*\\..*\\-.*");
			if (!fileName.contains(exp))
			{
				cout << "Error: format of file isn't format of sl dictionaries.\n";
				return 1;
			}
			fileName = QDir::currentPath() + '/' + fileName;
		}
		else
		{
			QString name = QFileInfo(fileName).fileName();
			QRegExp exp(".*\\..*\\-.*");
			if (!name.contains(exp))
			{
				cout << "Error: format of file isn't format of sl dictionaries.\n";
				return 1;
			}
		}	
			
		if (!QFile::exists(fileName))
		{
			cout << "File with such name doesn't exist.\n";
			exit(0);
		}
	}
	
	QApplication app(argc,argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));	

	MainWindow mainWindow(fileName);
	mainWindow.show();
		
	return app.exec();
}


