#pragma once
#include "UpgradeMessage.h"
#include <map>
#include <vector>
#include <list>
using namespace std;

struct TreeNode
{
	int nChildCnt = 0;
	int nLine = 0;
	int nColumn = 0;
	EmImageType emType = ImageType_Null;
	TreeNode* pParent = NULL;
	TreeNode* pChild[3] = { NULL };
};

class CTreeNode
{
public:
	static list<TreeNode*>		NodePool;						// TreeNode ÄÚ´æ³Ø

	static TreeNode* CreateNode() 
	{
		TreeNode* pTmpNode = NULL;
		if (NodePool.size() > 0)
		{
			pTmpNode = NodePool.back();
			NodePool.pop_back();
		}
		else {
			pTmpNode = new TreeNode;
		}

		memset(pTmpNode, 0, sizeof(*pTmpNode));
		return pTmpNode;
	}

	static void FreeAllNode(TreeNode* pNode)
	{
		if (!pNode) {
			return;
		}

		for (int i = 0; i < 3; ++i)
		{
			if (pNode->pChild[i]) {
				FreeAllNode(pNode->pChild[i]);
			}
		}
		NodePool.push_back(pNode);
	}

	static void TravelAllNode(TreeNode* pNode, RollResult& result, map<TreeNode*, vector<int>>& mapReward)
	{
		if (!pNode) {
			return;
		}

		if (pNode->nColumn == 0 && pNode->pParent) {
			mapReward.insert(make_pair(pNode, vector<int>()));

		}

		for (int i = 0; i < 3; ++i)
		{
			if (pNode->pChild[i]) {
				TravelAllNode(pNode->pChild[i], result, mapReward);
			}
		}

		if (pNode->pChild[0] == NULL &&
			pNode->pChild[1] == NULL &&
			pNode->pChild[2] == NULL &&
			pNode->nColumn >= 2)
		{
			bool bFindDstImage = false;
			TreeNode* pTmpNode = pNode;
			while (pTmpNode->pParent)
			{
				if (!pTmpNode->pParent->pParent)
				{
					auto itorMap = mapReward.find(pTmpNode);
					if (itorMap != mapReward.end()) {
						itorMap->second.push_back(pNode->nColumn + 1);
					}
					break;
				}
				pTmpNode = pTmpNode->pParent;
			}
		}
	}
};
