#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include "DialogForQuastions.h"

DialogForQuastions::DialogForQuastions(Mode mode,QWidget *parent) : QDialog(parent) {
	textLabel = new QLabel;
	textLabel->setWordWrap(true);
	iconLabel = new QLabel;
	iconLabel->hide();
	
	saveButton = 0;
	dontSaveButton = 0;
	
	cancelButton =  new QPushButton(tr("Cancel"));
	cancelButton->setShortcut(QKeySequence("Esc"));
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelWasClicked()));
	
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	
	switch (mode) {
		case SaveOrNotDocument:
			saveButton = new QPushButton(tr("Save"));
			connect(saveButton,SIGNAL(clicked()),this,SLOT(saveWasClicked()));
		
			dontSaveButton = new QPushButton(tr("Don't save"));
			connect(dontSaveButton,SIGNAL(clicked()),this,SLOT(dontSaveWasClicked()));
		
			buttonsLayout->addWidget(saveButton);
			buttonsLayout->addWidget(dontSaveButton);
			buttonsLayout->addStretch();
			buttonsLayout->addWidget(cancelButton);
			resize(400,125);
			break;
		case 
	}
	
	QVBoxLayout *iconLabelLayout = new QVBoxLayout;
	iconLabelLayout->addWidget(iconLabel);
	iconLabelLayout->addStretch();
	
	QVBoxLayout *textLabelLayout = new QVBoxLayout;
	textLabelLayout->addWidget(textLabel);
	textLabelLayout->addStretch();
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addLayout(iconLabelLayout);
	topLayout->addLayout(textLabelLayout,1);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);
}

DialogForQuastions::~DialogForQuastions() {
	delete textLabel;
	delete iconLabel;
	delete cancelButton;
	
	if (saveButton) {
		delete saveButton;
		delete dontSaveButton;
	}
}

Result DialogForQuastions::getResult() const {
	return currentResult;
}

void DialogForQuastions::cancelWasClicked() {
	currentResult = Cancel;
	hide();
}

void DialogForQuastions::saveWasClicked() {
	currentResult = Save;
	hide();
}

void DialogForQuastions::dontSaveWasClicked() {
	currentResult = DontSave;
	hide();
}

void DialogForQuastions::setText(const QString& text) {
	textLabel->setText(text);
}

void DialogForQuastions::setIcon(const QIcon& icon) {
	iconLabel->show();
	iconLabel->setPixmap(icon.pixmap(64,64));
}
