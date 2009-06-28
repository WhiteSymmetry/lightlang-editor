#ifndef TABSWIDGET_H
#define TABSWIDGET_H

#include <QtGui/QTabWidget>

class TabWidget;
class QToolButton;
class DatabaseCenter;
class Menu;
class PopupWindow;

class TabsWidget : public QTabWidget
{
	Q_OBJECT
	signals:
		void closeTabButtonClicked();
		void showStatusMessage(const QString& message);
	public slots:
		TabWidget* openNewTab(const QString& tabTitle = QString());
		void closeCurrentTab();
		void moveNextTab();
		void movePreviousTab();
		
		void previewCurrentTranslation();
		void showFindWidgetInCurrentTab();
		void setFocusOnCurrentTab();
		void redoActionInCurrentTab();
		void undoActionInCurrentTab();
		void cutInCurrentTab();
		void copyInCurrentTab();
		void pasteInCurrentTab();
		
		void pasteItalicInCurrentTab();
		void pasteUnderlineInCurrentTab();
		void pasteOfficeWordInCurrentTab();
		void pasteBoldInCurrentTab();
		void pasteSoundInCurrentTab();
		void pasteIndentInCurrentTab();
		void pasteLinkInCurrentTab();
	public:
		TabsWidget(DatabaseCenter *databaseCenter,QWidget *parent = 0);
		~TabsWidget();
	
		void setEditorMenu(Menu *menu);
		void setUpdateTranslationInterval(int interval);
		void useHighlighting(bool highlighting);
	private slots:
		void renameTab(int index,const QString& name);
		void currentTabChanged(int index);
	private:
		DatabaseCenter *databaseCenter;
	
		int updateTranslationInterval;
		bool highlightTranslation;
		
		QList<TabWidget *> tabs;
		Menu *editorMenu;
		
		PopupWindow *previewPopupWindow;
	
		QToolButton *newTabButton;
		QToolButton *closeCurrentTabButton;
};

#endif
