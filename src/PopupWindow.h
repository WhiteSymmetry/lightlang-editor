#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QtGui/QDialog>

class QTextBrowser;
class QLabel;
class QToolButton;
class QCursor;
class QHideEvent;
class QShowEvent;

class PopupWindow : public QDialog
{
	Q_OBJECT
	signals:
		void closed();
		void closedAfterChanges();
	private:
		QTextBrowser *browser;
		QToolButton *closeButton;
		QLabel *header;
		QCursor *cursor;
		bool wasChanges;
	private slots:
		void textChanged();
	public:
		PopupWindow(QWidget *parent = 0);
		~PopupWindow();
		void setHeaderText(const QString &headerText);
		
		QString getText() const;
		void setText(const QString &text);
		
		void setReadOnly(bool readOnly);
	public slots:
		void showPopup();
		void hideEvent(QHideEvent *);
		void showEvent(QShowEvent *);
};


#endif
