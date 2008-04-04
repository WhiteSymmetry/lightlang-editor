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
#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "global.h"

class QMainWindow;
class QLineEdit;
class QPushButton;
class QToolButton;
class QToolBar;
class HighLighter;
class QTextBrowser;
class QDockWidget;
class QListWidgetItem;
class AutoSearchPanel;
class BookmarksPanel;
class PreviewPanel;
class HistoryPanel;
class QTabWidget;
class AboutDictDialog;
class QLabel;
class QTimer;

class CentralWidget : public QWidget
{
	Q_OBJECT
	protected:
		void mouseDoubleClickEvent(QMouseEvent *event);
	private:
		QList<bool> boolSets;
		QList<int> intSets;
		QLabel *mainLabel;
		QMainWindow *localTextEditMainWidget;
		QMainWindow *localOutSideMainWidget;
		QLineEdit *mainLine;
		QPushButton *clearButton;
		
				
		QToolButton *addTabButton;
		QToolButton *removeTabButton;
		
		QList<bool> undoList;
		QList<bool> redoList;
		QList<QTextEdit*> textEdits;
		QList<HighLighter*> highLighters;
		
		QTextEdit *currentTextEdit;
		QTabWidget *tabWidget;
		bool welcomePage;
		QToolBar *pasteToolBar;
		QToolBar *recordsToolBar;
		QToolBar *specialToolBar;
		
		QSqlDatabase db;
		QSqlQuery query;
		
		QTimer *timer;
		QLabel *warning;
		
		PreviewPanel *previewPanel;		
		AutoSearchPanel *autoSearchPanel;
		HistoryPanel *historyPanel;
		BookmarksPanel *bookmarksPanel;
		
		QString currentAbout;
		AboutDictDialog *aboutDictDialog;
		
		QPushButton *searchButton;
		
		QAction *pasteBoldAction;
		QAction *pasteItalicAction;
		QAction *pasteUnderlineAction;
		QAction *pasteSpecialAction;
		QAction *pasteLinkAction;
		QAction *pasteBlockAction;
		QAction *pasteSoundAction;
		QAction* previewAction;
		QAction *undoAction;
		QAction *redoAction;
		QAction *zoomInAction;
		QAction *zoomOutAction;
		QAction *changeStatusAction;
		QAction *searchAction;
		QAction *addAction;
		QAction *removeAction;
		QAction *editAction;
		QAction *aboutDictAction;
		
		void setRecord(QSqlRecord* record);
		char* getColorByEnum(const int) const;
		int getIndexOfEmptyPage();
		void setEnablePasteActions(bool);
	public:
		CentralWidget();
		void setDatabase(QSqlDatabase& db,QSqlQuery& query);
		void clear();
		void clearAll();
		QString getBookmarks();
		QString getHistory();
		QByteArray getPreviewGeometry();
		QByteArray getLocalMainWidgetState();
		QByteArray getAutoSearchState();
		QString& getAboutDict();
		void setHistory(const QString history);
		void setBookmarks(const QString bookmarks);
		bool getExtendedSearchStatus();
		void setAboutDict(const QString& str);
		void setSettings(QList<bool>& boolean_settings,QList<int>& int_settings);
		QAction* getAction(int what_from_public_actions);
		void showWelcomePage();	
				
		enum Message_statuses { Bad, Good  };	
		enum Public_Actions { SearchAction, AddAction, RemoveAction, EditAction, AboutDictAction };
	private slots:
		void pasteBold();
		void pasteItalic();
		void pasteUnderline();
		void pasteSpecial();
		void pasteLink();
		void pasteBlock();
		void pasteSound();
		
		void editRecord();
		void addRecord();
		void removeRecord();
		void changeStatus();
		void hideWarning();
		void moveByItem(QString& word,int from = FromMainLine);
		void find(int from = FromMainLine);
		void updateProgram();
		void setPreview();
		void undoEnabled(bool);
		void redoEnabled(bool);
		void addBookMark();
		void indexChanged(int);
		void changeAboutDict();
		void showAboutDict();
		
		void addTab();
		void removeTab(int index = -1 );
		void redo();
		void undo();
		void zoomIn();
		void zoomOut();
	public slots:
		void checkMainLineContent(int from = FromMainLine);
		void showMessage(int mode,QString text);
	
};


#endif
