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
	
// 	useStatusesCheckBox = new QCheckBox(tr("Use marks"));
	
	showSideButtonCheckBox = new QCheckBox(tr("Show side bar"));
	
	popupWindow = new PopupWindow;
	
	updateTranslationInfoButton = new InfoButton(popupWindow);
	updateTranslationInfoButton->setPopupHeaderText(tr("Translation renovation"));
	updateTranslationInfoButton->setPopupText(tr("If you notice, the editor updates translation, when you stop word entering. You can set the time between stop of entering and translation renovation. If you set zero time, this function will be blocked."));
	
	useHighlightingInfoButton = new InfoButton(popupWindow);
	useHighlightingInfoButton->setPopupHeaderText(tr("Use highlighting"));
	useHighlightingInfoButton->setPopupText(tr("LightLang Editor try to highlight translation to concentrate you attention on SL tags, but you can disable it."));
	/*
	useStatusesInfoButton = new InfoButton(popupWindow);
	useStatusesInfoButton->setPopupHeaderText(tr("Marks usage"));
	useStatusesInfoButton->setPopupText(tr("It's very difficult to remember all unfinished translations, that's why we decided to include function \"marks\". You can mark different translations and return to them with special search panel function \"Show all marked words\"."));
	*/
	showSideButtonInfoButton = new InfoButton(popupWindow);
	showSideButtonInfoButton->setPopupHeaderText(tr("Side bar"));
	showSideButtonInfoButton->setPopupText(tr("This bar is situated at right side of the program when you edit any dictionary. If click on it, you will able to see dictionary searching."));
	
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
// 	mainLayout->addWidget(useStatusesInfoButton,5,0);
// 	mainLayout->addWidget(useStatusesCheckBox,5,1);
	mainLayout->addWidget(showSideButtonInfoButton,5,0);
	mainLayout->addWidget(showSideButtonCheckBox,5,1);
	mainLayout->setColumnStretch(1,1);
	mainLayout->setRowStretch(6,1);
	
	setLayout(mainLayout);
	setWindowTitle(tr("Your preferences"));
	setMinimumSize(sizeHint());
}

SettingsWidget::~SettingsWidget() {
	delete updateTranslationInfoButton;
	delete useHighlightingInfoButton;
// 	delete useStatusesInfoButton;
	delete showSideButtonInfoButton;
	delete popupWindow;
	delete headerLabel;
	delete introductionLabel;
	delete updateTranslationTimeSpinBox;
	delete useHighlightingCheckBox;
// 	delete useStatusesCheckBox;
	delete showSideButtonCheckBox;
}


void SettingsWidget::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.setValue("GeneralSettings/TranslationRenovation",updateTranslationTimeSpinBox->value());
	settings.setValue("GeneralSettings/UseHighlighting",useHighlightingCheckBox->isChecked());
// 	settings.setValue("GeneralSettings/UseStatuses",useStatusesCheckBox->isChecked());
	settings.setValue("GeneralSettings/ShowSideBar",showSideButtonCheckBox->isChecked());
	emit (updateSettings());
}

void SettingsWidget::loadSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	
	updateTranslationTimeSpinBox->setValue(settings.value("GeneralSettings/TranslationRenovation",0.8).toDouble());
	useHighlightingCheckBox->setChecked(settings.value("GeneralSettings/UseHighlighting",true).toBool());
// 	useStatusesCheckBox->setChecked(settings.value("GeneralSettings/UseStatuses",false).toBool());
	showSideButtonCheckBox->setChecked(settings.value("GeneralSettings/ShowSideBar",true).toBool());
	
	connect(updateTranslationTimeSpinBox,SIGNAL(valueChanged(double)),this,SLOT(saveSettings()));
	connect(useHighlightingCheckBox,SIGNAL(clicked()),this,SLOT(saveSettings()));
// 	connect(useStatusesCheckBox,SIGNAL(clicked()),this,SLOT(saveSettings()));
	connect(showSideButtonCheckBox,SIGNAL(clicked()),this,SLOT(saveSettings()));
	
	emit (updateSettings());
}

int SettingsWidget::translationRenovation() const {
	return int(updateTranslationTimeSpinBox->value()*1000);
}
/*
bool SettingsWidget::useStatuses() const {
	return useStatusesCheckBox->isChecked();
}*/

bool SettingsWidget::useHighlighting() const {
	return useHighlightingCheckBox->isChecked();
}

bool SettingsWidget::showSideBar() const {
	return showSideButtonCheckBox->isChecked();
}
