#include "HighLighter.h"
#include "TranslationEditor.h"

TranslationEditor::TranslationEditor() {
	highLighter = new HighLighter(document());
	setReadOnly(false);
	setOrientation(BrowserWithWidgets::Vertical);
	setPosition(BrowserWithWidgets::Right);
}

TranslationEditor::~TranslationEditor() {
	delete highLighter;
}
