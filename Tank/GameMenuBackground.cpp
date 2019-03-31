#include "GameMenuBackground.h"
#include "stdafx.h"
#include "CGame.h"


CGameMenuBackground::CGameMenuBackground()
{
	m_imagebackground = Image::FromFile(_T("menu_background.png"));
}


CGameMenuBackground::~CGameMenuBackground()
{
}
void CGameMenuBackground::Draw(Graphics &gh) const
{
	gh.DrawImage(m_imagebackground,m_rect);
}