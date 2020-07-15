#include "Config.h"
#include "WinRandom.h"
#include <map>

int CConfig::ImageProbability[5][12] = { 0 };
WinRandom CConfig::Random;

CConfig::CConfig()
{
	LoadConfig();
}

CConfig* CConfig::GetInstance()
{
	if (!SConfig) {
		SConfig = new CConfig;
	}
		
	return SConfig;
}

std::string CConfig::GetCurrentDir()
{
	static char szFilePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
	(strrchr(szFilePath, '\\'))[1] = 0;
	return szFilePath;
}

bool CConfig::LoadConfig()
{
	std::string strDir = GetCurrentDir();
	strDir += "config.txt";

	FILE* fpConfig = fopen(strDir.c_str(), "r");
	if (!fpConfig)
	{
		printf("config.txt ��ʧ��\n");
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

	printf("�趨���ʣ�\n");
	strProbability = "��\t";
	char szLog[512] = { 0 };
	for (int i = 1; i < ImageType_Max; ++i)
	{
		sprintf_s(szLog, "%s \t", GetImageName((EmImageType)i).c_str());
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
}

int CConfig::GetImageMultiple(UINT nKind, UINT nNum)
{
	// ͼ������ ����ͼ�꣬��������ͼ���������
	// ���ʶ�������100�����С���ȴ��������
	static int ImageMultiple[][6] = {
		{ 0 },							// ��Чͼ��
		{ 0, 0, 0, 100,	600, 1200 },	// ����
		{ 0, 0, 0, 60,	400, 1000 },	// ����
		{ 0, 0, 0, 60,	400, 1000 },	// ����
		{ 0, 0, 0, 60,	200, 800 },		// ����
		{ 0, 0, 0, 20,	120, 400 },		// ����
		{ 0, 0, 0, 20,	120, 400 },		// ����
		{ 0, 0, 0, 20,	80,	 400 },		// ����
		{ 0, 0, 0, 20,	60,  300 },		// ����
		{ 0, 0, 0, 20,	60,  300 }		// �Ǻ�«
	};

	if (nKind >= sizeof(ImageMultiple) / sizeof(ImageMultiple[0]) ||
		nNum >= sizeof(ImageMultiple[0]) / sizeof(float))
	{
		return 0;
	}

	return ImageMultiple[nKind][nNum];
}

EmImageType CConfig::GetRandomImage(int nColumn)
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
}

std::string CConfig::GetImageName(EmImageType emImage)
{
	static std::map<EmImageType, std::string> MapImageName = {
		{ ImageType_CaiShen, "����" },
		{ ImageType_MiLe, "����" },
		{ ImageType_LaoZhe, "����" },
		{ ImageType_JinYu, "����" },
		{ ImageType_FuDai, "����" },
		{ ImageType_RuYi, "����" },
		{ ImageType_XianTao, "����" },
		{ ImageType_BaoZi, "����" },
		{ ImageType_TangHuLu, "�Ǻ�«" },
		{ ImageType_JingLuo, "����" },
		{ ImageType_CaiShenDao, "����" }
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

