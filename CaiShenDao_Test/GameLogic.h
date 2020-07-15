#pragma once
#include <functional>
#include "UpgradeMessage.h"
#include "TreeNode.h"
#include "Config.h"

class CGameLogic
{
public:
	CGameLogic(int nBetMoney, int nCnt);
	~CGameLogic() {}

public:
	void RunResult(std::function<void(int, RollResult&)> funRb);

public:
	void CalGameResult(RollResult& result, char cbUserWin);
	void GetResultSubFun(RollResult& result, TreeNode* pParentNode, int nColumn);
	void GetResult(RollResult& result);
	void GetResultProbability();

protected:
	int					m_iLeftFreeGamesCnt;			//���ʣ�������Ϸ����
	__int64				m_iFreeGameWinMoney;			//�����Ϸ�ۼ�Ӯȡ

	bool				m_bFreeGame;					//��ǰ���Ƿ����
	RollResult			m_CurrentResult;				//���ֽ��
	char				m_cbUserCtrl;					//��Ӯ����

	int					m_iBetMoney;
	int					m_RunCnt;
	int					m_ImageProbability[COLUMN_NUM][ImageType_Max];
	int					m_TotalWin;
	int					m_nReadRunCnt;
};



