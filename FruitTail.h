#pragma once

class FruitTail
{
public:
	FruitTail();
	void Reset(int nSecondPass, int nEndAnimal);
	void Update();
	bool GetCurAnimal(int& nCurCartoonIndex);

private:
	void Update(bool bPlaySound);

private:
	int m_nLastAnimal;
	int m_CartoonElapsIndex;
	int	m_CartoonTimeElapseArray[300];	//���ƿ�ͨ��ת��ʱ����
	int	m_nCurCartoonIndex;				//��ǰӦ�û����ĸ���ͨͼ��
	Vec2 m_PtStep5;

	Net_LONGLONG m_dwUse;
	Net_LONGLONG m_dwStart;
	int m_nAllStep;
	int soundID;
};