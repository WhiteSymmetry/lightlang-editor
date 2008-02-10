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
#include "ifareader.h"
#include "global.h"

IfaReader::IfaReader()
{
	isThereTitle = false;
	isThereIconPath = false;
	isThereDescription = false;
	isThereProgramPath = false;
	isThereOptions = false;
	valid = true;
}

bool IfaReader::startElement(const QString& /*namespace*/,const QString& /*localName*/,const QString& qName, const QXmlAttributes& attributes)
{
	if ( qName == "title" )
	{
		if ( attributes.value("lang") == lang  )
			isThereTitle = true;
		else
		if ( attributes.value("lang").isEmpty() && title.isEmpty() )
			isThereTitle = true;
	}
	else
	if ( qName == "description" )
	{
		if ( attributes.value("lang") == lang  )
			isThereDescription = true;
		else
		if ( attributes.value("lang").isEmpty() && description.isEmpty() )
			isThereDescription = true;
	}
	else
	if ( qName == "icon" )
		isThereIconPath = true;
	else
	if ( qName == "path" )
		isThereProgramPath = true;
	else
	if ( qName == "options" )
		isThereOptions = true;
	return true;
}

bool IfaReader::characters(const QString& str)
{		
	if ( isThereTitle )
	{
		title = str;
		isThereTitle = false;
	} 
	else
	if ( isThereDescription )
	{
		description = str;
		isThereDescription = false;
	}
	else
	if ( isThereIconPath )
	{
		iconPath = str;
		isThereIconPath = false;
	}
	else
	if ( isThereProgramPath )
	{
		programPath = str;
		isThereProgramPath = false;
	}
	else
	if ( isThereOptions )
	{
		options = str;
		isThereOptions = false;
	}
	return true;
}

bool IfaReader::fatalError(const QXmlParseException &)
{
	valid = false;
	return false;
}

QString IfaReader::getTitle()
{
	return title.trimmed();	
}

QString IfaReader::getDescription()
{
	return description.trimmed();
}

QString IfaReader::getIconPath()
{
	return iconPath;
}

QString IfaReader::getProgramPath()
{
	return programPath;
}

QString IfaReader::getOptions()
{
	return options;
}

bool IfaReader::isValid()
{
	return valid;
}

void IfaReader::clear()
{
	title.clear();
	description.clear();
	iconPath.clear();
	programPath.clear();
	tempString.clear();
	lang.clear();
	options.clear();
}
