#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QtGui/QSplitter>

class QTimer;
class QSplitter;

class StackedWidget : public QSplitter
{
	Q_OBJECT
	signals:
		void forwardAvailable(bool isAvailable);
		void backwardAvailable(bool isAvailable);
		void currentIndexChanged(int currentIndex);
	public slots:
		void forward();
		void backward();
		void setCurrentIndex(int index);
		void setCurrentWidget(QWidget *widget);
	public:
		enum FlipSpeed { Normal, VerySlow, Slow, Fast, Immediately };
	
		StackedWidget();
		~StackedWidget();
		
		void addNewWidget(QWidget *widget);
		
		bool isBackwardAvailable() const;
		bool isForwardAvailable() const;
		
		void setFlipSpeed(FlipSpeed speed);
		FlipSpeed getFlipSpeed() const;
	
		int getCurrentIndex() const;
		QWidget *getCurrentWidget() const;
	private slots:
		void updateSizes();
	private:
		bool flipToNext;
		QTimer *timer;
	
		int currentIndex;
		FlipSpeed currentFlipSpeed;
};

#endif
