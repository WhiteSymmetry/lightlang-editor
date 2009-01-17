#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include "StatusBarLabel.h"

StatusBarLabel::StatusBarLabel(QWidget *) {
	
	timer = new QTimer;
	timer->setInterval(10);
	connect(timer,SIGNAL(timeout()),this,SLOT(update()));
	illumination = 0;
	decolour = false;
	
	QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,0,0));
    setPalette(palette);
}

StatusBarLabel::~StatusBarLabel() {
	delete timer;
}

void StatusBarLabel::paintEvent(QPaintEvent *) {
	if (isHidden())
		timer->stop();
	QPainter painter(this);
	QColor backgroundColor = palette().brush(QPalette::Background).color();
    QColor foregroundColor = QColor(0,0,0);
	if (decolour)
		illumination -= 2;
	else
		illumination += 2;
	if (illumination < 0)
		illumination = 0;
	if (illumination > 255)
		illumination = 255;
	backgroundColor.setRgb(200, 228, 239,illumination);
	painter.setBrush(backgroundColor);
    painter.setPen(backgroundColor);
    painter.drawRect(QRect(0, 0, width(), height()));

    painter.setPen(foregroundColor);
	painter.setFont(QFont("Sans",10,1));
    int flags = Qt::AlignVCenter | Qt::TextWordWrap;
    painter.drawText(QRect(5, 0, width(), height()), flags, currentMessage);

	painter.end();
	if (illumination >= 255)
		decolour = true;
	else if (illumination == 0) {
		decolour = false;
	}
}

void StatusBarLabel::setMessage(const QString& message) {
	currentMessage = message;
	timer->start();
}
