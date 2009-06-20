#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>
#include "About.h"
#include "const.h"

About::About(QWidget *parent) : QDialog(parent) {
	iconLabel = new QLabel;
	iconLabel->setPixmap(QIcon(":/icons/lle.png").pixmap(100,100));
		
	textLabel = new QLabel;
	textLabel->setWordWrap(true);
	
	textLabel->setText("<center><h3>" + tr("LightLang Editor") + "</h3></center>"
					+ "<b>" + tr("Developers") + ":</b> <br>"
						+ "&nbsp;&nbsp;&nbsp;&nbsp;<i>Tikhonov Sergey</i><br>"
					+ "<b>" + tr("Assistans") + ":</b> <br>"
						+ "&nbsp;&nbsp;&nbsp;&nbsp;<i>Devaev Maxim</i><br>"
						+ "&nbsp;&nbsp;&nbsp;&nbsp;<i>Nasyrov Renat</i><br>"
						+ "&nbsp;&nbsp;&nbsp;&nbsp;<i>Fomkin Vladimir</i><br>"
					+ "<b>" + tr("Testers") + ":</b> <br>"
						+ "&nbsp;&nbsp;&nbsp;&nbsp;<i>Kolbin Yaroslav</i><br>"
						+ "&nbsp;&nbsp;&nbsp;&nbsp;<i>Ursul Alexey</i><br>"
					+ "<font size='2'>" + tr("Copyright (c) 2007-2009 Tikhonov Sergey and ViaLinx Laboratories. %1 All offers and reports you can send to e-mail: <b>sstikhonov@gmail.com</b>").arg("<br>") + "</font>");
			
	setWindowTitle(tr("About LightLang Editor"));
	
	QHBoxLayout *iconLayout = new QHBoxLayout;
	iconLayout->addStretch();
	iconLayout->addWidget(iconLabel);
	iconLayout->addStretch();
	
	mainLayout = new QVBoxLayout;
	mainLayout->addLayout(iconLayout);
	mainLayout->addWidget(textLabel);
	
	setLayout(mainLayout);
	setFixedSize(sizeHint());
}

About::~About() {
	delete iconLabel;
	delete textLabel;
	
	delete mainLayout;
}

