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
	startLoadingFromRowNumber = 0;
	
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
	currentDictionaryPath = path;
	
	return true;
}

QString LoadDictionaryThread::getDictionaryInformation() const {
	return currentDictionaryInformationString;
}

QString LoadDictionaryThread::getDictionaryPath() const {
	return currentDictionaryPath;
}

void LoadDictionaryThread::run() {
	int rowsStepsCount = 0;

	int count = 0;
	while (!currentStream->atEnd() && startLoadingFromRowNumber > count) {
	    currentStream->readLine();
	    count++;
	}

	while (!currentStream->atEnd()) {
		if (canceled || stopped)
			break;
		
		QString tempString = currentStream->readLine();

		if (rowsStepsCount > 200) {
			emit (rowChanged(currentRow));
			rowsStepsCount = 0;
		} else {
			rowsStepsCount++;
		}
		currentRow++;

		if (tempString.isEmpty())
			continue;

		if (tempString[0] == '#') {
			currentDictionaryInformationString += tempString.remove(0,1) + '\n';
			continue;
		}

		int index = tempString.indexOf("  ");
		if (index > -1)
			if (!databaseCenter->addNewWord(tempString.left(index),tempString.right(tempString.length() - index).trimmed()))
				qDebug() << "[LoadDictionaryThread] " << tempString.right(tempString.length() - index).trimmed();
	}
	if (!canceled && !stopped) {
		emit (rowChanged(currentRow));
		emit (loadingFinished());
	}
	startLoadingFromRowNumber = 0;
}

bool LoadDictionaryThread::isStopped() const {
	return stopped;
}

bool LoadDictionaryThread::isCanceled() const {
	return canceled;
}

int LoadDictionaryThread::getCurrentRow() const {
    return currentRow;
}

bool LoadDictionaryThread::startLoading(const QString& dictionaryPath,int startLoadingFromRow) {
	if (!setDictionaryPath(dictionaryPath))
		return false;
	currentDictionaryInformationString.clear();
	startLoadingFromRowNumber = startLoadingFromRow;
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
	startLoading(currentFile->fileName(),0);
}

void LoadDictionaryThread::continueLoading() {
	stopped = false;
	start();
}
