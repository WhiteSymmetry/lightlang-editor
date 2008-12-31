#ifndef NEWDICTWIDGET_H
#define NEWDICTWIDGET_H

#include <QtGui/QWidget>

class QLineEdit;
class QPushButton;
class QShowEvent;
class QLabel;
class QTimer;
class QToolButton;

class NewDictWidget : public QWidget
{
	Q_OBJECT
	signals:
		void createDictionary(const QString& name);
	public slots:
		void showWithRolling();
		void hideWithRolling();
	public:
		NewDictWidget();
		~NewDictWidget();
	
		void setInvalidNames(const QStringList& list);
	private slots:
		void checkNameFormat(const QString& name);
		void create();
		void updateSize();
	private:
		QTimer *timer;
		bool rollingToShow;
	
		QLineEdit *lineEdit;
		QLabel *warningLabel;
		QPushButton *createButton;
		QToolButton *closeButton;
	
		QStringList invalidNames;
	protected:
		void showEvent(QShowEvent *);
};


#endif
