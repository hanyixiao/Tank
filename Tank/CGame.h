#pragma once
#include "GameMenuPannel.h"
#include "CGameBackGround.h"
#include "TankEntry.h"

class CGame
{
public:
	CGame();
	~CGame();

	void SetHandle(HWND hWnd);

	bool EnterFrame(DWORD dwTime);

	void OnMouseMove(UINT nFlags, CPoint point);

	void OnLButtonUp(UINT nFlags, CPoint point);
private:
	HWND m_hWnd;
	//负责绘图处理
	void GameRunDraw();
	GameMenuPannel m_menuSelect; //开始菜单
	CGameBackground m_menu;  //开始菜单背景图
	//负责绘画游戏中的对象
	//输出fps
	void DrawFps(Graphics &gh);
	//记录游戏每秒有多少帧
	int m_fps{ 0 };

};

