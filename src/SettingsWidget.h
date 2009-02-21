#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtGui/QWidget>

class QLabel;
class QToolButton;
class QDoubleSpinBox;
class QCheckBox;
class InfoButton;
class PopupWindow;

class SettingsWidget : public QWidget
{
	Q_OBJECT
	signals:
		void closed();
		void updateSettings();
	public slots:
		void saveSettings();
	public:
		SettingsWidget();
		~SettingsWidget();
		
		int translationRenovation() const;
		bool useStatuses() const;
		bool useHighlighting() const;
		bool showTips() const;
		
		void loadSettings();
	private:
		QLabel *headerLabel;
		QToolButton *closeButton;
		QLabel *introductionLabel;
	
		QDoubleSpinBox *updateTranslationTimeSpinBox;
		QCheckBox *useHighlightingCheckBox;
		QCheckBox *useStatusesCheckBox;
		QCheckBox *showTipsCheckBox;
	
		PopupWindow *popupWindow;
		InfoButton *updateTranslationInfoButton;
		InfoButton *useHighlightingInfoButton;
		InfoButton *useStatusesInfoButton;
		InfoButton *showTipsInfoButton;
};

#endif
