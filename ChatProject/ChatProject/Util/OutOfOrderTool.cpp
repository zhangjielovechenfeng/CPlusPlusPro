#include "OutOfOrderTool.h"
#include <stddef.h>
#include <string.h>

int OutOfOrderTool::m_key[] = { 0xae, 0xbf, 0x56, 0x78, 0xab, 0xcd, 0xef, 0xf1 };

void OutOfOrderTool::NegativeOrder(char * data, int len)
{
	int convertData[len];
	memset(convertData, 0, len * sizeof(int));
	for (int i = 0; i < len; ++i)
	{
		convertData[i] = data[i];
	}
	for (int i = 0; i < len; ++i)
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
	for (int i = 0; i < len; ++i)
	{
		data[i] = convertData[i];
	}
}

void OutOfOrderTool::PositiveOrder(char * data, int len)
{
	int convertData[len];
	memset(convertData, 0, len * sizeof(int));

	for (int i = 0; i < len; ++i)
	{
		convertData[i] = data[i];
	}
	for (int i = 0; i < len; ++i)
	{
		if (i > 0)
		{
			int keyLen = sizeof(m_key) / sizeof(int);
			m_key[i % keyLen] = (m_key[i % keyLen] + data[i - 1]) ^ i;
			convertData[i] = (convertData[i] ^ m_key[i % keyLen]) + data[i - 1];
		}
		else
		{
			convertData[0] = convertData[0] ^ m_key[0];
		}
	}
	for (int i = 0; i < len; ++i)
	{
		data[i] = convertData[i];
	}
}