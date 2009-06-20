#ifndef MANUAL_H
#define MANUAL_H

#include <QtGui/QDialog>

class QComboBox;
class QTextBrowser;
class QLabel;

class Manual : public QDialog
{
	Q_OBJECT
	public:
		Manual(QWidget *parent = 0);
		~Manual();

		void saveSettings();
		void loadSettings();
	private slots:
		void showContentNumber(int index);
	private:
		QString language;
		
		QLabel *titleLabel;
		QComboBox *contentComboBox;
		QTextBrowser *textBrowser;
};


#endif

