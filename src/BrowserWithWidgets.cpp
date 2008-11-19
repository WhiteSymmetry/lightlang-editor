#include <QtGui/QGridLayout>
#include <QtGui/QFrame>
#include "BrowserWithWidgets.h"

BrowserWithWidgets::BrowserWithWidgets(QWidget *parent) : QTextBrowser(parent) {
	widgetsFrame = new QFrame;
	widgetsFrame->setFrameShape(QFrame::Box);
	widgetsFrame->setFrameShadow(QFrame::Raised);
	widgetsFrame->setStyleSheet("QFrame {border: 4px solid gray; border-radius: 14px; background-color: rgb(0, 0, 0, 30)}");
	
	widgetsFrameLayout = 0;
	mainLayout = 0;
		
	setPosition(Center);
	setOrientation(Horizontal);
}

BrowserWithWidgets::~BrowserWithWidgets() {
	delete widgetsFrameLayout;
	delete widgetsFrame;
	delete mainLayout;
}

void BrowserWithWidgets::addWidget(QWidget *newWidget) {
	widgets << newWidget;
	widgetsFrameLayout->addWidget(newWidget);
	widgetsFrame->setFixedSize(widgetsFrameLayout->minimumSize());
}

void BrowserWithWidgets::setOrientation(Orientation orientation) {
	
	if (widgetsFrameLayout)
		delete widgetsFrameLayout;
	
	if (orientation == Vertical)
		widgetsFrameLayout = new QVBoxLayout;
	else
		widgetsFrameLayout = new QHBoxLayout;
	
	foreach (QWidget *widget,widgets)
		widgetsFrameLayout->addWidget(widget);
	
	widgetsFrame->setLayout(widgetsFrameLayout);
}

void BrowserWithWidgets::setPosition(Position position) {
	
	if (mainLayout)
		delete mainLayout;
	
	mainLayout = new QGridLayout;
	
	switch (position) {
		case LeftTopCorner:
			mainLayout->addWidget(widgetsFrame,0,0,Qt::AlignTop | Qt::AlignLeft);
			break;
		case Top:
			mainLayout->addWidget(widgetsFrame,0,0,Qt::AlignHCenter | Qt::AlignTop);
			break;
		case RightTopCorner:
			mainLayout->addWidget(widgetsFrame,0,0,Qt::AlignTop | Qt::AlignRight);
			break;
		case Left:
			mainLayout->addWidget(widgetsFrame,0,0,Qt::AlignVCenter);
			mainLayout->setColumnStretch(1,1);
			break;
		case Right:
			mainLayout->addWidget(widgetsFrame,0,1,Qt::AlignVCenter);
			mainLayout->setColumnStretch(0,1);
			break;
		case LeftBottomCorner:
			mainLayout->addWidget(widgetsFrame,1,0);
			mainLayout->setRowStretch(0,1);
			mainLayout->setColumnStretch(1,1);
			break;
		case Bottom:
			mainLayout->addWidget(widgetsFrame,1,0,Qt::AlignHCenter);
			mainLayout->setRowStretch(0,1);
			break;
		case RightBottomCorner:
			mainLayout->addWidget(widgetsFrame,1,1,Qt::AlignRight);
			mainLayout->setRowStretch(0,1);
			mainLayout->setColumnStretch(0,1);
			break;
		case Center:
			mainLayout->addWidget(widgetsFrame,0,0);
			break;
	}
	setLayout(mainLayout);
}
