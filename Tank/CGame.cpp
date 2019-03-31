#include "stdafx.h"
#include "CGame.h"
#include <list>
#include <algorithm>
/*实现公有类方法
*/
#define KEYDOWN(vk) (GetAsyncKeyState(vk)&0x8000)
CGame::CGame()
{
	m_menuSelect.m_pParent = this;//设置菜单选项指向当前对象
	m_menuBackup.m_pParent = this;//设置菜单背景指向当前对象
}


CGame::~CGame()
{
}
//实现为外界提供的方法
//窗口句柄
//进入游戏帧
//处理鼠标移动消息
//处理鼠标左键消息
//设置游戏阶段
void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}
bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunLogic();
	GameRunDraw();
	return false;
}
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	//选择阶段
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnMouseMove(nFlags, point);//选择游戏类型
	}
	else
	{
		m_menuBackup.OnMouseMove(nFlags, point);//返回主菜单
	}
}
//处理左键抬起事件
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnLButtonUp(nFlags, point);
	}
	else {
		m_menuBackup.OnLButtonUp(nFlags, point);//返回主菜单
	}
}
//设置当前游戏所处在的阶段
//并根据步骤初始化
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();                 //调用初始化函数
}
/*实现初始化方法
 *
 *
*/
//生成游戏对象 初始化地图 对象位置等
bool CGame::GameInit()
{
	srand(GetTickCount());//初始化随机数生成器
	return (this->*m_initFunc[m_eStep])();//根据不同阶段调用不同函数
}

// 游戏初始化：选择阶段
bool CGame::GameInitMenu()
{
	return true;
}

//游戏初始化：人机大战菜单阶段
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);
	return true;
}

//游戏初始化：人机大战
bool CGame::GameInitOne2Bot()
{
	for (;;)
	{
		m_map.LoadMap();
		//玩家一
		{
			m_player01 = CPlayer(0, 0, TEXT("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("调整player01位置失败"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);//放置玩家一到空地中心
			}
		}
		//机器人
		{
			m_bot = CBot(0, 0, TEXT("tank_bot.png"));
			PointF ptCenter;//这里与前面不冲突吗？？？？？？
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("调整bot位置失败"));
			}
			else {
				m_bot.SetCenterPoint(ptCenter);
			}
		}
		m_lstBullets.clear();		//清空子弹数组
		//判断是否合法
		{
			//获取机器人 玩家一的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(TEXT("获得坦克位置错误"));
				goto _Init_End;
			}
			//判断玩家一和电脑控制的坦克是否联通，如果不连通的话必须重新设置
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY,path);
			if (!path.empty()) {
				goto _Init_End;//可以联通直接跳到函数尾部初始化结束
			}

		}
	}
_Init_End:
	return true;
}
//游戏初始化：人机大战结束
bool CGame::GameInitOne2BotEnd() 
{
	return true;
}

//游戏初始化：双人大战菜单阶段
bool CGame::GameInitOne2OneMenu()
{
	//设置两个玩家的Keymenu位置：屏幕的正中央
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);


	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu02.SetCenterPoint(pt);
	m_keymenu02.SetStop(false);


	return true;
}

//游戏初始化双人大战
bool CGame::GameInitOne2One()
{
	for (;;)
	{
		m_map.LoadMap();
		//画坦克
		{
			m_player01 = CPlayer(0, 0, TEXT("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("调整player01位置失败"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);//放置玩家一到空地中心
			}
		}
		{
			m_player02 = CPlayer(0, 0, TEXT("tank_player2.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("调整player02位置失败"));
			}
			else {
				m_player02.SetCenterPoint(ptCenter);//放置玩家一到空地中心
			}
		}
		m_lstBullets.clear();
		//判断是否合法
		{
			//获取机器人 玩家一的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(TEXT("获得坦克位置错误"));
				break;
			}
			//判断玩家一和电脑控制的坦克是否联通，如果不连通的话必须重新设置
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty()) {
				break;
			}
			//不可以联通则说明此次初始化失败，继续尝试初始化
		}
		
	}
	return true;
}
//游戏初始化：双人大战结束
bool CGame::GameInitOne2OneEnd()
{
	return true;
}
/*实现游戏逻辑处理方法
 *1.维护子弹状态
 *2.维护机器人状态
*/
void CGame::GameRunLogic()
{
	//根据不同阶段调用不同的函数
	//m_eStep = EGameTypeOne2Bot;
	(this->*m_logicFunc[m_eStep])();
}

void CGame::GameRunLogicOnMenu()
{
	//to do nothing
}

void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M')) {
		m_keymenu01.SetStop();//如果按下m键停止菜单动画
	}
	if (m_keymenu01.GetStop()) {
		SetStep(EGameTypeOne2Bot);
	}
}

//游戏逻辑处理
void CGame::GameRunLogicOnOne2Bot()
{
	//维护状态
	//移除列表中不必要的子弹，并给相应坦克增加子弹
	RemoveTimeoutBullets();

	//检查子弹是否击中坦克，击中要使坦克爆炸
	for (auto &blt : m_lstBullets) {
		if (m_bot.IsHitted(blt)) {
			m_bot.Bomb();//坦克爆炸
			m_eStep = EGameTypeOne2BotEnd;//游戏结束
			blt.SetActive(false);//子弹不再有效
		}
		if (m_player01.IsHitted(blt)) {
			m_player01.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}
		break;//为啥这里要有break？
	}
	ProcessHitBullets();//子弹运动维护

	AI();

	//按键处理
	{
		if (KEYDOWN(VK_LEFT))
		{
			m_player01.RotateLeft();//玩家1向左转
		}
		if (KEYDOWN(VK_RIGHT))
		{
			m_player01.RotateRight();
		}
		if (KEYDOWN(VK_UP))
		{   //撞墙检测
			if (m_map.IsHitTheWall(m_player01, true)) {
				m_player01.ChangeDirection(true);
			}
			else {
				m_player01.Forward();
			}
		}
		if (KEYDOWN(VK_DOWN))
		{
			if (m_map.IsHitTheWall(m_player01, true)) {
				m_player01.ChangeDirection(true);
			}
			else {
				m_player01.Backward();
			}
		}
		if (KEYDOWN('M'))
		{
			CBullet blt;
			if (m_player01.Fire(blt)) {       //开火
				m_lstBullets.push_back(blt);  //将子弹加入到地图中去
			}
		}
		if (KEYDOWN('I'))
		{
			//to do nothing
		}
	}

}
//游戏逻辑处理：人机大战结束
void CGame::GameRunLogicOnOne2BotEnd()
{
	//按键处理 不再接受按键
	RemoveTimeoutBullets();

	//子弹是否装上墙体，
	ProcessHitBullets();
}

//游戏逻辑处理：双人大战菜单阶段
void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M')) {
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q')) {
		m_keymenu02.SetStop();
	}
	if (m_keymenu01.GetStop() && m_keymenu02.GetStop()) {
		SetStep(EGameTypeOne2One);
	}
}

//游戏逻辑处理：双人游戏大战
void CGame::GameRunLogicOnOne2One()
{
	//按键处理
	{
		{
			if (KEYDOWN(VK_LEFT))
			{
				m_player01.RotateLeft();//玩家1向左转
			}
			if (KEYDOWN(VK_RIGHT))
			{
				m_player01.RotateRight();
			}
			if (KEYDOWN(VK_UP))
			{   //撞墙检测
				if (m_map.IsHitTheWall(m_player01, true)) {
					m_player01.ChangeDirection(true);
				}
				else {
					m_player01.Forward();
				}
			}
			if (KEYDOWN(VK_DOWN))
			{
				if (m_map.IsHitTheWall(m_player01, true)) {
					m_player01.ChangeDirection(true);
				}
				else {
					m_player01.Backward();
				}
			}
			if (KEYDOWN('M'))
			{
				CBullet blt;
				if (m_player01.Fire(blt)) {       //开火
					m_lstBullets.push_back(blt);  //将子弹加入到地图中去
				}
			}
		}
		{
			if (KEYDOWN('A'))
			{
				m_player02.RotateLeft();//玩家1向左转
			}
			if (KEYDOWN('D'))
			{
				m_player02.RotateRight();
			}
			if (KEYDOWN('W'))
			{   //撞墙检测
				if (m_map.IsHitTheWall(m_player02, true)) {
					m_player02.ChangeDirection(true);
				}
				else {
					m_player02.Forward();
				}
			}
			if (KEYDOWN('S'))
			{
				if (m_map.IsHitTheWall(m_player02, true)) {
					m_player02.ChangeDirection(true);
				}
				else {
					m_player02.Backward();
				}
			}
			if (KEYDOWN('Q'))
			{
				CBullet blt;
				if (m_player02.Fire(blt)) {       //开火
					m_lstBullets.push_back(blt);  //将子弹加入到地图中去
				}
			}
		}
		if (KEYDOWN('Z'))
		{
			if (m_map.IsCanKillTarget(m_player01, m_player02)) {
				AfxMessageBox(TEXT("能够打到"));
			}
		}
	}
	//先判断状态
	//移除无效的子弹，并给相应的坦克增加子弹
	for (auto &blt : m_lstBullets) {
		if (!blt.IsActive()) {
			continue;
		}
		if (m_player01.IsHitted(blt)) {
			m_player01.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
		if (m_player02.IsHitted(blt)) {
			m_player02.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
	}
	ProcessHitBullets();
}
//游戏逻辑处理：双人大战结束
void CGame::GameRunLogicOnOne2OneEnd()
{
	//按键处理
	RemoveTimeoutBullets();

	ProcessHitBullets();

}

/*实现游戏的绘图处理方法
 *
 *
*/
//游戏绘图
void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	CDC *dc = CClientDC::FromHandle(hdc);
	CDC m_dcMemory;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBit = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc());
	gh.Clear(Color::White);
	gh.ResetClip();
	
	//m_eStep = EGameTypeOne2BotMenu;
	(this->*m_drawFunc[m_eStep])(gh);
	//画入内存
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), //复制到屏幕
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	::ReleaseDC(m_hWnd, hdc);
	
	return;//这个函数的返回类型是void 这里使用return是为了中断程序
}

// 选择阶段
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
	m_menuSelect.Draw(gh);
}

//人机大战：菜单阶段
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
	m_menu.Draw(gh);
	m_keymenu01.Draw(gh);
}

//人机大战
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_bot.Draw(gh);
	for (auto b : m_lstBullets) { //画已经发射出的子弹
		b.Draw(gh);
	}
	DrawFps(gh);

}
//人机大战结束
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_bot.Draw(gh);
	for (auto b : m_lstBullets) { //画已经发射出的子弹
		b.Draw(gh);
	}
	DrawFps(gh);
	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;//设置为菜单状态
	}
}
//双人大战：菜单状态
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
	m_menu.Draw(gh);
	m_keymenu01.Draw(gh);
	m_keymenu02.Draw(gh);
}
//双人大战
void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
	m_menuBackup.Draw(gh);

	m_map.Draw(gh);

	m_player01.Draw(gh);
	
	m_player02.Draw(gh);
	for (auto b : m_lstBullets) {
		b.Draw(gh);
	}
	DrawFps(gh);
}
//双人大战结束
void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);

	m_map.Draw(gh);

	m_player01.Draw(gh);

	m_player02.Draw(gh);
	for (auto b : m_lstBullets) {
		b.Draw(gh);
	}
	DrawFps(gh);
	if (m_player01.IsBombEnd() || m_player02.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;//设置游戏为菜单状态
	}
 }
/*实现游戏辅助方法
 *
*/
//画fps
void CGame::DrawFps(Graphics &gh)
{
	static int fps = 0;       //定义静态变量，没次进入函数时保存上次的值
	m_fps++;                   //记录已经画了多少帧
	static DWORD dwLast = GetTickCount(); //记录上次输出fps
	if (GetTickCount() - dwLast >= 1000) {
		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();            //记录本次输出的时间
	}
	//输出fps
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);//将fps格式化到字符串
		SolidBrush brush(Color(0x00, 0x00, 0xff));//创建红色的画刷
		Gdiplus::Font font(_T("宋体"),10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right-100),
			static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), - 1, &font, origin, &brush);//正式输出文字
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
 }
//移除超时的子弹
void CGame::RemoveTimeoutBullets()
{
	auto  itRemove = std::remove_if(m_lstBullets.begin(),
		m_lstBullets.end(),
		[](CBullet &blt)->bool {
		return blt.IsTimeout();
	});
	for (auto it = itRemove; it != m_lstBullets.end(); ++it)
	{
		it->SetActive(false);         //设置为无效
		it->GetOwner()->AddBullet(*it);  //对应的坦克增加子弹
	}
	//从本地删除子弹
	m_lstBullets.erase(itRemove, m_lstBullets.end());
}
//子弹维护
void CGame::ProcessHitBullets()
{
	for (auto &blt : m_lstBullets)
	{
		m_map.HitWallProcess(blt);
		blt.Move();
	}
}
void CGame::AI()
{
	static CGameTimer acTimer(-1, 150.5);
	if (acTimer.IsTimeval()) {
		int startX, startY, targetX, targetY;
		if (!m_map.FindObjPosition(m_bot, startX, startY) || !m_map.FindObjPosition(m_player01, targetX, targetY)) {
			return;
		}
		float fDirNext = 0;
		if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY)) {
			return;
		}
		//获得机器人。地图中空格地中心点
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX,startY);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

		//判断坦克是否已经到达中心点
		if (!rc.Contains(ptTankCenter)) {
			m_bot.Forward();
			return;
		}
		else {
			m_bot.SetDirection(fDirNext);
			float dir;
			//判断是否可以击中玩家一
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir))
			{
				CBullet blt;
				if (m_bot.Fire(blt)) {
					m_lstBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();
		}
	}
}
