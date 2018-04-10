#pragma once

#include <vector>
#include "TimeWheelBucket.h"

using namespace std;

/*
	ʱ����
*/
typedef vector<TimeWheelBucket*>  TimeWheelBucketVec;
class TimeWheel
{
public:
	TimeWheel(int index);
	~TimeWheel();

	int GetCursor();

	// ����false��ʾʱ��������һ�֣����������¿�ʼ
	bool SetCursor(int cursor);

	// ��ʼ������
	bool InitTimeWheel(int bucketNum);

	// ��������
	bool Run();

	// ���붨ʱ��
	void InsertTimer(Timer& timer);

	// ����������ȡʱ����Ͱ
	TimeWheelBucket* GetTimeWheelBucketByIndex(int index);

	// �Ƴ���ʱ������
	bool RemoveTimerTask(Timer & timer);

private:
	int					m_index;				// ʱ���ֵ������ţ���0��ʼ��������������
	int					m_cursor;				// �����α꣬�൱��ʱ�ӵ�ָ����
	TimeWheelBucketVec	m_timeWheelBucketVec;	// ʱ����Ͱvec
};
