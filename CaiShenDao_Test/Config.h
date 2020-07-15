#pragma once
#include "UpgradeMessage.h"
#include "WinRandom.h"
#include <map>

class CConfig
{
public:
	CConfig(){}
	~CConfig(){}

	static WinRandom Random;
	static int ImageProbability[5][12];

	static string GetCurrentDir() 
	{
		static char szFilePath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
		(strrchr(szFilePath, '\\'))[1] = 0;
		return szFilePath;
	}

	static bool LoadConfig()
	{

		std::string strDir = GetCurrentDir();
		strDir += "config.txt";

		FILE* fpConfig = fopen(strDir.c_str(), "r");
		if (!fpConfig)
		{
			printf("config.txt 打开失败\n");
			return false;
		}

		memset(ImageProbability, 0, sizeof(ImageProbability));

		char fileBuf[1024] = { 0 };
		for (int i = 0; i < 5; ++i)
		{
			fgets(fileBuf, sizeof(fileBuf), fpConfig);
			char* strNums = strtok(fileBuf, "//");
			if (strNums) {
				int nColumn = 1;
				char* pNum = strtok(strNums, ",");
				while (pNum)
				{
					ImageProbability[i][nColumn++] = atoi(pNum);
					pNum = strtok(NULL, ",");
				}
			}

			if (feof(fpConfig)) {
				break;
			}
		}

		fclose(fpConfig);

		string strProbability;
		for (int i = 0; i < 5; ++i)
		{
			char szTmp[128];
			for (int j = 0; j < 12; ++j)
			{
				_itoa(ImageProbability[i][j], szTmp, 10);
				strProbability += szTmp;
				strProbability += j == 11 ? "" : "-";
			}
			strProbability += "\n";
		}

		printf(strProbability.c_str());

		printf("设定概率：\n");
		strProbability = "列\t";
		char szLog[512] = { 0 };
		for (int i = 1; i < ImageType_Max; ++i)
		{
			sprintf_s(szLog, "%s \t", CConfig::GetImageName((EmImageType)i).c_str());
			strProbability += szLog;
		}

		strProbability += "\n";
		for (int column = 0; column < 5; ++column) {
			sprintf_s(szLog, "%d\t", column);
			strProbability += szLog;

			int nTotal = 0;
			for (int i = 0; i < ImageType_Max; ++i)
			{
				nTotal += ImageProbability[column][i];
			}

			for (int i = 1; i < ImageType_Max; ++i)
			{
				sprintf_s(szLog, "%.02f%%%%\t", ImageProbability[column][i] * 1.0 / nTotal * 100.0);
				strProbability += szLog;
			}
			strProbability += "\n";
		}
		printf(strProbability.c_str());
		printf("\n");
		return true;
	};

	static int GetImageMultiple(UINT nKind, UINT nNum)
	{
		// 图案倍率 行是图标，列是连续图标个数倍率
		// 倍率都乘以了100，规避小数度带来的误差
		static int ImageMultiple[][6] = {
			{ 0 },							// 无效图案
			{ 0, 0, 0, 100,	600, 1200 },	// 财神
			{ 0, 0, 0, 60,	400, 1000 },	// 弥勒
			{ 0, 0, 0, 60,	400, 1000 },	// 老者
			{ 0, 0, 0, 60,	200, 800 },		// 金鱼
			{ 0, 0, 0, 20,	120, 400 },		// 福袋
			{ 0, 0, 0, 20,	120, 400 },		// 如意
			{ 0, 0, 0, 20,	80,	 400 },		// 仙桃
			{ 0, 0, 0, 20,	60,  300 },		// 包子
			{ 0, 0, 0, 20,	60,  300 }		// 糖葫芦
		};

		if (nKind >= sizeof(ImageMultiple) / sizeof(ImageMultiple[0]) ||
			nNum >= sizeof(ImageMultiple[0]) / sizeof(float))
		{
			return 0;
		}

		return ImageMultiple[nKind][nNum];
	}

	static EmImageType GetRandomImage(int nColumn)
	{
		int nTotal = 0;
		for (int i = 0; i < ImageType_Max; ++i)
		{
			nTotal += ImageProbability[nColumn][i];
		}

		int nTmpNum = 0;
		int nRandNum = Random.randUnsigned() % nTotal + 1;
		for (EmImageType i = ImageType_CaiShen; i < ImageType_Max; )
		{
			if (nRandNum > nTmpNum && nRandNum <= nTmpNum + ImageProbability[nColumn][i]) {
				return i;
			}

			nTmpNum += ImageProbability[nColumn][i];
			i = (EmImageType)(i + 1);
		}

		return ImageType_Null;
	};

	static std::string GetImageName(EmImageType emImage)
	{
		static std::map<EmImageType, std::string> MapImageName = {
			{ ImageType_CaiShen, "财神" },
			{ ImageType_MiLe, "弥勒" },
			{ ImageType_LaoZhe, "老者" },
			{ ImageType_JinYu, "金鱼" },
			{ ImageType_FuDai, "福袋" },
			{ ImageType_RuYi, "如意" },
			{ ImageType_XianTao, "仙桃" },
			{ ImageType_BaoZi, "包子" },
			{ ImageType_TangHuLu, "糖葫芦" },
			{ ImageType_JingLuo, "金锣" },
			{ ImageType_CaiShenDao, "财神到" }
		};

		std::string strTmp = "NULL";
		auto itorMap = MapImageName.find(emImage);
		if (itorMap != MapImageName.end())
		{
			strTmp = itorMap->second;
		}

		if (strTmp.length() <= 4)
		{
			strTmp += " ";
		}

		return strTmp;
	}
};

