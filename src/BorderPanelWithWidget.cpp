#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QTimer>
#include "BorderPanelWithWidget.h"

BorderPanelWithWidget::BorderPanelWithWidget(Orientation defaultOrientation) : widget(0) {
	timer = new QTimer;
	timer->setInterval(20);
	connect(timer,SIGNAL(timeout()),this,SLOT(updateWidgetSize()));
	rollToShowWidget = false;
	
	hideButton = new QPushButton;
	hideButton->setFlat(true);
	connect(hideButton,SIGNAL(clicked()),this,SLOT(hideOrShow()));
	
	mainLayout = 0;
	
	setOrientation(defaultOrientation);
}

BorderPanelWithWidget::~BorderPanelWithWidget() {
	delete timer;
	delete hideButton;
	delete mainLayout;
}

void BorderPanelWithWidget::setWidget(QWidget *w) {
	widget = w;
	widget->setMaximumHeight(widget->sizeHint().height());
	setOrientation(currentOrientation);
}

QWidget *BorderPanelWithWidget::getWidget() const {
	return widget;
}

void BorderPanelWithWidget::hideOrShow() {
	if (widget) {
		if (widget->maximumHeight() == 0)
			rollToShowWidget = true;
		else
			rollToShowWidget = false;
		timer->start();
	}
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
		hideButton->setText(tr("Hide progress bar"));
	}
	
	mainLayout->addWidget(hideButton);	
	if (widget)
		mainLayout->addWidget(widget);
	mainLayout->setContentsMargins(0,0,0,0);
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
		hideButton->setIcon(QIcon(widget->maximumHeight() != 0 ? ":/icons/left.png" : ":/icons/right.png"));
	else {
		hideButton->setIcon(QIcon(widget->maximumHeight() != 0 ? ":/icons/down.png" : ":/icons/up.png"));
		if (widget->maximumHeight() != 0)
		    hideButton->setText(tr("Hide progress bar"));
		else
		    hideButton->setText(tr("Show progress bar"));
	}
}

void BorderPanelWithWidget::updateWidgetSize() {
	if (rollToShowWidget) {
		if (widget->height() + 10 >= widget->sizeHint().height())
			widget->setMaximumHeight(widget->sizeHint().height());
		else
			widget->setMaximumHeight(widget->height() + 10);
		if (widget->height() != widget->sizeHint().height())
			timer->start();
		else {
			timer->stop();
			updateHideButtonIcon();
		}
	} else {
		if (widget->height() - 10 <= 0)
			widget->setMaximumHeight(0);
		else
			widget->setMaximumHeight(widget->height() - 10);
		if (widget->height() != 0)
			timer->start();
		else {
			timer->stop();
			updateHideButtonIcon();
		}
	}
	widget->resize(widget->width(),widget->sizeHint().height());
}
