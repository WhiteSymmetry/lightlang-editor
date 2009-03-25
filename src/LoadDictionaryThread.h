#ifndef LOADDICTIONARYTHREAD_H
#define LOADDICTIONARYTHREAD_H

#include <QtCore/QThread>

class DatabaseCenter;
class QTextStream;
class QFile;

class LoadDictionaryThread : public QThread	
{
	Q_OBJECT
	signals:
		void loadingFinished();

		void rowsCounted(int rows);
		void rowChanged(int row);
	public slots:
		void continueLoading();
		bool startLoading(const QString& dictionaryPath);
		void cancelLoading();
		void stopLoading();
		void restartLoading();
	public:
		LoadDictionaryThread();
		~LoadDictionaryThread();
	
		bool isCanceled() const;
		bool isStopped() const;
	
		QString getDictionaryInformation() const;
		QString getDictionaryPath() const;
	protected:
		void run();
	private:
		bool setDictionaryPath(const QString& path);
	
		volatile bool stopped;
		volatile bool canceled;
	
		QString currentDictionaryInformationString;
		QString currentDictionaryPath;
	
		DatabaseCenter *databaseCenter;
	
		QFile *currentFile;
		QTextStream *currentStream;
	
		int rows;
		int currentRow;
};


#endif
