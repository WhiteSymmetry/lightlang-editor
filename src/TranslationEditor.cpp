#include <QtGui/QContextMenuEvent>
#include "HighLighter.h"
#include "Menu.h"
#include "TranslationEditor.h"

TranslationEditor::TranslationEditor() {
	highLighter = new HighLighter(document());
	setReadOnly(false);
	setOrientation(BrowserWithWidgets::Vertical);
	setPosition(BrowserWithWidgets::Right);
	menu = 0;
}

TranslationEditor::~TranslationEditor() {
	delete highLighter;
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
