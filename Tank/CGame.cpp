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
	m_menuSelect.OnMouseMove(nFlags, point);//选择游戏类型
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
												//画入内存
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), //复制到屏幕
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();

	return;//这个函数的返回类型是void 这里使用return是为了中断程序

}

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
	//m_menuSelect.Draw(gh);
	//画出一幅图片
	//{
	//	CRect rc;
	//	GetClientRect(m_hWnd, rc);
	//	// 载入需要绘制的图片
	//	Gdiplus::Image *img = Image::FromFile(_T("menu_background.png"));
	//	//画出图片
	//	gh.DrawImage(img, rc.left, rc.top, rc.Width(), rc.Height());

	//}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
 }
//void CGame::GameRunDraw()
//{
//	HDC hdc = ::GetDC(m_hWnd);
//	CRect rect;
//	GetClientRect(m_hWnd, &rect);
//	/*类CClientDC派生于CDC，在构造时调用了Windows函数GetDC，在析构时调用了ReleaseDC。
//	这意味着和CClientDC对象相关的设备上下文是窗口的客户区。
//    要了解有关CClientDC的更详细的信息，请参阅联机文档“Visual C++程序员指南”中的“设备上下文”部分。
//	*/
//	CDC *dc = CClientDC::FromHandle(hdc);
//	/*
//	CDC类定义的是设备上下文对象的类。
//	CDC对象提供处理显示器或打印机等设备上下文的成员函数，以及处理与窗口客户区对应的显示上下文的成员。
//	通过CDC对象的成员函数进行所有的绘图。
//	类对设备上下文操作提供了成员函数，处理绘图工具。安全型图形设备接口（GDI）对象收集，以及处理颜色和调色板。
//	它还为获取和设置绘图属性、映射，处理视点、窗口扩展、转换坐标，处理区域、剪贴、绘制直线及绘制简单椭圆和多
//	边形等形状提供了成员函数。
//	另外还为绘制文本、处理字体，使用打印机跳转，滚动和播放元文件提供成员函数。
//	使用CDC对象时要构造它，然后调用与它平等的、使用设备上下文的Windows函数的成员函数。
//	hwndFrom
//	正在发送通知的控件的窗口句柄。若要将该句柄转换成 CWnd 指针，请使用 CWnd::FromHandle
//	*/
//	CDC m_dcMemory;
//	CBitmap bmp;
//	bmp.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
//	CBitmap *pOldBitMap = m_dcMemory.SelectObject(&bmp);
//	Graphics gh(m_dcMemory.GetSafeHdc());//构造对象
//	gh.Clear(Color::White);
//	gh.ResetClip();
//	//划入内存
//	{
//		m_menu.Draw(gh);
//		m_menuSelect.Draw(gh);
//	}
//	DrawFps(gh);
//	//复制到屏幕
//	::BitBlt(hdc, 0, 0, rect.Width(), rect.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
//	dc->DeleteDC();
//	return; 
//}