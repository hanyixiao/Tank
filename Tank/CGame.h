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
	//�����ͼ����
	void GameRunDraw();
	GameMenuPannel m_menuSelect; //��ʼ�˵�
	CGameBackground m_menu;  //��ʼ�˵�����ͼ
	//����滭��Ϸ�еĶ���
	//���fps
	void DrawFps(Graphics &gh);
	//��¼��Ϸÿ���ж���֡
	int m_fps{ 0 };

};

