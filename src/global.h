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
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

extern QString PREFIX;
extern QString PROGRAM_PATH;
extern QString HOME_PATH;
extern QString ICONS_PATH;
extern QString PICTURES_PATH;
extern QString SETTINGS_ICONS_PATH;
extern QString MAIN_ICON;
extern QString BOOKMARKS_PATH;
extern QString DATABASES_PATH;
extern QString ABOUTS_PATH;
extern QString HISTORIES_PATH;
extern QString IFA_PATH;
extern QString lang;
extern QString TransDir;

enum Settings
		{ // Bool settings:
			UpdateTransDuringEntering = 0, 
			UpdatePreviewDuringEntering, 
			OpenRecentFile, 
			OpenWordsInNewTabs, 
			ShowSplashScreen, 
			ShowAutoSearch, 
			ShowBookmarks, 
			ShowHistory, 
			ShowPreviewApart,
			HighLightTrans, 
			SearchWordsByBegining, 
			MoveBySingleClick, 
			ShowMarksInAutoSearch, 
			ShowMarksInTabs,
			// Int settings:
			MinimumRecords=0
		};
enum ManagersModes 
		{ 
			DontCloseManagerAvaible, 
			CloseManagerAvaible 
		};

enum InputSituations 
		{ 
			FromMainLine = 0, 
			FromBookmarks, 
			FromHistory, 
			FromAutoSearch 
		};

#endif
