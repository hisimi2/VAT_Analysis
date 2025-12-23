/**********************************************************************************************************************
*
* CoreMotionApi.h
*
* This file contains the declarations of the CoreMotion module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the CoreMotion module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_CORE_MOTION_API_H
#define WMX3_CORE_MOTION_API_H
#include <windows.h>
#include "WMX3Api.h"

namespace wmx3Api{

    namespace constants {
        static const int maxFlightRecorderBufferSize = 5000;
        static const int maxInPosChannel = 5;
        static const int maxSyncGroup = 64;
        static const int maxTriggerEvents = 8;
        static const unsigned long long int maxProfileUnsignedInput = 274877906943ULL;
    }

    class CoreMotionErrorCode : public ErrorCode{
    public:
        enum {
            ParameterSettingsInvalid = 0x00010000,
            ServoOnError,
            SyncAxisSingleTurnMismatch,
            PositionNotPassed,
            NotPausedDuringHoming,
            OpenWaitEventError,
            ParameterFileParseError,
            SyncGroupIDOutOfRange,
            SyncGroupMasterAxisUndefined,
            ReferenceAxisOfActiveCompensation,
            CurrentEmergencyStopLevelIsHigher,
            AcyclicStatusError,
            AxisExecutingPhaseShift,
            AxisSettingSyncGearRatio,
            NotSynchronizedToSpecifiedMasterAxis,
            OpenWaitSharedMemoryError,
            WaitEngineError,
            IsSyncSlave,
            IsSyncMaster,
            IsCombineSyncMaster,
            NotServoOn,
            NotIdleOrPosState,
            NotPosState,
            NotIdleOrJogState,
            NotIdleOrVelocityState,
            NotIdleOrIntplState,
            NotIdleOrPosOrJogState,
            NotPosOrJogOrVelocityState,
            NotJogState,
            NotTorqueState,
            NotIdleOrTorqueState,
            NotIdleOrSyncState,
            NotECAMState,
            NotDancerControlState,
            NotIdleOrDancerControlState,
            NotPosMode,
            NotVelocityMode,
            NotTrqMode,
            NotPosOrVelocityMode,
            NotSyncSlave,
            NotHomeState,
            AxisNotPaused,
            AxisArgumentOutOfRange,
            ProfileArgumentOutOfRange,
            PositionArgumentOutOfRange,
            VelocityArgumentOutOfRange,
            AccArgumentOutOfRange,
            DecArgumentOutOfRange,
            JerkAccArgumentOutOfRange,
            JerkDecArgumentOutOfRange,
            JerkAccRatioArgumentOutOfRange,
            JerkDecRatioArgumentOutOfRange,
            StartingVelocityArgumentOutOfRange,
            EndVelocityArgumentOutOfRange,
            SecondVelocityArgumentOutOfRange,
            RunTimeArgumentOutOfRange,
            TimeArgumentOutOfRange,
            TriggerTypeArgumentOutOfRange,
            TriggerAxisArgumentOutOfRange,
            TriggerValueArgumentOutOfRange,
            TriggerTypeNotSupported,
            AccDecCouldNotBeCalculated,
            NoFreeVirtualAxis,
            NoFreeVirtualAxisAddlData,
            AxisNotSingleTurn,
            AxisIsSingleTurn,
            ProfileErrorOnTargetPos,
            ProfileUsageError,
            ProfileAccelerationIsInvalid,
            ProfileDecelerationIsInvalid,
            ProfileJerkIsInvalid,
            ProfileVelocityIsInvalid,
            ProfileNullCalculation,
            ProfileNoMotion,
            ProfileUserRangeIsInvalid,
            OtherProfileError,
            ProfileUnknownError,
            ProfileSetupFailed,
            ProfileCancelOvertravel,
            PointCountOutOfRange,
            WaitTimeout,
            ProfileTypeNotSupported,
            AxisMismatch,
            NoRotationalMotion,
            DeviceIDError,
            InvalidAxis,
            ChannelExecutingCommands,
            ChannelIsNotExecutingCommands,
            ChannelNotEnoughSpace,
            ChannelIsStopping,
            ChannelIsNotStopped,
            ChannelBufferMemoryNotAllocated,
            ChannelBufferMemoryAlreadyAllocated,
            ChannelBufferMemoryFailedToAllocate,
            ProcessingOverride,
            CircularInterpolationArgumentOutOfRange,
            EmergencyStopSignalOn,
            SyncGroupDoesNotContainAxis,
            SyncGroupParameterOutOfRange,
            CurrentAxisCommandModeNotSupported,
            MasterAxisOutOfRange,
            SlaveAxisOutOfRange,
            SlaveAxisCountOutOfRange,
            EnabledArgumentOutOfRange,
            MovingAverageTimeArgumentOutOfRange,
            NumEventsOutOfRange,
            RegisterNumberOutOfRange,
            SyncGearRatioOutOfRange,
            PosChangePerCycleOutOfRange,
            MaxTorqueLimitOutOfRange,
            TorqueArgumentOutOfRange,
            MaxMotorSpeedOutOfRange,
            RampCycleTimeOutOfRange,
            RampRateOutOfRange,
            CircularInterpolationTypeOutOfRange,
            CircularInterpolationCenterPosTooCloseToInitialPos,
            CircularInterpolationEndPosTooCloseToInitialPos,
            CircularInterpolationThroughPosTooCloseToInitialPos,
            CircularInterpolationThroughPosTooCloseToEndPos,
            CircularInterpolationPathGenerationError,
            HelicalInterpolationTypeOutOfRange,
            HelicalInterpolationCenterPosTooCloseToInitialPos,
            HelicalInterpolationPathGenerationError,
            AxisCommandModeOutOfRange,
            ZPulseDetectionUnsupported,
            TouchProbeDetectionUnsupported,
            HomeSwitchDetectionUnsupported,
            LimitSwitchDetectionUnsupported
        };
    };

    class CoreMotionAxisLogInput {
    public:
        CoreMotionAxisLogInput();
        unsigned char servoOn;
        unsigned char servoOffline;
        unsigned char commandPos;
        unsigned char feedbackPos;
        unsigned char compCommandPos;
        unsigned char compFeedbackPos;
        unsigned char encoderCommandPos;
        unsigned char encoderFeedbackPos;
        unsigned char commandVelocity;
        unsigned char feedbackVelocity;
        unsigned char encoderCommandVelocity;
        unsigned char encoderFeedbackVelocity;
        unsigned char commandTrq;
        unsigned char feedbackTrq;
        unsigned char encoderCommandTrq;
        unsigned char encoderFeedbackTrq;
        unsigned char followingError;
        unsigned char homeOffset;
        unsigned char homeSwitch;
        unsigned char homeState;
        unsigned char inPosFlag;
        unsigned char inPosFlag2;
        unsigned char inPosFlag3;
        unsigned char inPosFlag4;
        unsigned char inPosFlag5;
        unsigned char commandDistributionEndFlag;
        unsigned char posSetFlag;
        unsigned char delayedPosSetFlag;
        unsigned char positiveLS;
        unsigned char negativeLS;
        unsigned char opState;
        unsigned char detailOpState;
        unsigned char userOffsetCommandPos;
        unsigned char userOffsetFeedbackPos;
        unsigned char axisCommandMode;
        unsigned char axisCommandModeFeedback;
    };

    class CoreMotionLogInput : public LogInput {
    public:
        CoreMotionLogInput();
        unsigned int GetModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, unsigned int* dataSize);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, unsigned int dataSize);

        AxisSelection axisSelection;
        CoreMotionAxisLogInput axisOptions;
    };

    class CoreMotionAxisLogOutput {
    public:
        CoreMotionAxisLogOutput();
        bool servoOn;
        bool servoOffline;
        double commandPos;
        double feedbackPos;
        double compCommandPos;
        double compFeedbackPos;
        int encoderCommandPos;
        int encoderFeedbackPos;
        double commandVelocity;
        double feedbackVelocity;
        int encoderCommandVelocity;
        int encoderFeedbackVelocity;
        double commandTrq;
        double feedbackTrq;
        int encoderCommandTrq;
        int encoderFeedbackTrq;
        double followingError;
        double homeOffset;
        bool homeSwitch;
        int homeState;
        bool inPosFlag;
        bool inPosFlag2;
        bool inPosFlag3;
        bool inPosFlag4;
        bool inPosFlag5;
        bool commandDistributionEndFlag;
        bool posSetFlag;
        bool delayedPosSetFlag;
        bool positiveLS;
        bool negativeLS;
        int opState;
        int detailOpState;
        double userOffsetCommandPos;
        double userOffsetFeedbackPos;
        int axisCommandMode;
        int axisCommandModeFeedback;
    };

    class CoreMotionLogOutput : public LogOutput {
    public:
        CoreMotionLogOutput();
        unsigned int GetModuleId();
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* cfgData, unsigned int cfgDataSize, unsigned char* data, unsigned int dataSize, unsigned int dataIndex, unsigned int storeIndex);

        CoreMotionAxisLogOutput axisData[constants::maxLogOutputDataSize][constants::maxAxes];
    };

    class CoreMotionEventInput : public EventInput {
    public:
        enum CoreMotionEventInputType {
            EqualPos,
            GreaterPos,
            LessPos,
            EqualVelocity,
            GreaterVelocity,
            LessVelocity,
            EqualTrq,
            GreaterTrq,
            LessTrq,

            OpState,
            AxisCmdMode,
            InPos,
            PosSET,
            DelayedPosSET,
            CommandDistributedEnd,
            RemainingTime,
            RemainingDistance,
            CompletedTime,
            CompletedDistance,
            DistanceToTarget
        };
        CoreMotionEventInputType inputFunction;
        union InputFunctionArguments {
            InputFunctionArguments();

            struct EqualPos {
                unsigned int axis;
                double pos;
                unsigned char disablePositiveDirection;
                unsigned char disableNegativeDirection;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableTolerance;
                double tolerance;
            }equalPos;

            struct GreaterPos {
                unsigned int axis;
                double pos;
                unsigned char disablePositiveDirection;
                unsigned char disableNegativeDirection;
                unsigned char useFeedback;
                unsigned char invert;
            }greaterPos;

            struct LessPos {
                unsigned int axis;
                double pos;
                unsigned char disablePositiveDirection;
                unsigned char disableNegativeDirection;
                unsigned char useFeedback;
                unsigned char invert;
            }lessPos;

            struct EqualVelocity {
                unsigned int axis;
                double velocity;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableTolerance;
                double tolerance;
                unsigned char enableUnsigned;
            }equalVelocity;

            struct GreaterVelocity {
                unsigned int axis;
                double velocity;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableUnsigned;
            }greaterVelocity;

            struct LessVelocity {
                unsigned int axis;
                double velocity;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableUnsigned;
            }lessVelocity;

            struct EqualTrq {
                unsigned int axis;
                double trq;
                unsigned char invert;
                unsigned char enableTolerance;
                double tolerance;
                unsigned char enableUnsigned;
            }equalTrq;

            struct GreaterTrq {
                unsigned int axis;
                double trq;
                unsigned char invert;
                unsigned char enableUnsigned;
            }greaterTrq;

            struct LessTrq {
                unsigned int axis;
                double trq;
                unsigned char invert;
                unsigned char enableUnsigned;
            }lessTrq;

            struct OpState {
                unsigned int axis;
                OperationState::T opState;
                unsigned char invert;
            }opState;

            struct AxisCmdMode {
                unsigned int axis;
                AxisCommandMode::T axisCommandMode;
                unsigned char useFeedback;
                unsigned char invert;
            }axisCmdMode;

            struct InPos {
                unsigned int axis;
                unsigned char invert;
                unsigned int channel;
            }inPos;

            struct PosSET {
                unsigned int axis;
                unsigned char invert;
            }posSET;

            struct DelayedPosSET {
                unsigned int axis;
                unsigned char invert;
            }delayedPosSET;

            struct CommandDistributedEnd {
                unsigned int axis;
                unsigned char invert;
            }commandDistributedEnd;

            struct RemainingTime {
                unsigned int axis;
                unsigned char invert;
                double timeMilliseconds;
                unsigned char disableIdleAxisTrigger;
            }remainingTime;

            struct RemainingDistance {
                unsigned int axis;
                unsigned char invert;
                double distance;
                unsigned char disableIdleAxisTrigger;
            }remainingDistance;

            struct CompletedTime {
                unsigned int axis;
                unsigned char invert;
                double timeMilliseconds;
                unsigned char disableIdleAxisTrigger;
            }completedTime;

            struct CompletedDistance {
                unsigned int axis;
                unsigned char invert;
                double distance;
                unsigned char disableIdleAxisTrigger;
            }completedDistance;

            struct DistanceToTarget {
                unsigned int axis;
                unsigned char invert;
                double distance;
                unsigned char disableIdleAxisTrigger;
            }distanceToTarget;
        }input;

        CoreMotionEventInput();

        int GetInputModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, int* dataSize, char *initialState);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, int dataSize);
    };

    class CoreMotionEventOutput : public EventOutput {
    public:
        enum CoreMotionEventOutputType {
            StopSingleAxis,
            StartSinglePos,
            StartSingleMov,
            StartMultiplePos,
            StartMultipleMov,
            LinearIntplPos,
            LinearIntplMov,
            ExecQuickStopSingleAxis,
            OverrideVelSingleAxis,
            ExecEStop,
            TriggerFlightRecorder,
            ResetFlightRecorder
        };

        CoreMotionEventOutputType outputFunction;
        union OutputFunctionArguments {
            OutputFunctionArguments();

            struct StopSingleAxis {
                int axis;
            }stopSingleAxis;

            struct StartSinglePos {
                int axis;
                double target;

                ProfileType::T type;
                double velocity;
                double acc;
                double dec;
                double jerkAcc;
                double jerkDec;
                double jerkAccRatio;
                double jerkDecRatio;
                double accTime;
                double decTime;
                double startingVelocity;
                double endVelocity;
                double secondVelocity;
            }startSinglePos;

            struct StartSingleMov {
                int axis;
                double target;

                ProfileType::T type;
                double velocity;
                double acc;
                double dec;
                double jerkAcc;
                double jerkDec;
                double jerkAccRatio;
                double jerkDecRatio;
                double accTime;
                double decTime;
                double startingVelocity;
                double endVelocity;
                double secondVelocity;
            }startSingleMov;

            struct StartMultiplePos {
                unsigned int axisCount;
                int axis[constants::maxEventInterpolationAxes];
                double target[constants::maxEventInterpolationAxes];

                ProfileType::T type[constants::maxEventInterpolationAxes];
                double velocity[constants::maxEventInterpolationAxes];
                double acc[constants::maxEventInterpolationAxes];
                double dec[constants::maxEventInterpolationAxes];
                double jerkAcc[constants::maxEventInterpolationAxes];
                double jerkDec[constants::maxEventInterpolationAxes];
                double jerkAccRatio[constants::maxEventInterpolationAxes];
                double jerkDecRatio[constants::maxEventInterpolationAxes];
                double accTime[constants::maxEventInterpolationAxes];
                double decTime[constants::maxEventInterpolationAxes];
                double startingVelocity[constants::maxEventInterpolationAxes];
                double endVelocity[constants::maxEventInterpolationAxes];
                double secondVelocity[constants::maxEventInterpolationAxes];
            }startMultiplePos;

            struct StartMultipleMov {
                unsigned int axisCount;
                int axis[constants::maxEventInterpolationAxes];
                double target[constants::maxEventInterpolationAxes];

                ProfileType::T type[constants::maxEventInterpolationAxes];
                double velocity[constants::maxEventInterpolationAxes];
                double acc[constants::maxEventInterpolationAxes];
                double dec[constants::maxEventInterpolationAxes];
                double jerkAcc[constants::maxEventInterpolationAxes];
                double jerkDec[constants::maxEventInterpolationAxes];
                double jerkAccRatio[constants::maxEventInterpolationAxes];
                double jerkDecRatio[constants::maxEventInterpolationAxes];
                double accTime[constants::maxEventInterpolationAxes];
                double decTime[constants::maxEventInterpolationAxes];
                double startingVelocity[constants::maxEventInterpolationAxes];
                double endVelocity[constants::maxEventInterpolationAxes];
                double secondVelocity[constants::maxEventInterpolationAxes];
            }startMultipleMov;

            struct LinearIntplPos {
                unsigned int axisCount;
                int axis[constants::maxEventInterpolationAxes];
                double target[constants::maxEventInterpolationAxes];
                double maxVelocity[constants::maxEventInterpolationAxes];
                double maxAcc[constants::maxEventInterpolationAxes];
                double maxDec[constants::maxEventInterpolationAxes];
                double maxJerkAcc[constants::maxEventInterpolationAxes];
                double maxJerkDec[constants::maxEventInterpolationAxes];

                ProfileType::T type;
                double velocity;
                double acc;
                double dec;
                double jerkAcc;
                double jerkDec;
                double jerkAccRatio;
                double jerkDecRatio;
                double accTime;
                double decTime;
                double startingVelocity;
                double endVelocity;
                double secondVelocity;
            }linearIntplPos;

            struct LinearIntplMov {
                unsigned int axisCount;
                int axis[constants::maxEventInterpolationAxes];
                double target[constants::maxEventInterpolationAxes];
                double maxVelocity[constants::maxEventInterpolationAxes];
                double maxAcc[constants::maxEventInterpolationAxes];
                double maxDec[constants::maxEventInterpolationAxes];
                double maxJerkAcc[constants::maxEventInterpolationAxes];
                double maxJerkDec[constants::maxEventInterpolationAxes];

                ProfileType::T type;
                double velocity;
                double acc;
                double dec;
                double jerkAcc;
                double jerkDec;
                double jerkAccRatio;
                double jerkDecRatio;
                double accTime;
                double decTime;
                double startingVelocity;
                double endVelocity;
                double secondVelocity;
            }linearIntplMov;

            struct ExecQuickStopSingleAxis {
                int axis;
            }execQuickStopSingleAxis;

            struct OverrideVelSingleAxis {
                int axis;
                double velocity;
            }overrideVelSingleAxis;

            struct ExecEStop {
                EStopLevel::T eStopLevel;
            }execEStop;

            struct TriggerFlightRecorder {
                unsigned int reserved;
            }triggerFlightRecorder;

            struct ResetFlightRecorder {
                unsigned int reserved;
            }resetFlightRecorder;
        }output;

        CoreMotionEventOutput();

        int GetOutputModuleId();
        WMX3APIFUNC GetOutputData(unsigned char* buff, int buffSize, int* dataSize);
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* data, int dataSize);
    };

    class HomeState {
    public:
        enum T {
            Idle,
            ZPulseSearch,
            ZPulseSearchReverse,
            ZPulseSearchPauseReverse,
            HSSearch,
            HSSearchPause,
            HSAndZPulseSearch,
            HSAndZPulseSearchPause,
            HSOffSearch,
            HSOffSearchPause,
            HSOffAndZPulseSearch,
            HSOffAndZPulseSearchPause,
            LSSearch,
            LSSearchPause,
            HSClearReverse,
            HSClearReversePause,
            HSFallingEdgeSearchReverse,
            HSFallingEdgeSearchReversePause,
            LSFallingEdgeSearchReverse,
            LSFallingEdgeSearchReversePause,
            TouchProbeSearch,
            TouchProbeSearchPause,
            SecondHSSearch,
            SecondHSSearchPause,
            SecondTouchProbeSearch,
            SecondTouchProbeSearchPause,
            MechanicalEndDetection,
            HomeShift,
            HomeShiftPause,
            Cancel
        };
    };

    class HomeError {
    public:
        enum T {
            NoError,
            LSTriggered,
            MaxLSRevDistanceTraveled,
            MaxHSOnAtStartRevDistanceTraveled,
            ZPulseDistanceCheckError
        };
    };

    class AxisSyncMode {
    public:
        enum T {
            NoSync,
            NoOffset,
            VelocityOffset,
            SymmetricVelocityOffset
        };
    };

    class AxisCompensation{
    public:
        AxisCompensation();

        double pitchErrorCompensation;
        double pitchErrorCompensation2D;
        double backlashCompensation;
        double totalPosCompensation;
    };

    class AxisSupportedFunction{
    public:
        AxisSupportedFunction();
        bool posFeedbackSupport;
        bool posCommandSupport;
        bool velocityFeedbackSupport;
        bool velocityCommandSupport;
        bool velocityOffsetSupport;
        bool trqFeedbackSupport;
        bool trqCommandSupport;
        bool maxTrqLimitSupport;
        bool positiveTrqLimitSupport;
        bool negativeTrqLimitSupport;
        bool maxMotorSpeedSupport;
    };

    class CoreMotionAxisStatus{
    public:
        CoreMotionAxisStatus();
        bool servoOn;
        bool servoOffline;
        bool ampAlarm;
        int ampAlarmCode;
        int masterAxis;
        int secondMasterAxis;
        double posCmd;
        double actualPos;
        double compPosCmd;
        double compActualPos;
        double syncPosCmd;
        double syncActualPos;
        int encoderCommand;
        int encoderFeedback;
        long long accumulatedEncoderFeedback;
        double velocityCmd;
        double actualVelocity;
        double velocityLag;
        double torqueCmd;
        double actualTorque;
        double actualFollowingError;
        AxisCompensation compensation;
        AxisSupportedFunction axisSupportedFunction;
        OperationState::T opState;
        DetailOperationState::T detailOpState;
        AxisCommandMode::T axisCommandMode;
        AxisCommandMode::T axisCommandModeFeedback;
        AxisSyncMode::T axisSyncMode;
        double syncOffset;
        double syncPhaseOffset;
        double syncGearRatio;
        double profileTotalMilliseconds;
        double profileAccMilliseconds;
        double profileCruiseMilliseconds;
        double profileDecMilliseconds;
        double profileRemainingMilliseconds;
        double profileCompletedMilliseconds;
        double profileTargetPos;
        double profileTotalDistance;
        double profileRemainingDistance;
        double profileCompletedDistance;
        double intplVelocity;
        int intplSegment;
        bool followingErrorAlarm;
        bool commandReady;
        bool waitingForTrigger;
        bool motionPaused;
        bool motionComplete;
        bool execSuperimposedMotion;
        double cmdAcc;
        bool accFlag;
        bool decFlag;
        bool inPos;
        bool inPos2;
        bool inPos3;
        bool inPos4;
        bool inPos5;
        bool cmdDistributionEnd;
        bool posSet;
        bool delayedPosSet;
        unsigned int cmdDistributionEndDelayedPosSetDiff;
        bool positiveLS;
        bool negativeLS;
        bool nearPositiveLS;
        bool nearNegativeLS;
        bool externalPositiveLS;
        bool externalNegativeLS;
        bool positiveSoftLimit;
        bool negativeSoftLimit;
        HomeState::T homeState;
        HomeError::T homeError;
        double homeOffset;
        bool homeSwitch;
        bool homeDone;
        bool homePaused;
        bool cmdPosToFbPosFlag;
        unsigned int singleTurnCounter;
        double userOffset;
        double userOffsetPosCmd;
        double userOffsetActualPos;
        double userVelocityOffset;
        double userTorqueOffset;
		double vibrationPosMin;
		double vibrationPosMax;
		double vibrationPosAvg;
		double vibrationVelMin;
		double vibrationVelMax;
		double vibrationVelAvg;
		double vibrationTrqMin;
		double vibrationTrqMax;
		double vibrationTrqAvg;
    };

    class CoreMotionStatus{
    public:
        CoreMotionStatus();
        bool invalidLicenseError;
        EngineState::T engineState;
        int numOfInterrupts;
        double cycleTimeMilliseconds[constants::maxInterrupts];
        long long cycleCounter[constants::maxInterrupts];
        bool emergencyStop;
        EStopLevel::T emergencyStopLevel;
        CoreMotionAxisStatus axesStatus[constants::maxAxes];
    };

    class TriggerType {
    public:
        enum T {
            RemainingTime,
            RemainingDistance,
            SameTimeCompletion,
            CompletedTime,
            CompletedDistance,
            StaggeredTimeCompletion,
            StaggeredDistanceCompletion,
            DistanceToTarget
        };
    };

    class TriggerEventInputFunction {
    public:
        enum T {
            IOBit,
            NotIOBit,
            OrIOBit,
            AndIOBit,
            XorIOBit,
            NandIOBit,
            NorIOBit,
            XnorIOBit,

            MBit,
            NotMBit,
            OrMBit,
            AndMBit,
            XorMBit,
            NandMBit,
            NorMBit,
            XnorMBit,

            Reg,
            NotReg,
            OrReg,
            AndReg,
            XorReg,
            NandReg,
            NorReg,
            XnorReg,

            EqualPos,
            GreaterPos,
            LessPos,
            EqualVelocity,
            GreaterVelocity,
            LessVelocity,
            EqualTrq,
            GreaterTrq,
            LessTrq,

            RemainingTime,
            RemainingDistance,
            SameTimeCompletion,
            CompletedTime,
            CompletedDistance,
            StaggeredTimeCompletion,
            StaggeredDistanceCompletion,
            DistanceToTarget
        };
    };

    class TriggerEventOutputFunction {
    public:
        enum T {
            TriggerMotion,
            SetReg
        };
    };

    class TriggerEvent{
    public:
        TriggerEvent();
        TriggerEventInputFunction::T inputFunction;

        union TriggerEventInputFunctionArguments {
            TriggerEventInputFunctionArguments();

            struct IOBit {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;
                IOSourceType::T ioSourceType;
            }ioBit;
            struct NotIOBit {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;
                IOSourceType::T ioSourceType;
            }notIOBit;
            struct OrIOBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
                IOSourceType::T ioSourceType[2];
            }orIOBit;
            struct AndIOBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
                IOSourceType::T ioSourceType[2];
            }andIOBit;
            struct XorIOBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
                IOSourceType::T ioSourceType[2];
            }xorIOBit;
            struct NandIOBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
                IOSourceType::T ioSourceType[2];
            }nandIOBit;
            struct NorIOBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
                IOSourceType::T ioSourceType[2];
            }norIOBit;
            struct XnorIOBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
                IOSourceType::T ioSourceType[2];
            }xnorIOBit;

            struct MBit {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;
            }mBit;
            struct NotMBit {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;
            }notMBit;
            struct OrMBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
            }orMBit;
            struct AndMBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
            }andMBit;
            struct XorMBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
            }xorMBit;
            struct NandMBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
            }nandMBit;
            struct NorMBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
            }norMBit;
            struct XnorMBit {
                unsigned int byteAddress[2];
                unsigned char bitAddress[2];
                unsigned char invert[2];
            }xnorMBit;

            struct Reg {
                unsigned int regNumber;
                unsigned char invert;
            }reg;
            struct NotReg {
                unsigned int regNumber;
                unsigned char invert;
            }notReg;
            struct OrReg {
                unsigned int regNumber[2];
                unsigned char invert[2];
            }orReg;
            struct AndReg {
                unsigned int regNumber[2];
                unsigned char invert[2];
            }andReg;
            struct XorReg {
                unsigned int regNumber[2];
                unsigned char invert[2];
            }xorReg;
            struct NandReg {
                unsigned int regNumber[2];
                unsigned char invert[2];
            }nandReg;
            struct NorReg {
                unsigned int regNumber[2];
                unsigned char invert[2];
            }norReg;
            struct XnorReg {
                unsigned int regNumber[2];
                unsigned char invert[2];
            }xnorReg;

            struct EqualPos {
                unsigned int axis;
                double pos;
                unsigned char disablePositiveDirection;
                unsigned char disableNegativeDirection;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableTolerance;
                double tolerance;
            }equalPos;
            struct GreaterPos {
                unsigned int axis;
                double pos;
                unsigned char disablePositiveDirection;
                unsigned char disableNegativeDirection;
                unsigned char useFeedback;
                unsigned char invert;
            }greaterPos;
            struct LessPos {
                unsigned int axis;
                double pos;
                unsigned char disablePositiveDirection;
                unsigned char disableNegativeDirection;
                unsigned char useFeedback;
                unsigned char invert;
            }lessPos;
            struct EqualVelocity {
                unsigned int axis;
                double velocity;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableTolerance;
                double tolerance;
                unsigned char enableUnsigned;
            }equalVelocity;
            struct GreaterVelocity {
                unsigned int axis;
                double velocity;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableUnsigned;
            }greaterVelocity;
            struct LessVelocity {
                unsigned int axis;
                double velocity;
                unsigned char useFeedback;
                unsigned char invert;
                unsigned char enableUnsigned;
            }lessVelocity;
            struct EqualTrq {
                unsigned int axis;
                double trq;
                unsigned char invert;
                unsigned char enableTolerance;
                double tolerance;
                unsigned char enableUnsigned;
            }equalTrq;
            struct GreaterTrq {
                unsigned int axis;
                double trq;
                unsigned char invert;
                unsigned char enableUnsigned;
            }greaterTrq;
            struct LessTrq {
                unsigned int axis;
                double trq;
                unsigned char invert;
                unsigned char enableUnsigned;
            }lessTrq;

            struct RemainingTime {
                unsigned int axis;
                unsigned char invert;
                double timeMilliseconds;
            }remainingTime;
            struct RemainingDistance {
                unsigned int axis;
                unsigned char invert;
                double distance;
            }remainingDistance;
            struct SameTimeCompletion {
                unsigned int axis;
            }sameTimeCompletion;
            struct CompletedTime {
                unsigned int axis;
                unsigned char invert;
                double timeMilliseconds;
            }completedTime;
            struct CompletedDistance {
                unsigned int axis;
                unsigned char invert;
                double distance;
            }completedDistance;
            struct StaggeredTimeCompletion {
                unsigned int axis;
                double timeMilliseconds;
            }staggeredTimeCompletion;
            struct StaggeredDistanceCompletion {
                unsigned int axis;
                double distance;
            }staggeredDistanceCompletion;
            struct DistanceToTarget {
                unsigned int axis;
                unsigned char invert;
                double distance;
            }distanceToTarget;

        }input;

        TriggerEventOutputFunction::T outputFunction;
        union TriggerEventOutputFunctionArguments {
            TriggerEventOutputFunctionArguments();

            struct TriggerMotion {
                unsigned int reserved;
            }triggerMotion;
            struct SetReg {
                unsigned int regNumber;

            }setReg;
        }output;

    };

    class TriggerEvents{
    public:
        TriggerEvents();
        unsigned int numEvents;
        TriggerEvent event[constants::maxTriggerEvents];
    };

    class Trigger {
    public:
        Trigger();
        Trigger(int triggerAxis, TriggerType::T triggerType, double triggerValue);

        int triggerAxis;
        TriggerType::T triggerType;
        double triggerValue;
    };

    class CoreMotion;

    class AxisControl {
    public:
        CoreMotion *cmApi;
        AxisControl(CoreMotion *f) : cmApi(f) {}

        bool IsDeviceValid();

        WMX3APIFUNC SetServoOn(int axis, int newStatus);
        WMX3APIFUNC SetServoOn(AxisSelection* pAxisSelection, int newStatus);
        WMX3APIFUNC ClearAmpAlarm(int axis);
        WMX3APIFUNC ClearAmpAlarm(AxisSelection* pAxisSelection);
        WMX3APIFUNC ClearAxisAlarm(int axis);
        WMX3APIFUNC ClearAxisAlarm(AxisSelection* pAxisSelection);
        WMX3APIFUNC SetAxisCommandMode(int axis, AxisCommandMode::T mode);
        WMX3APIFUNC SetAxisCommandMode(AxisSelection* pAxisSelection, AxisCommandMode::T mode);
        WMX3APIFUNC GetAxisCommandMode(int axis, AxisCommandMode::T *pMode);
        WMX3APIFUNC GetPosCommand(int axis, double *pPosition);
        WMX3APIFUNC GetPosFeedback(int axis, double *pPosition);
        WMX3APIFUNC GetVelCommand(int axis, double *pVelocity);
        WMX3APIFUNC GetVelFeedback(int axis, double *pVelocity);

    };

    class Config {
    public:
        CoreMotion *cmApi;
        Config(CoreMotion *f) : cmApi(f){}

        bool IsDeviceValid();

        class VelocityMonitorSource {
        public:
            enum T {
                UseVelocityFeedback,
                CalculateFromPositionFeedback
            };
        };

        class FeedbackParam{
        public:
            FeedbackParam();
            double inPosWidth;
            double inPosWidth2;
            double inPosWidth3;
            double inPosWidth4;
            double inPosWidth5;
            VelocityMonitorSource::T velocityMonitorSource;
            double posSetWidth;
            double delayedPosSetWidth;
            double delayedPosSetMilliseconds;
        };

        class HomeDirection {
        public:
            enum T {
                Positive,
                Negative
            };
        };

        class HomeType {
        public:
            enum T {
                CurrentPos,
                ZPulse,
                HS,
                HSHS,
                HSZPulse,
                HSReverseZPulse,
                HSOff,
                HSOffZPulse,
                HSOffReverseZPulse,
                LSReverseZPulse,
                NearLSReverseZPulse,
                ExternalLSReverseZPulse,
                TouchProbe,
                HSTouchProbe,
                LS,
                NearLS,
                ExternalLS,
                MechanicalEndDetection,
                MechanicalEndDetectionHS,
                MechanicalEndDetectionLS,
                MechanicalEndDetectionReverseZPulse
            };
        };

        class HomeParam{
        public:
            HomeParam();
            HomeType::T homeType;
            HomeDirection::T homeDirection;
            double homingVelocitySlow;
            double homingVelocitySlowAcc;
            double homingVelocitySlowDec;
            double homingVelocityFast;
            double homingVelocityFastAcc;
            double homingVelocityFastDec;
            double homingReverseDistance;
            double homeShiftVelocity;
            double homeShiftAcc;
            double homeShiftDec;
            double homeShiftDistance;
            bool invertHSPolarity;
            unsigned int multipleZPulse;
            bool roundPosCmdAfterHoming;
            bool pauseMode;
            double maxHSOnAtStartReverseDistance;
            double maxLSReverseDistance;
            unsigned int zPulseDistanceCheck;
            double homePosition;
            bool gantryHomingUseSlaveHS;
            bool gantryHomingUseSlaveLS;
            bool gantryHomingUseSlaveZPulse;
            bool gantryHomingUseSlaveTouchProbe;
            bool gantryHomingUseSlaveMechanicalEnd;
            bool gantryHomingRetainSyncOffset;
            bool immediateStopAtLS;
            double mechanicalEndDetectionPosDiff;
            double mechanicalEndDetectionTimeMilliseconds;
            bool mechanicalEndDetectionIgnoreLS;
            double mechanicalEndDetectionFirstTorqueLimit;
            double mechanicalEndDetectionSecondTorqueLimit;
            bool openLoopHoming;
            bool clearHomeDoneOnServoOff;
            bool clearHomeDoneOnCommStop;
        };

        class LimitSwitchType {
        public:
            enum T {
                None,
                ServoOff,
                DecServoOff,
                Dec,
                SlowDecServoOff,
                SlowDec,
                SeparatePositiveLSNegativeLS
            };
        };

        class LimitSwitchDirection {
        public:
            enum T {
                Normal,
                Reverse
            };
        };

        class LimitParam{
        public:
            LimitParam();
            LimitSwitchType::T lsType;
            LimitSwitchType::T positiveLSType;
            LimitSwitchType::T negativeLSType;
            bool invertPositiveLSPolarity;
            bool invertNegativeLSPolarity;
            LimitSwitchType::T nearLSType;
            LimitSwitchType::T nearPositiveLSType;
            LimitSwitchType::T nearNegativeLSType;
            int nearPositiveLSByte;
            int nearPositiveLSBit;
            bool invertNearPositiveLSPolarity;
            int nearNegativeLSByte;
            int nearNegativeLSBit;
            bool invertNearNegativeLSPolarity;
            LimitSwitchType::T externalLSType;
            LimitSwitchType::T externalPositiveLSType;
            LimitSwitchType::T externalNegativeLSType;
            int externalPositiveLSByte;
            int externalPositiveLSBit;
            bool invertExternalPositiveLSPolarity;
            int externalNegativeLSByte;
            int externalNegativeLSBit;
            bool invertExternalNegativeLSPolarity;
            LimitSwitchType::T softLimitType;
            LimitSwitchType::T positiveSoftLimitType;
            LimitSwitchType::T negativeSoftLimitType;
            double softLimitPositivePos;
            double softLimitNegativePos;
            double lsDec;
            double lsSlowDec;
            bool allLSDuringHoming;
            LimitSwitchDirection::T lsDirection;
        };

        class ProhibitOvertravelType {
        public:
            enum T {
                Disabled,
                ChangeDeceleration,
                ChangeInitialVelocity
            };
        };

        class LinearIntplOverrideType {
        public:
            enum T {
                Smoothing,
                Blending,
                FastBlending
            };
        };

        class LinearIntplProfileCalcMode {
        public:
            enum T {
                AxisLimit,
                MatchSlowestAxis,
                MatchFarthestAxis
            };
        };

        class CircularIntplOverrideType {
        public:
            enum T {
                Blending,
                FastBlending
            };
        };

        class MotionParam{
        public:
            MotionParam();
            double quickStopDec;
            ProhibitOvertravelType::T prohibitOvertravel;
            LinearIntplOverrideType::T linearIntplOverrideType;
            unsigned int linearIntplOverrideSmoothPercent;
            CircularIntplOverrideType::T circularIntplOverrideType;
            bool interruptedIntplUseQuickStop;
            bool singleTurnReduceToHalfTurn;
            bool enableGlobalStartingVelocity;
            double globalStartingVelocity;
            bool enableGlobalEndVelocity;
            double globalEndVelocity;
            bool enableGlobalMinVelocity;
            double globalMinVelocity;
            bool enableGlobalMovingAverageProfileTimeMilliseconds;
            double globalMovingAverageProfileTimeMilliseconds;
            bool apiWaitUntilMotionStart;
            LinearIntplProfileCalcMode::T linearIntplProfileCalcMode;
        };

        class FollowingErrorAlarmType {
        public:
            enum T {
                NoAction,
                QuickStop
            };
        };

        class VelocityFollowingErrorAlarmType {
        public:
            enum T {
                NoAction,
                QuickStop,
                Warning
            };
        };

        class AlarmParam{
        public:
            AlarmParam();
            double followingErrorStopped;
            double followingErrorMoving;
            FollowingErrorAlarmType::T followingErrorType;
            double velocityFollowingErrorStopped;
            double velocityFollowingErrorStoppedMilliseconds;
            double velocityFollowingErrorMoving;
            double velocityFollowingErrorMovingMilliseconds;
            VelocityFollowingErrorAlarmType::T velocityFollowingErrorType;
            bool servoOffDuringAmpAlarm;
            int servoOnFollowingError;
        };

        class SyncCompensationMode {
        public:
            enum T {
                None,
                VelocityOffset,
                SymmetricVelocityOffset
            };
        };

        class MasterDesyncType {
        public:
            enum T {
                NoAction,
                ServoOff,
                DecServoOff,
                Dec
            };
        };

        class SlaveDesyncType {
        public:
            enum T {
                NoAction,
                ServoOff,
                ResolveSync
            };
        };

        class SyncParam{
        public:
            SyncParam();
            int inSyncWidth;
            double syncGain;
            SyncCompensationMode::T syncCompensationMode;
            MasterDesyncType::T masterDesyncType;
            double masterDesyncDec;
            SlaveDesyncType::T slaveDesyncType;
            double slaveDesyncDec;
            bool matchPos;
        };

        class FlightRecorderParam{
        public:
            FlightRecorderParam();
            bool enableFlightRecorder;
            bool flightRecorderTimeStamp;
            bool collectAxisFlightRecorderData[constants::maxAxes];
            bool triggerFlightRecorderOnAmpAlarm;
        };

        class EStopSignalSource {
        public:
            enum T {
                Input,
                Output,
                UserMemory
            };
        };

        class EStopStatusSignalDestination {
        public:
            enum T {
                Output,
                UserMemory
            };
        };

        class EStopLevel1Type {
        public:
            enum T {
                Dec,
                DecServoOff
            };
        };

        class EmergencyStopParam{
        public:
            EmergencyStopParam();

            double eStopDec[constants::maxAxes];
            bool enableEStopSignal;
            EStopSignalSource::T eStopSignalSource;
            EStopLevel::T eStopSignalLevel;
            bool invertEStopSignalPolarity;
            unsigned int eStopSignalByteAddress;
            unsigned char eStopSignalBitAddress;
            bool enableEStopStatusSignal;
            EStopStatusSignalDestination::T eStopStatusSignalDestination;
            bool invertEStopStatusSignalPolarity;
            unsigned int eStopStatusSignalByteAddress;
            unsigned char eStopStatusSignalBitAddress;
            EStopLevel1Type::T eStopLevel1Type;
        };

        class SystemParam{
        public:
            SystemParam();
            FeedbackParam feedbackParam[constants::maxAxes];
            HomeParam homeParam[constants::maxAxes];
            LimitParam limitParam[constants::maxAxes];
            MotionParam motionParam[constants::maxAxes];
            AlarmParam alarmParam[constants::maxAxes];
            SyncParam syncParam[constants::maxAxes];
            FlightRecorderParam flightRecorderParam;
            EmergencyStopParam emergencyStopParam;

        };

        class AxisParam{
        public:
            AxisParam();
            AxisCommandMode::T axisCommandMode[constants::maxAxes];
            double gearRatioNumerator[constants::maxAxes];
            double gearRatioDenominator[constants::maxAxes];
            bool singleTurnMode[constants::maxAxes];
            unsigned int singleTurnEncoderCount[constants::maxAxes];
            double maxTrqLimit[constants::maxAxes];
            double negativeTrqLimit[constants::maxAxes];
            double positiveTrqLimit[constants::maxAxes];
            double axisUnit[constants::maxAxes];
            double velocityFeedforwardGain[constants::maxAxes];
            char axisPolarity[constants::maxAxes];
            double maxMotorSpeed[constants::maxAxes];
            bool absoluteEncoderMode[constants::maxAxes];
            double absoluteEncoderHomeOffset[constants::maxAxes];
            bool encoderRangeMode[constants::maxAxes];
            int encoderRangeLow[constants::maxAxes];
            int encoderRangeHigh[constants::maxAxes];
        };

        WMX3APIFUNC SetParam(SystemParam *pParam, SystemParam *pParamError = NULL);
        WMX3APIFUNC GetParam(SystemParam *pParam);
        WMX3APIFUNC SetParam(int axis, SystemParam *pParam, SystemParam *pParamError = NULL);
        WMX3APIFUNC GetParam(int axis, SystemParam *pParam);
        WMX3APIFUNC SetAxisParam(AxisParam *pParam, AxisParam *pParamError = NULL);
        WMX3APIFUNC GetAxisParam(AxisParam *pParam);
        WMX3APIFUNC SetAxisParam(int axis, AxisParam *pParam, AxisParam *pParamError = NULL);
        WMX3APIFUNC GetAxisParam(int axis, AxisParam *pParam);
        WMX3APIFUNC SetGearRatio(int axis, double numerator, double denominator);
        WMX3APIFUNC SetSingleTurn(int axis, bool enable, unsigned int encoderCount);
        WMX3APIFUNC SetMovingAverageProfileTime(int axis, double milliseconds);
        WMX3APIFUNC SetAxisUnit(int axis, double unit);
        WMX3APIFUNC SetVelocityFeedforwardGain(int axis, double gain);
        WMX3APIFUNC SetAxisPolarity(int axis, char polarity);
        WMX3APIFUNC SetAbsoluteEncoderMode(int axis, bool enable);
        WMX3APIFUNC SetAbsoluteEncoderHomeOffset(int axis, double offset);
        WMX3APIFUNC SetEncoderRange(int axis, bool enable, int encoderRangeLow, int encoderRangeHigh);
        WMX3APIFUNC GetGearRatio(int axis, double *pNumerator, double *pDenominator);
        WMX3APIFUNC GetSingleTurn(int axis, bool *pEnable, unsigned int *pEncoderCount);
        WMX3APIFUNC GetMovingAverageProfileTime(int axis, double *pMilliseconds);
        WMX3APIFUNC GetAxisUnit(int axis, double *pUnit);
        WMX3APIFUNC GetVelocityFeedforwardGain(int axis, double *pGain);
        WMX3APIFUNC GetAxisPolarity(int axis, char *pPolarity);
        WMX3APIFUNC GetAbsoluteEncoderMode(int axis, bool *pEnable);
        WMX3APIFUNC GetAbsoluteEncoderHomeOffset(int axis, double *pOffset);
        WMX3APIFUNC GetEncoderRange(int axis, bool *pEnable, int *pEncoderRangeLow, int *pEncoderRangeHigh);
        WMX3APIFUNC SetFeedbackParam(int axis, FeedbackParam *pParam, FeedbackParam *pParamError = NULL);
        WMX3APIFUNC SetHomeParam(int axis, HomeParam *pParam, HomeParam *pParamError = NULL);
        WMX3APIFUNC SetLimitParam(int axis, LimitParam *pParam, LimitParam *pParamError = NULL);
        WMX3APIFUNC SetMotionParam(int axis, MotionParam *pParam, MotionParam *pParamError = NULL);
        WMX3APIFUNC SetAlarmParam(int axis, AlarmParam *pParam, AlarmParam *pParamError = NULL);
        WMX3APIFUNC SetSyncParam(int axis, SyncParam *pParam, SyncParam *pParamError = NULL);
        WMX3APIFUNC SetFlightRecorderParam(FlightRecorderParam *pParam, FlightRecorderParam *pParamError = NULL);
        WMX3APIFUNC SetFlightRecorderPath(char *pPath);
        WMX3APIFUNC SetFlightRecorderPath(wchar_t *pPath);
        WMX3APIFUNC SetEmergencyStopParam(EmergencyStopParam *pParam, EmergencyStopParam *pParamError = NULL);
        WMX3APIFUNC GetFeedbackParam(int axis, FeedbackParam *pParam);
        WMX3APIFUNC GetHomeParam(int axis, HomeParam *pParam);
        WMX3APIFUNC GetLimitParam(int axis, LimitParam *pParam);
        WMX3APIFUNC GetMotionParam(int axis, MotionParam *pParam);
        WMX3APIFUNC GetAlarmParam(int axis, AlarmParam *pParam);
        WMX3APIFUNC GetSyncParam(int axis, SyncParam *pParam);
        WMX3APIFUNC GetFlightRecorderParam(FlightRecorderParam *pParam);
        WMX3APIFUNC GetEmergencyStopParam(EmergencyStopParam *pParam);
        WMX3APIFUNC GetDefaultParam(SystemParam *pParam);
        WMX3APIFUNC GetDefaultAxisParam(AxisParam *pAxisParam);
        WMX3APIFUNC Export(char *pPath, SystemParam *pParam);
        WMX3APIFUNC Export(wchar_t *pPath, SystemParam *pParam);
        WMX3APIFUNC Export(char *pPath, AxisParam *pAxisParam);
        WMX3APIFUNC Export(wchar_t *pPath, AxisParam *pAxisParam);
        WMX3APIFUNC Export(char *pPath, SystemParam *pParam, AxisParam *pAxisParam);
        WMX3APIFUNC Export(wchar_t *pPath, SystemParam *pParam, AxisParam *pAxisParam);
        WMX3APIFUNC Export(char *pPath, SystemParam *pParam, int axis);
        WMX3APIFUNC Export(wchar_t *pPath, SystemParam *pParam, int axis);
        WMX3APIFUNC Export(char *pPath, AxisParam *pAxisParam, int axis);
        WMX3APIFUNC Export(wchar_t *pPath, AxisParam *pAxisParam, int axis);
        WMX3APIFUNC Export(char *pPath, SystemParam *pParam, AxisParam *pAxisParam, int axis);
        WMX3APIFUNC Export(wchar_t *pPath, SystemParam *pParam, AxisParam *pAxisParam, int axis);
        WMX3APIFUNC Import(char *pPath, SystemParam *pParam);
        WMX3APIFUNC Import(wchar_t *pPath, SystemParam *pParam);
        WMX3APIFUNC Import(char *pPath, AxisParam *pAxisParam);
        WMX3APIFUNC Import(wchar_t *pPath, AxisParam *pAxisParam);
        WMX3APIFUNC Import(char *pPath, SystemParam *pParam, AxisParam *pAxisParam);
        WMX3APIFUNC Import(wchar_t *pPath, SystemParam *pParam, AxisParam *pAxisParam);
        WMX3APIFUNC Import(char *pPath, SystemParam *pParam, int axis, AxisSelection *copyParamToAxes = NULL);
        WMX3APIFUNC Import(wchar_t *pPath, SystemParam *pParam, int axis, AxisSelection *copyParamToAxes = NULL);
        WMX3APIFUNC Import(char *pPath, AxisParam *pAxisParam, int axis, AxisSelection *copyParamToAxes = NULL);
        WMX3APIFUNC Import(wchar_t *pPath, AxisParam *pAxisParam, int axis, AxisSelection *copyParamToAxes = NULL);
        WMX3APIFUNC Import(char *pPath, SystemParam *pParam, AxisParam *pAxisParam, int axis, AxisSelection *copyParamToAxes = NULL);
        WMX3APIFUNC Import(wchar_t *pPath, SystemParam *pParam, AxisParam *pAxisParam, int axis, AxisSelection *copyParamToAxes = NULL);
        WMX3APIFUNC GetAndExportAll(char *pPath);
        WMX3APIFUNC GetAndExportAll(wchar_t *pPath);
        WMX3APIFUNC ImportAndSetAll(char *pPath, SystemParam *pParamError = NULL, AxisParam *pAxisParamError = NULL);
        WMX3APIFUNC ImportAndSetAll(wchar_t *pPath, SystemParam *pParamError = NULL, AxisParam *pAxisParamError = NULL);
    };

    class Sync {
    public:
        CoreMotion *cmApi;
        Sync(CoreMotion *f) : cmApi(f){}

        bool IsDeviceValid();

        class SyncOptions {
        public:
            SyncOptions();
            bool useMasterFeedback;
        };

        class SyncCombineOptions {
        public:
            SyncCombineOptions();
            bool useMaster1Feedback;
            bool useMaster2Feedback;
        };

        class SyncGroupStartupType {
        public:
            enum T {
                Normal,
                CatchUp
            };
        };

        class SyncGroup{
        public:
            SyncGroup();
            int masterAxis;
            int slaveAxisCount;
            int slaveAxis[constants::maxAxes-1];
            unsigned char servoOnOffSynchronization;
            SyncGroupStartupType::T startupType;
            unsigned int gantryLoopCycleRatio;
            double maxCatchUpDistance;
            double catchUpVelocity;
            double catchUpAcc;
            double syncErrorTolerance;
            unsigned char useMasterFeedback;
        };

        class SyncGroupStatus{
        public:
            SyncGroupStatus();
            unsigned char enabled;
            unsigned char homeDone;
            unsigned char catchUpError;
            unsigned char syncError;
            unsigned char axisError;
            unsigned char interruptError;
        };

        WMX3APIFUNC SetSyncMasterSlave(int masterAxis, int slaveAxis, SyncOptions *pSyncOptions = NULL);
        WMX3APIFUNC SetSyncCombine(int masterAxis1, int masterAxis2, int slaveAxis, SyncCombineOptions *pSyncCombineOptions = NULL);
        WMX3APIFUNC SetAbsoluteSyncPhase(int slaveAxis, double phase, Profile *pProfile);
        WMX3APIFUNC AddRelativeSyncPhase(int slaveAxis, double phase, Profile *pProfile);
        WMX3APIFUNC SetSyncGearRatio(int slaveAxis, double gearRatio, Profile *pProfile);
        WMX3APIFUNC SetSyncGearRatio(int masterAxis, int slaveAxis, double gearRatio, Profile *pProfile);
        WMX3APIFUNC SyncToJog(int slaveAxis, Profile *pProfile);
        WMX3APIFUNC SyncToJog(int slaveAxis);
        WMX3APIFUNC ResolveSync(int slaveAxis);
        WMX3APIFUNC SetSyncGroup(int groupId, SyncGroup syncGroup);
        WMX3APIFUNC GetSyncGroup(int groupId, SyncGroup *pSyncGroup);
        WMX3APIFUNC AddAxisToSyncGroup(int groupId, int axis, unsigned char isMaster);
        WMX3APIFUNC RemoveAxisFromSyncGroup(int groupId, int axis);
        WMX3APIFUNC EnableSyncGroup(int groupId, unsigned char enable);
        WMX3APIFUNC GetSyncGroupStatus(int groupId, SyncGroupStatus *pStatus);
        WMX3APIFUNC ClearSyncGroupError(int groupId);
    };


    class Home {
    public:
        CoreMotion *cmApi;
        Home(CoreMotion *f) : cmApi(f) {}

        bool IsDeviceValid();

        class AxisHomeData{
        public:
            AxisHomeData();
            double distHStoZPulse;
            double distLStoZPulse;
            double latchedZPulseEncoder;
            double latchedLimitSwitchEncoder;
            double latchedHomeSwitchEncoder;
            double latchedFirstHomeSwitchEncoder;
            double latchedTouchProbeEncoder;
            double distZPulseToMasterZPulse;
            double distLSToMasterLS;
            double distHSToMasterHS;
        };

        class HomeData {
        public:
            HomeData();
            AxisHomeData axesHomeData[constants::maxAxes];
        };

        WMX3APIFUNC StartHome(int axis);
        WMX3APIFUNC StartHome(AxisSelection *pAxisSelection);
        WMX3APIFUNC Continue(int axis);
        WMX3APIFUNC Continue(AxisSelection *pAxisSelection);
        WMX3APIFUNC Cancel(int axis);
        WMX3APIFUNC Cancel(AxisSelection *pAxisSelection);
        WMX3APIFUNC SetCommandPos(int axis, double position);
        WMX3APIFUNC SetFeedbackPos(int axis, double position);
        WMX3APIFUNC SetHomeDone(int axis, unsigned char value);
        WMX3APIFUNC SetCommandPosToFeedbackPos(int axis, double posChangePerCycle = 0);
        WMX3APIFUNC GetHomeData(HomeData *pHomeData);
    };


    class Velocity {
    public:
        CoreMotion *cmApi;
        Velocity(CoreMotion *f) : cmApi(f) {}

        bool IsDeviceValid();

        class VelCommand {
        public:
            VelCommand();
            VelCommand(int axis, Profile profile);

            int axis;
            Profile profile;
        };

        class TimedVelCommand {
        public:
            TimedVelCommand();
            TimedVelCommand(int axis, double runTimeMilliseconds, Profile profile);

            int axis;
            double runTimeMilliseconds;
            Profile profile;
        };

        class TriggerVelCommand {
        public:
            TriggerVelCommand();
            TriggerVelCommand(int axis, Profile profile, Trigger trigger);

            int axis;
            Profile profile;
            Trigger trigger;
        };

        class TimeCommand {
        public:
            TimeCommand();
            TimeCommand(int axis, double timeMilliseconds);

            int axis;
            double timeMilliseconds;
        };

        WMX3APIFUNC StartVel(VelCommand *pVelCommand);
        WMX3APIFUNC StartVel(TimedVelCommand *pTimedVelCommand);
        WMX3APIFUNC StartVel(VelCommand *pVelCommand, double maxTrqLimit);

        WMX3APIFUNC StartVel(unsigned int numCommands, VelCommand *pVelCommand);
        WMX3APIFUNC StartVel(unsigned int numCommands, TimedVelCommand *pTimedVelCommand);

        WMX3APIFUNC Stop(int axis);
        WMX3APIFUNC Stop(AxisSelection *pAxisSelection);
        WMX3APIFUNC ExecQuickStop(int axis);
        WMX3APIFUNC ExecQuickStop(AxisSelection *pAxisSelection);
        WMX3APIFUNC ExecTimedStop(int axis, double timeMilliseconds);
        WMX3APIFUNC ExecTimedStop(AxisSelection *pAxisSelection, double timeMilliseconds);
        WMX3APIFUNC ExecTimedStop(unsigned int numCommands, TimeCommand *pTimeCommand);

        WMX3APIFUNC SetMaxMotorSpeed(int axis, double speed);
        WMX3APIFUNC GetMaxMotorSpeed(int axis, double *pSpeed);

        WMX3APIFUNC OverrideVel(VelCommand *pVelCommand);
        WMX3APIFUNC OverrideVel(unsigned int numCommands, VelCommand *pVelCommand);

        WMX3APIFUNC StartPosToVel(TriggerVelCommand *pTriggerVelCommand);
        WMX3APIFUNC StartPosToVel(unsigned int numCommands, TriggerVelCommand *pTriggerVelCommand);
        WMX3APIFUNC StartPosToVel(VelCommand *pVelCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartPosToVel(unsigned int numCommands, VelCommand *pVelCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartTrqToVel(TriggerVelCommand *pTriggerVelCommand);
        WMX3APIFUNC StartTrqToVel(unsigned int numCommands, TriggerVelCommand *pTriggerVelCommand);
        WMX3APIFUNC StartTrqToVel(VelCommand *pVelCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartTrqToVel(unsigned int numCommands, VelCommand *pVelCommand, TriggerEvents *pTriggerEvents);
    };

    class Torque {
    public:
        CoreMotion *cmApi;
        Torque(CoreMotion *f) : cmApi(f) {}

        bool IsDeviceValid();

        class TrqCommand {
        public:
            TrqCommand();
            TrqCommand(int axis, double torque);

            int axis;
            double torque;
        };

        class TriggerTrqCommand {
        public:
            TriggerTrqCommand();
            TriggerTrqCommand(int axis, double torque, Trigger trigger);

            int axis;
            double torque;
            Trigger trigger;
        };

        WMX3APIFUNC SetMaxTrqLimit(int axis, double torque);
        WMX3APIFUNC GetMaxTrqLimit(int axis, double *pTorque);
        WMX3APIFUNC SetPositiveTrqLimit(int axis, double torque);
        WMX3APIFUNC GetPositiveTrqLimit(int axis, double *pTorque);
        WMX3APIFUNC SetNegativeTrqLimit(int axis, double torque);
        WMX3APIFUNC GetNegativeTrqLimit(int axis, double *pTorque);

        WMX3APIFUNC StartTrq(TrqCommand *pTrqCommand);
        WMX3APIFUNC StartTrq(unsigned int numCommands, TrqCommand *pTrqCommand);
        WMX3APIFUNC StartTrq(TrqCommand *pTrqCommand, double maxMotorSpeed);
        WMX3APIFUNC StartRampTimeTrq(TrqCommand *pTrqCommand, unsigned int rampCycleTime);
        WMX3APIFUNC StartRampTimeTrq(TrqCommand *pTrqCommand, unsigned int rampCycleTime, double maxMotorSpeed);
        WMX3APIFUNC StartRampRateTrq(TrqCommand *pTrqCommand, double rampRate);
        WMX3APIFUNC StartRampRateTrq(TrqCommand *pTrqCommand, double rampRate, double maxMotorSpeed);

        WMX3APIFUNC StopTrq(int axis);
        WMX3APIFUNC StopTrq(AxisSelection *pAxisSelection);

        WMX3APIFUNC StartPosToTrq(TriggerTrqCommand *pTriggerTrqCommand);
        WMX3APIFUNC StartPosToTrq(unsigned int numCommands, TriggerTrqCommand *pTriggerTrqCommand);
        WMX3APIFUNC StartPosToTrq(TrqCommand *pTrqCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartPosToTrq(unsigned int numCommands, TrqCommand *pTrqCommand, TriggerEvents *pTriggerEvents);
		WMX3APIFUNC StartPosToRampTimeTrq(TrqCommand *pTrqCommand, unsigned int rampCycleTime, Trigger *pTrigger);
		WMX3APIFUNC StartPosToRampTimeTrq(TrqCommand *pTrqCommand, unsigned int rampCycleTime, TriggerEvents *pTriggerEvents);
		WMX3APIFUNC StartPosToRampRateTrq(TrqCommand *pTrqCommand, double rampRate, Trigger *pTrigger);
		WMX3APIFUNC StartPosToRampRateTrq(TrqCommand *pTrqCommand, double rampRate, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartVelToTrq(TriggerTrqCommand *pTriggerTrqCommand);
        WMX3APIFUNC StartVelToTrq(unsigned int numCommands, TriggerTrqCommand *pTriggerTrqCommand);
        WMX3APIFUNC StartVelToTrq(TrqCommand *pTrqCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartVelToTrq(unsigned int numCommands, TrqCommand *pTrqCommand, TriggerEvents *pTriggerEvents);
		WMX3APIFUNC StartVelToRampTimeTrq(TrqCommand *pTrqCommand, unsigned int rampCycleTime, Trigger *pTrigger);
		WMX3APIFUNC StartVelToRampTimeTrq(TrqCommand *pTrqCommand, unsigned int rampCycleTime, TriggerEvents *pTriggerEvents);
		WMX3APIFUNC StartVelToRampRateTrq(TrqCommand *pTrqCommand, double rampRate, Trigger *pTrigger);
		WMX3APIFUNC StartVelToRampRateTrq(TrqCommand *pTrqCommand, double rampRate, TriggerEvents *pTriggerEvents);
    };

    class Motion{
    public:
        CoreMotion *cmApi;
        Motion(CoreMotion *f) : cmApi(f){}

        bool IsDeviceValid();

        class PosCommand {
        public:
            PosCommand();
            PosCommand(int axis, double target, Profile profile);

            int axis;
            double target;
            Profile profile;
        };

        class TriggerPosCommand {
        public:
            TriggerPosCommand();
            TriggerPosCommand(int axis, double target, Profile profile, Trigger trigger);

            int axis;
            double target;
            Profile profile;
            Trigger trigger;
        };

        class LinearIntplCommand {
        public:
            LinearIntplCommand();

            unsigned int axisCount;
            int axis[constants::maxAxes];
            double target[constants::maxAxes];
            double maxVelocity[constants::maxAxes];
            double maxAcc[constants::maxAxes];
            double maxDec[constants::maxAxes];
            double maxJerkAcc[constants::maxAxes];
            double maxJerkDec[constants::maxAxes];
            Profile profile;
        };

        class CenterAndLengthCircularIntplCommand{
        public:
            CenterAndLengthCircularIntplCommand();
            int axis[2];
            double centerPos[2];
            double arcLengthDegree;
            unsigned char clockwise;
            Profile profile;
        };

        class CenterAndEndCircularIntplCommand{
        public:
            CenterAndEndCircularIntplCommand();
            int axis[2];
            double centerPos[2];
            double endPos[2];
            unsigned char clockwise;
            Profile profile;
        };

        class ThroughAndEndCircularIntplCommand{
        public:
            ThroughAndEndCircularIntplCommand();
            int axis[2];
            double throughPos[2];
            double endPos[2];
            Profile profile;
        };

        class LengthAndEndCircularIntplCommand{
        public:
            LengthAndEndCircularIntplCommand();
            int axis[2];
            double arcLengthDegree;
            unsigned char clockwise;
            double endPos[2];
            Profile profile;
        };

        class RadiusAndEndCircularIntplCommand {
        public:
            RadiusAndEndCircularIntplCommand();
            int axis[2];
            double radius;
            unsigned char clockwise;
            double endPos[2];
            Profile profile;
        };

        class ThroughAndEnd3DCircularIntplCommand{
        public:
            ThroughAndEnd3DCircularIntplCommand();
            int axis[3];
            double throughPos[3];
            double endPos[3];
            Profile profile;
        };

        class HelicalIntplProfileType {
        public:
            enum T {
                Helical,
                Circular,
                Linear
            };
        };

        class HelicalIntplCommand {
        public:
            HelicalIntplCommand();
            int axis[2];
            int zAxis;
            double centerPos[2];
            double zEndPos;
            double arcLengthDegree;
            unsigned char clockwise;
            HelicalIntplProfileType::T helicalProfileType;
            Profile profile;
        };

        class JogCommand {
        public:
            JogCommand();
            JogCommand(int axis, Profile profile);

            int axis;
            Profile profile;
        };

        class TimedJogCommand {
        public:
            TimedJogCommand();
            TimedJogCommand(int axis, double runTimeMilliseconds, Profile profile);

            int axis;
            double runTimeMilliseconds;
            Profile profile;
        };

        class TriggerJogCommand {
        public:
            TriggerJogCommand();
            TriggerJogCommand(int axis, Profile profile, Trigger trigger);

            int axis;
            Profile profile;
            Trigger trigger;
        };

        class PosToJogCommand {
        public:
            PosToJogCommand();
            PosToJogCommand(int axis, double target, unsigned char unsignedVelocity, Profile posProfile, Profile jogProfile, Trigger trigger);

            int axis;
            double target;
            unsigned char unsignedVelocity;
            Profile posProfile;
            Profile jogProfile;
            Trigger trigger;
        };

        class StopCommand {
        public:
            StopCommand();
            StopCommand(int axis, double dec);

            int axis;
            double dec;
        };

        class TimeCommand {
        public:
            TimeCommand();
            TimeCommand(int axis, double timeMilliseconds);

            int axis;
            double timeMilliseconds;
        };

        class SimulatePosCommand {
        public:
            SimulatePosCommand();
            SimulatePosCommand(PosCommand posCommand, unsigned char setStartPos, double startPos);

            PosCommand posCommand;
            unsigned char setStartPos;
            double startPos;
        };

        class SimulateLinearIntplCommand {
        public:
            SimulateLinearIntplCommand();

            LinearIntplCommand linearIntplCommand;
            unsigned char setStartPos[constants::maxAxes];
            double startPos[constants::maxAxes];
        };

        class WaitConditionType {
        public:
            enum T {
                AxisIdle,
                MotionStarted,
                MotionStartedOverrideReady,
                Event
            };
        };

        class WaitCondition {
        public:
            WaitCondition();

            WaitConditionType::T waitConditionType;
            int axisCount;
            int axis[constants::maxAxes];
            unsigned int eventID;
        };

        WMX3APIFUNC StartPos(PosCommand *pPosCommand);
        WMX3APIFUNC StartMov(PosCommand *pPosCommand);
        WMX3APIFUNC StartPos(TriggerPosCommand *pPosCommand);
        WMX3APIFUNC StartMov(TriggerPosCommand *pPosCommand);
        WMX3APIFUNC StartPos(PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartMov(PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartPos(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC StartMov(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC StartPos(unsigned int numCommands, TriggerPosCommand *pPosCommand);
        WMX3APIFUNC StartMov(unsigned int numCommands, TriggerPosCommand *pPosCommand);
        WMX3APIFUNC StartPos(unsigned int numCommands, PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartMov(unsigned int numCommands, PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartLinearIntplPos(LinearIntplCommand *pLinearIntplCommand);
        WMX3APIFUNC StartLinearIntplMov(LinearIntplCommand *pLinearIntplCommand);
        WMX3APIFUNC StartLinearIntplPos(LinearIntplCommand *pLinearIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartLinearIntplMov(LinearIntplCommand *pLinearIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartLinearIntplPos(LinearIntplCommand *pLinearIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartLinearIntplMov(LinearIntplCommand *pLinearIntplCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartCircularIntplPos(CenterAndLengthCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplMov(CenterAndLengthCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplPos(CenterAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplMov(CenterAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplPos(ThroughAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplMov(ThroughAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplPos(LengthAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplMov(LengthAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplPos(RadiusAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplMov(RadiusAndEndCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplPos(ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand);
        WMX3APIFUNC StartCircularIntplMov(ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand);

        WMX3APIFUNC StartCircularIntplPos(CenterAndLengthCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplMov(CenterAndLengthCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplPos(CenterAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplMov(CenterAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplPos(ThroughAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplMov(ThroughAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplPos(LengthAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplMov(LengthAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplPos(RadiusAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplMov(RadiusAndEndCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplPos(ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartCircularIntplMov(ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, Trigger *pTrigger);

        WMX3APIFUNC StartCircularIntplPos(CenterAndLengthCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplMov(CenterAndLengthCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplPos(CenterAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplMov(CenterAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplPos(ThroughAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplMov(ThroughAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplPos(LengthAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplMov(LengthAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplPos(RadiusAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplMov(RadiusAndEndCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplPos(ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartCircularIntplMov(ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartHelicalIntplPos(HelicalIntplCommand *pHelicalIntplCommand);
        WMX3APIFUNC StartHelicalIntplMov(HelicalIntplCommand *pHelicalIntplCommand);
        WMX3APIFUNC StartHelicalIntplPos(HelicalIntplCommand *pHelicalIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartHelicalIntplMov(HelicalIntplCommand *pHelicalIntplCommand, Trigger *pTrigger);
        WMX3APIFUNC StartHelicalIntplPos(HelicalIntplCommand *pHelicalIntplCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartHelicalIntplMov(HelicalIntplCommand *pHelicalIntplCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartVelToPos(TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartVelToMov(TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartVelToPos(unsigned int numCommands, TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartVelToMov(unsigned int numCommands, TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartVelToPos(PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartVelToMov(PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartVelToPos(unsigned int numCommands, PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartVelToMov(unsigned int numCommands, PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartTrqToPos(TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartTrqToMov(TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartTrqToPos(unsigned int numCommands, TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartTrqToMov(unsigned int numCommands, TriggerPosCommand *pTriggerPosCommand);
        WMX3APIFUNC StartTrqToPos(PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartTrqToMov(PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartTrqToPos(unsigned int numCommands, PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartTrqToMov(unsigned int numCommands, PosCommand *pPosCommand, TriggerEvents *pTriggerEvents);

        WMX3APIFUNC StartJog(JogCommand *pJogCommand);
        WMX3APIFUNC StartJog(TimedJogCommand *pTimedJogCommand);

        WMX3APIFUNC StartJog(unsigned int numCommands, JogCommand *pJogCommand);
        WMX3APIFUNC StartJog(unsigned int numCommands, TimedJogCommand *pTimedJogCommand);

        WMX3APIFUNC StartPosToJog(TriggerJogCommand *pTriggerJogCommand);
        WMX3APIFUNC StartPosToJog(unsigned int numCommands, TriggerJogCommand *pTriggerJogCommand);
        WMX3APIFUNC StartPosToJog(JogCommand *pJogCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartPosToJog(unsigned int numCommands, JogCommand *pJogCommand, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartPosToJog(PosToJogCommand *pPosToJogCommand);
        WMX3APIFUNC StartPosToJog(unsigned int numCommands, PosToJogCommand *pPosToJogCommand);
        WMX3APIFUNC StartMovToJog(PosToJogCommand *pPosToJogCommand);
        WMX3APIFUNC StartMovToJog(unsigned int numCommands, PosToJogCommand *pPosToJogCommand);

        WMX3APIFUNC Stop(int axis);
        WMX3APIFUNC Stop(AxisSelection *pAxisSelection);
        WMX3APIFUNC Stop(int axis, double dec);
        WMX3APIFUNC Stop(unsigned int numCommands, StopCommand *pStopCommand);
        WMX3APIFUNC ExecQuickStop(int axis);
        WMX3APIFUNC ExecQuickStop(AxisSelection *pAxisSelection);
        WMX3APIFUNC ExecTimedStop(int axis, double timeMilliseconds);
        WMX3APIFUNC ExecTimedStop(AxisSelection *pAxisSelection, double timeMilliseconds);
        WMX3APIFUNC ExecTimedStop(unsigned int numCommands, TimeCommand *pTimeCommand);
        WMX3APIFUNC Wait(int axis);
        WMX3APIFUNC Wait(AxisSelection *pAxisSelection);
        WMX3APIFUNC Wait(WaitCondition *pWaitCondition);
        WMX3APIFUNC Wait(int axis, unsigned int waitTimeMilliseconds);
        WMX3APIFUNC Wait(AxisSelection *pAxisSelection, unsigned int waitTimeMilliseconds);
        WMX3APIFUNC Wait(WaitCondition *pWaitCondition, unsigned int waitTimeMilliseconds);
        WMX3APIFUNC Pause(int axis);
        WMX3APIFUNC Pause(AxisSelection *pAxisSelection);
        WMX3APIFUNC Resume(int axis);
        WMX3APIFUNC Resume(AxisSelection *pAxisSelection);

        WMX3APIFUNC OverridePos(PosCommand *pPosCommand);
        WMX3APIFUNC OverrideMov(PosCommand *pPosCommand);
        WMX3APIFUNC OverrideVel(PosCommand *pPosCommand);
        WMX3APIFUNC OverrideAcc(PosCommand *pPosCommand);
        WMX3APIFUNC OverrideDec(PosCommand *pPosCommand);
        WMX3APIFUNC OverrideJerkAcc(PosCommand *pPosCommand);
        WMX3APIFUNC OverrideJerkDec(PosCommand *pPosCommand);
        WMX3APIFUNC OverrideProfile(PosCommand *pPosCommand);

        WMX3APIFUNC OverridePos(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC OverrideMov(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC OverrideVel(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC OverrideAcc(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC OverrideDec(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC OverrideJerkAcc(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC OverrideJerkDec(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC OverrideProfile(unsigned int numCommands, PosCommand *pPosCommand);

        WMX3APIFUNC StopJogAtPos(PosCommand *pPosCommand);
        WMX3APIFUNC StopJogAtPos(unsigned int numCommands, PosCommand *pPosCommand);

        WMX3APIFUNC SuperimposeMov(PosCommand *pPosCommand);
        WMX3APIFUNC SuperimposeMov(unsigned int numCommands, PosCommand *pPosCommand);
        WMX3APIFUNC StopSuperimpose(int axis);
        WMX3APIFUNC StopSuperimpose(AxisSelection *pAxisSelection);

        WMX3APIFUNC SimulatePos(SimulatePosCommand *pSimulatePosCommand, double *pPeakVelocity,
            double *pTotalTimeMilliseconds, double *pAccelerationTimeMilliseconds,
            double *pCruiseTimeMilliseconds, double *pDecelerationTimeMilliseconds);
        WMX3APIFUNC SimulateLinearIntplPos(SimulateLinearIntplCommand *pSimulateLinearIntplCommand,
            double *pPeakVelocity, double *pTotalTimeMilliseconds, double *pAccelerationTimeMilliseconds,
            double *pCruiseTimeMilliseconds, double *pDecelerationTimeMilliseconds);
        WMX3APIFUNC SimulatePosAtTime(SimulatePosCommand *pSimulatePosCommand, int returnType,
            double timeMilliseconds, double *pCalculatedPos, double *pCalculatedVel);
        WMX3APIFUNC SimulatePosAtTime(SimulateLinearIntplCommand *pSimulateLinearIntplCommand, double timeMilliseconds,
            double *pPosArray, double *pMoveDistance, double *pRemainDistance, double *pTotalDistance);
        WMX3APIFUNC SimulateTimeAtPos(SimulatePosCommand *pSimulatePosCommand, double specificPos,
            double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds);
        WMX3APIFUNC SimulateTimeAtDist(SimulateLinearIntplCommand *pSimulateLinearIntplCommand, double specificDistance,
            double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds, double *pTotalTimeMilliseconds);
    };



    class CoreMotion{
        friend class AxisControl;
        friend class Motion;
        friend class Home;
        friend class Velocity;
        friend class Torque;
        friend class Sync;
        friend class Config;

    private:
        WMX3Api *wmx3Api;
        int statChnlId;
        HANDLE acyclicStatusHndl;
        void *acyclicStatusMem;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
    public:
        CoreMotion(WMX3Api *f);
        CoreMotion(const CoreMotion& src);
        CoreMotion& operator=(const CoreMotion& src);
        CoreMotion();
        ~CoreMotion();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC GetStatus(CoreMotionStatus* status);
        WMX3APIFUNC ClearVibrationStatus(int axis);
        WMX3APIFUNC ClearVibrationStatus(AxisSelection* pAxisSelection);
        WMX3APIFUNC ExecEStop(EStopLevel::T level);
        WMX3APIFUNC ReleaseEStop();
        WMX3APIFUNC TriggerFlightRecorder();
        WMX3APIFUNC ResetFlightRecorder();

        AxisControl *axisControl;
        Motion *motion;
        Home *home;
        Velocity *velocity;
        Torque *torque;
        Sync *sync;
        Config *config;
    };
}

#endif
