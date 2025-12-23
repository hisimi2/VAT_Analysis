#include "StdAfx.h"
#include "LastRobotStatus.h"


CLastRobotStatus::CLastRobotStatus(void)
{
}

CLastRobotStatus::CLastRobotStatus(const CLastRobotStatus& src)
{
	m_vClamp = src.m_vClamp;
	m_vMotorPos = src.m_vMotorPos;
}

CLastRobotStatus::~CLastRobotStatus(void)
{
}

CLastRobotStatus& CLastRobotStatus::operator = (const CLastRobotStatus& src)
{
	if( this == &src )
	{
		return *this;
	}
	m_vClamp = src.m_vClamp;
	m_vMotorPos = src.m_vMotorPos;
	return *this;
}
