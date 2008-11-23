#include <QtGui/QHBoxLayout>
#include "ProgressBarWithWidgets.h"

ProgressBarWithWidgets::ProgressBarWithWidgets() {
	mainLayout = new QHBoxLayout;
	mainLayout->setContentsMargins(0,0,0,0);
	setLayout(mainLayout);
}

ProgressBarWithWidgets::~ProgressBarWithWidgets() {
	delete mainLayout;
}

void ProgressBarWithWidgets::addWidget(QWidget *widget) {
	mainLayout->addWidget(widget);
}

void ProgressBarWithWidgets::addStretch() {
	mainLayout->addStretch();
}
