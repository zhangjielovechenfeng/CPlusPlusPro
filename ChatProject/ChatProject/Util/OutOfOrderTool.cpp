#include "OutOfOrderTool.h"
#include <stddef.h>
#include <string.h>

int OutOfOrderTool::m_key[] = { 0xae, 0xbf, 0x56, 0x78, 0xab, 0xcd, 0xef, 0xf1 };

char * OutOfOrderTool::PositiveOrder(char * data)
{
	int convertData[strlen(data)];
	for (int i = 0; i < strlen(data); ++i)
	{
		convertData[i] = data[i];
	}
	for (int i = 0; i < strlen(data); ++i)
	{
		if (i > 0)
		{
			int keyLen = sizeof(m_key) / sizeof(int);
			m_key[i % keyLen] = (m_key[i % keyLen] + convertData[i - 1]) ^ i;
			convertData[i] = (convertData[i] - convertData[i - 1]) ^ m_key[i % keyLen];
		}
		else
		{
			convertData[0] = convertData[0] ^ m_key[0];
		}
	}
	for (int i = 0; i < strlen(data); ++i)
	{
		data[i] = convertData[i];
	}
	return data;
}

char * OutOfOrderTool::NegativeOrder(char * data)
{
	int convertData[strlen(data)];
	int tmpData[strlen(data)];

	for (int i = 0; i < strlen(data); ++i)
	{
		convertData[i] = data[i];
		tmpData[i] = data[i];
	}
	for (int i = 0; i < strlen(data); ++i)
	{
		if (i > 0)
		{
			int keyLen = sizeof(m_key) / sizeof(int);
			m_key[i % keyLen] = (m_key[i % keyLen] + tmpData[i - 1]) ^ i;
			convertData[i] = (convertData[i] ^ m_key[i % keyLen]) + tmpData[i - 1];
		}
		else
		{
			convertData[0] = convertData[0] ^ m_key[0];
		}
	}
	for (int i = 0; i < strlen(data); ++i)
	{
		data[i] = convertData[i];
	}

	return data;
}