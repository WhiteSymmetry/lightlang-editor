#ifndef TABSWIDGET_H
#define TABSWIDGET_H

#include <QtGui/QTabWidget>

class TabWidget;
class QToolButton;
class DatabaseCenter;
class Menu;
class QAction;

class TabsWidget : public QTabWidget
{
	Q_OBJECT
	signals:
		void closeTabButtonClicked();
		void showStatusMessage(const QString& message);
	public slots:
		TabWidget* openNewTab(const QString& tabTitle = QString());
		void closeCurrentTab();
		void setAllTipsHidden(bool toHide);
		void moveNextTab();
		void movePreviousTab();
		void showFindWidgetInCurrentTab();
		void setFocusOnCurrentTab();
	public:
		TabsWidget(DatabaseCenter *databaseCenter,QWidget *parent = 0);
		~TabsWidget();
	
		void setEditorMenu(Menu *menu);
		void setUpdateTranslationInterval(int interval);
		void useHighlighting(bool highlighting);
	private slots:
		void renameTab(int index,const QString& name);
		void currentTabChanged(int index);
		void hideAllTips();
		void showNextTipInCurrentTab();
		void showPreviousTipInCurrentTab();
	private:
		DatabaseCenter *databaseCenter;
	
		bool showTips;
		int updateTranslationInterval;
		bool highlightTranslation;
		
		QList<TabWidget *> tabs;
		Menu *editorMenu;
		
		Menu *tipsMenu;
		QAction *hideAllTipsAction;
		QAction *nextTipAction;
		QAction *previousTipAction;
	
		QToolButton *newTabButton;
		QToolButton *closeCurrentTabButton;
};

#endif
