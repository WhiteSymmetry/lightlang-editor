#ifndef STATUSBARLABEL_H
#define STATUSBARLABEL_H

#include <QtGui/QLabel>

class QPaintEvent;
class QTimer;
	
class StatusBarLabel : public QLabel
{
	Q_OBJECT
	public slots:
		void setMessage(const QString& message);
	public:
		StatusBarLabel(QWidget *parent = 0);
		~StatusBarLabel();
	protected:
		void paintEvent(QPaintEvent *);
	private:
		QTimer *timer;
		int  illumination;
		QString currentMessage;
		bool decolour;
};


#endif
