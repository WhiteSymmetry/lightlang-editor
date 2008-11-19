#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>
#include "About.h"

About::About(QWidget *parent) : QDialog(parent) {
	iconLabel = new QLabel;
	iconLabel->setPixmap(QPixmap(":/images/about.png"));
		
	textLabel = new QLabel;
	textLabel->setWordWrap(true);
	
	textLabel->setText(tr("Copyright (c) 2007-2008 Tikhonov Sergey and ViaLinx Laboratories. All offers and reports you can send to e-mail: <b>sstikhonov@gmail.com</b>"));
			
	setWindowTitle(tr("About the program"));
	
	QVBoxLayout *textLayout = new QVBoxLayout;
	textLayout->setMargin(5);
	textLayout->addWidget(textLabel);
	
	mainLayout = new QVBoxLayout;
	mainLayout->setMargin(0);
	mainLayout->addWidget(iconLabel);
	mainLayout->addLayout(textLayout);
	mainLayout->addStretch();
	
	setLayout(mainLayout);
	setFixedSize(sizeHint());
}

About::~About() {
	delete iconLabel;
	delete textLabel;
	
	delete mainLayout;
}
