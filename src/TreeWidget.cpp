#include <QtGui/QContextMenuEvent>
#include "Menu.h"
#include "TreeWidget.h"

TreeWidget::TreeWidget(bool isContextMenuEnabled) {
	contextMenu = 0;
	contextMenuEnabled = isContextMenuEnabled;
	setContextMenuEnabled(contextMenuEnabled);
}

void TreeWidget::setContextMenuEnabled(bool isEnabled) {
	// If context menu is enabled and context menu object doesn't exist create it
	if (isEnabled && !contextMenu) contextMenu = new Menu;
	contextMenuEnabled = isEnabled;
}

void TreeWidget::setContextMenuHeader(const QString &headerText) {
	if (contextMenu)
		contextMenu->setHeaderText(headerText);
}

void TreeWidget::setContextMenuIcon(const QIcon &icon) {
	if (contextMenu)
		contextMenu->setHeaderIcon(icon);
}

void TreeWidget::addContextMenuAction(QAction *newAction) {
	if (contextMenu)
		contextMenu->addAction(newAction);
}

void TreeWidget::addContextMenuSeparator() {
	if (contextMenu)
		contextMenu->addSeparator();
}

void TreeWidget::contextMenuEvent(QContextMenuEvent *event) {
	if (!contextMenuEnabled)
		return;
	emit(contextMenuEventSignal());
	if (itemAt(event->x(),event->y())) {
		contextMenu->move(event->globalX(),event->globalY());
		contextMenu->show();
	}
}
