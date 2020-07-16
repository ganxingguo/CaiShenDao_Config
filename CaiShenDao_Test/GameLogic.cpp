#include "GameLogic.h"

CGameLogic::CGameLogic(int nBetMoney, int nRunNum)
	: m_iBetMoney(nBetMoney)
	, m_RunCnt(nRunNum)
{
	m_TotalWin = 0;
	m_iLeftFreeGamesCnt = 0;
	m_iFreeGameWinMoney = 0;
	m_bFreeGame = false;
	m_cbUserCtrl = 0;
	m_nReadRunCnt = 0;
	memset(m_ImageProbability, 0, sizeof(m_ImageProbability));
	memset(&m_CurrentResult, 0, sizeof(m_CurrentResult));
}

void CGameLogic::RunResult(std::function<void(int, RollResult&)> funRb)
{
	for (int i = 0; i < m_RunCnt; )
	{
		int nTmp = SConfig->RandInt(100);
		if (nTmp < SConfig->GetConfigMultiple(UserWinRat))
		{
			m_cbUserCtrl = 1;
		}
		else {
			m_cbUserCtrl = -1;
		}

		if (m_iLeftFreeGamesCnt <= 0) {
			m_bFreeGame = false;
			m_iFreeGameWinMoney = 0;

			CalGameResult(m_CurrentResult, m_cbUserCtrl);
			m_iLeftFreeGamesCnt = m_CurrentResult.iFreeGameCnt;
			if (m_iLeftFreeGamesCnt <= 0)
			{
				m_TotalWin += m_CurrentResult.iWinMoney;
				funRb(i++, m_CurrentResult);
			}
		}
		else {
			m_bFreeGame = true;
			m_iLeftFreeGamesCnt--;

			RollResult tmpResult;
			CalGameResult(tmpResult, m_cbUserCtrl);

			m_iFreeGameWinMoney += tmpResult.iWinMoney;
			m_iLeftFreeGamesCnt += tmpResult.iFreeGameCnt;

			if (m_iLeftFreeGamesCnt <= 0)
			{
				m_CurrentResult.iWinMoney += m_iFreeGameWinMoney;
				m_TotalWin += m_CurrentResult.iWinMoney;

				funRb(i++, m_CurrentResult);
				m_iFreeGameWinMoney = 0;
			}
		}

	}
}

void CGameLogic::GetResultProbability()
{
	char szLog[128] = { 0 };
	string strProbability;
	printf("\nͳ�Ƹ���\n");


	strProbability = "��\t";
	for (int i = 1; i < ImageType_Max; ++i)
	{
		sprintf_s(szLog, "%s\t", SConfig->GetImageName((EmImageType)i).c_str());
		strProbability += szLog;
	}

	int nDen = m_nReadRunCnt * 3;
	strProbability += "\n";
	for (int column = 0; column < 5; ++column) {
		sprintf_s(szLog, "%d\t", column);
		strProbability += szLog;
		for (int i = 1; i < ImageType_Max; ++i)
		{
			sprintf_s(szLog, "%.2f%%%%\t", (m_ImageProbability[column][i] * 1.0 / nDen) * 100.0);
			strProbability += szLog;
		}
		strProbability += "\n";
	}

	printf(strProbability.c_str());
	printf("\n");

	printf("�ܼ���Ӯ NEW4.0��%d\n\n", m_TotalWin);
}

void CGameLogic::GetResultSubFun(RollResult& result, TreeNode* pParentNode, int nColumn)
{
	if (nColumn >= COLUMN_NUM) {
		return;
	}

	if (nColumn == 0) {
		for (int nLine = 0; nLine < LINE_NUM; ++nLine)
		{
			if (result.ImgType[nLine][nColumn] != ImageType_Null)
			{
				TreeNode* pTmpNode = CTreeNode::CreateNode();
				pTmpNode->emType = result.ImgType[nLine][nColumn];
				pTmpNode->nLine = nLine;
				pTmpNode->nColumn = nColumn;
				pTmpNode->pParent = pParentNode;
				pParentNode->pChild[pParentNode->nChildCnt++] = pTmpNode;
				GetResultSubFun(result, pTmpNode, nColumn + 1);
			}
		}
	}
	else {
		for (int nLine = 0; nLine < LINE_NUM; ++nLine)
		{
			if (result.ImgType[nLine][nColumn] != ImageType_Null &&
				(pParentNode->emType == ImageType_CaiShenDao ||
					result.ImgType[nLine][nColumn] == ImageType_CaiShenDao ||
					result.ImgType[nLine][nColumn] == pParentNode->emType))
			{
				TreeNode* pTmpNode = CTreeNode::CreateNode();
				if (result.ImgType[nLine][nColumn] == ImageType_CaiShenDao) {
					pTmpNode->emType = pParentNode->emType;
				}
				else {
					pTmpNode->emType = result.ImgType[nLine][nColumn];
				}
				pTmpNode->nLine = nLine;
				pTmpNode->nColumn = nColumn;
				pTmpNode->pParent = pParentNode;
				pParentNode->pChild[pParentNode->nChildCnt++] = pTmpNode;
				GetResultSubFun(result, pTmpNode, nColumn + 1);
			}
		}
	}
}

void CGameLogic::GetResult(RollResult& result)
{
	// �ݹ� ��ȡ���
	TreeNode* pHead = CTreeNode::CreateNode();
	GetResultSubFun(result, pHead, 0);

	// �������з�֧���ҵ����� >= 3��ͼ�� 
	map<TreeNode*, vector<int>> mapReward;
	CTreeNode::TravelAllNode(pHead, result, mapReward);
	CTreeNode::FreeAllNode(pHead);

	int nCnt = 0;
	auto itorMap = mapReward.begin();
	for (; itorMap != mapReward.end(); ++itorMap)
	{
		if (itorMap->second.size() > 0)
		{
			bool bSameImage = false;
			for (int i = 0; i < nCnt; ++i)
			{
				if (result.rewardImage[i] == itorMap->first->emType)
				{
					result.lineCnt[i][1] += itorMap->second.size();
					bSameImage = true;
					break;
				}
			}

			if (!bSameImage)
			{
				result.rewardImage[nCnt] = itorMap->first->emType;
				result.lineCnt[nCnt][0] = itorMap->second[0];
				result.lineCnt[nCnt][1] += itorMap->second.size();
				nCnt++;
			}

			if (itorMap->first->emType == ImageType_JingLuo) {
				result.iFreeGameCnt = 10;
			}
		}

		if (nCnt >= LINE_NUM) {
			break;
		}
	}
}

void CGameLogic::CalGameResult(RollResult& result, char cbUserWin)
{
	int nCount = 0;
	int nRand = 0;
	do
	{
		//��ȡ��Ϸ����ṹ��
		ZeroMemory(&result, sizeof(result));
		for (int i = 0; i < LINE_MAX; ++i)
		{
			memset(&result.allLines[i], 255, sizeof(LineInfo));
			result.allLines[i].iWinMoney = -1;
		}

		// ���ͼ��
		nCount++;
		for (int i = 0; i < LINE_NUM; ++i)
		{
			for (int j = 0; j < COLUMN_NUM; ++j)
			{
				result.ImgType[i][j] = SConfig->GetRandomImage(j);
				m_ImageProbability[j][result.ImgType[i][j]]++;
			}
		}
		m_nReadRunCnt++;

		GetResult(result);

		// �����ܱ���
		for (int i = 0; i < LINE_NUM; ++i)
		{
			if (result.rewardImage[i] > ImageType_Null &&
				result.rewardImage[i] < ImageType_JingLuo)
			{
				int nDstMultiple = SConfig->GetImageMultiple(result.rewardImage[i], result.lineCnt[i][0]);
				nDstMultiple *= result.lineCnt[i][1];
				result.multipleAndMoney[i][0] = nDstMultiple;

				// �����ǳ���100���ģ���������ط���Ҫ��100
				result.multipleAndMoney[i][1] = int(nDstMultiple * m_iBetMoney / 100);
			}
			result.iTotalMultiple += result.multipleAndMoney[i][0];
			result.iWinMoney += result.multipleAndMoney[i][1];
		}

		if (!m_bFreeGame) {
			result.iWinMoney -= m_iBetMoney;
		}

		//������ת��100~2916����50~99.9��10~49.9�������Ϸ(25%)
		if (result.iTotalMultiple >= 100 * 100)
		{
			nRand = SConfig->RandInt(100);
			if (nRand < SConfig->GetConfigMultiple(MultipleOnto100)) {
				continue;
			}
		}

		if (result.iTotalMultiple >= 50 * 100 && result.iTotalMultiple < 100 * 100)
		{
			nRand = SConfig->RandInt(100);
			if (nRand <  SConfig->GetConfigMultiple(Multiple50_100)) {
				continue;
			}
		}

		if (result.iTotalMultiple >= 10 * 100 && result.iTotalMultiple < 50 * 100)
		{
			nRand = SConfig->RandInt(100);
			if (nRand <  SConfig->GetConfigMultiple(Multiple10_50)) {
				continue;
			}
		}

		if (result.iTotalMultiple >= 3 * 100 && result.iTotalMultiple < 10 * 100)
		{
			nRand = SConfig->RandInt(100);
			if (nRand <  SConfig->GetConfigMultiple(Multiple3_10)) {
				continue;
			}
		}

		if (result.iTotalMultiple > 0 && result.iTotalMultiple < 3 * 100)
		{
			nRand = SConfig->RandInt(100);
			if (nRand <  SConfig->GetConfigMultiple(Multiple0_3)) {
				continue;
			}
		}
		
		if (result.iFreeGameCnt > 0)
		{
			nRand = SConfig->RandInt(100);
			if (nRand < SConfig->GetConfigMultiple(MultipleFreeGame)) {
				continue;
			}
		}

		//if (cbUserWin > 0 && result.iWinMoney <= 0 && result.iFreeGameCnt <= 0 ||
		//	cbUserWin < 0 && (result.iWinMoney > 0 || result.iFreeGameCnt > 0)) {
		//	continue;
		//}

		if (cbUserWin > 0 && result.iTotalMultiple <= 0 && result.iFreeGameCnt <= 0 ||
			cbUserWin < 0 && (result.iTotalMultiple > 0 || result.iFreeGameCnt) > 0)
		{
			continue;
		}

		break;
	} while (nCount < 5000);
}

