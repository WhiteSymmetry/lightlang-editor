#ifndef PROGRESSBARWITHWIDGETS_H
#define PROGRESSBARWITHWIDGETS_H

#include <QtGui/QProgressBar>

class QHBoxLayout;

class ProgressBarWithWidgets : public QProgressBar
{
	public:
		ProgressBarWithWidgets();
		~ProgressBarWithWidgets();
	
		void addWidget(QWidget *widget);
		void addStretch();
	private:
		QHBoxLayout *mainLayout;
};

#endif
