#pragma once

#include <string>

using namespace std;
/*
	错误日志
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
运行日志
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


