#ifndef QACTIONEX_H
#define QACTIONEX_H

#include <QtWidgets/QAction>
#include <QProcess>
#include <QSignalMapper>

class QActionEx : public QAction
{

	Q_OBJECT

public:
	QActionEx(QObject *parent);
	~QActionEx();
	
	QProcess *actionProcess() const
	{ return _actionProcess; }
	quint32 actionProcessId() const
	{ return _actionProcessId; }
	QString actionProcessName() const
	{ return _actionProcessName; }
	QString actionProcessFile() const
	{ return _actionProcessFile; }
	QStringList actionProcessArgs() const
	{ return _actionProcessArgs; }
	QSignalMapper *startedSignalMapper() const
	{ return _startedSignalMapper; }
	QSignalMapper *finishedSignalMapper() const
	{ return _finishedSignalMapper; }
	QSignalMapper *stateChangedSignalMapper() const
	{ return _stateChangedSignalMapper; }
	QSignalMapper *errorSignalMapper() const
	{ return _errorSignalMapper; }
	QSignalMapper *readyReadStandardOutputSignalMapper() const
	{ return _readyReadStandardOutputSignalMapper; }
	QSignalMapper *readyReadStandardErrorSignalMapper() const
	{ return _readyReadStandardErrorSignalMapper; }

	void setActionProcess(QProcess *process);
	void setActionProcessId(const quint32 &id);
    void setActionProcessName(const QString &name);
    void setActionProcessFile(const QString &file);
    void setActionProcessArgs(const QStringList &args);
    void setStartedSignalMapper(QSignalMapper *signalMapper);
    void setFinishedSignalMapper(QSignalMapper *signalMapper);
    void setStateChangedSignalMapper(QSignalMapper *signalMapper);
    void setErrorSignalMapper(QSignalMapper *signalMapper);
    void setReadyReadStandardOutputSignalMapper(QSignalMapper *signalMapper);
    void setReadyReadStandardErrorSignalMapper(QSignalMapper *signalMapper);
	bool startActionProcess(QStringList &args, int waitMsecs = 5000);
	bool closeActionProcess(int waitMsecs = 5000);


private:

	QProcess *_actionProcess;
	quint32 _actionProcessId;
	QString _actionProcessName;
	QString _actionProcessFile;
	QStringList _actionProcessArgs;
	QSignalMapper *_startedSignalMapper;
	QSignalMapper *_finishedSignalMapper;
	QSignalMapper *_stateChangedSignalMapper;
	QSignalMapper *_errorSignalMapper;
	QSignalMapper *_readyReadStandardOutputSignalMapper;
	QSignalMapper *_readyReadStandardErrorSignalMapper;

};

#endif // QACTIONEX_H
