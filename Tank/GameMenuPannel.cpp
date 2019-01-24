#include "GameMenuPannel.h"
#include "stdafx.h"
#include "CGame.h"
//构造函数
GameMenuPannel::GameMenuPannel()
{
	// 背景图
	m_imgBackground = Image::FromFile(_T("menu_backgroud.png"));
	//菜单选项一：人机大战
	m_menuItems[0].index = 0;
	for (int i = 0; i < 2; ++i) {
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_1player_%d.png"), i);   //格式化文件名
		auto imgPtr = Image::FromFile(path);              //载入图片
		RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()),
			static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[0].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));//图片存储在数组中
	}
	//菜单选项二：双人大战
	m_menuItems[1].index = 0;
	for (int i = 0; i < 2; ++i) {
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_2player_%d.png"), i);
		auto imgPtr = Image::FromFile(path);
		RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth()),
			static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[1].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));//图片储存在数组里
	}
}



GameMenuPannel::~GameMenuPannel()
{

}
//绘制自己
void GameMenuPannel::Draw(Graphics &gh)const
{
	//gh.DrawImage(m_imgBackground, m_rect);
	//画子菜单
	for (auto menuItem : m_menuItems) {
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);//获取菜单选项的图片
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);//获取菜单项的大小
		gh.DrawImage(img, rect);
	}
	//auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);//获取菜单选项的图片
	//auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);//获取菜单项的大小
	//gh.DrawImage(img, rect);
}
//处理鼠标移事件
void GameMenuPannel::OnMouseMove(UINT nFlags, CPoint point) {
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	//画子菜单
	for (auto &menuItem : m_menuItems) {
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt)) {//判断是否包含当前鼠标位置
			menuItem.index = 1;
		}
		else {
			menuItem.index = 0;//不包含显示第0张图片
		}
	}
}

//处理鼠标左键抬起事件
void GameMenuPannel::OnLButtonUp(UINT flags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	{
		auto &menuItem = m_menuItems[0];
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt)) {
			//人机大战：暂时不做处理
			return;
		}
	}
	{
		auto &menuItem = m_menuItems[1];
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt)) {
			//双人大战，暂时不做处理
			return;
		}
	}
}
