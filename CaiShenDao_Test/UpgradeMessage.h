#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("Slotomania")
#define GAME_DLL_AUTHOR                 TEXT("����")    
#define GAME_DLL_NOTE                   TEXT("���� -- ��Ϸ���")

//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾
#define SUPPER_TYPE						1

//��Ϸ��Ϣ
#define NAME_ID							40066000						//���� ID
#define PLAY_COUNT						1								//��Ϸ����

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_PLAY_GAME					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

#define GAME_ID_SIZE					20				//�ƾ�ID����
#define LINE_NUM						3				//����
#define COLUMN_NUM						5				//����
#define LINE_MAX						243				//�������ֵ

#pragma pack(1)

using BYTE = unsigned char;

enum EmImageType : BYTE
{
	ImageType_Null = 0,				// ��Чͼ��
	ImageType_CaiShen = 1,			// ����
	ImageType_MiLe = 2,				// ����
	ImageType_LaoZhe = 3,			// ����
	ImageType_JinYu = 4,			// ����
	ImageType_FuDai = 5,			// ����
	ImageType_RuYi = 6,				// ����
	ImageType_XianTao = 7,			// ����
	ImageType_BaoZi = 8,			// ����
	ImageType_TangHuLu = 9,			// �Ǻ�«
	ImageType_JingLuo = 10,			// ����
	ImageType_CaiShenDao = 11,		// ����
	ImageType_Max = 12
};

struct LineInfo
{
	BYTE		points[COLUMN_NUM] = { 255 };			// ����
	int			iWinMoney = 0;							// ӮǮ
};

//������� 
struct RollResult
{
	char			szGameID[GAME_ID_SIZE];				// �ƾ�ID
	__int64			iWinMoney;							// ����ӮǮ��
	int				iTotalMultiple;						// �ܱ���
	int				iFreeGameCnt;						// �����Ϸ����
	EmImageType		ImgType[LINE_NUM][COLUMN_NUM];
	bool			bRewardImage[LINE_NUM][COLUMN_NUM];	// ͼʾ
	EmImageType		rewardImage[LINE_NUM];				// �н�ͼ��
	int				multipleAndMoney[LINE_NUM][2];		// ͼ������ 0���н����� 1���н����
	BYTE			lineCnt[LINE_NUM][2];				// ����ͳ�� 0�б�ʾ���� 1�б�ʾ��������
	int				lineTotalCnt;						// �������ܼ�
	LineInfo		allLines[LINE_MAX];					// �н���������
};


////////////////////////////////////// ��Ϣ�ṹ  /////////////////////////
// ��Ϸ��������
struct CMD_S_GameStation
{
	BYTE		    byGameStation;						// ��Ϸ״̬
	__int64			iWinMoney;							// ������Ϸ��Ӯ
	__int64			iFreeWinMoney;						// �����Ϸ�ۼ�ӮǮ
	int				iFreeGames;							// �ܹ������Ϸ����
	int				iLeftFreeGameCnt;					// ʣ�������Ϸ����
	int				iBetMoney;							// �������ע���
	bool			bRewardImage[LINE_NUM][COLUMN_NUM];	// �н�ͼ��
	BYTE			iImageResult[LINE_NUM][COLUMN_NUM];	// ͼ�����
	char			szGameID[GAME_ID_SIZE];				// �ƾ�ID
	int				lineTotalCnt;						// �������ܼ�
	LineInfo		allLines[LINE_MAX];					// �н���������
};

//��ʼ����
struct CMD_C_StartRoll
{
	int				iBetMoney;							// ��ע���
};


// �������
struct CMD_S_TypeScroll
{
	__int64			iWinMoney;							// ��Ӯ��Ǯ
	__int64			iFreeWinMoney;						// �����Ϸ�ۼ�Ӯȡ
	int				iLeftFreeGameCnt;					// ʣ�������Ϸ����
	bool			bFreeGame;							// �Ƿ������Ϸ
	bool			bRewardImage[LINE_NUM][COLUMN_NUM];	// �н�ͼ��
	BYTE			iImageResult[LINE_NUM][COLUMN_NUM];	// ͼ�����
	char			szGameID[GAME_ID_SIZE];				// �ƾ�ID
	int				lineTotalCnt;						// �������ܼ�
	LineInfo		allLines[LINE_MAX];					// �н���������
};

#pragma pack()
/********************************************************************************/

// ����Э��
/**************************����ˡ���>�ͻ�����Ϣ**************************/
#define ASS_S_ROLL_RESULT					60				//�������

/**************************�ͻ��ˡ���>�������Ϣ**************************/
#define	ASS_C_START_ROLL					80				//������Ϸ����

#endif