#pragma once

#include <string>

using namespace std;
/*
	������־
*/
class LogErr
{
public:
	LogErr();
	~LogErr();
	
	static bool WriteLogToFile(char* logData, int logDataLen);
private:
	static const string m_savePath;
};

/*
������־
*/
class LogRun
{
public:
	LogRun();
	~LogRun();

	static bool WriteLogToFile(char* logData, int logDataLen);
private:
	static const string m_savePath;
};


