#ifndef DICTIONARIESMANAGER
#define DICTIONARIESMANAGER

#include <QtGui/QDialog>

class TreeWidget;
class QAction;
class QPushButton;

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
	private:
		TreeWidget *treeWidget;
	
		QAction *openAction;
		QAction *removeAction;
		QAction *addToSlAction;
	
		QPushButton *openButton;
		QPushButton *removeButton;
		QPushButton *addToSlButton;
};


#endif
