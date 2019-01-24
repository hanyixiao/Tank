#include "GameMenuPannel.h"
#include "stdafx.h"
#include "CGame.h"
//���캯��
GameMenuPannel::GameMenuPannel()
{
	// ����ͼ
	m_imgBackground = Image::FromFile(_T("menu_backgroud.png"));
	//�˵�ѡ��һ���˻���ս
	m_menuItems[0].index = 0;
	for (int i = 0; i < 2; ++i) {
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_1player_%d.png"), i);   //��ʽ���ļ���
		auto imgPtr = Image::FromFile(path);              //����ͼƬ
		RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()),
			static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[0].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));//ͼƬ�洢��������
	}
	//�˵�ѡ�����˫�˴�ս
	m_menuItems[1].index = 0;
	for (int i = 0; i < 2; ++i) {
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_2player_%d.png"), i);
		auto imgPtr = Image::FromFile(path);
		RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth()),
			static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[1].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));//ͼƬ������������
	}
}



GameMenuPannel::~GameMenuPannel()
{

}
//�����Լ�
void GameMenuPannel::Draw(Graphics &gh)const
{
	//gh.DrawImage(m_imgBackground, m_rect);
	//���Ӳ˵�
	for (auto menuItem : m_menuItems) {
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);//��ȡ�˵�ѡ���ͼƬ
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);//��ȡ�˵���Ĵ�С
		gh.DrawImage(img, rect);
	}
	//auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);//��ȡ�˵�ѡ���ͼƬ
	//auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);//��ȡ�˵���Ĵ�С
	//gh.DrawImage(img, rect);
}
//����������¼�
void GameMenuPannel::OnMouseMove(UINT nFlags, CPoint point) {
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	//���Ӳ˵�
	for (auto &menuItem : m_menuItems) {
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt)) {//�ж��Ƿ������ǰ���λ��
			menuItem.index = 1;
		}
		else {
			menuItem.index = 0;//��������ʾ��0��ͼƬ
		}
	}
}

//����������̧���¼�
void GameMenuPannel::OnLButtonUp(UINT flags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	{
		auto &menuItem = m_menuItems[0];
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt)) {
			//�˻���ս����ʱ��������
			return;
		}
	}
	{
		auto &menuItem = m_menuItems[1];
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);
		if (rect.Contains(pt)) {
			//˫�˴�ս����ʱ��������
			return;
		}
	}
}
