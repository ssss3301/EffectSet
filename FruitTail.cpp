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
	//������ת����, ����Ϊ9����ת
	int nStopStep = nEndAnimal;				//������ת����ͣ����λ��

	memset(m_CartoonTimeElapseArray, 0, sizeof(m_CartoonTimeElapseArray));
	m_CartoonElapsIndex = 0;					//���³�ʼ����ͨ��ͼ��ʱ�������±�

	m_nCurCartoonIndex = m_nLastAnimal;			//���³�ʼ����ͨ��ͼλ�������±�
	int nTimeElaps1 = 600 - 80;						//��һ�͵ھŽ׶�
	int nTimeElaps2 = 300 - 60;						//�ڶ��͵ڰ˽׶�
	int nTimeElaps3 = 200 - 40;						//�����͵��߽׶�
	int nTimeElaps4 = 100 - 20;						//���ĺ͵����׶�
	int nTimeElaps5 = 30;							//����׶Σ�ÿ����ʱ������Ϊ500����

	int nAllStep = 96 + nStopStep - m_nLastAnimal;	//�ܹ���ת��ô���ʱ������
	m_nAllStep = nAllStep;
	int n1 = 2 * 2;						//��һ�׶�5��ʱ������
	int n2 = 3 * 2;						//�ڶ��׶�4��ʱ������
	int n3 = 4 * 2;
	int n4 = 5 * 2;


	int n6 = 5 * 2;
	int n7 = 4 * 2;
	int n8 = 3 * 2;
	int n9 = 2 * 2;
	int n5 = nAllStep - n1 - n2 - n3 - n4 - n6 - n7 - n8 - n9;		//����׶���ת5��ʱ������

	//��һ�׶�ʱ��ֵ(0-5)
	for (int i = 0; i < n1; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps1;

	//�ڶ��׶�ʱ��ֵ
	for (int i = n1; i < n1 + n2; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps2;

	//�����׶�ʱ��ֵ
	for (int i = n1 + n2; i < n1 + n2 + n3; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps3;

	//���Ľ׶�ʱ��ֵ
	for (int i = n1 + n2 + n3; i < n1 + n2 + n3 + n4; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps4;

	//����׶�ʱ��ֵ
	for (int i = n1 + n2 + n3 + n4; i < n1 + n2 + n3 + n4 + n5; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps5;

	m_PtStep5 = Vec2(n1 + n2 + n3 + n4, n1 + n2 + n3 + n4 + n5);

	//�����׶�
	for (int i = n1 + n2 + n3 + n4 + n5; i < n1 + n2 + n3 + n4 + n5 + n6; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps4;

	//���߽׶�
	for (int i = n1 + n2 + n3 + n4 + n5 + n6; i < n1 + n2 + n3 + n4 + n5 + n6 + n7; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps3;

	//�ڰ˽׶�
	for (int i = n1 + n2 + n3 + n4 + n5 + n6 + n7; i < n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps2;

	//�ھŽ׶�
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
