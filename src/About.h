#ifndef ABOUT_H
#define ABOUT_H

#include <QtGui/QDialog>

class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class About : public QDialog
{
	Q_OBJECT
	private:
		QLabel *iconLabel;
		QLabel *textLabel;
		QHBoxLayout *topLayout;
		QHBoxLayout *bottomLayout;
		QVBoxLayout *mainLayout;
	public:
		About(QWidget *parent = 0);
		~About();
};

#endif
