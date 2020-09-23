#include "qactionex.h"
#include <QFile>

QActionEx::QActionEx(QObject *parent)
	: QAction(parent)
{
	_actionProcess = NULL;
	_actionProcessId = 0;
	_actionProcessName = "";
	_actionProcessFile = "";
	_actionProcessArgs;
	_startedSignalMapper = NULL;
	_finishedSignalMapper = NULL;
	_stateChangedSignalMapper = NULL;
	_errorSignalMapper = NULL;
	_readyReadStandardOutputSignalMapper = NULL;
	_readyReadStandardErrorSignalMapper = NULL;
}

QActionEx::~QActionEx()
{

}

void QActionEx::setActionProcess(QProcess *process)
{
	_actionProcess = process;
}

void QActionEx::setActionProcessId(const quint32 &id)
{
	_actionProcessId = id;
}

void QActionEx::setActionProcessName(const QString &name)
{
	_actionProcessName = name;
}

void QActionEx::setActionProcessFile(const QString &file)
{
	_actionProcessFile = file;
}

void QActionEx::setActionProcessArgs(const QStringList &args)
{
	_actionProcessArgs = args;
}

void QActionEx::setStartedSignalMapper(QSignalMapper *signalMapper)
{
	_startedSignalMapper = signalMapper;
}

void QActionEx::setFinishedSignalMapper(QSignalMapper *signalMapper)
{
	_finishedSignalMapper = signalMapper;
}

void QActionEx::setStateChangedSignalMapper(QSignalMapper *signalMapper)
{
	_stateChangedSignalMapper = signalMapper;
}

void QActionEx::setErrorSignalMapper(QSignalMapper *signalMapper)
{
	_errorSignalMapper = signalMapper;
}

void QActionEx::setReadyReadStandardOutputSignalMapper(QSignalMapper *signalMapper)
{
	_readyReadStandardOutputSignalMapper = signalMapper;
}

void QActionEx::setReadyReadStandardErrorSignalMapper(QSignalMapper *signalMapper)
{
	_readyReadStandardErrorSignalMapper = signalMapper;
}


bool QActionEx::startActionProcess(QStringList &args, int waitMsecs)
{
	if(_actionProcess == NULL || !QFile::exists(_actionProcessFile))
	{
		return false;
	}

	if(args.size() == 0)
	{
		args = _actionProcessArgs;
	}

	_actionProcess->start(_actionProcessFile,args);
	_actionProcess->waitForStarted(waitMsecs);

	if(_startedSignalMapper != NULL && _startedSignalMapper->mapping(_actionProcessId) == NULL)
	{
		_startedSignalMapper->setMapping(_actionProcess,_actionProcessId);
		connect(_actionProcess, SIGNAL(started()), _startedSignalMapper, SLOT(map())); 
	}
	if(_finishedSignalMapper != NULL && _finishedSignalMapper->mapping(_actionProcessId) == NULL)
	{
		_finishedSignalMapper->setMapping(_actionProcess,_actionProcessId);
		connect(_actionProcess, SIGNAL(finished( int, QProcess::ExitStatus)), _finishedSignalMapper, SLOT(map())); 
	}
	if(_stateChangedSignalMapper != NULL && _stateChangedSignalMapper->mapping(_actionProcessId) == NULL)
	{
		_stateChangedSignalMapper->setMapping(_actionProcess,_actionProcessId);
		connect(_actionProcess, SIGNAL(stateChanged( QProcess::ProcessState)), _stateChangedSignalMapper, SLOT(map()));
	}
	if(_errorSignalMapper != NULL && _errorSignalMapper->mapping(_actionProcessId) == NULL)
	{
		_errorSignalMapper->setMapping(_actionProcess,_actionProcessId);
		connect(_actionProcess, SIGNAL(error(QProcess::ProcessError)), _errorSignalMapper, SLOT(map()));
	}
	if(_readyReadStandardOutputSignalMapper != NULL && _readyReadStandardOutputSignalMapper->mapping(_actionProcessId) == NULL)
	{
		_readyReadStandardOutputSignalMapper->setMapping(_actionProcess,_actionProcessId);
		connect(_actionProcess, SIGNAL(readyReadStandardOutput()), _readyReadStandardOutputSignalMapper, SLOT(map()));
	}
	if(_readyReadStandardErrorSignalMapper != NULL && _readyReadStandardErrorSignalMapper->mapping(_actionProcessId) == NULL)
	{
		_readyReadStandardErrorSignalMapper->setMapping(_actionProcess,_actionProcessId);
		connect(_actionProcess, SIGNAL(readyReadStandardError()), _readyReadStandardErrorSignalMapper, SLOT(map()));
	}
	
	return true;
}

bool QActionEx::closeActionProcess(int waitMsecs)
{
	if(_actionProcess == NULL)
	{
		return false;
	}

	_actionProcess->close();
	_actionProcess->waitForFinished(waitMsecs);

	if(_startedSignalMapper != NULL)
	{
		disconnect(_actionProcess, SIGNAL(started()), _startedSignalMapper, SLOT(map())); 
	}
	if(_finishedSignalMapper != NULL)
	{
		disconnect(_actionProcess, SIGNAL(finished( int, QProcess::ExitStatus)), _finishedSignalMapper, SLOT(map())); 
	}
	if(_stateChangedSignalMapper != NULL)
	{
		disconnect(_actionProcess, SIGNAL(stateChanged( QProcess::ProcessState)), _stateChangedSignalMapper, SLOT(map()));
	}
	if(_errorSignalMapper != NULL)
	{
		disconnect(_actionProcess, SIGNAL(error(QProcess::ProcessError)), _errorSignalMapper, SLOT(map()));
	}
	if(_readyReadStandardOutputSignalMapper != NULL)
	{
		disconnect(_actionProcess, SIGNAL(readyReadStandardOutput()), _readyReadStandardOutputSignalMapper, SLOT(map()));
	}
	if(_readyReadStandardErrorSignalMapper != NULL)
	{
		disconnect(_actionProcess, SIGNAL(readyReadStandardError()), _readyReadStandardErrorSignalMapper, SLOT(map()));
	}

	return true;
}
