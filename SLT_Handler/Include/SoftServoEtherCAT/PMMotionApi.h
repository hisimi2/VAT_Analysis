/**********************************************************************************************************************
*
* PMMotionApi.h
*
* This file contains the declarations of the PMMotion module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the PMMotion module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_PM_MOTION_API_H
#define WMX3_PM_MOTION_API_H
#include <windows.h>
#include "WMX3Api.h"

namespace wmx3Api{

	class PMMotionErrorCode : public ErrorCode{
	public:
		enum {
			PMErrorCode = 0x00019000,
			AxisIsOffline,
			AxisIsNotServoOn,
			AxisIsInUse,
			PPModeNotSupported,
			PVModeNotSupported,
			TQModeNotSupported,
			HMModeNotSupported,
			InvalidProfileSetting,
		};

		static int ConvertPMMotionErrorCode(int errCode);
	};

	namespace constants {
		static const int maxPMAxes = 256;
	};

	class PMMotionOperationMode{
	public:
		enum T {
			PP,
			HM,
			PV,
			TQ,
			Unknown
		};
	};

	class PMMotion;

	class PMAxisControl {
	public:
		PMMotion *pmApi;
		PMAxisControl(PMMotion *f) : pmApi(f) {}

        bool IsDeviceValid();

		class HomeProfile {
		public:
			HomeProfile();
			HomeProfile(int axis, unsigned char method, long speedForSwitch, long speedForZero, long acc, long homeOffset = 0);
			
			int axis;
			unsigned char method;
			long speedForSwitch;
			long speedForZero;
			long acc;
			long homeOffset;
		};

		class VelocityProfile {
		public:
			VelocityProfile();
			VelocityProfile(int axis, long vel, long acc, long maxTrqLimit = 0);
			VelocityProfile(int axis, long vel, long acc, long dec, long maxTrqLimit = 0);

			int axis;
			long vel;
			long acc;
			long dec;
			long maxTrqLimit;
		};

		class TorqueProfile {
		public:
			TorqueProfile();
			TorqueProfile(int axis, long torque, long maxMotorSpeed = 0);

			int axis;
			long torque;
			long maxMotorSpeed;
		};
		
		class HomeProfiles {
		public:
			HomeProfiles();

			int axisCount;
			HomeProfile profiles[constants::maxPMAxes];
		};

		class VelocityProfiles {
		public:
			VelocityProfiles();

			int axisCount;
			VelocityProfile profiles[constants::maxPMAxes];
		};

		class TorqueProfiles {
		public:
			TorqueProfiles();

			int axisCount;
			TorqueProfile profiles[constants::maxPMAxes];
		};

		WMX3APIFUNC SetServoOn(int axis, int newStatus);
		WMX3APIFUNC SetServoOn(AxisSelection* axisSel, int newStatus);
		WMX3APIFUNC ClearAmpAlarm(int axis);
		WMX3APIFUNC ClearAmpAlarm(AxisSelection* axisSel);

		WMX3APIFUNC SetHomeProfile(HomeProfile *pHomeProfile);
		WMX3APIFUNC SetHomeProfiles(HomeProfiles *pHomeProfiles);
		WMX3APIFUNC SetVelProfile(VelocityProfile *pVelProfile);
		WMX3APIFUNC SetVelProfiles(VelocityProfiles *pVelProfiles);
		WMX3APIFUNC SetTrqProfile(TorqueProfile *pTrqProfile);
		WMX3APIFUNC SetTrqProfiles(TorqueProfiles *pTrqProfiles);

		WMX3APIFUNC GetPosCommand(int axis, long *pPosition);
		WMX3APIFUNC GetPosFeedback(int axis, long *pPosition);
		WMX3APIFUNC GetVelCommand(int axis, long *pVelocity);
		WMX3APIFUNC GetVelFeedback(int axis, long *pVelocity);

	};

	class PMAxisMotion{
	public:
		PMMotion *pmApi;
		PMAxisMotion(PMMotion *f) : pmApi(f){}

        bool IsDeviceValid();

		class PosCommand {
		public:
			PosCommand();
			PosCommand(int axis, long target, long vel, long acc, long dec);

			int axis;
			long target;
			long vel;
			long acc;
			long dec;
		};

		WMX3APIFUNC StartPos(int axis, long target, bool immediately = true);
		WMX3APIFUNC StartMov(int axis, long target, bool immediately = true);
		WMX3APIFUNC StartPos(int axis, long target, long maxTrqLimit, bool immediately = true);
		WMX3APIFUNC StartMov(int axis, long target, long maxTrqLimit, bool immediately = true);
		WMX3APIFUNC StartPos(PosCommand *pPosCommand, bool immediately = true);
		WMX3APIFUNC StartMov(PosCommand *pPosCommand, bool immediately = true);
		WMX3APIFUNC StartPos(PosCommand *pPosCommand, long maxTrqLimit, bool immediately = true);
		WMX3APIFUNC StartMov(PosCommand *pPosCommand, long maxTrqLimit, bool immediately = true);

		WMX3APIFUNC StartPos(unsigned int numCommands, PosCommand *pPosCommand, bool immediately = true);
		WMX3APIFUNC StartMov(unsigned int numCommands, PosCommand *pPosCommand, bool immediately = true);
		WMX3APIFUNC StartPos(unsigned int numCommands, PosCommand *pPosCommand, long *maxTrqLimits, bool immediately = true);
		WMX3APIFUNC StartMov(unsigned int numCommands, PosCommand *pPosCommand, long *maxTrqLimits, bool immediately = true);

		WMX3APIFUNC Stop(int axis);
		WMX3APIFUNC Stop(AxisSelection *pAxisSelection);
		WMX3APIFUNC Pause(int axis);
		WMX3APIFUNC Pause(AxisSelection *pAxisSelection);
		WMX3APIFUNC Resume(int axis);
		WMX3APIFUNC Resume(AxisSelection *pAxisSelection);
	};

	class PMAxisHome {
	public:
		PMMotion *pmApi;
		PMAxisHome(PMMotion *f) : pmApi(f) {}

        bool IsDeviceValid();

		class HomeCommand{
		public:
			HomeCommand();
			HomeCommand(int axis, unsigned char method, long speedForSwitch, long speedForZero, long acc, long homeOffset = 0);

			unsigned char method;
			int axis;
			long speedForSwitch;
			long speedForZero;
			long acc;
			long homeOffset;
		};

		WMX3APIFUNC StartHome(int axis);
		WMX3APIFUNC StartHome(HomeCommand *pHomeCommand);
		WMX3APIFUNC StartHome(unsigned int numCommands, HomeCommand *pHomeCommand);
		WMX3APIFUNC Continue(int axis);
		WMX3APIFUNC Continue(AxisSelection *pAxisSelection);
		WMX3APIFUNC Cancel(int axis);
		WMX3APIFUNC Cancel(AxisSelection *pAxisSelection);
	};

	class PMAxisVelocity {
	public:
		PMMotion *pmApi;
		PMAxisVelocity(PMMotion *f) : pmApi(f) {}

        bool IsDeviceValid();

		class VelCommand {
		public:
			VelCommand();
			VelCommand(int axis, long vel, long acc, long dec = 0);

			int axis;
			long vel;
			long acc;
			long dec;
		};

		WMX3APIFUNC StartVel(int axis);
		WMX3APIFUNC StartVel(int axis, long maxTrqLimit);
		WMX3APIFUNC StartVel(VelCommand *pVelCommand);
		WMX3APIFUNC StartVel(VelCommand *pVelCommand, long maxTrqLimit);

		WMX3APIFUNC StartVel(unsigned int numCommands, VelCommand *pVelCommand);
		WMX3APIFUNC StartVel(unsigned int numCommands, VelCommand *pVelCommand, long *maxTrqLimits);

		WMX3APIFUNC Stop(int axis);
		WMX3APIFUNC Stop(AxisSelection *pAxisSelection);
	};

	class PMAxisTorque { 
	public:
		PMMotion *pmApi;
		PMAxisTorque(PMMotion *f) : pmApi(f) {}

        bool IsDeviceValid();

		class TrqCommand {
		public:
			TrqCommand();
			TrqCommand(int axis, long torque);

			int axis;
			long torque;
		};

		WMX3APIFUNC StartTrq(int axis);
		WMX3APIFUNC StartTrq(int axis, long maxMotorSpeed);
		WMX3APIFUNC StartTrq(TrqCommand *pTrqCommand);
		WMX3APIFUNC StartTrq(TrqCommand *pTrqCommand, long maxMotorSpeed);
		WMX3APIFUNC StartTrq(unsigned int numCommands, TrqCommand *pTrqCommand);
		WMX3APIFUNC StartTrq(unsigned int numCommands, TrqCommand *pTrqCommand, long *maxMotorSpeeds);

		WMX3APIFUNC StopTrq(int axis);
		WMX3APIFUNC StopTrq(AxisSelection *pAxisSelection);
	};


	class PMMotionAxisStatus{
	public:
		PMMotionAxisStatus();
		bool servoOn;
		bool servoOffline;
		bool ampAlarm;
		int ampAlarmCode;
		PMMotionOperationMode::T operationMode;
		long posCmd;
		long actualPos;
		long velocityCmd;
		long actualVelocity;
		long torqueCmd;
		long actualTorque;
		bool followingErrorAlarm;
		bool inPos;
		bool positiveLS;
		bool negativeLS;
		bool homeError;
		bool homeSwitch;
		bool homeDone;
		bool halt;
	};

	class PMMotionStatus{
	public:
		PMMotionStatus();
		bool invalidLicenseError;
		EngineState::T engineState;
		int numOfInterrupts;
		double cycleTimeMilliseconds[constants::maxInterrupts];
		long long cycleCounter[constants::maxInterrupts];
		PMMotionAxisStatus axesStatus[constants::maxPMAxes];
	};

	class PMMotion {
		friend class PMAxisControl;
		friend class PMAxisMotion;
		friend class PMAxisHome;
		friend class PMAxisVelocity;
		friend class PMAxisTorque;

	private:
		WMX3Api *wmx3Api; 
		int statChnlId;
		bool isSelfDev;
		void init(WMX3Api *f);
		void close();

		PMAxisControl::HomeProfiles* homeProfiles;
		PMAxisControl::VelocityProfiles* velProfiles;
		PMAxisControl::TorqueProfiles* trqProfiles;

	public:
		PMMotion(WMX3Api *f);
		PMMotion(const PMMotion& src);
		PMMotion& operator=(const PMMotion& src);
		PMMotion();
		~PMMotion();

		static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
		static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

		bool IsDeviceValid();

		WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

		WMX3APIFUNC GetStatus(PMMotionStatus* status);

		PMAxisControl *axisControl;
		PMAxisMotion *motion;
		PMAxisHome *home;
		PMAxisVelocity *velocity;
		PMAxisTorque *torque;
	};
}

#endif