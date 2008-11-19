#ifndef MANUAL_H
#define MANUAL_H

#include <QtGui/QDialog>
#include <QtCore/QUrl>

class QPushButton;
class QTextBrowser;
class QListWidget;
class QSplitter;
class QLabel;

class Manual : public QDialog
{
	Q_OBJECT
	public:
		Manual(QWidget* parent = 0);
		~Manual();
		QByteArray getState();
	private slots:
		void changePage(int);
		void changePage(const QUrl&);
		void backward();
		void forward();
	private:
		QPushButton *backwardButton;
		QPushButton *forwardButton;
		QTextBrowser *browser;
		QListWidget *listWidget;
		QSplitter *splitter;
		QLabel *headerLabel;
	
		QString language;
		
		void addItem(const QString title,const QString url);
};

#endif
