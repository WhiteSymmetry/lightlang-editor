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
#include <QDir>
#include <QString>
#include <QLocale>
#include "global.h"

QString PREFIX = "/usr";

QString PROGRAM_PATH = PREFIX + "/lib/lileditor/";
QString HOME_PATH = QDir::homePath() + "/.lileditor/";
QString ICONS_PATH = PROGRAM_PATH + "icons/";
QString PICTURES_PATH = PROGRAM_PATH + "pictures/";
QString MAIN_ICON = ICONS_PATH + "lle.png";
QString BOOKMARKS_PATH = HOME_PATH + "bookmarks/";
QString DATABASES_PATH = HOME_PATH + "databases/";
QString ABOUTS_PATH = HOME_PATH + "abouts/";
QString HISTORIES_PATH = HOME_PATH + "histories/";
QString IFA_PATH = HOME_PATH + "ifa/";
QString SETTINGS_ICONS_PATH = ICONS_PATH + "settings/";

QString lang = QLocale().name().remove(QLocale().name().indexOf("_"),QLocale().name().length()).isEmpty() ? "en" :
		 QLocale().name().remove(QLocale().name().indexOf("_"),QLocale().name().length());
QString TransDir = PROGRAM_PATH + "trans/";