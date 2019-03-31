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
	//设置输出窗口的句柄
	void SetHandle(HWND hWnd);
	//进入游戏帧
	bool EnterFrame(DWORD dwTime);
	//处理鼠标游戏时间
	void OnMouseMove(UINT nFlags, CPoint point);
	//处理鼠标左键抬起时间
	void OnLButtonUp(UINT nFlags, CPoint point);
	//bool GameInitOne2Bot();
	enum EGameType {
		EGameTypeMenu = 0,//选择阶段
		EGameTypeOne2BotMenu,//人机大战菜单阶段
		EGameTypeOne2Bot,//人机大战
		EGameTypeOne2BotEnd,//人机大战结束
		EGameTypeOne2OneMenu,//双人大战菜单阶段
		EGameTypeOne2One,//双人大战
		EGameTypeOne2OneEnd,//双人大战结束阶段
		EGameTypeCount,//有7个阶段
	};
	//设置当前游戏所处阶段并根据步骤初始化
	void SetStep(CGame::EGameType step);
private:
	//窗口
	HWND m_hWnd;
	/*游戏初始化
	生成游戏对象，初始化地图，对象位置等
	*/
	bool GameInit();

	bool GameInitMenu();

	bool GameInitOne2BotMenu();

	bool GameInitOne2Bot();

	bool GameInitOne2BotEnd();

	bool GameInitOne2OneMenu();
	
	bool GameInitOne2One();

	bool GameInitOne2OneEnd();

	//把上诉方法放入数组，方便调用
	bool (CGame::*m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,//选择阶段
		&CGame::GameInitOne2BotMenu,//人机大战键盘提示
		&CGame::GameInitOne2Bot,//人机大战
		&CGame::GameInitOne2BotEnd,//人机大战结束
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd,
	};

	//负责绘图处理
	//void GameRunDraw();
	/*游戏逻辑处理
	 *1.维护子弹状态
	 *2.维护机器人的自由移动，自动发射子弹
	 *3.维护玩家的坦克状态
	 *碰撞检测
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
	/*游戏绘图处理
	*负责绘图游戏中的对象
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
	/*声明辅助方法
	 *移动子弹
	 *维护子弹
	 *子弹的运行轨迹
	 *电脑自动寻迹
	 *输出游戏帧数
	*/
	void RemoveTimeoutBullets();

	void ProcessHitBullets();

	void AI();

	void DrawFps(Graphics &gh);

	/*初始化私有成员
     *m_ member成员的意思
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

