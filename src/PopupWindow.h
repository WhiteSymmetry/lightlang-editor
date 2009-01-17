#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QtGui/QDialog>

class QTextBrowser;
class QLabel;
class QToolButton;
class QCursor;

class PopupWindow : public QDialog
{
	Q_OBJECT
	private:
		QTextBrowser *browser;
		QToolButton *closeButton;
		QLabel *header;
		QCursor *cursor;
	public:
		PopupWindow(QWidget *parent = 0);
		~PopupWindow();
		void setText(const QString &text);
		void setHeaderText(const QString &headerText);
	public slots:
		void showPopup();
};


#endif
