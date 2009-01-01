#ifndef MANUAL_H
#define MANUAL_H

#include <QtGui/QDialog>
#include <QtGui/QTextBrowser>
#include <QtCore/QUrl>

class QToolButton;
class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class QTimer;

struct Link
{
	QToolButton *linkView;
	QString linkSource;
	Link(QToolButton *view,const QString& source) {
		linkView = view;
		linkSource = source;
	}
};

class ManualBrowserWithWidgets : public QTextBrowser
{
	Q_OBJECT
	public:
		ManualBrowserWithWidgets();
		~ManualBrowserWithWidgets();
		
		void addLink(const QString& linkHeader,const QString& sourcePath);
	private slots:
		void changeSource();
		void updateLinksSize();
		void showLinks();
	private:
		QTimer *timer;
		bool rollToDown;
	
		QFrame *buttonsFrame;
		QHBoxLayout *buttonsFrameLayout;
	
		QFrame *linksFrame;
		QVBoxLayout *linksFrameLayout;
	
		QToolButton *showLinksButton;
		QToolButton *backwardButton;
		QToolButton *forwardButton;
	
		QList<Link> links;
};

class Manual : public QDialog
{
	public:
		Manual(QWidget *parent = 0);
		~Manual();

		void saveSettings();
		void loadSettings();
	private:
		ManualBrowserWithWidgets *browser;
		QString language;
};


#endif

