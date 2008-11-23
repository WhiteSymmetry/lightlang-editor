#ifndef LOADDICTIONARYTHREAD_H
#define LOADDICTIONARYTHREAD_H

#include <QtCore/QThread>

class DatabaseCenter;

class LoadDictionaryThread : public QThread	
{
	Q_OBJECT
	signals:
		void rowsCounted(int rows);
		void rowChanged(int row);
	public slots:
		void stop();
		void cancel();
	public:
		LoadDictionaryThread();
		~LoadDictionaryThread();
	
		bool isSuccessful() const;
	
		QString getAboutDict() const;
		void setDictionaryPath(const QString& path);
	protected:
		void run();
	private:
		volatile bool stopped;
		volatile bool canceled;
		QString currentPath;
	
		bool successful;
	
		QString currentAboutDictionaryString;
	
		DatabaseCenter *databaseCenter;
};


#endif
