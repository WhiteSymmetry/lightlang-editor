#include <QtGui/QAction>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTreeWidgetItem>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>
#include "TreeWidget.h"
#include "DictionariesManager.h"

inline bool createConnection() {
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setHostName("localhost");
	database.setUserName("Vialinx");
	database.setPassword("IMP");
	database.setDatabaseName("EditorData/Control/DictionariesDB");
	if (!database.open()) {
		qDebug() << "[DictionariesManager] Cannot to open database with name DictionariesDB, because" << database.lastError().text();
		return false;
	} else
		qDebug() << "[DictionariesManager] Database with name DictionariesDB was opened";
	return true;
}

DictionariesManager::DictionariesManager(QWidget *parent) : QDialog(parent) {
	treeWidget = new TreeWidget(true);
	treeWidget->setHeaderLabels(QStringList() << tr("Direction") << tr("Name"));
	treeWidget->setContextMenuHeader(tr("Actions"));
	treeWidget->setContextMenuIcon(QIcon(":/icons/dicts_manager.png"));
	
	openAction = new QAction(treeWidget);
	openAction->setText(tr("Open"));
	openAction->setIcon(QIcon(":/icons/open.png"));
	connect(openAction,SIGNAL(triggered()),this,SLOT(sendSignalToOpenDatabase()));
	
	removeAction = new QAction(treeWidget);
	removeAction->setText(tr("Remove"));
	removeAction->setIcon(QIcon(":/icons/cancel.png"));
	connect(removeAction,SIGNAL(triggered()),this,SLOT(sendSignalToRemoveDatabase()));
	
	addToSlAction = new QAction(treeWidget);
	addToSlAction->setText(tr("Add to SL"));
	addToSlAction->setIcon(QIcon(":/icons/new_tab.png"));
	
	openButton = new QPushButton;
	openButton->setFlat(true);
	openButton->setIcon(QIcon(":/icons/open.png"));
	connect(openButton,SIGNAL(clicked()),this,SLOT(sendSignalToOpenDatabase()));
	
	removeButton = new QPushButton;
	removeButton->setFlat(true);
	removeButton->setIcon(QIcon(":/icons/cancel.png"));
	connect(removeButton,SIGNAL(clicked()),this,SLOT(sendSignalToRemoveDatabase()));
	
	addToSlButton = new QPushButton;
	addToSlButton->setFlat(true);
	addToSlButton->setIcon(QIcon(":/icons/new_tab.png"));
	
	treeWidget->addContextMenuAction(openAction);
	treeWidget->addContextMenuAction(removeAction);
	treeWidget->addContextMenuAction(addToSlAction);
	
	QVBoxLayout *actionsLayout = new QVBoxLayout;
	actionsLayout->addWidget(openButton);
	actionsLayout->addWidget(removeButton);
	actionsLayout->addWidget(addToSlButton);
	actionsLayout->addStretch();
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(treeWidget);
	mainLayout->addLayout(actionsLayout);
	setLayout(mainLayout);
	
	setWindowTitle(tr("Dictionaries Manager"));
	setWindowIcon(QIcon(":/icons/dicts_manager.png"));
	
	createConnection();
	
	QSqlQuery query;
	query.exec("CREATE TABLE IF NOT EXISTS dicts(`name` TEXT,`direction` TEXT,`path` TEXT,`about` TEXT, UNIQUE(`name`))");
	if (!query.isActive())
		qDebug() << "[DictionariesManager] Cannot create table, because: " << query.lastError().text();
	
	query.exec("SELECT * FROM dicts");
	while (query.next()) {
		QTreeWidgetItem *newItem = new QTreeWidgetItem(treeWidget);
		newItem->setText(0,query.record().value(1).toString());
		newItem->setText(1,query.record().value(0).toString());
	}
}

DictionariesManager::~DictionariesManager() {
	delete openAction;
	delete removeAction;
	delete addToSlAction;
	delete addToSlButton;
	delete openButton;
	delete removeButton;
	delete treeWidget;
}

void DictionariesManager::addDictionary(const QString& fullName,const QString& path,const QString& about) {
	QString nameWithDirection = fullName;
	QString direction = nameWithDirection.split(".").last();
	QString name = nameWithDirection.remove("." + direction);
	QTreeWidgetItem *newItem = new QTreeWidgetItem(treeWidget);
	newItem->setText(0,direction);
	newItem->setText(1,name);

	QSqlQuery query;
	query.exec(QString("INSERT INTO `dicts`(name,direction,path,about) VALUES('%1','%2','%3','%4')")
														.arg(name).arg(direction).arg(path).arg(about));
	if (!query.isActive())
		qDebug() << "[DictionariesManager] Cannot insert new dictionary into table, because: " << query.lastError().text();
}

void DictionariesManager::sendSignalToOpenDatabase() {
	QTreeWidgetItem *current = treeWidget->currentItem();
	if (current == 0)
		return;
	emit (openDatabaseWithName(current->text(1) + "." + current->text(0)));
	hide();
}

void DictionariesManager::sendSignalToRemoveDatabase() {
	QTreeWidgetItem *current = treeWidget->currentItem();
	if (current == 0)
		return;
	QString name = current->text(1);
	QSqlQuery query;
	query.exec(QString("DELETE FROM `dicts` WHERE name = \"%1\"").arg(name));
	if (!query.isActive())
		qDebug() << "[DictionariesManager] Cannot delete dictionary" << name << "from table, because: " << query.lastError().text();
	emit (removeDatabaseWithName(name + "." + current->text(0)));
	delete current;
}

QStringList DictionariesManager::getExistingDictionaries() {
	QSqlQuery query;
	query.exec("SELECT * FROM dicts");
	QStringList list;
	while (query.next())
		list << (query.record().value(0).toString() + "." + query.record().value(1).toString());
	return list;
}
