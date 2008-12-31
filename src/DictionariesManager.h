#ifndef DICTIONARIESMANAGER_H
#define DICTIONARIESMANAGER_H

#include <QtGui/QDialog>
#include "TreeWidget.h"

class QAction;
class QToolButton;
class QHBoxLayout;
class QMessageBox;
class QPushButton;
	
class TreeWidgetWithButtons : public TreeWidget
{
	public:
		TreeWidgetWithButtons(bool b);
		~TreeWidgetWithButtons();
	
		void addWidget(QWidget *widget);
		void addStretch();
	private:
		QHBoxLayout *mainLayout;
};

class DictionariesManager : public QDialog
{
	Q_OBJECT
	signals:
		void openDatabaseWithName(const QString& name);
		void removeDatabaseWithName(const QString& name);
	public:
		DictionariesManager(QWidget *parent = 0);
		~DictionariesManager();
	
		void addDictionary(const QString& fullName,const QString& path = QString(),const QString& about = QString());
	
		QStringList getExistingDictionaries();
	private slots:
		void sendSignalToOpenDatabase();
		void sendSignalToRemoveDatabase();
		void removeCurrentDictionary();
	private:
		QMessageBox *removeOrNotDicitionaryDialog;
		QPushButton *removeDictionaryButton;
		QPushButton *cancelRemovingDictionaryButton;
	
		TreeWidgetWithButtons *treeWidget;
	
		QAction *openAction;
		QAction *removeAction;
		QAction *addToSlAction;
	
		QToolButton *openButton;
		QToolButton *removeButton;
		QToolButton *addToSlButton;
};


#endif
