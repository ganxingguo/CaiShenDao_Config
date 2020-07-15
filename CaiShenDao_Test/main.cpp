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

	// 打开log文件
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	int nRandNum = SConfig->Random.randUnsigned() % 1000;
	char szFilename[256] = { 0 };
	sprintf(szFilename, "%02d-%02d_%02d-%02d-%02d_%04d.txt",
		tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, nRandNum);

	string strDir = SConfig->GetCurrentDir();
	strDir += szFilename;

	FILE *fp = fopen(strDir.c_str(), "w+");
	if (!fp) {
		printf("%s 打开失败 errno=%d\n", strDir.c_str(), errno);
		return 0;
	}

	int nDstNum = 0;;
	printf("输赢结果按下注100的计算，输入循环次数：");
	scanf("%d", &nDstNum);
	if (nDstNum <= 0) {
		return 0;
	}

	CGameLogic* pLogic = new CGameLogic(100, nDstNum);

	std::string strLog;
	char szTmp[512] = { 0 };

	pLogic->RunResult([&](int nCnt, RollResult& result) {
		strLog = "";
		sprintf_s(szTmp, "第%d次 触发免费：%d 赢钱：%I64d\n",
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
			printf("\r写入%d", nCnt);
		}
	});

	printf("\r写入完成 %d 条\n", nDstNum);
	fclose(fp);

	// 概率统计
	pLogic->GetResultProbability();

	delete pLogic;
	system("pause");

	return 0;
}