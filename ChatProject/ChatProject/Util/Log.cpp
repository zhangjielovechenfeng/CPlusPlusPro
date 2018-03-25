#include "Log.h"
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

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
	DIR* dir = opendir("../Log");
	if (NULL == dir)
	{
		if (0 != mkdir("../Log", S_IWRITE | S_IREAD))
		{
			return false;
		}
	}

	ofstream file(m_savePath.c_str(), ios::app | ios::out);

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
	DIR* dir = opendir("../Log");
	if (NULL == dir)
	{
		if (0 != mkdir("../Log", S_IWRITE | S_IREAD))
		{
			return false;
		}
	}
	ofstream file(m_savePath.c_str(), ios::app | ios::out);

	file.write(logData, logDataLen);

	file.close();
	return true;
}
