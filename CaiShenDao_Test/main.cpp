#include <stdio.h>
#include <time.h>
#include <string>
#include "UpgradeMessage.h"
#include "WinRandom.h"
#include "TreeNode.h"
#include "Config.h"
#include "GameLogic.h"

int main()
{
	if (!SConfig->LoadConfig()) {
		return 0;
	}

	// ��log�ļ�
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	int nRandNum = SConfig->RandInt(1000);
	char szFilename[256] = { 0 };
	sprintf(szFilename, "%02d-%02d_%02d-%02d-%02d_%04d.txt",
		tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, nRandNum);

	string strDir = SConfig->GetCurrentDir();
	strDir += szFilename;

	FILE *fp = fopen(strDir.c_str(), "w+");
	if (!fp) {
		printf("%s ��ʧ�� errno=%d\n", strDir.c_str(), errno);
		return 0;
	}

	int nDstNum = 0;;
	printf("��Ӯ�������ע100�ļ��㣬����ѭ��������");
	scanf("%d", &nDstNum);
	if (nDstNum <= 0) {
		return 0;
	}

	CGameLogic* pLogic = new CGameLogic(100, nDstNum);

	std::string strLog;
	char szTmp[512] = { 0 };

	pLogic->RunResult([&](int nCnt, RollResult& result) {
		strLog = "";
		sprintf_s(szTmp, "��%d�� ������ѣ�%d ӮǮ��%I64d\n",
			nCnt + 1, result.iFreeGameCnt, result.iWinMoney);
		strLog += szTmp;

		for (int i = 0; i < LINE_NUM; ++i)
		{
			sprintf_s(szTmp, "%s\t%s\t%s\t%s\t%s\n",
				SConfig->GetImageName(result.ImgType[i][0]).c_str(),
				SConfig->GetImageName(result.ImgType[i][1]).c_str(),
				SConfig->GetImageName(result.ImgType[i][2]).c_str(),
				SConfig->GetImageName(result.ImgType[i][3]).c_str(),
				SConfig->GetImageName(result.ImgType[i][4]).c_str());
			strLog += szTmp;
		}

		strLog += "\n";
		fwrite(strLog.c_str(), strLog.length(), 1, fp);

		if (nCnt % 1000 == 0)
		{
			printf("\rд��%d", nCnt);
		}
	});

	printf("\rд����� %d ��\n", nDstNum);
	fclose(fp);

	// ����ͳ��
	pLogic->GetResultProbability();

	delete pLogic;
	system("pause");

	return 0;
}