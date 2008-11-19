#ifndef NEWDICTWIDGET_H
#define NEWDICTWIDGET_H

#include <QtGui/QWidget>

class QLineEdit;
class QPushButton;
class QShowEvent;
class QLabel;

class NewDictWidget : public QWidget
{
	Q_OBJECT
	signals:
		void createDictionary(const QString& name);
	public:
		NewDictWidget();
		~NewDictWidget();
	
		void setInvalidNames(const QStringList& list);
	private slots:
		void checkNameFormat(const QString& name);
		void create();
	private:
		QLineEdit *lineEdit;
		QLabel *warningLabel;
		QPushButton *createButton;
		QPushButton *closeButton;
	
		QStringList invalidNames;
	protected:
		void showEvent(QShowEvent *);
};


#endif
