#pragma once
#include "UpgradeMessage.h"
#include "WinRandom.h"
#include <map>
#include <string>

using namespace std;

#define	UserWinRat				"UserWinRat"		// ����ʤ��
#define	MultipleOnto100			"MultipleOnto100"	// [100+�� ��ת����
#define	Multiple50_100			"Multiple50_100"	// [50, 100)������ת����
#define	Multiple10_50			"Multiple10_50"		// [10, 50)������ת����
#define Multiple3_10			"Multiple3_10"		// [3, 10)������ת����
#define Multiple0_3				"Multiple0_3"		// (0, 3)������ת����
#define	MultipleFreeGame		"MultipleFreeGame"	// �����Ϸ��ת����

class CConfig
{
public:
	static CConfig* GetInstance();

public:
	string GetCurrentDir();
	bool LoadConfig();
	int RandInt(int nMax, int nMin = 0);
	int GetImageMultiple(UINT nKind, UINT nNum);
	EmImageType GetRandomImage(int nColumn);
	std::string GetImageName(EmImageType emImage);
	int GetConfigMultiple(string strKey);

private:
	CConfig() {};
	~CConfig(){}

private:
	WinRandom			m_Random;
	int					m_ImageProbability[5][12];
	map<string, int>	m_mapRat;
};

static CConfig* SConfig = CConfig::GetInstance();
