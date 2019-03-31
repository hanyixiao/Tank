#pragma once
#include "stdafx.h"
#include <gdiplus.h>
#include <math.h>
#include <cmath>
using namespace Gdiplus;
class CGameEntry
{
public:
	CGameEntry()
	{

	}
	virtual ~CGameEntry()
	{

	}
public:
	virtual void Draw(Gdiplus::Graphics &gh) const = 0;//用于绘制自己的纯虚函数

};
//可移动的物件
class CGameEntryMoveable :public CGameEntry
{
public:
	CGameEntryMoveable()
	{

	}
	virtual ~CGameEntryMoveable()
	{

	}
	//复制构造方法
	CGameEntryMoveable(const CGameEntryMoveable &rhs)
	{
		if (this != &rhs)
		{
			this->m_bActive = rhs.m_bActive;// 是否有效
			this->m_rect = rhs.m_rect;//占用的区域
			this->m_fSpeed = rhs.m_fSpeed;
			this->m_direction = rhs.m_direction;
			this->m_directionTurn = rhs.m_directionTurn;//每次旋转角度

		}
	}
	//重载赋值运算
	CGameEntryMoveable &operator=(const CGameEntryMoveable &rhs)
	{
		if (this != &rhs) {
			this->m_bActive = rhs.m_bActive;// 是否有效
			this->m_rect = rhs.m_rect;//占用的区域
			this->m_fSpeed = rhs.m_fSpeed;
			this->m_direction = rhs.m_direction;
			this->m_directionTurn = rhs.m_directionTurn;//每次旋转角度
		}
		return *this;
	}
public:
	virtual void Draw(Gdiplus::Graphics &gh) const = 0;
	/**********可移动物体的共同属性***********/
	//右转
	virtual void RotateRight()
	{
		m_direction += m_directionTurn;
		if (m_direction >= 360)//满一周后减去360度
		{
			m_direction -= 360;
		}
	}
	virtual void RotateLeft()
	{
		m_direction -= m_directionTurn;
		if (m_direction <= 0) {
			m_direction = static_cast<float>(360 - m_directionTurn);
		}
	}
	//前移
	virtual void Forward()
	{
		//根据当前角度，在xy方向上分别加减
		m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed)* sin(GetDirectionArc()));
		m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual void Backward()
	{
		m_rect.X-= static_cast<Gdiplus::REAL>((m_fSpeed)* sin(GetDirectionArc()));
		m_rect.Y+= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual float GetXSpeed()const
	{
		return static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	}
	//下一步前进的位置
	virtual RectF ForwardNextRect()const
	{
		RectF rc = m_rect;
		rc.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
		return rc;
	}
	virtual RectF BackwardNextRect()const
	{
		RectF rc = m_rect;
		rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
		return rc;

	}
	//获取头部位置：图片为矩形：头部位置是角度是0时
	//距离中心点为半径（对角线/2），重置自己在X轴上的位置
	virtual PointF GetHeadPos()const
	{
		PointF ptCenter = GetCenterPoint();
		PointF ptHead = ptCenter;
		float fRadius = std::sqrt(std::pow(m_rect.Width / 2, 2) +
			std::pow(m_rect.Height / 2, 2));//计算半径
		ptHead.X += fRadius * sin(GetDirectionArc());//计算头部X坐标
		ptHead.Y -= fRadius * cos(GetDirectionArc());//计算头部Y坐标
		return ptHead;
	}
	//获取中心点坐标
	virtual PointF GetCenterPoint()const
	{
		PointF center = PointF(m_rect.X + m_rect.Width / 2,
			m_rect.Y + m_rect.Height / 2);
		return center;

	}
	//设置中心点坐标
	virtual void SetCenterPoint(const PointF &ptCenter)
	{
		PointF cen = GetCenterPoint();
		cen.X = ptCenter.X - cen.X;
		cen.Y = ptCenter.Y - cen.Y;
		m_rect.Offset(cen);

	}
#ifndef PI 
#define PI (3.1415926f)
#endif
	//设置角度单位是a*pi
	virtual void SetDirectionArc(float dir)
	{
		m_direction = dir * 180.0f/PI;
	}
	//设置角度（单位是度）
	virtual void SetDirection(float dir)
	{
		m_direction = dir;
	}
	//获取当前角度
	virtual float GetDirectionArc()const
	{
		return PI * m_direction / 180.0f;
	}
	virtual float GetDirection()const
	{
		return m_direction;
	}
	virtual void SetDirectionTurnArc(float dir)
	{
		m_directionTurn = PI * dir / 180.0f;
	}
	virtual void SetDirectionTurn(float dir)
	{
		m_direction = dir;
	}
	//获得当前的角度（单位是度）
	virtual float GetDirectionTurnArc()const
	{
		return PI * m_directionTurn / 180.0f;
	}
	//获得当前的弧度
	virtual float GetDirectionTurn()const
	{
		return m_direction;
	}
	virtual bool IsActive()const
	{
		return m_bActive;
	}
	//是否有效
	virtual void SetActive(bool bActive)
	{
		m_bActive = bActive;
	}
	//占用范围
	virtual void SetRect(const RectF rect)
	{
		m_rect = rect;
	}
	virtual RectF GetRect()const
	{
		return m_rect;
	}
	virtual void SetSpeed(float speed)
	{
		m_fSpeed = speed;
	}
	virtual float GetSpeed() const
	{
		return m_fSpeed;
	}
private:
	bool m_bActive{ false };//是否有效
	RectF m_rect{ 0,0,100,100 };//占用的区域
	float m_fSpeed{ 10 };        //移动的速度
	float m_direction{ 0 };     //当前的角度
	float m_directionTurn{ 5 }; //每次旋转的角度
};

