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
	public:
		TranslationEditor();
	
		void setMenu(Menu *menu);
	private:
		Menu *menu;
	protected:
		void contextMenuEvent(QContextMenuEvent *event);
};


#endif
