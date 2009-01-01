#include <QtGui/QToolButton>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtCore/QTimer>
#include "ProgressBarWithWidgets.h"
#include "LoadDictionaryWidget.h"

LoadDictionaryWidget::LoadDictionaryWidget() : BorderPanelWithWidget(BorderPanelWithWidget::Horizontal) {
	timer = new QTimer;
	timer->setInterval(15);
	connect(timer,SIGNAL(timeout()),this,SLOT(updateSize()));
	rollToShow = false;
	
	progressBar = new ProgressBarWithWidgets;
	progressBar->setTextVisible(false);
	
	pauseLoadingButton = new QToolButton;
	pauseLoadingButton->setIcon(QIcon(":/icons/pause.png"));
	pauseLoadingButton->setAutoRaise(true);
	pauseLoadingButton->setToolTip(tr("Pause loading"));
	
	continueLoadingButton = new QToolButton;
	continueLoadingButton->setIcon(QIcon(":/icons/continue.png"));
	continueLoadingButton->setAutoRaise(true);
	continueLoadingButton->setToolTip(tr("Continue loading"));
	continueLoadingButton->hide();
	
	cancelLoadingButton = new QToolButton;
	cancelLoadingButton->setIcon(QIcon(":/icons/cancel.png"));
	cancelLoadingButton->setAutoRaise(true);
	cancelLoadingButton->setToolTip(tr("Cancel loading"));
	
	openLoadedDictionaryButton = new QPushButton;
	openLoadedDictionaryButton->setIcon(QIcon(":/icons/open.png"));
	openLoadedDictionaryButton->setText(tr("Open loaded dictionary"));
	connect(openLoadedDictionaryButton,SIGNAL(clicked()),this,SIGNAL(openLastLoadedDictionary()));
	openLoadedDictionaryButton->hide();
	
	textLabel = new QLabel(tr("Dictionary loading") + "...");
	
	progressBar->addWidget(cancelLoadingButton);
	progressBar->addStretch();
	progressBar->addWidget(textLabel);
	progressBar->addStretch();
	progressBar->addWidget(continueLoadingButton);
	progressBar->addWidget(pauseLoadingButton);
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(progressBar,1);
	mainLayout->addWidget(openLoadedDictionaryButton);
	
	QWidget *mainWidget = new QWidget;
	mainWidget->setLayout(mainLayout);
	
	setWidget(mainWidget);
	
	connect(cancelLoadingButton,SIGNAL(clicked()),this,SIGNAL(canceled()));
	connect(cancelLoadingButton,SIGNAL(clicked()),this,SLOT(hideWithRolling()));
	connect(pauseLoadingButton,SIGNAL(clicked()),this,SLOT(pauseLoading()));
	connect(continueLoadingButton,SIGNAL(clicked()),this,SLOT(continueLoading()));
}

LoadDictionaryWidget::~LoadDictionaryWidget() {
	delete timer;
	delete openLoadedDictionaryButton;
	delete cancelLoadingButton;
	delete continueLoadingButton;
	delete pauseLoadingButton;
	delete textLabel;
	delete progressBar;
}

void LoadDictionaryWidget::addValue() {
	progressBar->setValue(progressBar->value() + 1);
	if (progressBar->value() >= progressBar->maximum()) {
		openLoadedDictionaryButton->show();
		continueLoadingButton->hide();
		pauseLoadingButton->hide();
		cancelLoadingButton->hide();
		textLabel->setText(tr("Loading is finished"));
	}
}

void LoadDictionaryWidget::setMaximum(int max) {
	if (max > 0) {
		progressBar->setRange(0,max);
		progressBar->setValue(0);
	}
}

void LoadDictionaryWidget::reset() {
	progressBar->reset();
	continueLoadingButton->hide();
	pauseLoadingButton->show();
	cancelLoadingButton->show();
	textLabel->setText(tr("Dictionary is loading") + "...");	
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

void LoadDictionaryWidget::showWithRolling() {
	show();
	rollToShow = true;
	timer->start();
}

void LoadDictionaryWidget::hideWithRolling() {
	rollToShow = false;
	timer->start();
}

void LoadDictionaryWidget::updateSize() {
	if (rollToShow) {
		if (height() + 10 >= sizeHint().height())
			setMaximumHeight(sizeHint().height());
		else
			setMaximumHeight(height() + 10);
		if (height() != sizeHint().height())
			timer->start();
		else {
			timer->stop();
		}
	} else {
		if (height() - 10 <= 0)
			setMaximumHeight(0);
		else
			setMaximumHeight(height() - 10);
		if (height() != 0)
			timer->start();
		else {
			timer->stop();
			hide();
		}
	}
	resize(width(),sizeHint().height());
}

