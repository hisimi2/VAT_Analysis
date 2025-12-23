#pragma once
#include "OneAxis.h"
#include "DEF_MOTION.h"
#include "BbxmnpClient.h"
#include "Logger.h"
#include "TimeCheck.h"
#include "ConfigData.h"
#include "DEF_ERROR_CODE.h"

#define		HOME_ESCAPE_PULSE		(100.0)		// pulse
#define		MOVE_CHECK_TIME			(15.0)		//second

class OneAxis_Z_PickerMotor : public OneAxis
{
public:
	OneAxis_Z_PickerMotor();
	~OneAxis_Z_PickerMotor();

	CLogger m_MtLog;
	CLogger m_EncoderLog;
	CLogger m_TestYEncoder;
	//BOOL m_bOriginFlag;
	std::vector<int> m_vOutPicker;
	void	bbwmnpAxisInfo(int nSubZAxisNum, double scale, int Zr_direction = 1, int pulse_dir = 1, int move_dir = 1);
	BOOL	GetAmpFaultStat();
	BOOL    Origin();
	BOOL	OriginWhile(int mode = 1, BOOL MsgDisplay = FALSE);
	bool	IsHomeSensorOn();
	BOOL	GetServoStat();
	bool	IsHomeSensorOn(_eBbxmnpAxis eAxis);
	BOOL	GetServoState(_eBbxmnpAxis eAxis);
	int		ServoOn();
	int		ServoOff();
	int		V_MoveStop();
	int     V_MoveStop(_eBbxmnpAxis eAxis);
	int		SetSlowStop();
	int		SetSlowStopEach(_eBbxmnpAxis eAxis);
	int		AlarmReset();
	int		MoveWhile(double pos, double speed = 0, int accel = 0, int dccel = 0, BOOL No_Check_safety = FALSE);
	int		R_MoveWhile(double pos, double speed = 0, int accel = 0);
	BOOL	GetOriginFlag();
	void	SetOriginFlag(bool bFlag);
	BOOL	IsOrgCompleted(ERR_CODE *pError = 0);
	void ClearOrgFlag();

	double	GetCommand(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);
	int		V_Move(_eJogDir eJogDir, int nAxisCnt, int* anAxes, double speed, int accel = 0);
	int		V_Move(_eJogDir eJogDir, _eBbxmnpAxis eAxis, double speed, int accel = 0);
	int		Move(std::vector<CPoint> vPicker, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, int nBoardType = 0, BOOL bNoUseOffset = FALSE);
	int		Move(_eBbxmnpAxis eAxis, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, int nBoardType = 0, BOOL bNoUseOffset = FALSE);
	int		MoveWhile(std::vector<CPoint> vPicker,double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL bNoUseOffset = FALSE);
	int		MoveWhile(_eBbxmnpAxis eAxis, double pos, double speed = 0, int accel = 0, int dccel = 0,BOOL No_Check_safety = FALSE);
	int		R_MoveWhile(std::vector<CPoint> vPicker, double pos, double speed = 0, int accel = 0);
	void	SetInitMotorParam(double r_vel, int r_acc, int r_dcc, double o_vel, int o_acc, double pos, double neg, double scalefactor);
	int    GetCurrentPos(double &dPos);
	int	   GetCurrenPosAvg(double &dPos);
	double	GetCurrentPos(double &dPos, _eBbxmnpAxis eAxis);
	double GetActual(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);
	int		MotionDone();
	double GetAllLastPos(int nPickerNo);

	double LimitValueCheck(BOOL bPosi);

	int	R_MoveWhileForChkOrign(std::vector<CPoint> vPicker, double pos, double speed/*=0*/, int accel/*=0*/);
	int	R_MoveWhileForChkOrign(double pos, double speed/*=0*/, int accel/*=0*/);
	void SetPickerLastPosition(int nPickerNo, double dLastPos);
	double GetPickerLastPosition(int nPickerNo);
	int	AxisDone();
	int AxisDone(std::vector<CPoint>& vPicker, std::vector<CPoint>& vErrPicker);
	int ServoOn(_eBbxmnpAxis eAxis);
	int ServoOff(_eBbxmnpAxis eAxis);
	int GetPickerStartAndEndNumber();

	double GetSpeed();
	int		GetAcc();
	int		GetDcc();

private:
//	void	SetHome(int active);

protected:
	CTimerCheck		m_tm;


};

