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
	int	m_CartoonTimeElapseArray[300];	//控制卡通旋转的时间间隔
	int	m_nCurCartoonIndex;				//当前应该绘制哪个卡通图案
	Vec2 m_PtStep5;

	Net_LONGLONG m_dwUse;
	Net_LONGLONG m_dwStart;
	int m_nAllStep;
	int soundID;
};