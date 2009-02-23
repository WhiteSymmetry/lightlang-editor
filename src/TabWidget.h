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

class TabWidget : public QWidget
{
	Q_OBJECT
	signals:
		void renameTab(int index,const QString& name);
		void showStatusMessage(const QString& message);
	public slots:
		void showSearchingPanel();
	public:
		TabWidget(DatabaseCenter *databaseCenter,int index,int updateTranslationInterval);
		~TabWidget();
		
		void setUpdateTranslationInterval(int interval);
		void setTipsHidden(bool toHide);
	
		void setFocusAtThisTab();
		void setEditorMenu(Menu *menu);
		void setTipsMenu(Menu *menu);
	
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
		void formatSlStringIntoHtmlString(QString& str);
		void formatHtmlStringIntoSlString(QString& str);
	
		bool updateTranslationDuringEntering;
		QTimer *timer;
		
		EditorTipsWidget *editorTipsWidget;
		
		FindInTranslationPanel *findInTranslationPanel;
	
		int tabIndex;
	
		DatabaseCenter *databaseCenter;
	
		QLineEdit *lineEdit;
		QToolButton *clearLineEditButton;
		
		TranslationEditor *textEdit;
	
		QToolButton *addWordButton;
		QToolButton *editWordButton;
		QToolButton *removeWordButton;
		QToolButton *updateTranslationButton;
};

#endif
