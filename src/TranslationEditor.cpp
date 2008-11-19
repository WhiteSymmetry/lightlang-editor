#include "HighLighter.h"
#include "TranslationEditor.h"

TranslationEditor::TranslationEditor() {
	highLighter = new HighLighter(document());
}

TranslationEditor::~TranslationEditor() {
	delete highLighter;
}
