
// MainFrm.cpp: CMainFrame 类的实现
//

#include "stdafx.h"
#include "Tank.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//全局变量
#define GAME_WIN_W (800)
#define GAME_WIN_H (600)

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMainFrame 构造/析构
CMainFrame::CMainFrame()
{
#define MY_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | \
        WS_MINIMIZEBOX | FWS_ADDTOTITLE)
	Create(NULL, _T("韩一笑笑的坦克大战"), MY_STYLE, CRect(0, 0, GAME_WIN_W, GAME_WIN_H));
	//设置客户区大小
	{
		CRect rcCli;
		GetClientRect(rcCli);
		RECT rcFrame = { 0,0,
			m_iWidth + m_iWidth - rcCli.right,
			m_iHeight + m_iHeight - rcCli.bottom
		};
		MoveWindow(&rcFrame, TRUE);
	}
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	SetTimer(ETimerIdGameLoop, 0, NULL);
	m_game.SetHandle(GetSafeHwnd());
	return 0;
}
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{  
		case ETimerIdGameLoop: {                           //游戏循环ID
			static DWORD dwLastUpdata = GetTickCount();    //记录本次时刻
			if (GetTickCount() - dwLastUpdata >= 20) {     //判断时间间隔
				m_game.EnterFrame(GetTickCount());         //进入游戏帧处理
				dwLastUpdata = GetTickCount();             //记录时间间隔
			}
			//否则什么都不做
		}
		default:
			break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}
void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_game.OnMouseMove(nFlags, point);
	CFrameWnd::OnMouseMove(nFlags, point);
}
void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_game.OnLButtonUp(nFlags, point);
	CFrameWnd::OnLButtonUp(nFlags,point);
}