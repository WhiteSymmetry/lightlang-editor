#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QCheckBox>
#include <QtGui/QSpinBox>
#include "SettingsWidget.h"

SettingsWidget::SettingsWidget() {
	headerLabel = new QLabel("<font size='6'>" + tr("Preferences") + "</font>");
	
	closeButton = new QToolButton;
	closeButton->setAutoRaise(true);
	closeButton->setIcon(QIcon(":/icons/close.png"));
	closeButton->setToolTip(tr("Save changes and close settings"));
	connect(closeButton,SIGNAL(clicked()),this,SIGNAL(closed()));
	
	introductionLabel = new QLabel;
	introductionLabel->setWordWrap(true);
	introductionLabel->setText("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + tr("The LightLang Editor interface is so considered, that's why the options number is few. But we tried to explain all available options to make your work with our program faster and easier for you. If you think, that we should add some option, inform us about it on our forum: %1http://vialinx.org/forum/%2,please.").arg("<a href='http://vialinx.org/forum/'>").arg("</a>"));
	
	QLabel *updateTranslationLabel = new QLabel;
	updateTranslationLabel->setWordWrap(true);
	updateTranslationLabel->setText(tr("If you notice, the editor update translation, when you stop word entering. You can set the time between stop of entering and translation renovation. If you set zero time, this function will be blocked."));
	updateTranslationTimeSpinBox = new QSpinBox;
	QLabel *updateTranslationLittleLabel = new QLabel;
	updateTranslationLittleLabel->setWordWrap(true);
	updateTranslationLittleLabel->setText(tr("The time between translation renovation and stop of word entering"));
	
	QHBoxLayout *updateTranslationLayout = new QHBoxLayout;
	updateTranslationLayout->addWidget(updateTranslationTimeSpinBox);
	updateTranslationLayout->addWidget(updateTranslationLittleLabel,1);
	
	useHighlightingCheckBox = new QCheckBox(tr("Highlight translation"));
	
	QLabel *useStatusesLabel = new QLabel;
	useStatusesLabel->setWordWrap(true);
	useStatusesLabel->setText(tr("Use words statuses. If you don't finish to translate some word, you can mark it as \"Unfinished\" and return to translation of this word later. Of course, you can do it without this function, but searching of unfinished words will be easier with statuses usage."));
	useStatusesCheckBox = new QCheckBox;
	
	QHBoxLayout *useStatusesLayout = new QHBoxLayout;
	useStatusesLayout->addWidget(useStatusesCheckBox);
	useStatusesLayout->addWidget(useStatusesLabel,1);
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(headerLabel,1);
	topLayout->addWidget(closeButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(introductionLabel);
	mainLayout->addWidget(new QLabel("<hr>"));
	mainLayout->addWidget(updateTranslationLabel);
	mainLayout->addLayout(updateTranslationLayout);
	mainLayout->addWidget(new QLabel("<hr>"));
	mainLayout->addWidget(useHighlightingCheckBox);
	mainLayout->addWidget(new QLabel("<hr>"));
	mainLayout->addLayout(useStatusesLayout);
	mainLayout->addStretch();
	
	setLayout(mainLayout);
}

SettingsWidget::~SettingsWidget() {
	delete headerLabel;
	delete closeButton;
	delete introductionLabel;
	delete updateTranslationTimeSpinBox;
	delete useHighlightingCheckBox;
	delete useStatusesCheckBox;
}
