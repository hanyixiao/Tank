#include "CGameBackground.h"
#include "stdafx.h"
#include "CGame.h"

CGameBackground::CGameBackground()
{
	m_imgBackground = Image::FromFile(_T("menu_background.png"));//‘ÿ»Î±≥æ∞Õº
}


CGameBackground::~CGameBackground()
{
}

void CGameBackground::Draw(Graphics &gh) const
{
	gh.DrawImage(m_imgBackground, m_rect);
}