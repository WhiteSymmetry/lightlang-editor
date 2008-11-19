#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QDebug>
#include "DatabaseCenter.h"
#include "LoadDictionaryThread.h"

LoadDictionaryThread::LoadDictionaryThread() {
	databaseCenter = new DatabaseCenter;
	stopped = false;
}

LoadDictionaryThread::~LoadDictionaryThread() {
	delete databaseCenter;
}

void LoadDictionaryThread::setDictionaryPath(const QString& path) {
	currentPath = path;
}

void LoadDictionaryThread::stop() {
	stopped = true;
}

QString LoadDictionaryThread::getAboutDict() const {
	return currentAboutDictionaryString;
}

void LoadDictionaryThread::run() {
	currentAboutDictionaryString.clear();
	
	QFile dictFile(currentPath);
	if (!dictFile.open(QIODevice::ReadOnly)) {
		qDebug() << "[LoadDictionaryThread] Cannot open dictionary with path" << currentPath;
		successful = false;
		stop();
	}
	QString name = QFileInfo(currentPath).fileName();
	
	if (!stopped && !databaseCenter->setDatabaseName(name)) {
		successful = false;
		stop();
	}
	
	QTextStream dictStream(&dictFile);
	
	QString currentAboutDictionaryString;
	
	if (!stopped) {
		int rows(0);
		for (rows = 0; !dictStream.atEnd(); rows++)
			dictStream.readLine();
		emit (rowsCounted(rows));
		
		dictStream.seek(0);
	}
	int currentRow(0);
	while (!dictStream.atEnd()) {
		if (stopped) {
			successful = false;
			break;
		}
		
		QString tempString = dictStream.readLine().trimmed();
		emit (rowChanged(++currentRow));
		if (tempString.isEmpty())
			continue;
		if (tempString.startsWith("#")) {
			currentAboutDictionaryString += tempString.remove(0,1) + '\n';
			continue;
		}
		QStringList list = tempString.split("  ");
		if (list.count() >= 2) {
			QString word = list[0];
			QString translation;
			for (int i = 1; i < list.count(); i++)
				translation += list[i] + "  ";
			databaseCenter->addNewWord(word,translation);
		}
	}
	if (!stopped)
		successful = true;
	stopped = false;
}

bool LoadDictionaryThread::isSuccessful() const {
	return successful;
}
