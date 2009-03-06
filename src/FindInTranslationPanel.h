#ifndef FINDINTRANSLATIONPANEL_H
#define FINDINTRANSLATIONPANEL_H

#include <QtGui/QWidget>

class QLineEdit;
class QToolButton;
class QFrame;
class QKeyEvent;
class QHideEvent;

class FindInTranslationPanel : public QWidget
{
	Q_OBJECT
	signals:
		void searchSignal(const QString& expression);
		void findNextRequestSignal(const QString& expression);
		void findPreviousRequestSignal(const QString& expression);
		void wasHidden();
	public slots:
		void setRedPalette();
		void setDefaultPalette();
		void setGreenPalette();
	public:
		FindInTranslationPanel(QWidget *parent = 0);
		~FindInTranslationPanel();
		
		void setLineEditFocus();
	private slots:
		void findPreviousRequest();
		void findNextRequest();
		void search(const QString& expression);
	private:
		QLineEdit *lineEdit;
		QToolButton *closePanelButton;
		QToolButton *clearLineButton;
		QToolButton *previousEntryButton;
		QToolButton *nextEntryButton;
		QFrame *verticalFrame1;
		QFrame *verticalFrame2;
		
		QPalette redPalette;
		QPalette greenPalette;
		QPalette defaultPalette;
	protected:
		void keyPressEvent(QKeyEvent *);
		void hideEvent(QHideEvent *);
};

#endif
