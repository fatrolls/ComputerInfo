/***************************************************************************************************/
/*                       clock ��ȡ��������ʱ��                                                         */
/*                     1��clock   �ó������������������ռ��CPU��ʱ�䡣����������شӡ��������������̡���������    */
/* 									�е���clock()������ʱ֮���CPUʱ�Ӽ�ʱ��Ԫ��clock tick��������MSDN�г�֮Ϊ */
/* 									����ʱ�䣨wal-clock����������ʱ�䲻��ȡ���򷵻�-1������clock_t������      */
/* 									����ʱ����������͡�                                                 */
/*                     2���Զ��徫ȷʱ�䡣boost��                                                        */
/*                     3���Զ��� CMicroSecondClockTikt ΢��                                            */
/*                                                                                                   */
/*                                                                                                   */
/*                                                                                                   */
/***************************************************************************************************/

#pragma  once
#include <time.h>
#include <stdio.h>
#include "..\..\Time\MicrosecondTime.h"

#ifndef CLOCKTIKT_H
#define CLOCKTIKT_H

// ϵͳ�Դ���ȷʱ�䣺�뼶��
class CClockTikt{
public:

	CClockTikt(){};
	~CClockTikt(){};

public:

	virtual clock_t GetStartClock(){ return clock(); }
	virtual	clock_t GetEndClock(){ return clock(); }

	//����CLOCKS_PER_SEC����������ʾһ���ӻ��ж��ٸ�ʱ�Ӽ�ʱ��Ԫ���䶨�����£�#define CLOCKS_PER_SEC ((clock_t)1000)
	double GetTakenTime(clock_t m_start, clock_t m_end){ return (double)((m_end - m_start) / CLOCKS_PER_SEC); }

	// ʱ���ӡ
	void PrintTakenClock(clock_t m_start, clock_t m_end){
		cout << "Spend time: " << GetTakenTime(m_start, m_end) << "  seconds" << endl;
	}
};

class CMicroSecondClockTikt :public TimeConversion
{
public:

	CMicroSecondClockTikt(){};
	~CMicroSecondClockTikt(){};

public:

	virtual time_t GetStartClock(){ return GetUtcCaressing(); }
	virtual	time_t GetEndClock(){ return GetUtcCaressing(); }

	//����CLOCKS_PER_SEC����������ʾһ���ӻ��ж��ٸ�ʱ�Ӽ�ʱ��Ԫ���䶨�����£�#define CLOCKS_PER_SEC ((clock_t)1000)
	double GetTakenTime(time_t m_start, time_t m_end){ return (double)((m_end - m_start) / CLOCKS_PER_SEC); }

	// ʱ���ӡ
	void PrintTakenClock(time_t m_start, time_t m_end){
		cout << "Spend time: " << GetTakenTime(m_start, m_end) << "  Microseconds" << endl;
	}
};

#endif

// ���Դ���
//CClockTikt * m_clocktikt = new CClockTikt();
//��ʼʱ��
//m_clocktikt->GetStartClock();
//����ʱ��
//m_clocktikt->GetEndClock();
//m_clocktikt->PrintTakenClock(m_clocktikt->GetTakenTime());
//delete m_clocktikt;