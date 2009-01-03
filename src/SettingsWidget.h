#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtGui/QWidget>

class QLabel;
class QToolButton;
class QSpinBox;
class QCheckBox;

class SettingsWidget : public QWidget
{
	Q_OBJECT
	signals:
		void closed();
	public:
		SettingsWidget();
		~SettingsWidget();
	private:
		QLabel *headerLabel;
		QToolButton *closeButton;
		QLabel *introductionLabel;
	
		QSpinBox *updateTranslationTimeSpinBox;
		QCheckBox *useHighlightingCheckBox;
		QCheckBox *useStatusesCheckBox;
};

#endif
