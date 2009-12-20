#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtCore/QVariant>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
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
	databasesPath = QDir::toNativeSeparators(QDir::homePath() + "/.LilEditorData/Databases/");
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

	QSqlQuery *query = new QSqlQuery(QSqlDatabase::database(currentConnectionName));

	databasesWithQueries[databaseName] = query;

	currentConnectionName = databaseName;

	query->exec("CREATE TABLE IF NOT EXISTS main(`word` TEXT NOT NULL,`translation` TEXT NOT NULL,`mark` INTEGER(1) NOT NULL, UNIQUE (`word`))");
	if (!query->isActive())
		qDebug() << "[DatabaseCenter] Cannot create table, because: " << query->lastError().text();
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
	query.exec(QString("UPDATE `main` SET `translation` = \"%2\"  WHERE `word` = \"%1\"").arg(word.toLower()).arg(translation));
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Set translation for" << word << " with error:" << query.lastError().text();
	return query.isActive();
}

bool DatabaseCenter::addNewWord(const QString& word,const QString& translation) {
	QSqlQuery *query = databasesWithQueries[currentConnectionName];
	
	query->prepare("INSERT INTO main(word,translation,mark) VALUES(:word,:translation,:mark)");
	// TODO: Выкинуть все simplified() из таких контекстов, как ниже
	query->bindValue(":word",word.simplified().toLower());
	query->bindValue(":translation",translation.simplified());
	query->bindValue(":mark","0");
	query->exec();
	if (!query->isActive())
		qDebug() << "[DatabaseCenter] Add new word" << word << " with error:" << query->lastError().text();
	return query->isActive();
}

bool DatabaseCenter::removeWord(const QString& word) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("DELETE FROM `main` WHERE word = \"%1\"").arg(word.toLower()));
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Delete word" << word << " with error:" << query.lastError().text();
	return query.isActive();
}

bool DatabaseCenter::markWord(const QString& word,bool mark) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	if (mark)
		query.exec(QString("UPDATE `main` SET `mark` = '1'  WHERE `word` = \"%1\"").arg(word.toLower()));
	else
		query.exec(QString("UPDATE `main` SET `mark` = '0'  WHERE `word` = \"%1\"").arg(word.toLower()));
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Word" << word << " marked with error:" << query.lastError().text();
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
	query.exec(QString("SELECT mark FROM main WHERE word=\'%1\'").arg(word));
	query.next();
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

bool DatabaseCenter::isWordMarked(const QString& word) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("SELECT mark FROM main WHERE word=\'%1\'").arg(word.toLower()));
	query.next();
	return query.record().value(0).toBool();
}

QString DatabaseCenter::getCurrentDatabaseName() const {
	return currentConnectionName;
}

int DatabaseCenter::saveCurrentDatabaseAs(const QString& dictionaryPath,const QString& aboutDictionaryText) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	QFile file(dictionaryPath);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "[DatabaseCenter] Cannot save dictionary to" << dictionaryPath << "because of error number" << file.error();
		return file.error();
	}
	QTextStream stream(&file);
	if (!aboutDictionaryText.isEmpty())
		stream << '#' << aboutDictionaryText << '\n';
	query.exec("SELECT * FROM main");
	while (query.next()) {
		if (!query.record().value(1).toString().simplified().isEmpty())
			stream << query.record().value(0).toString().simplified() << "  " << query.record().value(1).toString().simplified() << '\n';
	}
	file.close();
	
	return 0;
}

QStringList DatabaseCenter::getWordsStartsWith(const QString& preString,int limit) {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	if (limit != 0)
		query.exec(QString("SELECT word FROM main WHERE word LIKE \'%1%\' LIMIT 0,%2").arg(preString).arg(limit));
	else
		query.exec(QString("SELECT word FROM main WHERE word LIKE \'%1%\'").arg(preString));
	QStringList list;
	while (query.next())
		list << query.record().value(0).toString();
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Get all words starts with" << preString << "with error" << query.lastError().text();
	return list;
}

QStringList DatabaseCenter::getAllMarkedWords() {
	QSqlQuery query(QSqlDatabase::database(currentConnectionName));
	query.exec(QString("SELECT word FROM main WHERE mark=1"));
	QStringList list;
	while (query.next())
		list << query.record().value(0).toString();
	if (!query.isActive())
		qDebug() << "[DatabaseCenter] Get all marked words with error" << query.lastError().text();
	return list;
}
