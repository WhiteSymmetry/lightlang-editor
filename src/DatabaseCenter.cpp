#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtCore/QVariant>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QMessageBox>
#include <QDebug>
#include "DatabaseCenter.h"

#define DEBUG

inline bool createConnection(const QString& dbName) {
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",dbName);
	database.setHostName("localhost");
	database.setUserName("Vialinx");
	database.setPassword("IMP");
	database.setDatabaseName(dbName);
	if (!database.open()) {
		qDebug() << "[DatabaseCenter] Cannot to open database with name" << dbName << "because" << database.lastError().text();
		return false;
	} else
		qDebug() << "[DatabaseCenter] Database with name" << dbName << "was opened";
	return true;
}

DatabaseCenter::DatabaseCenter() {
	databasesPath = QDir::toNativeSeparators(QDir::currentPath() + "/EditorData/Databases/");
}

DatabaseCenter::~DatabaseCenter() {
	if (!currentConnectionName.isEmpty())
		QSqlDatabase::removeDatabase(currentConnectionName);
}

bool DatabaseCenter::setDatabaseName(const QString& databaseName) {	
	QDir::setCurrent(databasesPath);

	if (!currentConnectionName.isEmpty()) {
		previousConnectionName = currentConnectionName;
		QSqlDatabase::removeDatabase(currentConnectionName);
	}
	
	qDebug() << "[DatabaseCenter] Database path became " << databasesPath + databaseName;
	
	if (!createConnection(databaseName))
		return false;
	
	currentConnectionName = databaseName;
	
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec("CREATE TABLE IF NOT EXISTS main(`word` TEXT NOT NULL,`translation` TEXT NOT NULL,`status` INTEGER(1) NOT NULL, UNIQUE (`word`))");
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Cannot create table, because: " << query.lastError().text();
	emit (databaseNameChanged(databaseName));
	return true;
}

QString DatabaseCenter::getTranslationForWord(const QString& word) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("SELECT `translation` FROM `main` WHERE word = \'%1\'").arg(word));
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Get translation for" << word << "with error:" << query.lastError().text();
	if (query.next())
		return query.value(0).toString();
	else
		return "";
}

bool DatabaseCenter::setTranslationForWord(const QString& word,const QString& translation) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("UPDATE `main` SET `translation` = \"%2\"  WHERE `word` = \"%1\"").arg(word).arg(translation));
	return query.isActive();
}

bool DatabaseCenter::addNewWord(const QString& word,const QString& translation) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("INSERT INTO main(word,translation,status) VALUES(\'%1\',\'%2\',\'0\')").arg(word.simplified()).arg(translation.simplified()));
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Add new word" << word << " with error:" << query.lastError().text();
	return query.isActive();
}

bool DatabaseCenter::removeWord(const QString& word) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("DELETE FROM `main` WHERE word = \"%1\"").arg(word));
	return query.isActive();
}

QList<WordWithTrans> DatabaseCenter::getAllWordsWithTranses() {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec("SELECT * FROM `main`");
	QList<WordWithTrans> list;
	while (query.next()) 
		list << WordWithTrans(query.record().value(0).toString().simplified(),query.record().value(1).toString().simplified());
	return list;
}

bool DatabaseCenter::isThereWordInDatabase(const QString& word) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("SELECT `status` FROM main WHERE word=\'%1\'").arg(word));
	return !query.isNull(0);
}

void DatabaseCenter::removeDatabaseWithName(const QString& databaseName) {
	QSqlDatabase::removeDatabase(databaseName);
	QDir(databasesPath).remove(databaseName);
	if (!previousConnectionName.isEmpty())
		setDatabaseName(previousConnectionName);
}

bool DatabaseCenter::doesDictionaryExist(const QString& pathToDict) {
	return QDir(databasesPath).exists(QFileInfo(pathToDict).fileName());
}

QString DatabaseCenter::getCurrentDatabaseName() const {
	return currentConnectionName;
}

