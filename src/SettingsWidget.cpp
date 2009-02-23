#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtCore/QSettings>
#include "PopupWindow.h"
#include "InfoButton.h"
#include "const.h"
#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QDialog(parent) {
	headerLabel = new QLabel("<font size='6'>" + tr("Preferences") + "</font>");
	
	introductionLabel = new QLabel;
	introductionLabel->setWordWrap(true);
	introductionLabel->setText("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + tr("The LightLang Editor interface is considered, that's why the options number is few. But we tried to explain all available options to make your work with our program faster and easier for you. If you think, that we should add some options, inform us about it on our forum: %1http://vialinx.org/forum/%2,please.").arg("<a href='http://vialinx.org/forum/'>").arg("</a>"));
	
	updateTranslationTimeSpinBox = new QDoubleSpinBox;
	updateTranslationTimeSpinBox->setSingleStep(0.1);
	updateTranslationTimeSpinBox->setMaximum(2.0);
	updateTranslationTimeSpinBox->setMinimum(0.0);
	updateTranslationTimeSpinBox->setSuffix(" " + tr("seconds"));
	updateTranslationTimeSpinBox->setDecimals(1);
	QLabel *updateTranslationLittleLabel = new QLabel;
	updateTranslationLittleLabel->setText(tr("The time between translation renovation and stop of word entering"));
	
	useHighlightingCheckBox = new QCheckBox(tr("Highlight translation"));
	
	useStatusesCheckBox = new QCheckBox(tr("Use statuses"));
	
	showTipsCheckBox = new QCheckBox(tr("Show tips in translation editor"));
	
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
	
	showTipsInfoButton = new InfoButton(popupWindow);
	showTipsInfoButton->setPopupHeaderText(tr("Tips"));
	showTipsInfoButton->setPopupText(tr("There are a lot of things, which can make your work with editor easier. Tips can help you to learn all these things"));
	
	QHBoxLayout *updateTranslationLayout = new QHBoxLayout;
	updateTranslationLayout->addWidget(updateTranslationTimeSpinBox);
	updateTranslationLayout->addWidget(updateTranslationLittleLabel,1);
	
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(headerLabel,0,0,1,2);
	mainLayout->addWidget(introductionLabel,1,0,1,2);
	mainLayout->addWidget(new QLabel("<hr>"),2,0,1,2);
	mainLayout->addWidget(updateTranslationInfoButton,3,0);
	mainLayout->addLayout(updateTranslationLayout,3,1);
	mainLayout->addWidget(useHighlightingInfoButton,4,0);
	mainLayout->addWidget(useHighlightingCheckBox,4,1);
	mainLayout->addWidget(useStatusesInfoButton,5,0);
	mainLayout->addWidget(useStatusesCheckBox,5,1);
	mainLayout->addWidget(showTipsInfoButton,6,0);
	mainLayout->addWidget(showTipsCheckBox,6,1);
	mainLayout->setColumnStretch(1,1);
	mainLayout->setRowStretch(7,1);
	
	setLayout(mainLayout);
	setWindowTitle(tr("Your preferences"));
	setMinimumSize(sizeHint());
}

SettingsWidget::~SettingsWidget() {
	delete showTipsInfoButton;
	delete updateTranslationInfoButton;
	delete useHighlightingInfoButton;
	delete useStatusesInfoButton;
	delete popupWindow;
	delete headerLabel;
	delete introductionLabel;
	delete showTipsCheckBox;
	delete updateTranslationTimeSpinBox;
	delete useHighlightingCheckBox;
	delete useStatusesCheckBox;
}


void SettingsWidget::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.setValue("GeneralSettings/TranslationRenovation",updateTranslationTimeSpinBox->value());
	settings.setValue("GeneralSettings/UseHighlighting",useHighlightingCheckBox->isChecked());
	settings.setValue("GeneralSettings/UseStatuses",useStatusesCheckBox->isChecked());
	settings.setValue("GeneralSettings/ShowTips",showTipsCheckBox->isChecked());
	emit (updateSettings());
}

void SettingsWidget::loadSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	
	updateTranslationTimeSpinBox->setValue(settings.value("GeneralSettings/TranslationRenovation",0.8).toDouble());
	useHighlightingCheckBox->setChecked(settings.value("GeneralSettings/UseHighlighting",true).toBool());
	useStatusesCheckBox->setChecked(settings.value("GeneralSettings/UseStatuses",false).toBool());
	showTipsCheckBox->setChecked(settings.value("GeneralSettings/ShowTips",false).toBool());
	
	connect(updateTranslationTimeSpinBox,SIGNAL(valueChanged(double)),this,SLOT(saveSettings()));
	connect(useHighlightingCheckBox,SIGNAL(clicked()),this,SLOT(saveSettings()));
	connect(useStatusesCheckBox,SIGNAL(clicked()),this,SLOT(saveSettings()));
	connect(showTipsCheckBox,SIGNAL(clicked()),this,SLOT(saveSettings()));
	
	emit (updateSettings());
}

int SettingsWidget::translationRenovation() const {
	return int(updateTranslationTimeSpinBox->value()*1000);
}

bool SettingsWidget::useStatuses() const {
	return useStatusesCheckBox->isChecked();
}

bool SettingsWidget::useHighlighting() const {
	return useHighlightingCheckBox->isChecked();
}

bool SettingsWidget::showTips() const {
	return showTipsCheckBox->isChecked();
}
