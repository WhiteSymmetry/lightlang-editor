#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include "LoadDictionaryWidget.h"

LoadDictionaryWidget::LoadDictionaryWidget() : BorderPanelWithWidget(BorderPanelWithWidget::Horizontal) {
	progressBar = new QProgressBar;
	progressBar->setTextVisible(false);
	
	cancelLoadingButton = new QPushButton;
	cancelLoadingButton->setIcon(QIcon(":/icons/apply.png"));
	cancelLoadingButton->setFlat(true);
	connect(cancelLoadingButton,SIGNAL(clicked()),this,SIGNAL(canceled()));
	
	cancelCancelingButton = new QPushButton;
	cancelCancelingButton->setIcon(QIcon(":/icons/cancel.png"));
	cancelCancelingButton->setFlat(true);
	
	askWidget = new QWidget;
	askWidget->hide();
	
	QHBoxLayout *askWidgetLayout = new QHBoxLayout;
	askWidgetLayout->addWidget(new QLabel(tr("Are you sure?")));
	askWidgetLayout->addWidget(cancelCancelingButton);
	askWidgetLayout->addWidget(cancelLoadingButton);
	askWidgetLayout->setMargin(0);
	askWidget->setLayout(askWidgetLayout);
	
	preCancelLoadingButton = new QPushButton(tr("Cancel"));
	connect(preCancelLoadingButton,SIGNAL(clicked()),askWidget,SLOT(show()));
	
	QHBoxLayout *progressBarLayout = new QHBoxLayout;
	progressBarLayout->addWidget(new QLabel(tr("Dictionary loading") + ":"));
	progressBarLayout->addWidget(progressBar,1);
	progressBarLayout->addWidget(preCancelLoadingButton);
	progressBarLayout->addWidget(askWidget);
	
	widgetWithProgressBar = new QWidget;
	widgetWithProgressBar->setLayout(progressBarLayout);
	
	setWidget(widgetWithProgressBar);
	
	connect(cancelCancelingButton,SIGNAL(clicked()),askWidget,SLOT(hide()));
}

LoadDictionaryWidget::~LoadDictionaryWidget() {
	delete progressBar;
	delete preCancelLoadingButton;
	delete cancelCancelingButton;
	delete cancelLoadingButton;
	delete askWidget;
	delete widgetWithProgressBar;
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

void LoadDictionaryWidget::showEvent(QShowEvent *) {
	askWidget->hide();
}
