#ifndef BORDERPANELWITHWIDGET_H
#define BORDERPANELWITHWIDGET_H

#include <QtGui/QWidget>

class QLayout;
class QPushButton;
class QTimer;

class BorderPanelWithWidget : public QWidget
{
	Q_OBJECT
	public:
		enum Orientation { Horizontal, Vertical };
		
		BorderPanelWithWidget(Orientation defaultOrientation = Vertical);
		~BorderPanelWithWidget();
	
		void setWidget(QWidget *widget);
		QWidget *getWidget() const;
		
		void setOrientation(Orientation orientation);
		int getOrientation() const;
	private slots:
		void hideOrShow();
		void updateWidgetSize();
	private:
		QTimer *timer;
		bool rollToShowWidget;
	
		void updateHideButtonIcon();
	
		Orientation currentOrientation;

		QWidget *widget;
	
		QLayout *mainLayout;
	
		QPushButton *hideButton;
};


#endif
