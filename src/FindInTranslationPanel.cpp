#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QKeyEvent>
#include "FindInTranslationPanel.h"

FindInTranslationPanel::FindInTranslationPanel(QWidget *parent) : QWidget(parent) {
	lineEdit = new QLineEdit;
	connect(lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(search(const QString&)));
	
	closePanelButton = new QToolButton;
	closePanelButton->setIcon(QIcon(":/icons/close.png"));
	closePanelButton->setAutoRaise(true);
	closePanelButton->setIconSize(QSize(16,16));
	connect(closePanelButton,SIGNAL(clicked()),this,SLOT(hide()));
	
	clearLineButton = new QToolButton;
	clearLineButton->setIcon(QIcon(":/icons/clear.png"));
	clearLineButton->setEnabled(false);
	clearLineButton->setAutoRaise(true);
	clearLineButton->setIconSize(QSize(16,16));
	connect(clearLineButton,SIGNAL(clicked()),lineEdit,SLOT(clear()));
	connect(clearLineButton,SIGNAL(clicked()),lineEdit,SLOT(setFocus()));
	
	previousEntryButton = new QToolButton;
	previousEntryButton->setIcon(QIcon(":/icons/backward.png"));
	previousEntryButton->setEnabled(false);
	previousEntryButton->setAutoRaise(true);
	previousEntryButton->setIconSize(QSize(16,16));
	previousEntryButton->setShortcut(QKeySequence("Shift+F3"));
	connect(previousEntryButton,SIGNAL(clicked()),this,SLOT(findPreviousRequest()));
	
	nextEntryButton = new QToolButton;
	nextEntryButton->setIcon(QIcon(":/icons/forward.png"));
	nextEntryButton->setEnabled(false);
	nextEntryButton->setAutoRaise(true);
	nextEntryButton->setIconSize(QSize(16,16));
	nextEntryButton->setShortcut(QKeySequence("F3"));
	connect(nextEntryButton,SIGNAL(clicked()),this,SLOT(findNextRequest()));
	connect(lineEdit,SIGNAL(returnPressed()),nextEntryButton,SLOT(animateClick()));
	
	verticalFrame1 = new QFrame;
	verticalFrame1->setFrameStyle(QFrame::VLine | QFrame::Sunken);
	verticalFrame1->setMinimumSize(10,22);
	
	verticalFrame2 = new QFrame;
	verticalFrame2->setFrameStyle(QFrame::VLine | QFrame::Sunken);
	verticalFrame2->setMinimumSize(10,22);
	
	defaultPalette = lineEdit->palette();
	redPalette.setColor(QPalette::Base,QColor(255,110,110));
	greenPalette.setColor(QPalette::Base,QColor(110,255,110));
		
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(closePanelButton);
	mainLayout->addWidget(verticalFrame1);
	mainLayout->addWidget(new QLabel(tr("Search") + ":"));
	mainLayout->addWidget(lineEdit,1);
	mainLayout->addWidget(clearLineButton);
	mainLayout->addWidget(verticalFrame2);
	mainLayout->addWidget(previousEntryButton);
	mainLayout->addWidget(nextEntryButton);
	mainLayout->setContentsMargins(0,0,0,0);
	setLayout(mainLayout);
}

FindInTranslationPanel::~FindInTranslationPanel() {
	delete lineEdit;
	delete closePanelButton;
	delete clearLineButton;
	delete previousEntryButton;
	delete nextEntryButton;
	delete verticalFrame1;
	delete verticalFrame2;
}

void FindInTranslationPanel::search(const QString& expression) {
	clearLineButton->setEnabled(!expression.isEmpty());
	previousEntryButton->setEnabled(!expression.isEmpty());
	nextEntryButton->setEnabled(!expression.isEmpty());
	emit (searchSignal(expression));
}

void FindInTranslationPanel::setRedPalette() {
	lineEdit->setPalette(redPalette);
}

void FindInTranslationPanel::setGreenPalette() {
	lineEdit->setPalette(greenPalette);
}

void FindInTranslationPanel::setDefaultPalette() {
	lineEdit->setPalette(defaultPalette);
}

void FindInTranslationPanel::findNextRequest() {
	QString expression = lineEdit->text();
	if (expression.isEmpty())
		return;
	emit (findNextRequestSignal(expression));
}

void FindInTranslationPanel::findPreviousRequest() {
	QString expression = lineEdit->text();
	if (expression.isEmpty())
		return;
	emit (findPreviousRequestSignal(expression));
}

void FindInTranslationPanel::keyPressEvent(QKeyEvent *keyEvent) {
	if (keyEvent->key() == Qt::Key_Escape)
		hide();
	QWidget::keyPressEvent(keyEvent);
}

void FindInTranslationPanel::setLineEditFocus() {
	lineEdit->setFocus();
}

void FindInTranslationPanel::hideEvent(QHideEvent* ) {
	emit (wasHidden());
}
