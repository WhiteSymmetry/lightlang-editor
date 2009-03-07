#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui/QWidget>

class QLineEdit;
class TranslationEditor;
class QAction;
class DatabaseCenter;
class QToolButton;
class QTimer;
class Menu;
class EditorTipsWidget;
class FindInTranslationPanel;
class HighLighter;

class TabWidget : public QWidget
{
	Q_OBJECT
	signals:
		void renameTab(int index,const QString& name);
		void showStatusMessage(const QString& message);
	public slots:
		void showSearchingPanel();
	public:
		TabWidget(QString firstWord,DatabaseCenter *databaseCenter,int index,int updateTranslationInterval);
		~TabWidget();
		
		void useHighlighting(bool highlighting);
		
		void setUpdateTranslationInterval(int interval);
		
		void setFocusAtThisTab();
		void setEditorMenu(Menu *menu);
		
		QString getTranslationAsHtml();
		
		void undo();
		void redo();
		void cut();
		void copy();
		void paste();
	
		void setHtml(const QString& htmlText);
		void setReadOnly(bool readOnly);
	private slots:
		void textChanged(const QString& text);
		void updateTranslation();
		void addWord();
		void editWord();
		void removeWord();
		void translationChanged();
	private:
		void resetButtonsAccessibility();
	
		bool updateTranslationDuringEntering;
		QTimer *timer;
		
		FindInTranslationPanel *findInTranslationPanel;
	
		int tabIndex;
	
		DatabaseCenter *databaseCenter;
		
		HighLighter *highlighter;
	
		QLineEdit *lineEdit;
		QToolButton *clearLineEditButton;
		
		TranslationEditor *textEdit;
	
		QToolButton *addWordToolButton;
		QToolButton *editWordToolButton;
		QToolButton *removeWordToolButton;
		QToolButton *updateTranslationButton;
};

#endif
