#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QFrame>
#include <QtCore/QTimer>
#include "TranslationEditor.h"
#include "DatabaseCenter.h"
#include "TabWidget.h"

const int TIMER_INTERVAL = 800;

TabWidget::TabWidget(DatabaseCenter *dbCenter,int index) {
	databaseCenter = dbCenter;
	
	tabIndex = index;
	
	timer = new QTimer;
	timer->setInterval(TIMER_INTERVAL);
	connect(timer,SIGNAL(timeout()),this,SLOT(updateTranslation()));
	
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
	removeWordButton->setShortcut(QKeySequence("Ctrl+Backspace"));
	removeWordButton->setToolTip(tr("Remove the phrase from dictionary database. Shortcut: Ctrl+Backspace"));
	connect(removeWordButton,SIGNAL(clicked()),this,SLOT(removeWord()));
	
	updateTranslationButton = new QToolButton;
	updateTranslationButton->setIcon(QIcon(":/icons/search.png"));
	updateTranslationButton->setShortcut(QKeySequence("Return"));
	connect(updateTranslationButton,SIGNAL(clicked()),this,SLOT(updateTranslation()));
	
	addWordButton->setEnabled(false);
	editWordButton->setEnabled(false);
	removeWordButton->setEnabled(false);
	
	QFrame *horizontalFrame = new QFrame;
	horizontalFrame->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	
	textEdit->addWidget(addWordButton);
	textEdit->addWidget(editWordButton);
	textEdit->addWidget(horizontalFrame);
	textEdit->addWidget(removeWordButton);
	
	clearLineEditButton = new QToolButton;
	clearLineEditButton->setAutoRaise(true);
	clearLineEditButton->setIcon(QIcon(":/icons/clear.png"));
	connect(clearLineEditButton,SIGNAL(clicked()),lineEdit,SLOT(clear()));
	
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
	timer->start();
}

void TabWidget::updateTranslation() {
	timer->stop();
	if (!lineEdit->text().isEmpty()) {
		QString translation = databaseCenter->getTranslationForWord(lineEdit->text());
		textEdit->setText(translation);
		
		addWordButton->setEnabled(translation.isEmpty());
		editWordButton->setEnabled(!translation.isEmpty());
		removeWordButton->setEnabled(!translation.isEmpty());
	} else {
		addWordButton->setEnabled(false);
		editWordButton->setEnabled(false);
		removeWordButton->setEnabled(false);
		textEdit->clear();
	}
	emit(renameTab(tabIndex,lineEdit->text()));
}

void TabWidget::addWord() {
	databaseCenter->addNewWord(lineEdit->text(),textEdit->toPlainText());
	updateTranslation();
}

void TabWidget::editWord() {
	databaseCenter->setTranslationForWord(lineEdit->text(),textEdit->toPlainText());
}

void TabWidget::removeWord() {
	databaseCenter->removeWord(lineEdit->text());
	updateTranslation();
}

void TabWidget::setFocusAtThisTab() {
	if (!lineEdit->hasFocus() && !textEdit->hasFocus())
		lineEdit->setFocus();
}
