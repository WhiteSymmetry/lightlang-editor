#ifndef INFOBUTTON_H
#define INFOBUTTON_H

#include <QtGui/QToolButton>

class PopupWindow;

class InfoButton : public QToolButton
{
	Q_OBJECT
	private slots:
		void setPopupData();
	private:
		PopupWindow *popupWindow;

		QString popupText;
		QString popupHeader;
	public:
		InfoButton(PopupWindow *popupWindow,const QString &popupText = QString(),const QString &popupHeader = QString());
		void setPopupText(const QString &popupText);
		void setPopupHeaderText(const QString &popupHeaderText);
};


#endif
