#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//文件名字定义
#define GAMENAME						TEXT("Slotomania")
#define GAME_DLL_AUTHOR                 TEXT("杭州")    
#define GAME_DLL_NOTE                   TEXT("牌类 -- 游戏组件")

//版本定义
#define GAME_MAX_VER					1								//现在最高版本
#define GAME_LESS_VER					1								//现在最低版本
#define GAME_CHANGE_VER					0								//修改版本
#define SUPPER_TYPE						1

//游戏信息
#define NAME_ID							40066000						//名字 ID
#define PLAY_COUNT						1								//游戏人数

//游戏状态定义
#define GS_WAIT_SETGAME					0				//等待东家设置状态
#define GS_WAIT_ARGEE					1				//等待同意设置
#define GS_PLAY_GAME					20				//发牌状态
#define GS_WAIT_BACK					21				//等待扣压底牌
#define GS_WAIT_NEXT					23				//等待下一盘开始 

#define GAME_ID_SIZE					20				//牌局ID长度
#define LINE_NUM						3				//行数
#define COLUMN_NUM						5				//列数
#define LINE_MAX						243				//行数最大值

#pragma pack(1)

using BYTE = unsigned char;

enum EmImageType : BYTE
{
	ImageType_Null = 0,				// 无效图案
	ImageType_CaiShen = 1,			// 财神
	ImageType_MiLe = 2,				// 弥勒
	ImageType_LaoZhe = 3,			// 老者
	ImageType_JinYu = 4,			// 金鱼
	ImageType_FuDai = 5,			// 福袋
	ImageType_RuYi = 6,				// 如意
	ImageType_XianTao = 7,			// 仙桃
	ImageType_BaoZi = 8,			// 包子
	ImageType_TangHuLu = 9,			// 糖葫芦
	ImageType_JingLuo = 10,			// 金锣
	ImageType_CaiShenDao = 11,		// 财神到
	ImageType_Max = 12
};

struct LineInfo
{
	BYTE		points[COLUMN_NUM] = { 255 };			// 坐标
	int			iWinMoney = 0;							// 赢钱
};

//滚动结果 
struct RollResult
{
	char			szGameID[GAME_ID_SIZE];				// 牌局ID
	__int64			iWinMoney;							// 基础赢钱数
	int				iTotalMultiple;						// 总倍数
	int				iFreeGameCnt;						// 免费游戏次数
	EmImageType		ImgType[LINE_NUM][COLUMN_NUM];
	bool			bRewardImage[LINE_NUM][COLUMN_NUM];	// 图示
	EmImageType		rewardImage[LINE_NUM];				// 中奖图案
	int				multipleAndMoney[LINE_NUM][2];		// 图案倍率 0行中奖倍数 1行中奖金额
	BYTE			lineCnt[LINE_NUM][2];				// 列数统计 0行表示列数 1行表示列数个数
	int				lineTotalCnt;						// 连线数总计
	LineInfo		allLines[LINE_MAX];					// 中奖连线详情
};


////////////////////////////////////// 消息结构  /////////////////////////
// 游戏场景数据
struct CMD_S_GameStation
{
	BYTE		    byGameStation;						// 游戏状态
	__int64			iWinMoney;							// 正常游戏输赢
	__int64			iFreeWinMoney;						// 免费游戏累计赢钱
	int				iFreeGames;							// 总共免费游戏次数
	int				iLeftFreeGameCnt;					// 剩余免费游戏次数
	int				iBetMoney;							// 最近的下注金额
	bool			bRewardImage[LINE_NUM][COLUMN_NUM];	// 中奖图案
	BYTE			iImageResult[LINE_NUM][COLUMN_NUM];	// 图案结果
	char			szGameID[GAME_ID_SIZE];				// 牌局ID
	int				lineTotalCnt;						// 连线数总计
	LineInfo		allLines[LINE_MAX];					// 中奖连线详情
};

//开始滚动
struct CMD_C_StartRoll
{
	int				iBetMoney;							// 下注金额
};


// 滚动结果
struct CMD_S_TypeScroll
{
	__int64			iWinMoney;							// 输赢金钱
	__int64			iFreeWinMoney;						// 免费游戏累计赢取
	int				iLeftFreeGameCnt;					// 剩余免费游戏次数
	bool			bFreeGame;							// 是否免费游戏
	bool			bRewardImage[LINE_NUM][COLUMN_NUM];	// 中奖图案
	BYTE			iImageResult[LINE_NUM][COLUMN_NUM];	// 图案结果
	char			szGameID[GAME_ID_SIZE];				// 牌局ID
	int				lineTotalCnt;						// 连线数总计
	LineInfo		allLines[LINE_MAX];					// 中奖连线详情
};

#pragma pack()
/********************************************************************************/

// 数据协议
/**************************服务端——>客户端消息**************************/
#define ASS_S_ROLL_RESULT					60				//滚动结果

/**************************客户端——>服务端消息**************************/
#define	ASS_C_START_ROLL					80				//启动游戏滚动

#endif
