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
#ifndef IFAREADER_H
#define IFAREADER_H

#include <QtXml>
#include <QString>

class IfaReader : public QXmlDefaultHandler
{
	private:
		QString title;
		QString description;
		QString iconPath;
		QString programPath;
		QString tempString;
		QString lang;
		QString options;
		bool isThereTitle;
		bool isThereIconPath;
		bool isThereDescription;
		bool isThereProgramPath;
		bool isThereOptions;
		bool valid;
		
		bool startElement(const QString&, const QString&, const QString&, const QXmlAttributes&);
		bool characters(const QString&);
		bool fatalError(const QXmlParseException&);
	public:
		IfaReader();
		QString getTitle();
		QString getDescription();
		QString getIconPath();
		QString getProgramPath();
		QString getOptions();
		bool isValid();
		void clear();
};

#endif