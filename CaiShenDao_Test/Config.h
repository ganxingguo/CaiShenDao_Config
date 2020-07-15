#pragma once
#include "UpgradeMessage.h"
#include "WinRandom.h"

class CConfig
{
public:
	static WinRandom Random;
	static int ImageProbability[5][12];
	static CConfig* GetInstance();

public:
	string GetCurrentDir();
	bool LoadConfig();
	int GetImageMultiple(UINT nKind, UINT nNum);
	EmImageType GetRandomImage(int nColumn);
	std::string GetImageName(EmImageType emImage);

private:
	CConfig();
	~CConfig(){}
};

static CConfig* SConfig = CConfig::GetInstance();
