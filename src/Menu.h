#ifndef MENU_H
#define MENU_H

#include <QtGui/QMenu>
#include <QtCore/QString>

class QLabel;
class QWidgetAction;
class QFrame;

// This menu class can create header in top of menu
class Menu : public QMenu
{
	private:
		QString headerText;
		// Action on the top of menu
		QWidgetAction *headerAction;
		QFrame *headerActionFrame;
		QLabel *headerLabel;
		QLabel *iconLabel;
		int iconSize;
	public:
		Menu(bool useSpecialHeader = true);
		~Menu();
		void setHeaderIcon(const QIcon &newHeaderIcon);
		void setHeaderText(const QString &newHeaderText);
};


#endif
