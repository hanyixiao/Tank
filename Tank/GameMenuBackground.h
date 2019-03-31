#pragma once
#include "GameEntry.h"
class CGame;
class CGameMenuBackground :CGameEntryMoveable
{
public:
	CGameMenuBackground();
	~CGameMenuBackground();
	void SetParent(CGame *g)
	{
		m_parent = g;
	};
	CGame *m_parent = { nullptr };
	//»­
	virtual void Draw(Graphics &gh) const;
	//×Ô¼ºµÄ·¶Î§
	RectF m_rect{ 0,0,800,600 };
	//
	Image *m_imagebackground;
};

