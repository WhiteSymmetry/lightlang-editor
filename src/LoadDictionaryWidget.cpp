#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include "ProgressBarWithWidgets.h"
#include "LoadDictionaryWidget.h"

LoadDictionaryWidget::LoadDictionaryWidget() : BorderPanelWithWidget(BorderPanelWithWidget::Horizontal) {
	progressBar = new ProgressBarWithWidgets;
	progressBar->setTextVisible(false);
	
	stopLoadingButton = new QPushButton;
	stopLoadingButton->setIcon(QIcon(":/icons/stop.png"));
	stopLoadingButton->setFlat(true);
	stopLoadingButton->setToolTip(tr("Stop loading"));
	
	cancelLoadingButton = new QPushButton;
	cancelLoadingButton->setIcon(QIcon(":/icons/cancel.png"));
	cancelLoadingButton->setFlat(true);
	cancelLoadingButton->setToolTip(tr("Cancel loading"));
	
	progressBar->addWidget(cancelLoadingButton);
	progressBar->addStretch();
	progressBar->addWidget(new QLabel(tr("Dictionary loading") + "..."));
	progressBar->addStretch();
	progressBar->addWidget(stopLoadingButton);
	
	setWidget(progressBar);
	
	connect(cancelLoadingButton,SIGNAL(clicked()),this,SIGNAL(canceled()));
	connect(stopLoadingButton,SIGNAL(clicked()),this,SIGNAL(stopped()));
}

LoadDictionaryWidget::~LoadDictionaryWidget() {
	delete cancelLoadingButton;
	delete stopLoadingButton;
	delete progressBar;
}

void LoadDictionaryWidget::addValue() {
	progressBar->setValue(progressBar->value() + 1);
}

void LoadDictionaryWidget::setMaximum(int max) {
	progressBar->setRange(0,max);
}

void LoadDictionaryWidget::reset() {
	progressBar->reset();
}
