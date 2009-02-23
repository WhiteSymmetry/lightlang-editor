#ifndef TRANSLATIONEDITOR_H
#define TRANSLATIONEDITOR_H

#include "BrowserWithWidgets.h"

class Menu;
class QAction;

class TranslationEditor : public BrowserWithWidgets
{
	Q_OBJECT
	signals:
		void focused();
		void setRedPalette();
		void setGreenPalette();
		void setDefaultPalette();
	public slots:
		void findFirst(const QString& expression);
		void findNext(const QString& expression);
		void findPrevious(const QString& expression);
	public:
		TranslationEditor();
	
		void setMenu(Menu *menu);
	private:
		Menu *menu;
		
		void findExpression(const QString& expression,bool backwardFlag = false);
	protected:
		void contextMenuEvent(QContextMenuEvent *event);
};


#endif
