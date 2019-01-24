#include "stdafx.h"
#include "CGame.h"
CGame::CGame()
{
}


CGame::~CGame()
{
}
void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}
bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunDraw();
	return false;
}
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_menuSelect.OnMouseMove(nFlags, point);//ѡ����Ϸ����
}
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_menuSelect.OnLButtonUp(nFlags, point);
}
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
	{ 
		m_menu.Draw(gh);
	   	m_menuSelect.Draw(gh);
		//m_menuSelect.Draw(gh);
	}
	DrawFps(gh);
												//�����ڴ�
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), //���Ƶ���Ļ
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();

	return;//��������ķ���������void ����ʹ��return��Ϊ���жϳ���

}

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
	//m_menuSelect.Draw(gh);
	//����һ��ͼƬ
	//{
	//	CRect rc;
	//	GetClientRect(m_hWnd, rc);
	//	// ������Ҫ���Ƶ�ͼƬ
	//	Gdiplus::Image *img = Image::FromFile(_T("menu_background.png"));
	//	//����ͼƬ
	//	gh.DrawImage(img, rc.left, rc.top, rc.Width(), rc.Height());

	//}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
 }
//void CGame::GameRunDraw()
//{
//	HDC hdc = ::GetDC(m_hWnd);
//	CRect rect;
//	GetClientRect(m_hWnd, &rect);
//	/*��CClientDC������CDC���ڹ���ʱ������Windows����GetDC��������ʱ������ReleaseDC��
//	����ζ�ź�CClientDC������ص��豸�������Ǵ��ڵĿͻ�����
//    Ҫ�˽��й�CClientDC�ĸ���ϸ����Ϣ������������ĵ���Visual C++����Աָ�ϡ��еġ��豸�����ġ����֡�
//	*/
//	CDC *dc = CClientDC::FromHandle(hdc);
//	/*
//	CDC�ඨ������豸�����Ķ�����ࡣ
//	CDC�����ṩ������ʾ�����ӡ�����豸�����ĵĳ�Ա�������Լ������봰�ڿͻ�����Ӧ����ʾ�����ĵĳ�Ա��
//	ͨ��CDC����ĳ�Ա�����������еĻ�ͼ��
//	����豸�����Ĳ����ṩ�˳�Ա�����������ͼ���ߡ���ȫ��ͼ���豸�ӿڣ�GDI�������ռ����Լ�������ɫ�͵�ɫ�塣
//	����Ϊ��ȡ�����û�ͼ���ԡ�ӳ�䣬�����ӵ㡢������չ��ת�����꣬�������򡢼���������ֱ�߼����Ƽ���Բ�Ͷ�
//	���ε���״�ṩ�˳�Ա������
//	���⻹Ϊ�����ı����������壬ʹ�ô�ӡ����ת�������Ͳ���Ԫ�ļ��ṩ��Ա������
//	ʹ��CDC����ʱҪ��������Ȼ���������ƽ�ȵġ�ʹ���豸�����ĵ�Windows�����ĳ�Ա������
//	hwndFrom
//	���ڷ���֪ͨ�Ŀؼ��Ĵ��ھ������Ҫ���þ��ת���� CWnd ָ�룬��ʹ�� CWnd::FromHandle
//	*/
//	CDC m_dcMemory;
//	CBitmap bmp;
//	bmp.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
//	CBitmap *pOldBitMap = m_dcMemory.SelectObject(&bmp);
//	Graphics gh(m_dcMemory.GetSafeHdc());//�������
//	gh.Clear(Color::White);
//	gh.ResetClip();
//	//�����ڴ�
//	{
//		m_menu.Draw(gh);
//		m_menuSelect.Draw(gh);
//	}
//	DrawFps(gh);
//	//���Ƶ���Ļ
//	::BitBlt(hdc, 0, 0, rect.Width(), rect.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
//	dc->DeleteDC();
//	return; 
//}