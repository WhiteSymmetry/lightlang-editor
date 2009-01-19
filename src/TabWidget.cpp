#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QFrame>
#include <QtGui/QCursor>
#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QtCore/QStack>
#include "TranslationEditor.h"
#include "DatabaseCenter.h"
#include "EditorTipsWidget.h"
#include "TabWidget.h"


TabWidget::TabWidget(DatabaseCenter *dbCenter,int index,int updateTranslationInterval) {
	databaseCenter = dbCenter;
	
	tabIndex = index;
	
	timer = new QTimer;
	setUpdateTranslationInterval(updateTranslationInterval);
	connect(timer,SIGNAL(timeout()),this,SLOT(updateTranslation()));
	
	editorTipsWidget = new EditorTipsWidget;
	connect(editorTipsWidget,SIGNAL(hideAllTips()),this,SIGNAL(hideAllTips()));
	
	editorTipsWidget->addMessages(QStringList()
				<< tr("Use Tab and Shift+Tab to navigate between fields")
				<< tr("Use hotkeys to edit,add and remove words")
				<< tr("Report about bugs to us by e-mail in documentation")
				<< tr("You can use statuses to point some words for yourself")
	);
	
	textEdit = new TranslationEditor;
	
	lineEdit = new QLineEdit;
	lineEdit->setToolTip(tr("Enter phrase here, which you want to translate"));
	connect(lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(textChanged(const QString&)));
	
	addWordButton = new QToolButton;
	addWordButton->setAutoRaise(true);
	addWordButton->setIcon(QIcon(":/icons/add.png"));
	addWordButton->setToolTip(tr("Add the phrase into dictionary database. Shortcut: Ctrl+Enter"));
	addWordButton->setShortcut(QKeySequence("Ctrl+Return"));
	connect(addWordButton,SIGNAL(clicked()),this,SLOT(addWord()));
	
	editWordButton = new QToolButton;
	editWordButton->setAutoRaise(true);
	editWordButton->setIcon(QIcon(":/icons/edit.png"));
	editWordButton->setShortcut(QKeySequence("Ctrl+S"));
	editWordButton->setToolTip(tr("Edit the phrase. Shourtcut: Ctrl+S"));
	connect(editWordButton,SIGNAL(clicked()),this,SLOT(editWord()));
	
	removeWordButton = new QToolButton;
	removeWordButton->setAutoRaise(true);
	removeWordButton->setIcon(QIcon(":/icons/remove.png"));
	removeWordButton->setShortcut(QKeySequence("Ctrl+Delete"));
	removeWordButton->setToolTip(tr("Remove the phrase from dictionary database. Shortcut: Ctrl+Backspace"));
	connect(removeWordButton,SIGNAL(clicked()),this,SLOT(removeWord()));
	
	updateTranslationButton = new QToolButton;
	updateTranslationButton->setIcon(QIcon(":/icons/search.png"));
	updateTranslationButton->setShortcut(QKeySequence("Return"));
	updateTranslationButton->setEnabled(false);
	connect(updateTranslationButton,SIGNAL(clicked()),this,SLOT(updateTranslation()));
	updateTranslationButton->setFocusPolicy(Qt::ClickFocus);
	
	addWordButton->setEnabled(false);
	editWordButton->setEnabled(false);
	removeWordButton->setEnabled(false);
	
	QFrame *horizontalFrame = new QFrame;
	horizontalFrame->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	
	textEdit->addWidget(addWordButton);
	textEdit->addWidget(editWordButton);
	textEdit->addWidget(horizontalFrame);
	textEdit->addWidget(removeWordButton);
	textEdit->addWidgetAt(TranslationEditor::RightBottomCorner,editorTipsWidget);
	connect(textEdit,SIGNAL(textChanged()),this,SLOT(translationChanged()));
	
	clearLineEditButton = new QToolButton;
	clearLineEditButton->setAutoRaise(true);
	clearLineEditButton->setIcon(QIcon(":/icons/clear.png"));
	connect(clearLineEditButton,SIGNAL(clicked()),lineEdit,SLOT(clear()));
	clearLineEditButton->setFocusPolicy(Qt::ClickFocus);
	
	QHBoxLayout *lineEditLayout = new QHBoxLayout;
	lineEditLayout->addWidget(lineEdit,1);
	lineEditLayout->addWidget(clearLineEditButton);
	lineEditLayout->addWidget(updateTranslationButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(lineEditLayout);
	mainLayout->addWidget(textEdit);
	mainLayout->setContentsMargins(3,3,3,3);
	setLayout(mainLayout);
}

TabWidget::~TabWidget() {
	delete editorTipsWidget;
	delete updateTranslationButton;
	delete addWordButton;
	delete editWordButton;
	delete removeWordButton;
	delete textEdit;
	delete lineEdit;
	delete clearLineEditButton;
}

void TabWidget::setHtml(const QString& htmlText) {
	textEdit->setHtml(htmlText);
}

void TabWidget::setReadOnly(bool readOnly) {
	textEdit->setReadOnly(readOnly);
}

void TabWidget::textChanged(const QString&) {
	addWordButton->setEnabled(false);
	editWordButton->setEnabled(false);
	removeWordButton->setEnabled(false);
	timer->stop();
	if (updateTranslationDuringEntering)
		timer->start();
	resetButtonsAccessibility();
}

void TabWidget::formatSlStringIntoHtmlString(QString& slString) {
	
	slString.replace("\\[","<b>").replace("\\]","</b>").replace("\\(","<i>").replace("\\)","</i>").replace("\\<","<font color='#0A7700'>").replace("\\>","</font>");
	slString.replace(QRegExp("\\_(.*)\\_"),"<u>\\1</u>");
	slString.replace(QRegExp("\\@(.*)\\@"),"<u><font color='#0000FF'>\\1</font></u>");
	slString.replace("\\<u","<u");
	slString.replace("\\</u>","</u>");
	slString.replace("\\</font>","</font>");
	
	QString htmlString;
	
	int blocksCount(0);
	for (int i = 0; i < slString.size(); i++) {
		if (slString.at(i) == '{') {
			if (blocksCount > 0)
				htmlString += "<br>&nbsp;&nbsp;&nbsp;";
			else
				htmlString += "<br>";
			blocksCount++;
		} else if (slString.at(i) == '}') {
			blocksCount--;
		} else if (slString.at(i) != '\\')
			htmlString += slString.at(i);
	}
	if (htmlString.startsWith("<br>"))
		htmlString.remove(0,4);
	slString = htmlString;
}

void TabWidget::formatHtmlStringIntoSlString(QString& htmlString) {
	htmlString.remove("</p>");
	htmlString.remove("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">");
	htmlString.remove("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">");
	htmlString.remove("p, li { white-space: pre-wrap; }");
	htmlString.remove("</style></head><body style=\" font-family:'DejaVu Sans'; font-size:9pt; font-weight:400; font-style:normal;\">");
	htmlString.remove("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">");
	htmlString.remove("</body></html>");
	
	// For office words
	htmlString.replace("<span style=\" color:#0a7700;\">","\\<");
	// For italic
	htmlString.replace("<span style=\" font-style:italic;\">","\\(");
	// For bold
	htmlString.replace("<span style=\" font-weight:600;\">","\\[");
	// For underline
	htmlString.replace("<span style=\" text-decoration: underline;\">","\\_");
	// For bold and italic text
	htmlString.replace("<span style=\" font-weight:600; font-style:italic;\">","\\BI");
	// For bold and underline
	htmlString.replace("<span style=\" font-weight:600; text-decoration: underline;\">","\\BU");
	// For italic and underline
	htmlString.replace("<span style=\" font-style:italic; text-decoration: underline;\">","\\IU");
	// For Link
	htmlString.replace("<span style=\" text-decoration: underline; color:#0000ff;\">","\\@");
	
	QStack<QString> stack;
	int size = htmlString.size();
	QMap<int,QString> indexesMap;
	for (int i = 0; i < size-2; i++) {
		if (htmlString.at(i) == '\\') {
			if (htmlString.at(i+1) == '<')
				stack.push("\\>");
			else if (htmlString.at(i+1) == '(')
				stack.push("\\)");
			else if (htmlString.at(i+1) == '[')
				stack.push("\\]");
			else if (htmlString.at(i+1) == '_')
				stack.push("\\_");
			else if (htmlString.at(i+1) == 'B' && htmlString.at(i+2) == 'I')
				stack.push("\\IB");
			else if (htmlString.at(i+1) == 'B' && htmlString.at(i+2) == 'U')
				stack.push("\\UB");
			else if (htmlString.at(i+1) == 'I' && htmlString.at(i+2) == 'U')
				stack.push("\\UI");
			else if (htmlString.at(i+1) == '@')
				stack.push("\\@");
		}
		else if (htmlString.mid(i,7) == "</span>")
			indexesMap[i+1] = stack.pop();
	}
	
	QMap<int, QString>::const_iterator j = indexesMap.constBegin();
	while (j != indexesMap.constEnd()) {
		htmlString.insert(j.key(),j.value());
		++j;
	}
	
	htmlString.replace("\\BI","\\[\\(");
	htmlString.replace("\\BU","\\[\\_");
	htmlString.replace("\\IU","\\(\\_");
	htmlString.replace("\\IB","\\)\\]");
	htmlString.replace("\\UB","\\_\\]");
	htmlString.replace("\\UI","\\_\\)");
	
	htmlString.remove(QRegExp("<span (.*)>"));
	htmlString.remove("</span>");
	
	htmlString.replace("<br />   ","\\}\\{");
	htmlString.replace("<br />","\\}");
	
	QList<int> indexes;
	int blocksCount(0);
	for (int i = 0; i < htmlString.size(); i++) {
		if (htmlString.at(i) == '{')
			blocksCount++;
		if (htmlString.at(i) == '}') {
			blocksCount--;
			if (blocksCount < 0) {
				indexes << i - 1;
				blocksCount++;
			}
		}
	}
	for (int i = indexes.size() - 1; i >= 0; i--)
		htmlString.remove(indexes[i],2);
	
	indexes.clear();
	for (int i = 0; i < htmlString.size(); i++)
		if (htmlString.at(i) == '}')
			if (htmlString.at(i+1) != '\\' && htmlString.at(i+2) != '{')
				indexes << i + 1;
	for (int i = indexes.size() - 1; i >= 0; i--)
		htmlString.insert(indexes[i],"\\}\\{");
	htmlString.insert(0,"\\{");
	htmlString += "\\}";
}


void TabWidget::updateTranslation() {
	timer->stop();
	if (!lineEdit->text().isEmpty()) {
		QString translation = databaseCenter->getTranslationForWord(lineEdit->text().toLower());
		bool wasFocus = textEdit->hasFocus();
		if (!translation.isEmpty()) {
			// Format blocks into indents
			int blocksCount(0);
			int size = translation.size();
			for (int i = size-1; i >= 0; i--) {
				if (translation.at(i) == '{') {
					if (blocksCount > 0)
						translation.insert(i,"\n   ");
					else
						translation.insert(i,"\n");
					blocksCount++;
				} else if (translation.at(i) == '}') {
					blocksCount--;
				}
			}
			translation.remove("\\{");
			translation.remove("\\}");
			textEdit->setText(translation);
		}
		else
			textEdit->clear();
		if (wasFocus)
			textEdit->setFocus();
	} else
		textEdit->clear();
	emit(renameTab(tabIndex,lineEdit->text()));
}

void TabWidget::resetButtonsAccessibility() {
	if (!lineEdit->text().isEmpty()) {
		bool translationIsEmpty = databaseCenter->getTranslationForWord(lineEdit->text().toLower()).isEmpty();
		addWordButton->setEnabled(translationIsEmpty);
		editWordButton->setEnabled(!translationIsEmpty);
		removeWordButton->setEnabled(!translationIsEmpty);
		updateTranslationButton->setEnabled(true);
	} else {
		addWordButton->setEnabled(false);
		editWordButton->setEnabled(false);
		removeWordButton->setEnabled(false);
		updateTranslationButton->setEnabled(false);
	}
}

void TabWidget::addWord() {
	QString word = lineEdit->text().toLower();
	QString translation = textEdit->toPlainText().trimmed();
	if (!translation.isEmpty()) {
		if (databaseCenter->addNewWord(word,translation))
			emit(showStatusMessage(tr("You have added the new word \"%1\"").arg(word)));
		else
			emit(showStatusMessage(tr("Cannot add the new word \"%1\"").arg(word)));
		resetButtonsAccessibility();
	} else
		textEdit->setFocus();
}

void TabWidget::editWord() {
	QString word = lineEdit->text().toLower();
	QString translation = textEdit->toPlainText().trimmed();
	if (!translation.isEmpty()) {
		if (databaseCenter->setTranslationForWord(word,translation))
			emit(showStatusMessage(tr("You have edited the word \"%1\"").arg(word)));
		else
			emit(showStatusMessage(tr("Cannot edit the word \"%1\"").arg(word)));
	}
}

void TabWidget::removeWord() {
	if (databaseCenter->removeWord(lineEdit->text().toLower()))
		emit(showStatusMessage(tr("You have removed the word \"%1\"").arg(lineEdit->text())));
	else
		emit(showStatusMessage(tr("Cannot remove the word \"%1\"").arg(lineEdit->text())));
	resetButtonsAccessibility();
}

void TabWidget::setFocusAtThisTab() {
	if (!lineEdit->hasFocus() && !textEdit->hasFocus())
		lineEdit->setFocus();
}

void TabWidget::setEditorMenu(Menu *menu) {
	textEdit->setMenu(menu);
}

void TabWidget::translationChanged() {
	if (timer->isActive())
		updateTranslation();
}

void TabWidget::setUpdateTranslationInterval(int interval) {
	updateTranslationDuringEntering = interval != 0;
	timer->setInterval(interval);
}

void TabWidget::hideTips() {
	editorTipsWidget->hide();
}
