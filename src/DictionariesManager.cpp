#include <QtGui/QAction>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>
#include "InfoButton.h"
#include "PopupWindow.h"
#include "DictionariesManager.h"

//==============TreeWidgetWithButtons==================//

TreeWidgetWithButtons::TreeWidgetWithButtons(bool b) : TreeWidget(b) {
	mainLayout = new QHBoxLayout;
	
	QVBoxLayout *verticalLayout = new QVBoxLayout;
	verticalLayout->addStretch();
	verticalLayout->addLayout(mainLayout);
	setLayout(verticalLayout);
}

TreeWidgetWithButtons::~TreeWidgetWithButtons() {
	delete mainLayout;
}

void TreeWidgetWithButtons::addWidget(QWidget *widget) {
	mainLayout->addWidget(widget);
};

void TreeWidgetWithButtons::addStretch() {
	mainLayout->addStretch();
}

//==============Dictionaries Manager==================//

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
	
	removeOrNotDicitionaryDialog = new QMessageBox(parent);
	removeOrNotDicitionaryDialog->setIconPixmap(QIcon(":/icons/lle.png").pixmap(64,64));
	removeOrNotDicitionaryDialog->setWindowTitle(tr("Notification"));
	removeOrNotDicitionaryDialog->setText("<b>" + tr("Are you sure that you want to remove loaded dictionary?") + "</b><br>" + tr("If you didn't save the dictionary on hard disk, all changes will be lost."));
	removeDictionaryButton = removeOrNotDicitionaryDialog->addButton(tr("Remove the dictionary"),QMessageBox::ActionRole);
	cancelRemovingDictionaryButton = removeOrNotDicitionaryDialog->addButton(tr("Cancel"),QMessageBox::ActionRole);
	
	treeWidget = new TreeWidgetWithButtons(true);
	treeWidget->setHeaderLabels(QStringList() << tr("Direction") << tr("Name"));
	treeWidget->setContextMenuHeader(tr("Actions"));
	treeWidget->setContextMenuIcon(QIcon(":/icons/dicts_manager.png"));
	connect(treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(sendSignalToOpenDatabase()));
	
	openAction = new QAction(treeWidget);
	openAction->setText(tr("Open"));
	openAction->setIcon(QIcon(":/icons/open.png"));
	connect(openAction,SIGNAL(triggered()),this,SLOT(sendSignalToOpenDatabase()));
	
	removeAction = new QAction(treeWidget);
	removeAction->setText(tr("Remove"));
	removeAction->setIcon(QIcon(":/icons/remove.png"));
	connect(removeAction,SIGNAL(triggered()),this,SLOT(removeCurrentDictionary()));
	
	openButton = new QToolButton;
	openButton->setAutoRaise(true);
	openButton->setIcon(QIcon(":/icons/open.png"));
	openButton->setToolTip(tr("Open the dictionary"));
	openButton->setIconSize(QSize(16,16));
	connect(openButton,SIGNAL(clicked()),this,SLOT(sendSignalToOpenDatabase()));
	
	removeButton = new QToolButton;
	removeButton->setAutoRaise(true);
	removeButton->setIcon(QIcon(":/icons/remove.png"));
	removeButton->setToolTip(tr("Remove the dictionary"));
	removeButton->setShortcut(QKeySequence("Delete"));
	removeButton->setIconSize(QSize(16,16));
	connect(removeButton,SIGNAL(clicked()),this,SLOT(removeCurrentDictionary()));
	
	PopupWindow *popupWindow = new PopupWindow(this);
	
	infoButton = new InfoButton(popupWindow);
	infoButton->setPopupText(tr("In this dialog you can see the list of loaded dictionaries. To start dictionary edition you should open it. Also you can remove some dictionary or add it to the programs \"SL, XSL\"."));
	infoButton->setPopupHeaderText(tr("Dictionaries manager"));
	
	treeWidget->addStretch();
	treeWidget->addWidget(openButton);
	treeWidget->addWidget(removeButton);
	treeWidget->addStretch();
	treeWidget->addWidget(infoButton);
	
	treeWidget->addContextMenuAction(openAction);
	treeWidget->addContextMenuAction(removeAction);
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(treeWidget);
	mainLayout->setContentsMargins(0,0,0,0);
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
	setMinimumSize(300,200);
}

DictionariesManager::~DictionariesManager() {
	delete cancelRemovingDictionaryButton;
	delete removeDictionaryButton;
	delete removeOrNotDicitionaryDialog;
	
	delete openAction;
	delete removeAction;
	delete openButton;
	delete infoButton;
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

void DictionariesManager::removeCurrentDictionary() {
	removeOrNotDicitionaryDialog->exec();
	if (removeOrNotDicitionaryDialog->clickedButton() == removeDictionaryButton)
		sendSignalToRemoveDatabase();
}

QString DictionariesManager::getPathForDictionaryWithName(const QString& dbName) {
	QSqlQuery query;
	query.exec(QString("SELECT `path` FROM `dicts` WHERE name = \"%1\"").arg(dbName));
	if (query.isValid()) {
		query.next();
		return query.record().value(0).toString();
	}
	return "";
}

QString DictionariesManager::getDictionaryAboutWithName(const QString& dbName) {
	QSqlQuery query;
	query.exec(QString("SELECT `about` FROM `dicts` WHERE name = \"%1\"").arg(dbName));
	if (query.isValid()) {
		query.next();
		return query.record().value(0).toString();
	}
	return "";
}
