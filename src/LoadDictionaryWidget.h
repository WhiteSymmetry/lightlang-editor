#ifndef LOADDICTIONARYWIDGET_H
#define LOADDICTIONARYWIDGET_H

#include "BorderPanelWithWidget.h"

class QPushButton;
class ProgressBarWithWidgets;
class QShowEvent;

class LoadDictionaryWidget : public BorderPanelWithWidget
{
	Q_OBJECT
	signals:
		void canceled();
		void stopped();
	public slots:
		void setMaximum(int max);
		void addValue();
	public:
		LoadDictionaryWidget();
		~LoadDictionaryWidget();
	
		void reset();
	private:
		ProgressBarWithWidgets *progressBar;
		QPushButton *cancelLoadingButton;
		QPushButton *stopLoadingButton;
};

#endif
