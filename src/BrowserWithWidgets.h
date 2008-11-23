#ifndef BROWSERWITHWIDGETS_H
#define BROWSERWITHWIDGETS_H

#include <QtGui/QTextBrowser>

class QFrame;
class QLayout;
class QGridLayout;

class BrowserWithWidgets : public QTextBrowser
{
	Q_OBJECT
	signals:
		void linkWasClicked(const QString& path);
	public:
		BrowserWithWidgets(QWidget *parent = 0);
		~BrowserWithWidgets();
	
		enum Position { LeftTopCorner, Top, RightTopCorner, Left, Right, LeftBottomCorner, Bottom, RightBottomCorner, Center };
		enum Orientation { Vertical, Horizontal };
		
		void setPosition(Position position);
		void setOrientation(Orientation orientation);
		
		void addWidget(QWidget *newWidget);
	private slots:
		void anchorClickedSlot(const QUrl& link);
		void updateCurrentText();
	private:
		QString currentText;
	
		QFrame *widgetsFrame;
		QLayout *widgetsFrameLayout;
		QGridLayout *mainLayout;
	
		QWidgetList widgets;
};

#endif
