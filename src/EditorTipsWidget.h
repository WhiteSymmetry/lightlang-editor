#ifndef EDITORTIPSWIDGET_H
#define EDITORTIPSWIDGET_H

#include <QtGui/QWidget>

class QTimer;
class QContextMenuEvent;
class Menu;
class QMouseEvent;

class EditorTipsWidget : public QWidget
{
	Q_OBJECT
	public slots:
		void nextTip();
		void previousTip();
	public:
		EditorTipsWidget();
		~EditorTipsWidget();
		
		void setContextMenu(Menu *menu);
		
		void addMessages(const QStringList& list);
	private:
		QString getNextMessage();
		
		QStringList messages;
		Menu *menu;
		QTimer *timer;
		int currentMessageIndex;
	protected:
		void mousePressEvent(QMouseEvent *);
		void paintEvent(QPaintEvent *);
		void contextMenuEvent(QContextMenuEvent *);
};

#endif
