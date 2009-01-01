#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtGui/QWidget>

class QLabel;

class SettingsWidget : public QWidget
{
	public:
		SettingsWidget();
		~SettingsWidget();
	private:
		QLabel *headerLabel;
};

#endif
