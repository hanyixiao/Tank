#include "stdafx.h"
#include "CGame.h"
#include <list>
#include <algorithm>
/*ʵ�ֹ����෽��
*/
#define KEYDOWN(vk) (GetAsyncKeyState(vk)&0x8000)
CGame::CGame()
{
	m_menuSelect.m_pParent = this;//���ò˵�ѡ��ָ��ǰ����
	m_menuBackup.m_pParent = this;//���ò˵�����ָ��ǰ����
}


CGame::~CGame()
{
}
//ʵ��Ϊ����ṩ�ķ���
//���ھ��
//������Ϸ֡
//��������ƶ���Ϣ
//������������Ϣ
//������Ϸ�׶�
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
	//ѡ��׶�
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnMouseMove(nFlags, point);//ѡ����Ϸ����
	}
	else
	{
		m_menuBackup.OnMouseMove(nFlags, point);//�������˵�
	}
}
//�������̧���¼�
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnLButtonUp(nFlags, point);
	}
	else {
		m_menuBackup.OnLButtonUp(nFlags, point);//�������˵�
	}
}
//���õ�ǰ��Ϸ�����ڵĽ׶�
//�����ݲ����ʼ��
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();                 //���ó�ʼ������
}
/*ʵ�ֳ�ʼ������
 *
 *
*/
//������Ϸ���� ��ʼ����ͼ ����λ�õ�
bool CGame::GameInit()
{
	srand(GetTickCount());//��ʼ�������������
	return (this->*m_initFunc[m_eStep])();//���ݲ�ͬ�׶ε��ò�ͬ����
}

// ��Ϸ��ʼ����ѡ��׶�
bool CGame::GameInitMenu()
{
	return true;
}

//��Ϸ��ʼ�����˻���ս�˵��׶�
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

//��Ϸ��ʼ�����˻���ս
bool CGame::GameInitOne2Bot()
{
	for (;;)
	{
		m_map.LoadMap();
		//���һ
		{
			m_player01 = CPlayer(0, 0, TEXT("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("����player01λ��ʧ��"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);//�������һ���յ�����
			}
		}
		//������
		{
			m_bot = CBot(0, 0, TEXT("tank_bot.png"));
			PointF ptCenter;//������ǰ�治��ͻ�𣿣���������
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("����botλ��ʧ��"));
			}
			else {
				m_bot.SetCenterPoint(ptCenter);
			}
		}
		m_lstBullets.clear();		//����ӵ�����
		//�ж��Ƿ�Ϸ�
		{
			//��ȡ������ ���һ��λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(TEXT("���̹��λ�ô���"));
				goto _Init_End;
			}
			//�ж����һ�͵��Կ��Ƶ�̹���Ƿ���ͨ���������ͨ�Ļ�������������
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY,path);
			if (!path.empty()) {
				goto _Init_End;//������ֱͨ����������β����ʼ������
			}

		}
	}
_Init_End:
	return true;
}
//��Ϸ��ʼ�����˻���ս����
bool CGame::GameInitOne2BotEnd() 
{
	return true;
}

//��Ϸ��ʼ����˫�˴�ս�˵��׶�
bool CGame::GameInitOne2OneMenu()
{
	//����������ҵ�Keymenuλ�ã���Ļ��������
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

//��Ϸ��ʼ��˫�˴�ս
bool CGame::GameInitOne2One()
{
	for (;;)
	{
		m_map.LoadMap();
		//��̹��
		{
			m_player01 = CPlayer(0, 0, TEXT("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("����player01λ��ʧ��"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);//�������һ���յ�����
			}
		}
		{
			m_player02 = CPlayer(0, 0, TEXT("tank_player2.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(TEXT("����player02λ��ʧ��"));
			}
			else {
				m_player02.SetCenterPoint(ptCenter);//�������һ���յ�����
			}
		}
		m_lstBullets.clear();
		//�ж��Ƿ�Ϸ�
		{
			//��ȡ������ ���һ��λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(TEXT("���̹��λ�ô���"));
				break;
			}
			//�ж����һ�͵��Կ��Ƶ�̹���Ƿ���ͨ���������ͨ�Ļ�������������
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty()) {
				break;
			}
			//��������ͨ��˵���˴γ�ʼ��ʧ�ܣ��������Գ�ʼ��
		}
		
	}
	return true;
}
//��Ϸ��ʼ����˫�˴�ս����
bool CGame::GameInitOne2OneEnd()
{
	return true;
}
/*ʵ����Ϸ�߼�������
 *1.ά���ӵ�״̬
 *2.ά��������״̬
*/
void CGame::GameRunLogic()
{
	//���ݲ�ͬ�׶ε��ò�ͬ�ĺ���
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
		m_keymenu01.SetStop();//�������m��ֹͣ�˵�����
	}
	if (m_keymenu01.GetStop()) {
		SetStep(EGameTypeOne2Bot);
	}
}

//��Ϸ�߼�����
void CGame::GameRunLogicOnOne2Bot()
{
	//ά��״̬
	//�Ƴ��б��в���Ҫ���ӵ���������Ӧ̹�������ӵ�
	RemoveTimeoutBullets();

	//����ӵ��Ƿ����̹�ˣ�����Ҫʹ̹�˱�ը
	for (auto &blt : m_lstBullets) {
		if (m_bot.IsHitted(blt)) {
			m_bot.Bomb();//̹�˱�ը
			m_eStep = EGameTypeOne2BotEnd;//��Ϸ����
			blt.SetActive(false);//�ӵ�������Ч
		}
		if (m_player01.IsHitted(blt)) {
			m_player01.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}
		break;//Ϊɶ����Ҫ��break��
	}
	ProcessHitBullets();//�ӵ��˶�ά��

	AI();

	//��������
	{
		if (KEYDOWN(VK_LEFT))
		{
			m_player01.RotateLeft();//���1����ת
		}
		if (KEYDOWN(VK_RIGHT))
		{
			m_player01.RotateRight();
		}
		if (KEYDOWN(VK_UP))
		{   //ײǽ���
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
			if (m_player01.Fire(blt)) {       //����
				m_lstBullets.push_back(blt);  //���ӵ����뵽��ͼ��ȥ
			}
		}
		if (KEYDOWN('I'))
		{
			//to do nothing
		}
	}

}
//��Ϸ�߼������˻���ս����
void CGame::GameRunLogicOnOne2BotEnd()
{
	//�������� ���ٽ��ܰ���
	RemoveTimeoutBullets();

	//�ӵ��Ƿ�װ��ǽ�壬
	ProcessHitBullets();
}

//��Ϸ�߼�����˫�˴�ս�˵��׶�
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

//��Ϸ�߼�����˫����Ϸ��ս
void CGame::GameRunLogicOnOne2One()
{
	//��������
	{
		{
			if (KEYDOWN(VK_LEFT))
			{
				m_player01.RotateLeft();//���1����ת
			}
			if (KEYDOWN(VK_RIGHT))
			{
				m_player01.RotateRight();
			}
			if (KEYDOWN(VK_UP))
			{   //ײǽ���
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
				if (m_player01.Fire(blt)) {       //����
					m_lstBullets.push_back(blt);  //���ӵ����뵽��ͼ��ȥ
				}
			}
		}
		{
			if (KEYDOWN('A'))
			{
				m_player02.RotateLeft();//���1����ת
			}
			if (KEYDOWN('D'))
			{
				m_player02.RotateRight();
			}
			if (KEYDOWN('W'))
			{   //ײǽ���
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
				if (m_player02.Fire(blt)) {       //����
					m_lstBullets.push_back(blt);  //���ӵ����뵽��ͼ��ȥ
				}
			}
		}
		if (KEYDOWN('Z'))
		{
			if (m_map.IsCanKillTarget(m_player01, m_player02)) {
				AfxMessageBox(TEXT("�ܹ���"));
			}
		}
	}
	//���ж�״̬
	//�Ƴ���Ч���ӵ���������Ӧ��̹�������ӵ�
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
//��Ϸ�߼�����˫�˴�ս����
void CGame::GameRunLogicOnOne2OneEnd()
{
	//��������
	RemoveTimeoutBullets();

	ProcessHitBullets();

}

/*ʵ����Ϸ�Ļ�ͼ������
 *
 *
*/
//��Ϸ��ͼ
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
	//�����ڴ�
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), //���Ƶ���Ļ
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	::ReleaseDC(m_hWnd, hdc);
	
	return;//��������ķ���������void ����ʹ��return��Ϊ���жϳ���
}

// ѡ��׶�
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
	m_menuSelect.Draw(gh);
}

//�˻���ս���˵��׶�
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
	m_menu.Draw(gh);
	m_keymenu01.Draw(gh);
}

//�˻���ս
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_bot.Draw(gh);
	for (auto b : m_lstBullets) { //���Ѿ���������ӵ�
		b.Draw(gh);
	}
	DrawFps(gh);

}
//�˻���ս����
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_bot.Draw(gh);
	for (auto b : m_lstBullets) { //���Ѿ���������ӵ�
		b.Draw(gh);
	}
	DrawFps(gh);
	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;//����Ϊ�˵�״̬
	}
}
//˫�˴�ս���˵�״̬
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
	m_menu.Draw(gh);
	m_keymenu01.Draw(gh);
	m_keymenu02.Draw(gh);
}
//˫�˴�ս
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
//˫�˴�ս����
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
		m_eStep = EGameTypeMenu;//������ϷΪ�˵�״̬
	}
 }
/*ʵ����Ϸ��������
 *
*/
//��fps
void CGame::DrawFps(Graphics &gh)
{
	static int fps = 0;       //���徲̬������û�ν��뺯��ʱ�����ϴε�ֵ
	m_fps++;                   //��¼�Ѿ����˶���֡
	static DWORD dwLast = GetTickCount(); //��¼�ϴ����fps
	if (GetTickCount() - dwLast >= 1000) {
		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();            //��¼���������ʱ��
	}
	//���fps
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);//��fps��ʽ�����ַ���
		SolidBrush brush(Color(0x00, 0x00, 0xff));//������ɫ�Ļ�ˢ
		Gdiplus::Font font(_T("����"),10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right-100),
			static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), - 1, &font, origin, &brush);//��ʽ�������
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
 }
//�Ƴ���ʱ���ӵ�
void CGame::RemoveTimeoutBullets()
{
	auto  itRemove = std::remove_if(m_lstBullets.begin(),
		m_lstBullets.end(),
		[](CBullet &blt)->bool {
		return blt.IsTimeout();
	});
	for (auto it = itRemove; it != m_lstBullets.end(); ++it)
	{
		it->SetActive(false);         //����Ϊ��Ч
		it->GetOwner()->AddBullet(*it);  //��Ӧ��̹�������ӵ�
	}
	//�ӱ���ɾ���ӵ�
	m_lstBullets.erase(itRemove, m_lstBullets.end());
}
//�ӵ�ά��
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
		//��û����ˡ���ͼ�пո�����ĵ�
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX,startY);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

		//�ж�̹���Ƿ��Ѿ��������ĵ�
		if (!rc.Contains(ptTankCenter)) {
			m_bot.Forward();
			return;
		}
		else {
			m_bot.SetDirection(fDirNext);
			float dir;
			//�ж��Ƿ���Ի������һ
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
