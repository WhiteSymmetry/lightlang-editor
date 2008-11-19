#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui/QWidget>

class QLineEdit;
class QPushButton;
class TranslationEditor;
class QAction;
class DatabaseCenter;

class TabWidget : public QWidget
{
	public:
		TabWidget(DatabaseCenter *databaseCenter);
		~TabWidget();
	
		void setHtml(const QString& htmlText);
		void setReadOnly(bool readOnly);
	private:
		DatabaseCenter *databaseCenter;
	
		QLineEdit *lineEdit;
		QPushButton *clearLineEditButton;
		QPushButton *searchButton;
		
		TranslationEditor *textEdit;
};

#endif
