#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>
#include "Menu.h"
#include "EditorTipsWidget.h"

EditorTipsWidget::EditorTipsWidget() {
	
	menu = 0;
	timer = new QTimer;
	timer->setInterval(10000);
	connect(timer,SIGNAL(timeout()),timer,SLOT(stop()));
	connect(timer,SIGNAL(timeout()),this,SLOT(update()));
	
	setToolTip(tr("Click on right mouse button to hide it"));
	
	currentMessageIndex = -1;
	
	setFixedSize(300,18);
	
	timer->start();
}

EditorTipsWidget::~EditorTipsWidget() {
	delete timer;
}

void EditorTipsWidget::paintEvent(QPaintEvent *) {
	if (isHidden())
		timer->stop();
	
	QPainter painter(this);
	
	QColor backgroundColor = palette().brush(QPalette::Background).color();
    QColor foregroundColor = QColor(150,150,150);
	backgroundColor.setRgb(240,240,240);
	
	painter.setBrush(QColor(100,100,100));
    painter.drawRoundedRect(QRect(0, 0, width(), height()),2.0,2.0);
	
	painter.setBrush(backgroundColor);
    painter.setPen(backgroundColor);
    painter.drawRoundedRect(QRect(1, 1, width()-1, height()-1),2.0,2.0);
    int flags = Qt::AlignVCenter | Qt::TextWordWrap;
    painter.setPen(foregroundColor);
    painter.drawText(QRect(5, 0, width(), height()), flags, getNextMessage());
	painter.end();
	
	timer->start();
}

void EditorTipsWidget::addMessages(const QStringList& addMessages) {
	messages += addMessages;
	foreach (QString message,addMessages)
		if (message.length()*6 >= width())
			setFixedSize(message.length()*6,18);
	if (messages.count() > 0)
		timer->start();
}

QString EditorTipsWidget::getNextMessage() {
	if (messages.count() == 0) {
		timer->stop();
		currentMessageIndex = -1;
		return QString();
	}
	if (timer->isActive() && currentMessageIndex < messages.count() && currentMessageIndex >= 0)
		return messages[currentMessageIndex];
	currentMessageIndex += 1;
	if (currentMessageIndex > messages.count() - 1)
		currentMessageIndex = 0;
	return messages[currentMessageIndex];
}

void EditorTipsWidget::contextMenuEvent(QContextMenuEvent *event) {
	if (menu) {
		menu->move(event->globalX(),event->globalY());
		menu->show();
	}
}

void EditorTipsWidget::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton)
		nextTip();
}

void EditorTipsWidget::setContextMenu(Menu *m) {
	menu = m;
}

void  EditorTipsWidget::previousTip() {
	if (messages.count() == 0) {
		timer->stop();
		currentMessageIndex = -1;
		return;
	}
	if (timer->isActive())
		timer->stop();
	currentMessageIndex--;
	if (currentMessageIndex < 0)
		currentMessageIndex = messages.count() - 1;
	timer->start();
	update();
}

void EditorTipsWidget::nextTip() {
	timer->stop();
	update();
}
