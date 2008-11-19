#ifndef LOADDICTIONARYWIDGET_H
#define LOADDICTIONARYWIDGET_H

#include "BorderPanelWithWidget.h"

class QPushButton;
class QProgressBar;
class QShowEvent;

class LoadDictionaryWidget : public BorderPanelWithWidget
{
	Q_OBJECT
	signals:
		void canceled();
	public slots:
		void setMaximum(int max);
		void addValue();
	public:
		LoadDictionaryWidget();
		~LoadDictionaryWidget();
	
		void reset();
	private:
		QWidget *widgetWithProgressBar;
		QWidget *askWidget;
	
		QProgressBar *progressBar;
		QPushButton *preCancelLoadingButton;
		QPushButton *cancelCancelingButton;
		QPushButton *cancelLoadingButton;
	protected:
		void showEvent(QShowEvent *);
};

#endif
