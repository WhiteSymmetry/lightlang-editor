#ifndef LOADDICTIONARYWIDGET_H
#define LOADDICTIONARYWIDGET_H

#include "BorderPanelWithWidget.h"

class QPushButton;
class ProgressBarWithWidgets;
class QShowEvent;
class QLabel;

class LoadDictionaryWidget : public BorderPanelWithWidget
{
	Q_OBJECT
	signals:
		void canceled();
		void paused();
		void continued();
	public slots:
		void setMaximum(int max);
		void addValue();
	public:
		LoadDictionaryWidget();
		~LoadDictionaryWidget();
	
		void reset();
	private slots:
		void pauseLoading();
		void continueLoading();
	private:
		ProgressBarWithWidgets *progressBar;
		QPushButton *cancelLoadingButton;
		QLabel *textLabel;
		QPushButton *pauseLoadingButton;
		QPushButton *continueLoadingButton;
};

#endif
