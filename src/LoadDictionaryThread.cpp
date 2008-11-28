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
	canceled = false;
	currentFile = new QFile;
	currentStream = new QTextStream(currentFile);
	
	rows = 0;
	currentRow = 0;
}

LoadDictionaryThread::~LoadDictionaryThread() {
	delete currentStream;
	delete currentFile;
	delete databaseCenter;
}

bool LoadDictionaryThread::setDictionaryPath(const QString& path) {
	if (currentFile->isOpen())
		currentFile->close();
	currentFile->setFileName(path);
	if (!currentFile->open(QIODevice::ReadOnly)) {
		qDebug() << "[LoadDictionaryThread] Cannot open dictionary with path" << path;
		return false;
	}
	
	QString name = QFileInfo(path).fileName();
	if (!databaseCenter->setDatabaseName(name)) {
		qDebug() << "[LoadDictionaryThread] Cannot set database name" << name;
		return false;
	}
	
	for (rows = 0; !currentStream->atEnd(); rows++)
		currentStream->readLine();
	emit (rowsCounted(rows));
	currentStream->seek(0);
	currentRow = 0;

	stopped = false;
	canceled = false;
	
	return true;
}

QString LoadDictionaryThread::getAboutDict() const {
	return currentAboutDictionaryString;
}

void LoadDictionaryThread::run() {
	while (!currentStream->atEnd()) {
		if (canceled || stopped)
			break;
		
		QString tempString = currentStream->readLine().trimmed();
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
	if (!canceled && !stopped)
		emit (loadingFinished());
}

bool LoadDictionaryThread::isStopped() const {
	return stopped;
}

bool LoadDictionaryThread::isCanceled() const {
	return canceled;
}

bool LoadDictionaryThread::startLoading(const QString& dictionaryPath) {
	if (!setDictionaryPath(dictionaryPath))
		return false;
	currentAboutDictionaryString.clear();
	continueLoading();
	return true;
}

void LoadDictionaryThread::cancelLoading() {
	canceled = true;
}

void LoadDictionaryThread::stopLoading() {
	stopped = true;
}

void LoadDictionaryThread::restartLoading() {
	startLoading(currentFile->fileName());
}

void LoadDictionaryThread::continueLoading() {
	stopped = false;
	start();
}
