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
#include <QtSql>
#include "centralwidget.h"
#include "autosearchpanel.h"
#include "highlighter.h"
#include "previewpanel.h"
#include "bookmarkspanel.h"
#include "historypanel.h"
#include "const.h"
#include "global.h"
#include "aboutdictdialog.h"


CentralWidget::CentralWidget()
{
	currentTextEdit = 0;
	welcomePage = false;
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	
	QWidget *bottomWidget = new QWidget;
	
	warning = new QLabel;
	timer = new QTimer;
	warning->setIndent(10);
	connect(timer,SIGNAL(timeout()),this,SLOT(hideWarning()));
	
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(warning);
	bottomLayout->addStretch();
	bottomWidget->setLayout(bottomLayout);
	
	localTextEditMainWidget = new QMainWindow;
	localOutSideMainWidget = new QMainWindow;	
	mainLine = new QLineEdit;
	connect(mainLine,SIGNAL(textChanged(const QString&)),this,SLOT(checkMainLineContent()));
	tabWidget = new QTabWidget;
	
	pasteToolBar = new QToolBar(tr("Format toolbar"),this); 
	pasteToolBar->setObjectName("formattoolbar");
	pasteToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	recordsToolBar = new QToolBar(tr("Records toolbar"),this); 
	recordsToolBar->setObjectName("recordstoolbar");
	recordsToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	specialToolBar = new QToolBar(tr("Special toolbar"),this);
	specialToolBar->setObjectName("specialtoolbar");
	specialToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	  
	previewPanel = new PreviewPanel(tr("Preview"),this);
	 
	autoSearchPanel = new AutoSearchPanel(tr("Auto-search"),this);	
	connect(autoSearchPanel,SIGNAL(signalToMove(QString&,int)),this,SLOT(moveByItem(QString&,int)));
	localOutSideMainWidget->addDockWidget(Qt::RightDockWidgetArea,autoSearchPanel);	
	
	aboutDictDialog = new AboutDictDialog(this);
	connect(aboutDictDialog,SIGNAL(saveButtonClicked()),this,SLOT(changeAboutDict()));
		
	bookmarksPanel = new BookmarksPanel(tr("Bookmarks"),this);
	bookmarksPanel->setAddActionEnable(false);
	bookmarksPanel->setRemoveActionEnable(false);
     connect(bookmarksPanel,SIGNAL(itemClicked(QString&,int)),this,SLOT(moveByItem(QString&,int)));
	connect(bookmarksPanel,SIGNAL(addBookMarkSignal()),this,SLOT(addBookMark()));
     localOutSideMainWidget->addDockWidget(Qt::LeftDockWidgetArea,bookmarksPanel);
		
	historyPanel = new HistoryPanel(tr("History"),this);
     connect(historyPanel,SIGNAL(itemClicked(QString&,int)),this,SLOT(moveByItem(QString&,int)));
     localOutSideMainWidget->addDockWidget(Qt::LeftDockWidgetArea,historyPanel);
     
     localOutSideMainWidget->splitDockWidget(bookmarksPanel,historyPanel,Qt::Vertical);
	                      
	
	addTabButton = new QToolButton(tabWidget);
	addTabButton->setIcon(QIcon(ICONS_PATH+"addtab.png"));
	addTabButton->setShortcut(QKeySequence("Ctrl+T"));
	addTabButton->setStatusTip(tr("Add tab"));
	addTabButton->setIconSize(QSize(16, 16));
	addTabButton->setCursor(Qt::ArrowCursor);
	addTabButton->setAutoRaise(true);
	connect(addTabButton,SIGNAL(clicked()),this,SLOT(addTab()));
	
	removeTabButton = new QToolButton(tabWidget);
	removeTabButton->setIcon(QIcon(ICONS_PATH+"removetab.png"));
	removeTabButton->setStatusTip(tr("Remove tab"));
	removeTabButton->setIconSize(QSize(16, 16));
	removeTabButton->setCursor(Qt::ArrowCursor);
	removeTabButton->setAutoRaise(true);
	removeTabButton->setEnabled(false);
	connect(removeTabButton,SIGNAL(clicked()),this,SLOT(removeTab()));
		
	pasteBlockAction = new QAction(pasteToolBar);
	pasteBlockAction->setText(tr("Paste block"));       
	pasteBlockAction->setShortcut(QKeySequence("Alt+S"));
	pasteBlockAction->setIcon(QIcon(ICONS_PATH + "text_block.png"));
	connect(pasteBlockAction,SIGNAL(triggered()),this,SLOT(pasteBlock()));
	pasteBlockAction->setStatusTip(tr("Paste teg \"text in the block\""));
	
	pasteBoldAction = new QAction(pasteToolBar);
	pasteBoldAction->setText(tr("Paste bold"));              
	pasteBoldAction->setShortcut(QKeySequence("Alt+B"));
	pasteBoldAction->setIcon(QIcon(ICONS_PATH + "text_bold.png"));
	connect(pasteBoldAction,SIGNAL(triggered()),this,SLOT(pasteBold()));
	pasteBoldAction->setStatusTip(tr("Paste teg \"bold text\""));
	
	pasteItalicAction = new QAction(pasteToolBar);
	pasteItalicAction->setText(tr("Paste italic"));
	pasteItalicAction->setIcon(QIcon(ICONS_PATH + "text_italic.png"));
	connect(pasteItalicAction,SIGNAL(triggered()),this,SLOT(pasteItalic()));
	pasteItalicAction->setStatusTip(tr("Paste teg \"italic text\""));
	
	pasteUnderlineAction = new QAction(pasteToolBar);
	pasteUnderlineAction->setText(tr("Paste underline"));
	pasteUnderlineAction->setIcon(QIcon(ICONS_PATH + "text_under.png"));
	pasteUnderlineAction->setShortcut(QKeySequence("Alt+U"));
	connect(pasteUnderlineAction,SIGNAL(triggered()),this,SLOT(pasteUnderline()));
	pasteUnderlineAction->setStatusTip(tr("Paste teg \"Underline text\""));
	
	pasteSpecialAction = new QAction(pasteToolBar);
	pasteSpecialAction->setText(tr("Paste office word"));
	pasteSpecialAction->setIcon(QIcon(ICONS_PATH + "text_special.png"));
	pasteSpecialAction->setShortcut(QKeySequence("Alt+O"));
	connect(pasteSpecialAction,SIGNAL(triggered()),this,SLOT(pasteSpecial()));
	pasteSpecialAction->setStatusTip(tr("Paste teg \"Office word\""));
	
	pasteLinkAction = new QAction(pasteToolBar);
	pasteLinkAction->setText(tr("Paste link"));
	pasteLinkAction->setIcon(QIcon(ICONS_PATH + "text_link.png"));
	pasteLinkAction->setShortcut(QKeySequence("Alt+L"));
	connect(pasteLinkAction,SIGNAL(triggered()),this,SLOT(pasteLink()));
	pasteLinkAction->setStatusTip(tr("Paste teg \"link to some word\""));
	
	pasteSoundAction = new QAction(this);
	pasteSoundAction->setText(tr("Paste sound"));
	pasteSoundAction->setStatusTip(tr("Paste teg \"sound of some words\""));
	pasteSoundAction->setShortcut(QKeySequence("Alt+S"));
	pasteSoundAction->setIcon(QIcon(ICONS_PATH + "text_sound.png"));
	connect(pasteSoundAction,SIGNAL(triggered()),this,SLOT(pasteSound()));
	// Create paste actions end;
	
	// records actions start:	
	addAction = new QAction(this);
	addAction->setText(tr("Add"));
	addAction->setIcon(QIcon(ICONS_PATH + "add.png"));
	addAction->setShortcut(QKeySequence("Ctrl+A")); 
	connect(addAction,SIGNAL(triggered()),this,SLOT(addRecord()));
	addAction->setStatusTip(tr("Add new word into database"));
	addAction->setEnabled(false);	
	
	removeAction = new QAction(this);
	removeAction->setText(tr("Remove"));
	removeAction->setShortcut(QKeySequence("Ctrl+D"));
	removeAction->setIcon(QIcon(ICONS_PATH + "remove.png"));
	connect(removeAction,SIGNAL(triggered()),this,SLOT(removeRecord()));
	removeAction->setEnabled(false);
	removeAction->setStatusTip(tr("Remove some word from database"));
	
	editAction = new QAction(this);
	editAction->setText(tr("Edit"));
	editAction->setShortcut(QKeySequence("Ctrl+S"));
	editAction->setIcon(QIcon(ICONS_PATH + "edit.png"));	
	connect(editAction,SIGNAL(triggered()),this,SLOT(editRecord()));
	editAction->setStatusTip(tr("Edit this record"));
	
	changeStatusAction = new QAction(this);
	changeStatusAction->setText(tr("Change status"));
	changeStatusAction->setStatusTip(tr("Change status of the record"));
	changeStatusAction->setIcon(QIcon(ICONS_PATH + "change_status.png"));
	changeStatusAction->setEnabled(false);
	connect(changeStatusAction,SIGNAL(triggered()),this,SLOT(changeStatus()));
	// records actions end;
	
	tabWidget->setCornerWidget(addTabButton,Qt::TopLeftCorner);
	tabWidget->setCornerWidget(removeTabButton,Qt::TopRightCorner);
	//==========================
	
	connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(indexChanged(int)));
	mainLabel = new QLabel(tr("Enter &word: "));
	mainLabel->setBuddy(mainLine);
	
     localOutSideMainWidget->splitDockWidget(bookmarksPanel,historyPanel,Qt::Vertical);
	
	clearButton = new QPushButton;
	clearButton->setShortcut(QKeySequence("Del"));
	clearButton->setIcon(QIcon(ICONS_PATH + "clear.png"));
	clearButton->setEnabled(false);
	clearButton->setFlat(true);
	clearButton->setStatusTip(tr("Clear word-line"));
	connect(clearButton,SIGNAL(clicked()),mainLine,SLOT(clear()));
	connect(clearButton,SIGNAL(clicked()),mainLine,SLOT(setFocus()));
	
	undoAction = new QAction(this);
	undoAction->setText(tr("Undo"));
	undoAction->setIcon(QIcon(ICONS_PATH + "undo.png"));
	undoAction->setShortcut(QKeySequence("Ctrl+Z"));
	undoAction->setEnabled(false);
	undoAction->setStatusTip(tr("Undo action"));
	connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));
	
	redoAction = new QAction(this);
	redoAction->setText(tr("Redo"));
	redoAction->setIcon(QIcon(ICONS_PATH + "redo.png"));
	redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));
	redoAction->setEnabled(false);
	redoAction->setStatusTip(tr("Redo action"));
	connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));
	
	zoomInAction = new QAction(this);
	zoomInAction->setText(tr("Zoom in"));
	zoomInAction->setIcon(QIcon(ICONS_PATH + "zoomin.png"));
	zoomInAction->setEnabled(false);
	zoomInAction->setStatusTip(tr("Zoom in"));
	connect(zoomInAction,SIGNAL(triggered()),this,SLOT(zoomIn()));
	
	zoomOutAction = new QAction(this);
	zoomOutAction->setText(tr("Zoom out"));
	zoomOutAction->setIcon(QIcon(ICONS_PATH + "zoomout.png"));
	zoomOutAction->setEnabled(false);
	zoomOutAction->setStatusTip(tr("Zoom out"));
	connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(zoomOut()));
	
	aboutDictAction = new QAction(this);
	aboutDictAction->setText(tr("Edit information"));
	aboutDictAction->setStatusTip(tr("Edit dictionary's information"));
	aboutDictAction->setIcon(QIcon(ICONS_PATH + "dict_information.png"));
	connect(aboutDictAction,SIGNAL(triggered()),this,SLOT(showAboutDict()));
	
	searchAction = new QAction(this);
	searchAction->setText(tr("Search"));
	searchAction->setStatusTip(tr("Search the word in database"));
	searchAction->setShortcut(QKeySequence("Enter"));
	searchAction->setEnabled(true);
	searchAction->setIcon(QIcon(ICONS_PATH + "search.png"));
	connect(searchAction,SIGNAL(triggered()),this,SLOT(checkMainLineContent()));
	connect(searchAction,SIGNAL(triggered()),mainLine,SLOT(setFocus()));
	
	searchButton = new QPushButton(QIcon(ICONS_PATH + "search.png"),tr("Search"));
	searchButton->setStatusTip(tr("Search the word in database"));
	searchButton->setShortcut(QKeySequence("Return"));
	searchButton->setEnabled(true);
	connect(searchButton,SIGNAL(clicked()),this,SLOT(checkMainLineContent())); 
	connect(searchButton,SIGNAL(clicked()),mainLine,SLOT(setFocus()));
	
	previewAction = new QAction(this);
	previewAction->setText(tr("Preview"));
	previewAction->setShortcut(QKeySequence("Ctrl+V"));
	previewAction->setIcon(QIcon(ICONS_PATH + "preview.png"));
	previewAction->setEnabled(false);
	connect(previewAction,SIGNAL(triggered()),this,SLOT(setPreview()));
		
	pasteToolBar->addAction(pasteBoldAction);
	pasteToolBar->addAction(pasteItalicAction);
	pasteToolBar->addAction(pasteUnderlineAction);
	pasteToolBar->addAction(pasteSpecialAction);
	pasteToolBar->addAction(pasteLinkAction);
	pasteToolBar->addAction(pasteBlockAction);
	pasteToolBar->addAction(pasteSoundAction);
	
	specialToolBar->addAction(undoAction);
	specialToolBar->addAction(redoAction);
	specialToolBar->addSeparator();
	specialToolBar->addAction(zoomInAction);
	specialToolBar->addAction(zoomOutAction);
	specialToolBar->addSeparator();
	specialToolBar->addAction(aboutDictAction);
	specialToolBar->addSeparator();
	specialToolBar->addAction(previewAction);
	
	recordsToolBar->addAction(addAction);
	recordsToolBar->addAction(removeAction); 
	recordsToolBar->addAction(editAction);  
	recordsToolBar->addSeparator();
	recordsToolBar->addAction(changeStatusAction);
	
	localTextEditMainWidget->addToolBar(Qt::TopToolBarArea,pasteToolBar);
	localTextEditMainWidget->addToolBar(Qt::TopToolBarArea,recordsToolBar);
	localTextEditMainWidget->addToolBar(Qt::TopToolBarArea,specialToolBar);
	settings.beginGroup("General");
	localTextEditMainWidget->restoreState(settings.value("TextEditPartState").toByteArray()); 
	localOutSideMainWidget->restoreState(settings.value("CentralPartState").toByteArray());
	settings.endGroup();	
		
	//===============
	
	localTextEditMainWidget->setCentralWidget(tabWidget);
	
	QWidget *tempWidget = new QWidget;
	
	QVBoxLayout *tempLayout = new QVBoxLayout;
	tempLayout->addWidget(localTextEditMainWidget);
	tempLayout->addWidget(bottomWidget);
	
	tempWidget->setLayout(tempLayout);
	
	warning->hide();
	localOutSideMainWidget->setCentralWidget(tempWidget);
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(mainLabel);
	topLayout->addWidget(mainLine);
	topLayout->addWidget(clearButton);
	topLayout->addWidget(searchButton);
		
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(localOutSideMainWidget);
		
	setLayout(mainLayout);
}

void CentralWidget::setRecord(QSqlRecord* record)
{
	QString trans = record->value(1).toString();
	QString word = record->value(0).toString();
	trans.replace("\\{","\n\\{\n");
	trans.replace("\\}","\n\\}\n");
		
	int indexOfEmptyPage = getIndexOfEmptyPage();
	if (welcomePage)
		addTab();
	else
	if (indexOfEmptyPage != -1)
		tabWidget->setCurrentIndex(indexOfEmptyPage);
	else
	if (boolSets[OpenWordsInNewTabs])
	{
		bool isThereSameTabs = false;
		for ( int i = 0; i < tabWidget->count(); i++ )
			if (tabWidget->tabText(i) == word )
			{
				isThereSameTabs = true;
				tabWidget->setCurrentIndex(i);
				return;
			}
		addTab();
	}
	tabWidget->setTabText(tabWidget->currentIndex(),word);
	tabWidget->setTabIcon(tabWidget->currentIndex(),
			QIcon(ICONS_PATH + (record->value(2) == 1 ? "was_edited" : "was_not_edited")  + ".png"));
	currentTextEdit->setPlainText(trans);	
	if (boolSets[HighLightTrans])
		highLighters << new HighLighter(currentTextEdit->document());
	connect(currentTextEdit,SIGNAL(undoAvailable(bool)),this,SLOT(undoEnabled(bool)));
	connect(currentTextEdit,SIGNAL(redoAvailable(bool)),this,SLOT(redoEnabled(bool)));	
	undoList << false;
	redoList << false;	
	
	if (boolSets[ShowBookmarks])
		bookmarksPanel->setAddActionEnable(true);
	addAction->setEnabled(false);
	editAction->setEnabled(true);
	removeAction->setEnabled(true);
	previewAction->setEnabled(true);
	if (boolSets[UpdatePreviewDuringEntering])
		previewPanel->setText(trans);
}

char* CentralWidget::getColorByEnum(const int mode) const
{
	if ( mode == Bad )
		return "#ff3b3b";
	return "#16cf4f";
}

int CentralWidget::getIndexOfEmptyPage()
{
	if (welcomePage)
		return 0;
	
	for (int i = 0; i < tabWidget->count(); i++)
		if (tabWidget->tabText(i) == "*")
			return i;
	return -1;
}

void CentralWidget::setEnablePasteActions(bool status)
{
	bookmarksPanel->setAddActionEnable(status);
	pasteBoldAction->setEnabled(status);
	pasteItalicAction->setEnabled(status);
	pasteUnderlineAction->setEnabled(status);
	pasteSpecialAction->setEnabled(status);
	pasteLinkAction->setEnabled(status);
	pasteBlockAction->setEnabled(status);
	pasteSoundAction->setEnabled(status);
}

void CentralWidget::showWelcomePage()
{
	addTab();
	currentTextEdit->setReadOnly(true);
	currentTextEdit->setHtml("<hr><table border=\"0\" width=\"100%\"><tr><td bgcolor=\"#DFEDFF\"><h2 align=\"center\"><em>" + 
		tr("Welcome to the LightLang - the system of electronic dictionaries.<br> LightLang Editor is a part of the system. Press \"F1\" to show information about" 
		" program.") + "</em></h2></td></tr></table><hr>");
	tabWidget->setTabText(tabWidget->currentIndex(),tr("Welcome"));
	welcomePage = true;
	removeTabButton->setEnabled(false);
	redoAction->setEnabled(false);
	undoAction->setEnabled(false);
	clear();
}

void CentralWidget::setDatabase(QSqlDatabase& mainDb,QSqlQuery& mainQuery)
{
	db = mainDb;
	query = mainQuery;
}

void CentralWidget::clear()
{
	autoSearchPanel->clear();
	mainLine->clear();
	previewPanel->clear();
	setEnablePasteActions(false);
	addAction->setEnabled(false);
	removeAction->setEnabled(false);
	editAction->setEnabled(false);
	changeStatusAction->setEnabled(false);
	zoomInAction->setEnabled(false);
	zoomOutAction->setEnabled(false);
}

void CentralWidget::clearAll()
{
	clear();
	historyPanel->clear();
	bookmarksPanel->clear();
	aboutDictDialog->clear();
}

QString CentralWidget::getBookmarks()
{
	return bookmarksPanel->getBookmarks();
}

QString CentralWidget::getHistory()
{
	return historyPanel->getHistory();
}

QByteArray CentralWidget::getPreviewGeometry()
{
	return localTextEditMainWidget->saveState();
}

QByteArray CentralWidget::getLocalMainWidgetState()
{
	return localOutSideMainWidget->saveState();
}

QString& CentralWidget::getAboutDict()
{
	return currentAbout;
}

void CentralWidget::setHistory(const QString history)
{
	historyPanel->setHistory(history);
}

void CentralWidget::setBookmarks(const QString bookmarks)
{
	bookmarksPanel->setBookmarks(bookmarks);
}

bool CentralWidget::getExtendedSearchStatus()
{
	return autoSearchPanel->getExtendedSearchStatus();
}

void CentralWidget::setAboutDict(const QString& about)
{
	currentAbout = about;
	aboutDictDialog->setAbout(about);
}

void CentralWidget::setSettings(QList<bool>& bool_sets,QList<int>& int_sets)
{
	boolSets = bool_sets;
	intSets = int_sets;	
	updateProgram();
}

QAction* CentralWidget::getAction(int what)
{
	switch (what)
	{
		case SearchAction:
			return searchAction;
		case AddAction:
			return addAction;
		case RemoveAction:
			return removeAction;
		case EditAction:
			return editAction;
	}
	return aboutDictAction;
}

void CentralWidget::pasteBold()
{
	if ( currentTextEdit->textCursor().selectionEnd() == currentTextEdit->textCursor().selectionStart() )
	{
		currentTextEdit->insertPlainText("\\[ \\]");	
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
	} else
	{	
		QString selText = currentTextEdit->textCursor().selectedText();
		currentTextEdit->insertPlainText("\\[" + selText + "\\]");
	}
}

void CentralWidget::pasteItalic()
{
	if ( currentTextEdit->textCursor().selectionEnd() == currentTextEdit->textCursor().selectionStart() )
	{
		currentTextEdit->insertPlainText("\\( \\)");	
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
	} else
	{	
		QString selText = currentTextEdit->textCursor().selectedText();
		currentTextEdit->insertPlainText("\\(" + selText + "\\)");
	}
}

void CentralWidget::pasteUnderline()
{
	if ( currentTextEdit->textCursor().selectionEnd() == currentTextEdit->textCursor().selectionStart() )
	{
		currentTextEdit->insertPlainText("\\_ \\_");	
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
	} else
	{	
		QString selText = currentTextEdit->textCursor().selectedText();
		currentTextEdit->insertPlainText("\\_" + selText + "\\_");
	}
}


void CentralWidget::pasteSpecial()
{
	if ( currentTextEdit->textCursor().selectionEnd() == currentTextEdit->textCursor().selectionStart() )
	{
		currentTextEdit->insertPlainText("\\< \\>");	
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
	} else
	{	
		QString selText = currentTextEdit->textCursor().selectedText();
		currentTextEdit->insertPlainText("\\<" + selText + "\\>");
	}
}


void CentralWidget::pasteLink()
{
	if ( currentTextEdit->textCursor().selectionEnd() == currentTextEdit->textCursor().selectionStart() )
	{
		currentTextEdit->insertPlainText("\\@ \\@");	
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
	} else
	{	
		QString selText = currentTextEdit->textCursor().selectedText();
		currentTextEdit->insertPlainText("\\@" + selText + "\\@");
	}
}


void CentralWidget::pasteBlock()
{
	if ( currentTextEdit->textCursor().selectionEnd() == currentTextEdit->textCursor().selectionStart() )
	{
		currentTextEdit->insertPlainText("\n\\{\n \n\\}\n");	
		for ( int i = 0; i < 5; i++ )
			currentTextEdit->moveCursor(QTextCursor::Left);
	} else
	{	
		QString selText = currentTextEdit->textCursor().selectedText();
		currentTextEdit->insertPlainText("\n\\{\n" + selText + "\n\\}\n");
	}
}


void CentralWidget::pasteSound()
{
	if ( currentTextEdit->textCursor().selectionEnd() == currentTextEdit->textCursor().selectionStart() )
	{
		currentTextEdit->insertPlainText("\\s : \\s");	
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
		currentTextEdit->moveCursor(QTextCursor::Left);
	} else
	{	
		QString selText = currentTextEdit->textCursor().selectedText();
		currentTextEdit->insertPlainText("\\s :" + selText + "\\s");
		for ( int i = 0; i < selText.length() + 3; i++ ) 
			currentTextEdit->moveCursor(QTextCursor::Left);
	}
}

void CentralWidget::editRecord()
{
	QString trans = currentTextEdit->toPlainText();
	if (!trans.isEmpty())
	{	
		query.exec(QString("UPDATE main SET  trans = \"%2\"  WHERE `word` = \"%1\"")
						.arg(mainLine->text().toLower()).arg(trans.simplified()));
		if ( !query.isActive() )
		{
			showMessage(Bad,tr("Database error:") + query.lastError().text());
			return;
		}
		showMessage(Good,tr("The record was edited"));		
	}
	else
		showMessage(Bad,tr("Translation is empty"));
}

void CentralWidget::addRecord()
{
	if (mainLine->text().trimmed().isEmpty())
	{
		showMessage(Bad,tr("Enter the word"));
		mainLine->setFocus();
		return;
	}
	QString trans = currentTextEdit->toPlainText();
	if ( !trans.isEmpty() )
	{
		QString word = mainLine->text().toLower().trimmed();
		// check is there the same word
		query.exec(QString("SELECT * FROM main WHERE word = \"%1\"").arg(word));
		if (query.next())
		{
			int r = QMessageBox::warning(this,tr("Warning"),tr("The same word in database is already exists\nReplace it?"),
									QMessageBox::No | QMessageBox::Default,QMessageBox::Yes);
			if (r == QMessageBox::No)
				return;
			else
				query.exec(QString("DELETE FROM main WHERE word = \"%1\"").arg(word));
		}
		//=============================
		
		
		query.exec(QString("INSERT INTO main(word,trans,stat) VALUES(\"%1\",\"%2\",\"0\")").arg(word).arg(trans));
		if ( !query.isActive() )
		{
			showMessage(Bad,tr("Database error:") + query.lastError().text());
			return;
		}
		showMessage(Good,tr("The record was added"));
		find();
	}
	else
		showMessage(Bad,tr("Translation is empty"));
} 

void CentralWidget::removeRecord()
{
	QString word = mainLine->text();
	query.exec(QString("DELETE FROM main WHERE word = \"%1\"").arg(word));
	if ( !query.isActive() )
	{
		showMessage(Bad,tr("Database error:") + query.lastError().text());
		return;
	}
	else
		showMessage(Good,tr("The record was removed"));
	
	for ( int i = 0; i < bookmarksPanel->count(); i++ )
		if ( bookmarksPanel->getText(i) == word )
			bookmarksPanel->setRowHidden(i,true);
	removeTab(tabWidget->currentIndex());
	mainLine->setFocus();
}

void CentralWidget::changeStatus()
{
	query.exec(QString("SELECT `stat` FROM main WHERE word = \"%1\"").arg(tabWidget->tabText(tabWidget->currentIndex())));
	query.next();
	int stat = query.record().value(0).toInt();
	query.exec(QString("UPDATE main SET `stat` = %1 WHERE word = \"%2\"").arg( stat == 1 ? '0' : '1' ).arg( tabWidget->tabText(tabWidget->currentIndex()) ) );
	
	if ( query.isActive() )
	{
		switch (stat)
		{
			case 0:
				tabWidget->setTabIcon(tabWidget->currentIndex(),QIcon(ICONS_PATH + "was_edited.png"));
				break;
			case 1:
				tabWidget->setTabIcon(tabWidget->currentIndex(),QIcon(ICONS_PATH + "was_not_edited.png"));
				break;
		}
		showMessage(Good,tr("The status was changed"));
	}
	else
		showMessage(Bad,tr("Database error:") + query.lastError().text());
}

void CentralWidget::hideWarning()
{
	warning->setText("");
}

void CentralWidget::moveByItem(QString& word,int from)
{
	mainLine->setText(word);
	if (!boolSets[UpdateTransDuringEntering])
		checkMainLineContent(from);
}

void CentralWidget::find(int from)
{
	QCoreApplication::processEvents();
	QString str = mainLine->text().toLower().trimmed();
	QSqlRecord record;
	query.exec(QString("SELECT * FROM main WHERE word = \"%1\"").arg(str));
	if ( query.next()  )
	{
		showMessage(Good,tr("The word was founded"));
		record = query.record();
		if ( from != FromHistory && boolSets[ShowHistory] )
			historyPanel->addItem(str);
		setRecord(&record);
	}
	else
	{
		showMessage(Bad,tr("The word was not founded"));
		if (!boolSets[OpenWordsInNewTabs])
		{
			removeTab(tabWidget->currentIndex());
			addTab();
		}
		else
		if ( getIndexOfEmptyPage() == -1 || welcomePage )
			addTab();
		else
			tabWidget->setCurrentIndex(getIndexOfEmptyPage());
	}
	if ( from != FromAutoSearch )
		if ( boolSets[ShowAutoSearch] )
		{
			autoSearchPanel->clear();
			int i = 0;
			if ( autoSearchPanel->getExtendedSearchStatus() )
				query.exec(QString("SELECT * FROM main WHERE word LIKE  %1")
						.arg(boolSets[SearchWordsByBegining] ?  QString("\"%2%\"") : QString("\"%%2%\"")).arg(str));
			else
				query.exec(QString("SELECT * FROM main WHERE word LIKE  %1 LIMIT 0,%3 ")
				.arg(boolSets[SearchWordsByBegining] ?  QString("\"%2%\"") : QString("\"%%2%\""))
				.arg(str).arg(intSets[MinimumRecords]));
			if ( !query.isActive() )
				showMessage(Bad,tr("Database error:") + query.lastError().text());
			while (query.next())
			{
				i++;
				record = query.record();
				autoSearchPanel->addItem(&record,boolSets[ShowMarksInAutoSearch]);
			}
			if ( i > 1 )
			{
				autoSearchPanel->setEnableActions(true);
				autoSearchPanel->setEnableOfFilter(true);
			}
			else
			{
				autoSearchPanel->setEnableActions(false);
				autoSearchPanel->setEnableOfFilter(false);
			}
		}
}

void CentralWidget::updateProgram()
{
	bool toUpdateMainLine = false; 
	if ( boolSets[ShowAutoSearch] )
	{
		autoSearchPanel->show();
		toUpdateMainLine = true;
		autoSearchPanel->setMoveBySingleClick(boolSets[MoveBySingleClick]);
		autoSearchPanel->activeSettings();
	}
	else
	{
 		autoSearchPanel->hide();
 		autoSearchPanel->setEnableActions(false);
 	}
 	
	if ( boolSets[ShowBookmarks] )
	{
		bookmarksPanel->show();
		toUpdateMainLine = true;
	}
	else
	{	
		bookmarksPanel->setAddActionEnable(false);
		bookmarksPanel->setRemoveActionEnable(false);
		bookmarksPanel->hide();
	}
	
	if ( !boolSets[UpdateTransDuringEntering] )
	{
		toUpdateMainLine = true;
		searchButton->show();
		searchButton->blockSignals(false);
		mainLine->blockSignals(true);
	}
	else
	{		
		searchButton->hide();
		mainLine->blockSignals(false);
		searchButton->blockSignals(true);
	}
	
	if ( !boolSets[ShowHistory] )
		historyPanel->hide();
	else
		historyPanel->show();
	
	if  ( !boolSets[ShowPreviewApart] )
	{
		delete previewPanel;
		previewPanel = new PreviewPanel(tr("Preview"),this);
		localTextEditMainWidget->addDockWidget(Qt::BottomDockWidgetArea,previewPanel);
		if (currentTextEdit)
		{
			QString currentText = currentTextEdit->toPlainText();
			previewPanel->setText(currentText);
		}
	}
	else
	{
		localTextEditMainWidget->removeDockWidget(previewPanel);
		delete previewPanel;
		previewPanel = new PreviewPanel(tr("Preview"));
		if (currentTextEdit)
		{
			previewPanel->setText(currentTextEdit->toPlainText());
			previewPanel->show();
		}
	}
	
	if ( !boolSets[HighLightTrans] )
	{
		for (int i = 0; i < highLighters.count(); i++ )
			highLighters[i]->setDocument(0);
	}
	else
	{
		for ( int i = 0; i < textEdits.count(); i++ )
			highLighters[i]->setDocument(textEdits[i]->document());
	}
	
	if ( boolSets[ShowMarksInTabs] && !welcomePage )
	{
		for ( int i = 0; i < tabWidget->count(); i++ )
		{
			query.exec(QString("SELECT `stat` FROM `main` WHERE word = \"%1\"").arg(tabWidget->tabText(i)));
			query.next();
			if ( query.record().value(0).toInt() == 1 )
				tabWidget->setTabIcon(i,QIcon(ICONS_PATH + "was_edited.png"));
			else
				tabWidget->setTabIcon(i,QIcon(ICONS_PATH + "was_not_edited.png"));
		}
	}
	else
		for ( int i = 0; i < tabWidget->count(); i++ )
			tabWidget->setTabIcon(i,QIcon());
	
	if ( toUpdateMainLine )
		checkMainLineContent();
		
	showMessage(Good,tr("All settings was updated"));
}

void CentralWidget::setPreview()
{
	previewPanel->setText(currentTextEdit->toPlainText());
}

void CentralWidget::redoEnabled(bool b)
{
	redoList[tabWidget->currentIndex()] = b;
	redoAction->setEnabled(b);
}

void CentralWidget::undoEnabled(bool b)
{
	undoList[tabWidget->currentIndex()] = b;
	undoAction->setEnabled(b);
}

void CentralWidget::addBookMark()
{
 	bookmarksPanel->addItem(tabWidget->tabText(tabWidget->currentIndex()));
}

void CentralWidget::indexChanged(int index)
{
	currentTextEdit = textEdits[index];
	undoAction->setEnabled(undoList[index]);
	redoAction->setEnabled(redoList[index]);
	if ( boolSets[UpdatePreviewDuringEntering] )
		previewPanel->setText(textEdits[index]->toPlainText());
	if ( boolSets[ShowBookmarks] )
		bookmarksPanel->setAddActionEnable(true);
	mainLine->blockSignals(true);
	mainLine->setText(tabWidget->tabText(tabWidget->currentIndex()));
	if (boolSets[UpdateTransDuringEntering])
		mainLine->blockSignals(false);
	if (!boolSets[UpdateTransDuringEntering])
		currentTextEdit->setFocus();
	if ( tabWidget->tabText(tabWidget->currentIndex()) == "*" )
	{
		changeStatusAction->setEnabled(false);
		editAction->setEnabled(false);
		addAction->setEnabled(true);
		previewAction->setEnabled(false);
		bookmarksPanel->setAddActionEnable(false);
		removeAction->setEnabled(false);
		mainLine->setFocus();
	}
	else
	{
		changeStatusAction->setEnabled(true);
		editAction->setEnabled(true);
		addAction->setEnabled(false);
		previewAction->setEnabled(true);
		bookmarksPanel->setAddActionEnable(true);
		removeAction->setEnabled(true);
	}
}

void CentralWidget::changeAboutDict()
{
	currentAbout = aboutDictDialog->getAbout();
}

void CentralWidget::showAboutDict()
{
	aboutDictDialog->show();
	aboutDictDialog->activateWindow();
}

void CentralWidget::addTab()
{
	if (welcomePage)
	{
		removeTab(0);
		welcomePage = false;
	}
	currentTextEdit = new QTextEdit;
	textEdits << currentTextEdit;
	highLighters << new HighLighter;
	if (boolSets[HighLightTrans])
		highLighters.last()->setDocument(currentTextEdit->document());
	tabWidget->addTab(currentTextEdit,"*");
	tabWidget->blockSignals(true);
	tabWidget->setCurrentWidget(currentTextEdit);
	tabWidget->blockSignals(false);
	redoList << false;
	undoList << false;
	connect(currentTextEdit,SIGNAL(undoAvailable(bool)),this,SLOT(undoEnabled(bool)));
	connect(currentTextEdit,SIGNAL(redoAvailable(bool)),this,SLOT(redoEnabled(bool)));
	setEnablePasteActions(true);
	addAction->setEnabled(true);
	editAction->setEnabled(false);
	changeStatusAction->setEnabled(false);
	previewAction->setEnabled(false);
	changeStatusAction->setEnabled(true);
	removeTabButton->setEnabled(true);
	removeAction->setEnabled(false);
	mainLine->setFocus();
}

void CentralWidget::removeTab(int id)
{
	int index;
	if ( id == -1 ) 
		index = tabWidget->currentIndex();
	else
		index = id;
	tabWidget->removeTab(index);
	textEdits.removeAt(index);
	highLighters.removeAt(index);
	undoList.removeAt(index);
	redoList.removeAt(index);
	if ( tabWidget->count() == 0 && !welcomePage )
		showWelcomePage();
	if (tabWidget->count() > 0 )
		currentTextEdit = textEdits[tabWidget->currentIndex()];
}

void CentralWidget::redo()
{
	currentTextEdit->redo();
}

void CentralWidget::undo()
{
	currentTextEdit->undo();
}

void CentralWidget::zoomIn()
{
	currentTextEdit->zoomIn();
}

void CentralWidget::zoomOut()
{
	currentTextEdit->zoomOut();
}

void CentralWidget::checkMainLineContent(int from)
{
	QString word = mainLine->text();
	if ( !word.isEmpty() && db.isOpen() )
	{
		zoomOutAction->setEnabled(true);
		zoomInAction->setEnabled(true); 		
 		find(from);
	}
	else
	{
		zoomOutAction->setEnabled(false);
		zoomInAction->setEnabled(false);   	
		clear();
	}	
	if ( !word.isEmpty() )
		clearButton->setEnabled(true);
	else
		clearButton->setEnabled(false);
}

void CentralWidget::showMessage(int mode,QString text)
{
	timer->setInterval(50000);
	warning->setText(QString("<font color =\'%1\'>").arg(getColorByEnum(mode))+text+"</font>");
	timer->start();
	warning->show();	
}

void CentralWidget::mouseDoubleClickEvent(QMouseEvent*)
{
	if (welcomePage)
		removeTab(0);
	addTab();
}

QByteArray CentralWidget::getAutoSearchState()
{
	return autoSearchPanel->getMainWidgetState();
}