#ifndef DIALOGFORQUASTIONS_H
#define DIALOGFORQUASTIONS_H

#include <QtGui/QMessageBox>

class QLabel;

class DialogForQuastions : public QMessageBox
{
	Q_OBJECT
	public:		
		DialogForQuastions(QWidget *parent = 0);
		~DialogForQuastions();
	
		void setHeaderText(const QString& headerText);
		void setText(const QString& text);
		void setIcon(const QIcon& icon);
	private:
		QLabel *headerLabel;
		QLabel *textLabel;
		QLabel *iconLabel;
};

#endif
