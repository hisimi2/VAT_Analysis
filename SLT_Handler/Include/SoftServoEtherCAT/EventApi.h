/**********************************************************************************************************************
*
* EventApi.h
*
* This file contains the declarations of the Event module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the Event module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_EVENT_API_H
#define WMX3_EVENT_API_H
#include <windows.h>
#include "WMX3Api.h"

namespace wmx3Api{

    namespace constants {
        static const int maxEvents = 512;
        static const int maxBitArray = 1024;
        static const int maxHardwareTouchProbeLatchedValues = 256;
        static const int maxTouchprobeChannel = 64;
        static const int maxPsoChannel = 64;
        static const int maxPsoData = 128;
        static const int maxPsoIoEncSize = 4;
        static const int maxPveloChannel = 64;
        static const int maxPveloData = 20;
        static const int maxPveloIoEncSize = 4;
        static const int maxEventInputDataSize = 8192;
        static const int maxEventOutputDataSize = 8192;
        static const int maxEventConfigureOfModuleDataSize = 8192;
    }

    class EventErrorCode : public ErrorCode{
    public:
        enum {
            MaxEventDefinitionsDefined = 0x00016000,
            MaxEventInputsDefined,
            MaxEventOutputsDefined,
            MaxEventBitArrayUsed,
            IDOutOfRange,
            IDNotDefined,
            IDBusyUpdating,
            ChannelUninitialized,
            IDInUse,
            HardTouchProbeChannelOutOfRange,
            EventInDataSizeOutOfRange,
            EventOutDataSizeOutOfRange,
            EventConfigureOfModuleDataSizeOutOfRange,
            EventNotDefined,
            EventInputModuleIdNotMatched,
            EventOutputModuleIdNotMatched,
            EventInputDataAllocateFailed,
            EventOutputDataAllocateFailed,
            GetFunctionDoesNotMatchSetFunction,
            ChannelEnabled,
            InterruptIdForActiveChannelError,
            InvalidModuleID,
            EventIsEnabled,
            EventIsClearing,
            SourceTypeOutOfRange,
            PointCountOutOfRange,
            IntervalOutOfRange
        };
    };

    class EventOption{
    public:
        EventOption();
        bool singleShot;
        bool disableAfterActivate;
        bool enable;
    };

    class CustomEventOption{
    public:
        CustomEventOption();

        EventOption option;
        char initialState;
    };

    class AllEventID{
    public:
        AllEventID();
        int count;
        int id[constants::maxEvents];
    };

    class EventApiEventInput : public EventInput{
    public:
        enum EventApiEventInputType {
            None,
            AnotherEvent,
            DelayAnotherEvent,
            OrEvent,
            AndEvent,
            XorEvent,
            DeviceCloseEvent,
            DeviceTimeoutEvent
        };

        EventApiEventInputType inputFunction;

        union InputFunctionArguments {
            InputFunctionArguments();

            struct None {
                unsigned int reserved;
            }none;

            struct AnotherEvent {
                unsigned int eventID;
                unsigned char invert;
            }anotherEvent;

            struct DelayAnotherEvent {
                unsigned int eventID;
                unsigned char invert;
                unsigned int delayTime;
            }delayAnotherEvent;

            struct OrEvent {
                unsigned int eventID[2];
                unsigned char invert[2];
                unsigned char invertResult;
            }orEvent;

            struct AndEvent {
                unsigned int eventID[2];
                unsigned char invert[2];
                unsigned char invertResult;
            }andEvent;

            struct XorEvent {
                unsigned int eventID[2];
                unsigned char invert[2];
                unsigned char invertResult;
            }xorEvent;

            struct DeviceCloseEvent {
                int deviceId;
            } deviceCloseEvent;

            struct DeviceTimeoutEvent {
                int deviceId;
            } deviceTimeoutEvent;
        }input;

        EventApiEventInput();

        int GetInputModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, int* dataSize, char *initialState);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, int dataSize);
    };

    class EventApiEventOutput : public EventOutput{
    public:
        enum EventApiEventOutputType {
            None,
            EnableAnotherEvent,
        };

        EventApiEventOutputType outputFunction;

        union OutputFunctionArguments {
            OutputFunctionArguments();

            struct None {
                unsigned int reserved;
            }none;

            struct EnableAnotherEvent {
                unsigned int targetEventID;
                unsigned char invert;
                unsigned char setOffState;
            }enableAnotherEvent;
        }output;

        EventApiEventOutput();

        int GetOutputModuleId();
        WMX3APIFUNC GetOutputData(unsigned char* buff, int buffSize, int* dataSize);
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* data, int dataSize);
    };

    class EventControl {
    public:

        //[Deprecated]
        class EventInputFunction{
        public:
            enum T {
                None,
                IOBit,
                NotIOBit,
                OrIOBit,
                AndIOBit,
                XorIOBit,
                NandIOBit,
                NorIOBit,
                XnorIOBit,
                DelayIOBit,

                MBit,
                NotMBit,
                OrMBit,
                AndMBit,
                XorMBit,
                NandMBit,
                NorMBit,
                XnorMBit,
                DelayMBit,

                AnotherEvent,
                DelayAnotherEvent,

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
                PosSET,
                DelayedPosSET,
                CommandDistributedEnd,
                RemainingTime,
                RemainingDistance,
                CompletedTime,
                CompletedDistance,

                Unknown
            };
        };

        //[Deprecated]
        class EventOutputFunction{
        public:
            enum T {
                None,
                SetIOOutBit,
                SetMBit,
                EnableAnotherEvent,
                StopSingleAxis,
                StartSinglePos,
                StartSingleMov,
                StartMultiplePos,
                StartMultipleMov,
                LinearIntplPos,
                LinearIntplMov,
                StartAPIBuffer,
                ExecQuickStopSingleAxis,
                OverrideVelSingleAxis,
                ExecEStop,
                TriggerFlightRecorder,
                ResetFlightRecorder,
                Unknown
            };
        };

        //[Deprecated]
        class Event{
        public:
            Event();
            EventInputFunction::T inputFunction;
            union InputFunctionArguments {
                InputFunctionArguments();

                struct None {
                    unsigned int reserved;
                }none;

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

                struct DelayIOBit {
                    unsigned int byteAddress;
                    unsigned char bitAddress;
                    unsigned char invert;
                    IOSourceType::T ioSourceType;
                    unsigned int delayTime;
                }delayIOBit;

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

                struct DelayMBit {
                    unsigned int byteAddress;
                    unsigned char bitAddress;
                    unsigned char invert;
                    unsigned int delayTime;
                }delayMBit;

                struct AnotherEvent {
                    unsigned int eventID;
                    unsigned char invert;
                }anotherEvent;

                struct DelayAnotherEvent {
                    unsigned int eventID;
                    unsigned char invert;
                    unsigned int delayTime;
                }delayAnotherEvent;

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
            }input;
            EventOutputFunction::T outputFunction;
            union OutputFunctionArguments {
                OutputFunctionArguments();

                struct None {
                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }none;

                struct SetIOOutBit {
                    unsigned int byteAddress;
                    unsigned char bitAddress;
                    unsigned char invert;

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                    unsigned char setOffState;
                }setIOOutBit;

                struct SetMBit {
                    unsigned int byteAddress;
                    unsigned char bitAddress;
                    unsigned char invert;

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                    unsigned char setOffState;
                }setMBit;

                struct EnableAnotherEvent {
                    unsigned int eventID;
                    unsigned char invert;

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                    unsigned char setOffState;
                }enableAnotherEvent;

                struct StopSingleAxis {
                    int axis;

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
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

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
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

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
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

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
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

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
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

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
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

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }linearIntplMov;

                struct StartAPIBuffer {
                    unsigned int channel;
                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }startAPIBuffer;

                struct ExecQuickStopSingleAxis {
                    int axis;

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }execQuickStopSingleAxis;

                struct OverrideVelSingleAxis {
                    int axis;
                    double velocity;

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }overrideVelSingleAxis;

                struct ExecEStop {
                    EStopLevel::T eStopLevel;

                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }execEStop;

                struct TriggerFlightRecorder {
                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }triggerFlightRecorder;

                struct ResetFlightRecorder {
                    unsigned char singleShot;
                    unsigned char disableAfterActivate;
                }resetFlightRecorder;
            }output;
            unsigned char enabled;
        };

        class TouchProbeSource {
        public:
            enum T {
                ZPulse,
                TouchProbe
            };
        };

        class TouchProbeMode {
        public:
            enum T {
                LatchFirst,
                LatchLast
            };
        };

        class HardwareTouchProbeStatus{
        public:
            HardwareTouchProbeStatus();
            int axis;
            unsigned char enabled;
            TouchProbeMode::T mode;
            TouchProbeSource::T triggerSource;
            unsigned int channel;
            unsigned char latched;
            unsigned int latchedValueCount;
            unsigned int latchedValue[constants::maxHardwareTouchProbeLatchedValues];
            double latchedPos[constants::maxHardwareTouchProbeLatchedValues];
        };

        class PSOOutputType{
        public:
            enum T {
                IOOutput,
                UserMemory
            };
        };

        class PSOOutput {
        public:
            PSOOutput();
            PSOOutput(PSOOutputType::T outputType, unsigned int byteAddress, unsigned char bitAddress, unsigned char invert);

            PSOOutputType::T outputType;
            unsigned int byteAddress;
            unsigned char bitAddress;
            unsigned char invert;
        };

        class PSOStatus{
        public:
            PSOStatus();
            unsigned char enabled;
            unsigned char active;
            int activeDataIndex;
            PSOOutput output;
        };

        class PSOOption {
        public:
            PSOOption();
            unsigned char allowTriggerOnStart;
        };

        class ComparatorSourceType {
        public:
            enum T {
                PosCommand,
                PosFeedback,
                FollowingError
            };
        };

        class ComparatorSource{
        public:
            ComparatorSource();
            ComparatorSource(ComparatorSourceType::T sourceType, int axis);

            ComparatorSourceType::T sourceType;
            int axis;
        };

        class PlannedVelocityData{
        public:
            PlannedVelocityData();
            PlannedVelocityData(double pos, double velocity);

            double pos;
            double velocity;
        };

        class PlannedVelocityStatus{
        public:
            PlannedVelocityStatus();
            unsigned char enabled;
            unsigned char active;
            int activeDataIndex;
            double activeVelocityCommand;
            int axis;
        };

        class ComparisonType {
        public:
            enum T {
                Disable,
                Equal,
                PositiveDirection,
                NegativeDirection,
                MoreThan,
                LessThan
            };
        };

    private:
        WMX3Api *wmx3Api;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
    public:
        EventControl(WMX3Api *f);
        EventControl(const EventControl& src);
        EventControl& operator=(const EventControl& src);
        EventControl();
        ~EventControl();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC SetEvent(int* pId, EventInput *pEventInput, EventOutput *pEventOutput, EventOption* pOption = NULL);
        WMX3APIFUNC SetEvent(int* pId, EventInput *pEventInput, EventOutput *pEventOutput, int id, EventOption* pOption = NULL);
        WMX3APIFUNC GetEventModuleId(int id, int* pInputModuleId, int* pOutputModuleId);
        WMX3APIFUNC GetEvent(int id, EventInput *pEventInput, EventOutput *pEventOutput, EventOption* pOption = NULL);
        WMX3APIFUNC GetEventOption(int id, EventOption* pOption);

        WMX3APIFUNC SetEventInput(int id, EventInput *pEventInput);
        WMX3APIFUNC SetEventOutput(int id, EventOutput *pEventOutput);

        WMX3APIFUNC GetEventInput(int id, EventInput* pEventInput);
        WMX3APIFUNC GetEventOutput(int id, EventOutput* pEventOutput);

        WMX3APIFUNC GetAllEventID(AllEventID *pEventIdData, int filterInputModuleId = -1, int filterOutputModuleId = -1);
        WMX3APIFUNC EnableEvent(int id, unsigned char enable);
        WMX3APIFUNC RemoveEvent(int id);
        WMX3APIFUNC ClearAllEvent();

        //[For Custom Event Developers]
        WMX3APIFUNC SetCustomEvent(int* pId, int inputModuleId, void* pInputData, int inputDataSize,
            int outputModuleId, void* pOutputData, int outputDataSize, int id, CustomEventOption* pOption = NULL);
        //[For Custom Event Developers]
        WMX3APIFUNC SetCustomEventInput(int id, int moduleId, void* pInputData, int inputDataSize);
        //[For Custom Event Developers]
        WMX3APIFUNC SetCustomEventOutput(int id, int moduleId, void* pOutputData, int outputDataSize);
        //[For Custom Event Developers]
        WMX3APIFUNC GetCustomEventInput(int id, void* pInputDataBuff, int inputDataBuffSize, int* pInputDataSize, int* pModuleId);
        //[For Custom Event Developers]
        WMX3APIFUNC GetCustomEventOutput(int id, void* pOutputDataBuff, int outputDataBuffSize, int* pOutputDataSize, int* pModuleId);
        //[For Custom Event Developers]
        WMX3APIFUNC SetCustomEventConfigureOfModule(int moduleId, void* pSetEventData, int setEventDataSize);

        //[Deprecated]
        WMX3APIFUNC SetEvent(int *pId, Event *pEventData);
        //[Deprecated]
        WMX3APIFUNC SetEvent(int *pId, Event *pEventData, int id);
        //[Deprecated]
        WMX3APIFUNC GetEvent(int id, Event *pEventData);

        WMX3APIFUNC SetSoftwareTouchProbe(unsigned int channel, unsigned char enable, int axis, int byteAddrs,
            int bitOffset, unsigned char logic, TouchProbeMode::T mode);
        WMX3APIFUNC EnableSoftwareTouchProbe(unsigned int channel, unsigned char enable);
        WMX3APIFUNC GetSoftwareTouchProbe(unsigned int channel, unsigned char *pEnabled, int *pAxis,
            int *pByteAddrs, int *pBitOffset, unsigned char *pLogic, TouchProbeMode::T *pMode);
        WMX3APIFUNC IsSoftwareTouchProbeLatched(unsigned int channel, unsigned char *pLatched);
        WMX3APIFUNC GetSoftwareTouchProbeCounterValue(unsigned int channel, unsigned char *pLatched,
            double *pCounterValue);
        WMX3APIFUNC SetHardwareTouchProbe(int axis, unsigned char enable, TouchProbeMode::T mode,
            TouchProbeSource::T triggerSource, unsigned int channel);
        WMX3APIFUNC GetHardwareTouchProbeStatus(int axis, HardwareTouchProbeStatus *pStatus);
        WMX3APIFUNC EnableHardwareTouchProbe(int axis, unsigned char enable);

        WMX3APIFUNC SetPSOConfig(unsigned int channel, ComparisonType::T type, ComparatorSource *pSource,
            PSOOutput *pOutput, double minDurationMilliseconds, PSOOption *pOption = NULL);
        WMX3APIFUNC GetPSOConfig(unsigned int channel, ComparisonType::T *pType, ComparatorSource *pSource,
            PSOOutput *pOutput, double *pMinDurationMilliseconds, PSOOption *pOption = NULL);
        WMX3APIFUNC SetPSOSingleData(unsigned int channel, double sData);
        WMX3APIFUNC SetPSOMultipleData(unsigned int channel, int count, double *pMData);
        WMX3APIFUNC SetPSOIntervalData(unsigned int channel, double rangeStart, double rangeEnd, double interval);
        WMX3APIFUNC GetPSOData(unsigned int channel, int *pCount, double *pMData, int maxCount);
        WMX3APIFUNC GetPSODataCount(unsigned int channel, int *pCount);
        WMX3APIFUNC GetPSOIntervalData(unsigned int channel, double *pRangeStart, double *pRangeEnd, double *pInterval);
        WMX3APIFUNC StartPSO(unsigned int channel);
        WMX3APIFUNC StopPSO(unsigned int channel);
        WMX3APIFUNC GetPSOStatus(unsigned int channel, PSOStatus *pStatus);

        WMX3APIFUNC SetPlannedVelOverrideConfig(unsigned int channel, ComparisonType::T type,
            ComparatorSource *pSource, int axis);
        WMX3APIFUNC GetPlannedVelOverrideConfig(unsigned int channel, ComparisonType::T *pType,
            ComparatorSource *pSource, int *pAxis);
        WMX3APIFUNC SetPlannedVelOverrideSingleData(unsigned int channel, PlannedVelocityData sData);
        WMX3APIFUNC SetPlannedVelOverrideMultipleData(unsigned int channel, int count, PlannedVelocityData *pMData);
        WMX3APIFUNC GetPlannedVelOverrideData(unsigned int channel, int *pCount, PlannedVelocityData *pMData, int maxCount);
        WMX3APIFUNC GetPlannedVelOverrideDataCount(unsigned int channel, int *pCount);
        WMX3APIFUNC StartPlannedVelOverride(unsigned int channel);
        WMX3APIFUNC StopPlannedVelOverride(unsigned int channel);
        WMX3APIFUNC GetPlannedVelOverrideStatus(unsigned int channel, PlannedVelocityStatus *pStatus);
    };
}

#endif
