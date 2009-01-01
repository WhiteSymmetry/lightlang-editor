#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include "SettingsWidget.h"

SettingsWidget::SettingsWidget() {
	headerLabel = new QLabel("<font size='6'>" + tr("Settings") + "</font>");
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(headerLabel);
	mainLayout->addStretch();
	
	setLayout(mainLayout);
}

SettingsWidget::~SettingsWidget() {
	delete headerLabel;
}
