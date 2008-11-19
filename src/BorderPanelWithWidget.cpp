#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include "BorderPanelWithWidget.h"

BorderPanelWithWidget::BorderPanelWithWidget(Orientation defaultOrientation) : widget(0) {
	hideButton = new QPushButton;
	hideButton->setFlat(true);
	connect(hideButton,SIGNAL(clicked()),this,SLOT(hideOrShow()));
	
	mainLayout = 0;
	
	setOrientation(defaultOrientation);
}

BorderPanelWithWidget::~BorderPanelWithWidget() {
	delete hideButton;
	delete mainLayout;
}

void BorderPanelWithWidget::setWidget(QWidget *w) {
	widget = w;
	setOrientation(currentOrientation);
}

QWidget *BorderPanelWithWidget::getWidget() const {
	return widget;
}

void BorderPanelWithWidget::hideOrShow() {
	if (widget)
		widget->setVisible(widget->isHidden());
	updateHideButtonIcon();
}

void BorderPanelWithWidget::setOrientation(Orientation orientation) {
	if (mainLayout)
		delete mainLayout;
	
	if (orientation == Vertical) {
		mainLayout = new QHBoxLayout;
		hideButton->setMaximumWidth(20);
		hideButton->setMaximumHeight(QWIDGETSIZE_MAX);
		hideButton->setIcon(QIcon(":/icons/left.png"));
	}
	else {
		mainLayout = new QVBoxLayout;
		hideButton->setMaximumHeight(20);
		hideButton->setMaximumWidth(QWIDGETSIZE_MAX);
		hideButton->setIcon(QIcon(":/icons/down.png"));
	}
	
	if (widget)
		mainLayout->addWidget(widget);
	mainLayout->addWidget(hideButton);
	mainLayout->setMargin(0);
	setLayout(mainLayout);
	
	currentOrientation = orientation;
	updateHideButtonIcon();
}

int BorderPanelWithWidget::getOrientation() const {
	return currentOrientation;
}

void BorderPanelWithWidget::updateHideButtonIcon() {
	if (widget == 0 || !isVisible())
		return;
	
	if (currentOrientation == Vertical)
		hideButton->setIcon(QIcon(widget->isVisible() ? ":/icons/left.png" : ":/icons/right.png"));
	else
		hideButton->setIcon(QIcon(widget->isVisible() ? ":/icons/down.png" : ":/icons/up.png"));
}