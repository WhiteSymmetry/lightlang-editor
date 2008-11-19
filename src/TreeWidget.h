#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QtGui/QTreeWidget>
#include <QtCore/QString>

class Menu;
class QAction;
class QContextMenuEvent;

// Class which can use special Menu with header
class TreeWidget : public QTreeWidget
{
	Q_OBJECT
	signals:
		void contextMenuEventSignal();
	private:
		Menu *contextMenu;
		bool contextMenuEnabled;
	public:
		TreeWidget(bool contextMenuEnabled = false);
		void setContextMenuEnabled(bool isEnabled);
		void setContextMenuHeader(const QString &headerText);
		void setContextMenuIcon(const QIcon &icon);
		void addContextMenuAction(QAction *newAction);
		void addContextMenuSeparator();
	protected:
		void contextMenuEvent(QContextMenuEvent *event);
};

#endif
