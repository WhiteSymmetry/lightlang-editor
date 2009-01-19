#ifndef EDITORTIPSWIDGET_H
#define EDITORTIPSWIDGET_H

#include <QtGui/QWidget>

class QTimer;
class QMouseEvent;

class EditorTipsWidget : public QWidget
{
	Q_OBJECT
	signals:
		void hideAllTips();
	public:
		EditorTipsWidget();
		~EditorTipsWidget();
		
		void addMessages(const QStringList& list);
	private:
		QString getNextMessage();
		
		QStringList messages;
		QTimer *timer;
		int currentMessageIndex;
	protected:
		void paintEvent(QPaintEvent *);
		void mousePressEvent(QMouseEvent *);
};

#endif
