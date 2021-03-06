#include <QtGui/QContextMenuEvent>
#include <QtGui/QTextCursor>
#include "Menu.h"
#include "TranslationEditor.h"

TranslationEditor::TranslationEditor() {
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

void TranslationEditor::findNext(const QString& expression) {
	findExpression(expression);
}

void TranslationEditor::findPrevious(const QString& expression) {
	findExpression(expression,true);
}

void TranslationEditor::findExpression(const QString& expression,bool backwardFlag) {
	QTextCursor cursor = textCursor();
	QTextCursor newCursor;
	if (cursor.hasSelection() && backwardFlag)
		cursor.setPosition(cursor.anchor(),QTextCursor::MoveAnchor);
	if (!backwardFlag) {
		newCursor = document()->find(expression,cursor);
		if (newCursor.isNull()) {
			newCursor = cursor;
			emit (setDefaultPalette());
		}
	} else {
		newCursor = document()->find(expression,cursor,QTextDocument::FindBackward);
		if (newCursor.isNull()) {
			newCursor = cursor;
			emit (setDefaultPalette());
		}
	}
	setTextCursor(newCursor);
}

void TranslationEditor::findFirst(const QString& expression) {
	QTextCursor myCursor, tempCursor;
	myCursor = textCursor();
	tempCursor = myCursor;
	
	myCursor.setPosition(myCursor.selectionStart(), QTextCursor::MoveAnchor);
	setTextCursor(myCursor);
	
	myCursor = document()->find(expression,myCursor);
	
	if (myCursor.isNull() && !expression.isEmpty()) {
		myCursor = document()->find(expression,false);
		if (myCursor.isNull()) {
			emit (setRedPalette());
			setTextCursor(tempCursor);
		} else {
			emit (setGreenPalette());
			tempCursor = myCursor;
			setTextCursor(myCursor);
			findExpression(expression);
		}
	} else if (myCursor.isNull()) {
		emit(setDefaultPalette());
		setTextCursor(tempCursor);
	} else {
		emit (setGreenPalette());
		findExpression(expression);
	}
}
