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
	int					m_iLeftFreeGamesCnt;			//玩家剩余免费游戏局数
	__int64				m_iFreeGameWinMoney;			//免费游戏累计赢取

	bool				m_bFreeGame;					//当前局是否免费
	RollResult			m_CurrentResult;				//当局结果
	char				m_cbUserCtrl;					//输赢控制

	int					m_iBetMoney;
	int					m_RunCnt;
	int					m_ImageProbability[COLUMN_NUM][ImageType_Max];
	int					m_TotalWin;
	int					m_nReadRunCnt;
};


