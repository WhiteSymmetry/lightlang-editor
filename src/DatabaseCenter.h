#ifndef DATABASECENTER_H
#define DATABASECENTER_H

#include <QtCore/QObject>

struct WordWithTrans
{
	QString word;
	QString translation;
	
	WordWithTrans(const QString& w, const QString& t) {
		word = w;
		translation = t;
	}
};

class DatabaseCenter : public QObject
{
	Q_OBJECT
	signals:
		void databaseNameChanged(const QString& name);
	public:
		DatabaseCenter();
		~DatabaseCenter();
		bool setDatabaseName(const QString& databaseName);
	
		void removeDatabaseWithName(const QString& databaseName);
	
		int saveCurrentDatabaseAs(const QString& dictPath,const QString& aboutDictionaryText);
	
		QStringList getWordsStartsWith(const QString& preString,int limit = 50);
		QStringList getAllMarkedWords();
		
		QString getTranslationForWord(const QString& word);
		bool isWordMarked(const QString& word);
		bool setTranslationForWord(const QString& word,const QString& translation);
		bool addNewWord(const QString& word,const QString& translation);
		bool removeWord(const QString& word);
		bool markWord(const QString& word, bool mark);
		QList<WordWithTrans> getAllWordsWithTranses();
	
		QString getCurrentDatabaseName() const;
	
		bool doesDictionaryExist(const QString& pathToDict);
		bool isThereWordInDatabase(const QString& word);
	private:
		QString currentConnectionName;
		QString previousConnectionName;
		QString databasesPath;
};

#endif
