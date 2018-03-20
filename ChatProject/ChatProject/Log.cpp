#include "stdafx.h"
#include "Log.h"
#include <fstream>

const string LogErr::m_savePath = "../Log/LogErr.txt";
const string LogRun::m_savePath = "../Log/LogRun.txt";
LogErr::LogErr()
{
}

LogErr::~LogErr()
{
}

bool LogErr::WriteLogToFile(char * logData, int logDataLen)
{
	ofstream file(m_savePath, ios::app | ios::out);

	file.write(logData, logDataLen);

	file.close();
	return true;
}



LogRun::LogRun()
{
}

LogRun::~LogRun()
{
}

bool LogRun::WriteLogToFile(char * logData, int logDataLen)
{
	ofstream file(m_savePath, ios::app | ios::out);

	file.write(logData, logDataLen);

	file.close();
	return true;
}
