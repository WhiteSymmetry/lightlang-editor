#ifndef LOADDICTIONARYWIDGET_H
#define LOADDICTIONARYWIDGET_H

#include "BorderPanelWithWidget.h"

class QToolButton;
class ProgressBarWithWidgets;
class QShowEvent;
class QLabel;
class QTimer;
class QPushButton;

class LoadDictionaryWidget : public BorderPanelWithWidget
{
	Q_OBJECT
	signals:
		void canceled();
		void paused();
		void continued();
		void openLastLoadedDictionary();
	public slots:
		void setMaximum(int max);
		void addValue();
		void showWithRolling();
		void hideWithRolling();
	public:
		LoadDictionaryWidget();
		~LoadDictionaryWidget();
	
		void reset();
		void loadingFinished();
	private slots:
		void pauseLoading();
		void continueLoading();
		void updateSize();
	private:
		QTimer *timer;
		bool rollToShow;
	
		ProgressBarWithWidgets *progressBar;
		QToolButton *cancelLoadingButton;
		QLabel *textLabel;
		QToolButton *pauseLoadingButton;
		QToolButton *continueLoadingButton;
		QPushButton *openLoadedDictionaryButton;
		QPushButton *closeButton;
};

#endif
