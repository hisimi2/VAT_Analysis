#pragma once

class CLastRobotStatus
{
public:
	CLastRobotStatus(void);
	CLastRobotStatus(const CLastRobotStatus& src);
	~CLastRobotStatus(void);

	CLastRobotStatus& operator = (const CLastRobotStatus& src);

	std::vector<int> m_vClamp;
	std::vector<double> m_vMotorPos;
};

