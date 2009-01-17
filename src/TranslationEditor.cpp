#include <QtGui/QContextMenuEvent>
#include "Menu.h"
#include "TranslationEditor.h"

TranslationEditor::TranslationEditor() {
	setReadOnly(false);
	setOrientation(BrowserWithWidgets::Vertical);
	setPosition(BrowserWithWidgets::Right);
	menu = 0;
}

void TranslationEditor::contextMenuEvent(QContextMenuEvent *event) {
	if (menu != 0) {
		menu->move(event->globalX(),event->globalY());
		menu->exec();
	}
}


void TranslationEditor::setMenu(Menu *m) {
	menu = m;
}
