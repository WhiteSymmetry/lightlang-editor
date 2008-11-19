#ifndef TRANSLATIONEDITOR_H
#define TRANSLATIONEDITOR_H

#include <QtGui/QTextEdit>

class HighLighter;

class TranslationEditor : public QTextEdit
{
	public:
		TranslationEditor();
		~TranslationEditor();
	private:
		HighLighter *highLighter;
};


#endif
