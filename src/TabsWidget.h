#ifndef TABSWIDGET_H
#define TABSWIDGET_H

#include <QtGui/QTabWidget>

class TabWidget;
class QToolButton;
class DatabaseCenter;
class Menu;

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
		
		void showFindWidgetInCurrentTab();
		void setFocusOnCurrentTab();
		void redoActionInCurrentTab();
		void undoActionInCurrentTab();
		void cutInCurrentTab();
		void copyInCurrentTab();
		void pasteInCurrentTab();
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
	
		QToolButton *newTabButton;
		QToolButton *closeCurrentTabButton;
};

#endif
