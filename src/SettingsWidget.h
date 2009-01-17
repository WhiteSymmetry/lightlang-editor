#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtGui/QWidget>

class QLabel;
class QToolButton;
class QSpinBox;
class QCheckBox;
class InfoButton;
class PopupWindow;

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
	
		PopupWindow *popupWindow;
		InfoButton *updateTranslationInfoButton;
		InfoButton *useHighlightingInfoButton;
		InfoButton *useStatusesInfoButton;
};

#endif
