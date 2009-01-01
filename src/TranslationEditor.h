#ifndef TRANSLATIONEDITOR_H
#define TRANSLATIONEDITOR_H

#include "BrowserWithWidgets.h"

class HighLighter;

class TranslationEditor : public BrowserWithWidgets
{
	public:
		TranslationEditor();
		~TranslationEditor();
	private:
		HighLighter *highLighter;
};


#endif
