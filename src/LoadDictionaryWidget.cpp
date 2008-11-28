#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include "ProgressBarWithWidgets.h"
#include "LoadDictionaryWidget.h"

LoadDictionaryWidget::LoadDictionaryWidget() : BorderPanelWithWidget(BorderPanelWithWidget::Horizontal) {
	progressBar = new ProgressBarWithWidgets;
	progressBar->setTextVisible(false);
	
	pauseLoadingButton = new QPushButton;
	pauseLoadingButton->setIcon(QIcon(":/icons/pause.png"));
	pauseLoadingButton->setFlat(true);
	pauseLoadingButton->setToolTip(tr("Pause"));
	
	continueLoadingButton = new QPushButton;
	continueLoadingButton->setIcon(QIcon(":/icons/continue.png"));
	continueLoadingButton->setFlat(true);
	continueLoadingButton->setToolTip(tr("Continue"));
	continueLoadingButton->hide();
	
	cancelLoadingButton = new QPushButton;
	cancelLoadingButton->setIcon(QIcon(":/icons/cancel.png"));
	cancelLoadingButton->setFlat(true);
	cancelLoadingButton->setToolTip(tr("Cancel"));
	
	textLabel = new QLabel(tr("Dictionary loading") + "...");
	
	progressBar->addWidget(cancelLoadingButton);
	progressBar->addStretch();
	progressBar->addWidget(textLabel);
	progressBar->addStretch();
	progressBar->addWidget(continueLoadingButton);
	progressBar->addWidget(pauseLoadingButton);
	
	setWidget(progressBar);
	
	connect(cancelLoadingButton,SIGNAL(clicked()),this,SIGNAL(canceled()));
	connect(cancelLoadingButton,SIGNAL(clicked()),this,SLOT(hide()));
	connect(pauseLoadingButton,SIGNAL(clicked()),this,SLOT(pauseLoading()));
	connect(continueLoadingButton,SIGNAL(clicked()),this,SLOT(continueLoading()));
}

LoadDictionaryWidget::~LoadDictionaryWidget() {
	delete cancelLoadingButton;
	delete continueLoadingButton;
	delete pauseLoadingButton;
	delete textLabel;
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
	continueLoadingButton->hide();
	pauseLoadingButton->show();
	textLabel->setText(tr("Dictionary loading") + "...");	
}

void LoadDictionaryWidget::pauseLoading() {
	emit (paused());
	pauseLoadingButton->hide();
	continueLoadingButton->show();
	textLabel->setText(tr("Loading was paused"));
}

void LoadDictionaryWidget::continueLoading() {
	emit (continued());
	continueLoadingButton->hide();
	pauseLoadingButton->show();
	textLabel->setText(tr("Dictionary loading") + "...");
}
