#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QToolButton>
#include <QtGui/QCheckBox>
#include <QtGui/QSpinBox>
#include "PopupWindow.h"
#include "InfoButton.h"
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
	
	updateTranslationTimeSpinBox = new QSpinBox;
	QLabel *updateTranslationLittleLabel = new QLabel;
	updateTranslationLittleLabel->setText(tr("The time between translation renovation and stop of word entering"));
	
	useHighlightingCheckBox = new QCheckBox(tr("Highlight translation"));
	useStatusesCheckBox = new QCheckBox(tr("Use statuses"));
	
	popupWindow = new PopupWindow;
	
	updateTranslationInfoButton = new InfoButton(popupWindow);
	updateTranslationInfoButton->setPopupHeaderText(tr("Translation renovation"));
	updateTranslationInfoButton->setPopupText(tr("If you notice, the editor updates translation, when you stop word entering. You can set the time between stop of entering and translation renovation. If you set zero time, this function will be blocked."));
	
	useHighlightingInfoButton = new InfoButton(popupWindow);
	useHighlightingInfoButton->setPopupHeaderText(tr("Use highlighting"));
	useHighlightingInfoButton->setPopupText(tr("LightLang Editor try to highlight translation to concentrate you attention on SL tags, but you can disable it."));
	
	useStatusesInfoButton = new InfoButton(popupWindow);
	useStatusesInfoButton->setPopupHeaderText(tr("Statuses usage"));
	useStatusesInfoButton->setPopupText(tr("Use words statuses. If you don't finish to translate some word, you can mark it as \"Unfinished\" and return to translation of this word later. Of course, you can do it without this function, but searching of unfinished words will be easier with statuses usage."));
	
	QHBoxLayout *updateTranslationLayout = new QHBoxLayout;
	updateTranslationLayout->addWidget(updateTranslationTimeSpinBox);
	updateTranslationLayout->addWidget(updateTranslationLittleLabel,1);
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(headerLabel,1);
	topLayout->addWidget(closeButton);
	
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addLayout(topLayout,0,0,1,2);
	mainLayout->addWidget(introductionLabel,1,0,1,2);
	mainLayout->addWidget(new QLabel("<hr>"),2,0,1,2);
	mainLayout->addWidget(updateTranslationInfoButton,3,0);
	mainLayout->addLayout(updateTranslationLayout,3,1);
	mainLayout->addWidget(useHighlightingInfoButton,4,0);
	mainLayout->addWidget(useHighlightingCheckBox,4,1);
	mainLayout->addWidget(useStatusesInfoButton,5,0);
	mainLayout->addWidget(useStatusesCheckBox,5,1);
	mainLayout->setColumnStretch(1,1);
	mainLayout->setRowStretch(6,1);
	
	setLayout(mainLayout);
}

SettingsWidget::~SettingsWidget() {
	delete updateTranslationInfoButton;
	delete useHighlightingInfoButton;
	delete useStatusesInfoButton;
	delete popupWindow;
	delete headerLabel;
	delete closeButton;
	delete introductionLabel;
	delete updateTranslationTimeSpinBox;
	delete useHighlightingCheckBox;
	delete useStatusesCheckBox;
}
