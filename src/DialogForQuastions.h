#ifndef DIALOGFORQUASTIONS_H
#define DIALOGFORQUASTIONS_H

#include <QtGui/QDialog>

class QPushButton;
class QLabel;

class DialogForQuastions : public QDialog
{
	Q_OBJECT
	public:
		enum Result { Save, DontSave, Cancel };
		enum Mode { SaveOrNotDocument, Error };
		
		DialogForQuastions(Mode mode,QWidget *parent = 0);
		~DialogForQuastions();
	
		Result getResult() const;
		
		void setText(const QString& text);
		void setIcon(const QIcon& icon);
	private slots:
		void saveWasClicked();
		void dontSaveWasClicked();
		void cancelWasClicked();
	private:
		Result currentResult;
	
		QLabel *textLabel;
		QLabel *iconLabel;
	
		QPushButton *saveButton;
		QPushButton *dontSaveButton;
		QPushButton *cancelButton;
};

#endif
