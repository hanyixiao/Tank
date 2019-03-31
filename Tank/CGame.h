#pragma once
#include "GameMenuPannel.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"
#include <list>
#include "GameMap.h"
#include "wall.h"
#include "GameMenuPannel.h"
#include "GameMenuPanel2.h"
#include "KeyMenuPlayer02.h"
#include "KeyMenuPlayer01.h"
#include "GameMenuBackground.h"

using namespace std;
class CGame
{
public:
	CGame();
	~CGame();
	//����������ڵľ��
	void SetHandle(HWND hWnd);
	//������Ϸ֡
	bool EnterFrame(DWORD dwTime);
	//���������Ϸʱ��
	void OnMouseMove(UINT nFlags, CPoint point);
	//����������̧��ʱ��
	void OnLButtonUp(UINT nFlags, CPoint point);
	//bool GameInitOne2Bot();
	enum EGameType {
		EGameTypeMenu = 0,//ѡ��׶�
		EGameTypeOne2BotMenu,//�˻���ս�˵��׶�
		EGameTypeOne2Bot,//�˻���ս
		EGameTypeOne2BotEnd,//�˻���ս����
		EGameTypeOne2OneMenu,//˫�˴�ս�˵��׶�
		EGameTypeOne2One,//˫�˴�ս
		EGameTypeOne2OneEnd,//˫�˴�ս�����׶�
		EGameTypeCount,//��7���׶�
	};
	//���õ�ǰ��Ϸ�����׶β����ݲ����ʼ��
	void SetStep(CGame::EGameType step);
private:
	//����
	HWND m_hWnd;
	/*��Ϸ��ʼ��
	������Ϸ���󣬳�ʼ����ͼ������λ�õ�
	*/
	bool GameInit();

	bool GameInitMenu();

	bool GameInitOne2BotMenu();

	bool GameInitOne2Bot();

	bool GameInitOne2BotEnd();

	bool GameInitOne2OneMenu();
	
	bool GameInitOne2One();

	bool GameInitOne2OneEnd();

	//�����߷����������飬�������
	bool (CGame::*m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,//ѡ��׶�
		&CGame::GameInitOne2BotMenu,//�˻���ս������ʾ
		&CGame::GameInitOne2Bot,//�˻���ս
		&CGame::GameInitOne2BotEnd,//�˻���ս����
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd,
	};

	//�����ͼ����
	//void GameRunDraw();
	/*��Ϸ�߼�����
	 *1.ά���ӵ�״̬
	 *2.ά�������˵������ƶ����Զ������ӵ�
	 *3.ά����ҵ�̹��״̬
	 *��ײ���
	**/
	void GameRunLogic();

	void GameRunLogicOnMenu();

	void GameRunLogicOnOne2BotMenu();

	void GameRunLogicOnOne2Bot();

	void GameRunLogicOnOne2BotEnd();

	void GameRunLogicOnOne2OneMenu();

	void GameRunLogicOnOne2One();

	void GameRunLogicOnOne2OneEnd();

	void (CGame::*m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicOnMenu,
		&CGame::GameRunLogicOnOne2BotMenu,
		&CGame::GameRunLogicOnOne2Bot,
		&CGame::GameRunLogicOnOne2BotEnd,
		&CGame::GameRunLogicOnOne2OneMenu,
		&CGame::GameRunLogicOnOne2One,
		&CGame::GameRunLogicOnOne2OneEnd,
	};
	/*��Ϸ��ͼ����
	*�����ͼ��Ϸ�еĶ���
	*/
	void GameRunDraw();
	
	void GameRunDrawOnMenu(Graphics &gh);

	void GameRunDrawOnOne2BotMenu(Graphics &gh);

	void GameRunDrawOnOne2Bot(Graphics &gh);

	void GameRunDrawOnOne2BotEnd(Graphics &gh);

	void GameRunDrawOnOne2OneMenu(Graphics &gh);

	void GameRunDrawOnOne2One(Graphics &gh);

	void GameRunDrawOnOne2OneEnd(Graphics &gh);
	
	void (CGame::*m_drawFunc[EGameTypeCount])(Graphics &) =
	{
		&CGame::GameRunDrawOnMenu,
		&CGame::GameRunDrawOnOne2BotMenu,
		&CGame::GameRunDrawOnOne2Bot,
		&CGame::GameRunDrawOnOne2BotEnd,
		&CGame::GameRunDrawOnOne2OneMenu,
		&CGame::GameRunDrawOnOne2One,
		&CGame::GameRunDrawOnOne2OneEnd,
	};
	/*������������
	 *�ƶ��ӵ�
	 *ά���ӵ�
	 *�ӵ������й켣
	 *�����Զ�Ѱ��
	 *�����Ϸ֡��
	*/
	void RemoveTimeoutBullets();

	void ProcessHitBullets();

	void AI();

	void DrawFps(Graphics &gh);

	/*��ʼ��˽�г�Ա
     *m_ member��Ա����˼
	*/
	int m_fps{ 0 };

	EGameType m_eStep{EGameTypeMenu};

	CPlayer m_player01;

	CPlayer m_player02;

	CBot m_bot;

	std::list<CBullet> m_lstBullets;

	CGameMap m_map{ 10,10,780,580 };

	GameMenuPannel m_menuSelect;

	CGameMenuBackground m_menu;

	CGameMenuPanel2 m_menuBackup;

	CKeyMenuPlayer01 m_keymenu01;
	CKeyMenuPlayer02 m_keymenu02;
};

