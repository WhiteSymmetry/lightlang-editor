#include <QtCore/QTimer>
#include "StackedWidget.h"

#define NORMAL_SPEED 50
#define VERY_SLOW_SPEED 100
#define SLOW_SPEED 75
#define FAST_SPEED 20
#define IMMEDIATELY_SPEED 1

StackedWidget::StackedWidget() {
	
	flipToNext = true;
	setHandleWidth(1);
	
	timer = new QTimer;
	connect(timer,SIGNAL(timeout()),this,SLOT(updateSizes()));
	
	setFlipSpeed(Fast);
	currentIndex = 0;
}

StackedWidget::~StackedWidget() {
	delete timer;
}

void StackedWidget::forward() {
	if (currentIndex < count()-1) {
		flipToNext = true;
		emit (backwardAvailable(isBackwardAvailable()));
		emit (forwardAvailable(isForwardAvailable()));
		timer->start();
	}
}

void StackedWidget::backward() {
	if (currentIndex > 0) {
		flipToNext = false;
		emit (backwardAvailable(isBackwardAvailable()));
		emit (forwardAvailable(isForwardAvailable()));
		timer->start();
	}
}

void StackedWidget::setCurrentIndex(int index) {
	if (index < 0)
		return;
	if (index > currentIndex)
		forward();
	else if (index < currentIndex)
		backward();
}

void StackedWidget::setCurrentWidget(QWidget *widget) {
	setCurrentIndex(indexOf(widget));
}

void StackedWidget::setFlipSpeed(FlipSpeed speed) {
	currentFlipSpeed = speed;
	switch (currentFlipSpeed) {
		case Normal:
			timer->setInterval(NORMAL_SPEED);
			break;
		case VerySlow:
			timer->setInterval(VERY_SLOW_SPEED);
			break;
		case Slow:
			timer->setInterval(SLOW_SPEED);
			break;
		case Fast:
			timer->setInterval(FAST_SPEED);
			break;
		case Immediately:
			timer->setInterval(IMMEDIATELY_SPEED);
			break;
	}
}

StackedWidget::FlipSpeed StackedWidget::getFlipSpeed() const {
	return currentFlipSpeed;
}

int StackedWidget::getCurrentIndex() const {
	return currentIndex;
}

QWidget* StackedWidget::getCurrentWidget() const {
	return widget(currentIndex);
}

void StackedWidget::updateSizes() {
	int  SPEED = int(width() / 3);
	QList<int> widgetsSizes;
	for (int i = 0; i < currentIndex; i++)
		widgetsSizes << 0;
	if (!flipToNext) {
		widgetsSizes.removeAt(widgetsSizes.count()-1);
		widgetsSizes << sizes()[currentIndex-1] + SPEED;
		widgetsSizes << sizes()[currentIndex] - SPEED;
		widgetsSizes << 0;
	} else {
		widgetsSizes << sizes()[currentIndex] - SPEED;
		widgetsSizes << sizes()[currentIndex+1] + SPEED;
	}
	for (int i = currentIndex + 2; i < count(); i++)
		widgetsSizes << 0;
	
	setSizes(widgetsSizes);
	
	if (widgetsSizes[currentIndex] != sizes()[currentIndex]) {
		widgetsSizes[currentIndex] = 0;
		setSizes(widgetsSizes);
	}
	
	if (widgetsSizes[currentIndex] <= 0) {
		timer->stop();
		if (flipToNext)
			currentIndex++;
		else
			currentIndex--;
	}
	else
		timer->start();
	
}

bool StackedWidget::isBackwardAvailable() const {
	return currentIndex > 0;
}

bool StackedWidget::isForwardAvailable() const {
	return currentIndex < count() - 1;
}

void StackedWidget::addNewWidget(QWidget *w) {
	addWidget(w);
	setCollapsible(count()-1,true);
	
	QList<int> widgetsSizes;
	for (int i = 0; i < currentIndex; i++)
		widgetsSizes << 0;
	widgetsSizes << widget(currentIndex)->width();
	for (int i = currentIndex + 1; i < count(); i++)
		widgetsSizes << 0;
	widgetsSizes << 0;
	setSizes(widgetsSizes);
}
