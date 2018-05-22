#include "stdafx.h"
#include "FruitTail.h"

FruitTail::FruitTail()
:m_nLastAnimal(0)
, m_CartoonElapsIndex(0)
, m_nCurCartoonIndex(0)
, m_dwUse(0)
, m_dwStart(0)
, m_nAllStep(0)
{
}


void FruitTail::Reset(int nSecondPass, int nEndAnimal)
{
	//////////////////////////////////////////////////////////////////////////
	//计算旋转步骤, 共分为9次旋转
	int nStopStep = nEndAnimal;				//本次旋转即将停靠的位置

	memset(m_CartoonTimeElapseArray, 0, sizeof(m_CartoonTimeElapseArray));
	m_CartoonElapsIndex = 0;					//重新初始化卡通绘图计时器数组下标

	m_nCurCartoonIndex = m_nLastAnimal;			//重新初始化卡通绘图位置数组下标
	int nTimeElaps1 = 600 - 80;						//第一和第九阶段
	int nTimeElaps2 = 300 - 60;						//第二和第八阶段
	int nTimeElaps3 = 200 - 40;						//第三和第七阶段
	int nTimeElaps4 = 100 - 20;						//第四和第六阶段
	int nTimeElaps5 = 30;							//第五阶段，每个定时器周期为500毫秒

	int nAllStep = 96 + nStopStep - m_nLastAnimal;	//总共旋转这么多个时钟周期
	m_nAllStep = nAllStep;
	int n1 = 2 * 2;						//第一阶段5个时钟周期
	int n2 = 3 * 2;						//第二阶段4个时钟周期
	int n3 = 4 * 2;
	int n4 = 5 * 2;


	int n6 = 5 * 2;
	int n7 = 4 * 2;
	int n8 = 3 * 2;
	int n9 = 2 * 2;
	int n5 = nAllStep - n1 - n2 - n3 - n4 - n6 - n7 - n8 - n9;		//第五阶段旋转5个时钟周期

	//第一阶段时钟值(0-5)
	for (int i = 0; i < n1; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps1;

	//第二阶段时钟值
	for (int i = n1; i < n1 + n2; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps2;

	//第三阶段时钟值
	for (int i = n1 + n2; i < n1 + n2 + n3; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps3;

	//第四阶段时钟值
	for (int i = n1 + n2 + n3; i < n1 + n2 + n3 + n4; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps4;

	//第五阶段时钟值
	for (int i = n1 + n2 + n3 + n4; i < n1 + n2 + n3 + n4 + n5; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps5;

	m_PtStep5 = Vec2(n1 + n2 + n3 + n4, n1 + n2 + n3 + n4 + n5);

	//第六阶段
	for (int i = n1 + n2 + n3 + n4 + n5; i < n1 + n2 + n3 + n4 + n5 + n6; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps4;

	//第七阶段
	for (int i = n1 + n2 + n3 + n4 + n5 + n6; i < n1 + n2 + n3 + n4 + n5 + n6 + n7; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps3;

	//第八阶段
	for (int i = n1 + n2 + n3 + n4 + n5 + n6 + n7; i < n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps2;

	//第九阶段
	for (int i = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8; i <= n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps1;

	m_nLastAnimal = nStopStep;

	struct timeval tv;
	gettimeofday(&tv, nullptr);

	m_dwStart = (Net_LONGLONG)tv.tv_sec *  1000 + tv.tv_usec / 1000.0;
	m_dwUse = m_CartoonTimeElapseArray[m_CartoonElapsIndex];

	soundID = -1;

	if (nSecondPass > 0)
	{
		m_dwStart -= nSecondPass * 1000;
		Update(false);
	}

}

void FruitTail::Update()
{
	Update(true);
}

void FruitTail::Update(bool bPlaySound)
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	Net_LONGLONG dwCur = (Net_LONGLONG)tv.tv_sec * 1000 + tv.tv_usec / 1000.0;
	Net_LONGLONG dwPass = dwCur - m_dwStart;

	while (m_CartoonElapsIndex < _countof(m_CartoonTimeElapseArray) && m_CartoonTimeElapseArray[m_CartoonElapsIndex] > 0)
	{
		if (m_dwUse >= dwPass)
		{
			break;
		}
		else
		{
			++m_CartoonElapsIndex;
			if (m_CartoonElapsIndex < _countof(m_CartoonTimeElapseArray) && m_CartoonTimeElapseArray[m_CartoonElapsIndex] > 0)
			{
				m_dwUse += m_CartoonTimeElapseArray[m_CartoonElapsIndex];
				m_nCurCartoonIndex = (m_nCurCartoonIndex + 1) % 24;

				if (bPlaySound)
				{
					
				}
			}
			else
			{
				break;
			}

		}
	}
}

bool FruitTail::GetCurAnimal(int& nCurCartoonIndex)
{
	nCurCartoonIndex = -1;

	if (m_CartoonElapsIndex < _countof(m_CartoonTimeElapseArray) && m_CartoonTimeElapseArray[m_CartoonElapsIndex] > 0)
	{
		nCurCartoonIndex = m_nCurCartoonIndex;
		return true;
	}

	return false;
}
