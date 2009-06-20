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
#include "FindInTranslationPanel.h"
#include "HighLighter.h"
#include "TabWidget.h"


TabWidget::TabWidget(QString firstWord,DatabaseCenter *dbCenter,int index,int updateTranslationInterval) {
	databaseCenter = dbCenter;
	
	tabIndex = index;
	
	highlighter = 0;
	
	findInTranslationPanel = new FindInTranslationPanel;
	findInTranslationPanel->hide();
	
	timer = new QTimer;
	setUpdateTranslationInterval(updateTranslationInterval);
	connect(timer,SIGNAL(timeout()),this,SLOT(updateTranslation()));
	
	notifLabel = new QLabel(tr("This word wasn't found in the dictionary. You can add it(by Ctrl+Enter), but firstly enter a translation."));
	notifLabel->hide();
	
	textEdit = new TranslationEditor;
	connect(textEdit,SIGNAL(showFindPanel()),this,SLOT(showSearchingPanel()));
	
	connect(findInTranslationPanel,SIGNAL(closed()),textEdit,SLOT(setFocus()));
	
	lineEdit = new QLineEdit;
	lineEdit->setToolTip(tr("Enter phrase here, which you want to translate"));
	connect(lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(textChanged(const QString&)));
	connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(updateTranslation()));
	
	addWordToolButton = new QToolButton;
	addWordToolButton->setAutoRaise(true);
	addWordToolButton->setIcon(QIcon(":/icons/add.png"));
	addWordToolButton->setToolTip(tr("Add the phrase into dictionary database. Shortcut: Ctrl+Enter"));
	addWordToolButton->setShortcut(QKeySequence("Ctrl+Return"));
	addWordToolButton->setIconSize(QSize(16,16));
	connect(addWordToolButton,SIGNAL(clicked()),this,SLOT(addWord()));
	
	editWordToolButton = new QToolButton;
	editWordToolButton->setAutoRaise(true);
	editWordToolButton->setIcon(QIcon(":/icons/edit.png"));
	editWordToolButton->setShortcut(QKeySequence("Ctrl+S"));
	editWordToolButton->setIconSize(QSize(16,16));
	editWordToolButton->setToolTip(tr("Edit the phrase. Shourtcut: Ctrl+S"));
	connect(editWordToolButton,SIGNAL(clicked()),this,SLOT(editWord()));
	
	removeWordToolButton = new QToolButton;
	removeWordToolButton->setAutoRaise(true);
	removeWordToolButton->setIcon(QIcon(":/icons/remove.png"));
	removeWordToolButton->setShortcut(QKeySequence("Ctrl+Delete"));
	removeWordToolButton->setIconSize(QSize(16,16));
	removeWordToolButton->setToolTip(tr("Remove the phrase from dictionary database. Shortcut: Ctrl+Delete"));
	connect(removeWordToolButton,SIGNAL(clicked()),this,SLOT(removeWord()));
	
	updateTranslationButton = new QToolButton;
	updateTranslationButton->setIcon(QIcon(":/icons/update.png"));
	updateTranslationButton->setShortcut(QKeySequence("Enter"));
	updateTranslationButton->setEnabled(false);
	updateTranslationButton->setIconSize(QSize(16,16));
	updateTranslationButton->setToolTip(tr("Update translation"));
	connect(updateTranslationButton,SIGNAL(clicked()),this,SLOT(updateTranslation()));
	updateTranslationButton->setFocusPolicy(Qt::ClickFocus);
	
	addWordToolButton->setEnabled(false);
	editWordToolButton->setEnabled(false);
	removeWordToolButton->setEnabled(false);
	
	connect(textEdit,SIGNAL(textChanged()),this,SLOT(translationChanged()));
	
	clearLineEditButton = new QToolButton;
	clearLineEditButton->setAutoRaise(true);
	clearLineEditButton->setIcon(QIcon(":/icons/clear.png"));
	clearLineEditButton->setIconSize(QSize(16,16));
	connect(clearLineEditButton,SIGNAL(clicked()),lineEdit,SLOT(clear()));
	connect(clearLineEditButton,SIGNAL(clicked()),lineEdit,SLOT(setFocus()));
	clearLineEditButton->setFocusPolicy(Qt::ClickFocus);
	
	connect(findInTranslationPanel,SIGNAL(searchSignal(const QString&)),textEdit,SLOT(findFirst(const QString&)));
	connect(findInTranslationPanel,SIGNAL(findNextRequestSignal(const QString&)),textEdit,SLOT(findNext(const QString&)));
	connect(findInTranslationPanel,SIGNAL(findPreviousRequestSignal(const QString&)),textEdit,SLOT(findPrevious(const QString&)));
	connect(textEdit,SIGNAL(setRedPalette()),findInTranslationPanel,SLOT(setRedPalette()));
	connect(textEdit,SIGNAL(setGreenPalette()),findInTranslationPanel,SLOT(setGreenPalette()));
	connect(textEdit,SIGNAL(setDefaultPalette()),findInTranslationPanel,SLOT(setDefaultPalette()));
	
	QVBoxLayout *toolButtonsLayout = new QVBoxLayout;
	toolButtonsLayout->addWidget(updateTranslationButton);
	toolButtonsLayout->addWidget(addWordToolButton);
	toolButtonsLayout->addWidget(editWordToolButton);
	toolButtonsLayout->addWidget(removeWordToolButton);
	toolButtonsLayout->addStretch();
	toolButtonsLayout->setContentsMargins(0,0,0,0);
	
	QHBoxLayout *lineEditLayout = new QHBoxLayout;
	lineEditLayout->addWidget(new QLabel(tr("Word") + ": "));
	lineEditLayout->addWidget(lineEdit,1);
	lineEditLayout->addWidget(clearLineEditButton);
	
	QVBoxLayout *verticalLayout = new QVBoxLayout;
	verticalLayout->addLayout(lineEditLayout);
	verticalLayout->addWidget(notifLabel);
	verticalLayout->addWidget(textEdit);
	
	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addLayout(verticalLayout);
	horizontalLayout->addLayout(toolButtonsLayout);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(horizontalLayout);
	mainLayout->addWidget(findInTranslationPanel);
	mainLayout->setContentsMargins(3,3,3,3);
	setLayout(mainLayout);
	
	if (!firstWord.isEmpty()) {
		lineEdit->setText(firstWord);
		updateTranslation();
	}
}

TabWidget::~TabWidget() {
	if (highlighter != 0)
		delete highlighter;
	delete updateTranslationButton;
	delete addWordToolButton;
	delete editWordToolButton;
	delete removeWordToolButton;
	delete textEdit;
	delete lineEdit;
	delete notifLabel;
	delete clearLineEditButton;
}

void TabWidget::setHtml(const QString& htmlText) {
	textEdit->setHtml(htmlText);
}

void TabWidget::setReadOnly(bool readOnly) {
	textEdit->setReadOnly(readOnly);
}

void TabWidget::textChanged(const QString&) {
	timer->stop();
	if (updateTranslationDuringEntering)
		timer->start();
	resetButtonsAccessibility();
}

void TabWidget::updateTranslation() {
	timer->stop();
	notifLabel->hide();
	if (!lineEdit->text().isEmpty()) {
		QString translation = databaseCenter->getTranslationForWord(lineEdit->text().toLower());
		bool wasFocus = textEdit->hasFocus();
		if (!translation.isEmpty()) {
			// Format blocks into indents
			translation.replace("\\{","\n\\{\n\t");
			translation.replace("\\}","\n\\}");
			textEdit->setText(translation);
		}
		else {
			textEdit->clear();
			notifLabel->show();
		}
		if (wasFocus)
			textEdit->setFocus();
	} else
		textEdit->clear();
	emit(renameTab(tabIndex,lineEdit->text()));
}

void TabWidget::resetButtonsAccessibility() {
	if (!lineEdit->text().isEmpty()) {
		bool translationIsEmpty = databaseCenter->getTranslationForWord(lineEdit->text().toLower()).isEmpty();
		addWordToolButton->setEnabled(translationIsEmpty);
		editWordToolButton->setEnabled(!translationIsEmpty);
		removeWordToolButton->setEnabled(!translationIsEmpty);
		updateTranslationButton->setEnabled(true);
	} else {
		addWordToolButton->setEnabled(false);
		editWordToolButton->setEnabled(false);
		removeWordToolButton->setEnabled(false);
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

void TabWidget::showSearchingPanel() {
	findInTranslationPanel->show();
	findInTranslationPanel->setLineEditFocus();
}

void TabWidget::useHighlighting(bool highlighting) {
	if (highlighting) {
		if (highlighter == 0)
			highlighter = new HighLighter(textEdit->document());
		else
			highlighter->setDocument(textEdit->document());
	} else
		if (highlighter != 0)
			highlighter->setDocument(0);
}

void TabWidget::undo() {
	textEdit->undo();
}

void TabWidget::redo() {
	textEdit->redo();
}

void TabWidget::cut() {
	textEdit->cut();
}

void TabWidget::copy() {
	textEdit->copy();
}

void TabWidget::paste() {
	textEdit->paste();
}

QString TabWidget::getTranslationAsHtml() {
	QString text = textEdit->toPlainText();
	text.replace("\\[","<b>").replace("\\]","</b>").replace("\\(","<i>")
	    .replace("\\)","</i>").replace("\\<","<font color=green>").replace("\\>","</font>")
	    .replace("\\{","<br>").replace("\n\\}","<br>").replace(QRegExp("\\\\s.*\\\\s"),QString("<img src=\"%1\">").arg(":/icons/sound.png"));
	text.replace(QRegExp("\\_(.*)\\_"),"<u>\\1</u>");
	text.replace(QRegExp("\\@(.*)\\@"),"<u><font color='#0000FF'>\\1</font></u>");
	text.replace("\\<u","<u");
	text.replace("\\</u>","</u>");
	text.replace("\\</font>","</font>");
	return text;
}
