/**********************************************************************************************************************
*
* ApiBufferApi.h
*
* This file contains the declarations of the ApiBuffer module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the ApiBuffer module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_APIBUFFER_API_H
#define WMX3_APIBUFFER_API_H
#include <windows.h>
#include "WMX3Api.h"


namespace wmx3Api{

    namespace constants {
        static const int maxApiBufferChannel = 255;
        static const int maxApiBufferErrorLog = 10;
        static const int maxDefaultApiBufferSize = 524288;
    }

    class ApiBufferErrorCode : public ErrorCode{
    public:
        enum {
            OverMaxLevel = 0x00012000,
            InvalidLevel,
            InvalidBuffControl,
            InvalidWatchTriggerRoutineChannel,
            WatchTriggerRoutineChannelSameAsExecChannel,
            RewindFailedFirstCommandOverwritten,
            AlreadyRecordingForSpecifiedChannel,
            NotRecording
        };
    };

    class ApiBufferConditionType {
    public:
        enum T {
            NeverTrue,
            AlwaysTrue,
            IOInput,
            IOOutput,
            UserMemory,
            MinimumTrq,
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
            DistanceToTarget,
            DecelerationStarted,
            AxisIdle,
            MotionStarted,
            MotionStartedOverrideReady,
            Event
        };
    };

    class ApiBufferState {
    public:
        enum T {
            Idle,
            Active,
            Stop,
            Delay
        };
    };

    class ApiBufferCondition{
    public:
        ApiBufferCondition();
        ApiBufferConditionType::T bufferConditionType;
        union ApiBufferConditionArguments {
            ApiBufferConditionArguments();

            struct NeverTrue {
                int reserved;
            }neverTrue;

            struct AlwaysTrue {
                int reserved;
            }alwaysTrue;

            struct IOInput {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;
            }ioInput;

            struct IOOutput {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;
            }ioOutput;

            struct UserMemory {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;
            }userMemory;
            
            struct MinimumTrq {
                int axis;
                double torque;
            }minimumTrq;

            struct OpState {
                int axis;
                OperationState::T opState;
            }opState;

            struct AxisCmdMode {
                int axis;
                AxisCommandMode::T axisCommandMode;
                unsigned char useFeedback;
            }axisCmdMode;

            struct InPos {
                int axis;
                unsigned int channel;
            }inPos;

            struct PosSET {
                int axis;
            }posSET;

            struct DelayedPosSET {
                int axis;
            }delayedPosSET;

            struct CommandDistributedEnd {
                int axis;
            }commandDistributedEnd;

            struct RemainingTime {
                int axis;
                double timeMilliseconds;
            }remainingTime;

            struct RemainingDistance {
                int axis;
                double distance;
            }remainingDistance;

            struct CompletedTime {
                int axis;
                double timeMilliseconds;
            }completedTime;

            struct CompletedDistance{
                int axis;
                double distance;
            }completedDistance;

            struct DistanceToTarget {
                int axis;
                double distance;
            }distanceToTarget;

            struct DecelerationStarted {
                int axis;
            }decelerationStarted;

            struct AxisIdle {
                unsigned int axisCount;
                int axis[constants::maxAxes];
            }axisIdle;

            struct MotionStarted {
                int axis;
            }motionStarted;

            struct MotionStartedOverrideReady {
                int axis;
            }motionStartedOverrideReady;

            struct Event {
                unsigned int eventID;
            }event;
        }arg;
    };

    class ApiBufferErrorLog {
    public:
        ApiBufferErrorLog();
        int execBlockNumber;
        int errorCode;
    };

    class ApiBufferStatus{
    public:
        ApiBufferStatus();
        ApiBufferState::T state;
        int blockCount;
        int remainingBlockCount;
        int bufferSize;
        int freeSize;
        int execBlockCount;
        int errorCount;
        ApiBufferErrorLog errorLog[constants::maxApiBufferErrorLog];
        bool watchError;
        int watchErrorCode;
        int watchErrorAxis;
    };

    class ApiBufferOptions {
    public:
        ApiBufferOptions();
        bool stopOnError;
        bool autoRewind;
        bool stopOnLastBlock;
    };

    class ApiBufferWatch {
    public:
        ApiBufferWatch();
        bool enableWatch;
        AxisSelection watchAxes;
        bool enableWatchTriggerRoutine;
        int watchTriggerRoutineChannel;
    };

    class ApiBufferEventOutput : public EventOutput {
    public:
        enum ApiBufferEventOutputType {
            StartAPIBuffer,
            HaltApiBuffer,
        };

        ApiBufferEventOutputType outputFunction;
        union OutputFunctionArguments {
            OutputFunctionArguments();

            struct StartAPIBuffer {
                unsigned int channel;
            }startAPIBuffer;

            struct HaltAPIBuffer {
                unsigned int channel;
            }haltAPIBuffer;
        }output;

        ApiBufferEventOutput();

        int GetOutputModuleId();
        WMX3APIFUNC GetOutputData(unsigned char* buff, int buffSize, int* dataSize);
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* data, int dataSize);
    };

    class ApiBuffer {
    private:
        WMX3Api *wmx3Api;
        int statChnlId;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
    public:
        ApiBuffer(WMX3Api *f);
        ApiBuffer(const ApiBuffer& src);
        ApiBuffer& operator=(const ApiBuffer& src);
        ApiBuffer();
        ~ApiBuffer();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC CreateApiBuffer(unsigned int channel, unsigned int size);
        WMX3APIFUNC CreateApiBuffer(unsigned int channel);
        WMX3APIFUNC CreateApiBuffer(unsigned int channel, unsigned int size, SizeUnit::T sizeUnit);
        WMX3APIFUNC FreeApiBuffer(unsigned int channel);

        WMX3APIFUNC StartRecordBufferChannel(unsigned int channel);
        WMX3APIFUNC EndRecordBufferChannel();
        WMX3APIFUNC GetRecordingBufferChannel(int* channel, unsigned char* enable);

        WMX3APIFUNC Execute(unsigned int channel);
        WMX3APIFUNC Halt(unsigned int channel);
        WMX3APIFUNC Clear(unsigned int channel);
        WMX3APIFUNC Rewind(unsigned int channel);

        WMX3APIFUNC GetStatus(unsigned int channel, ApiBufferStatus* pStatus);
        WMX3APIFUNC SetOptions(unsigned int channel, ApiBufferOptions *pOptions);
        WMX3APIFUNC GetOptions(unsigned int channel, ApiBufferOptions *pOptions);
        WMX3APIFUNC SetWatch(unsigned int channel, ApiBufferWatch *pWatch);
        WMX3APIFUNC GetWatch(unsigned int channel, ApiBufferWatch *pWatch);

        WMX3APIFUNC Sleep(unsigned int milliseconds);
        WMX3APIFUNC USleep(unsigned int microseconds);
        WMX3APIFUNC Wait(int axis);
        WMX3APIFUNC Wait(AxisSelection *pAxisSelection);
        WMX3APIFUNC Wait(ApiBufferCondition* condition);

        WMX3APIFUNC FlowIf(ApiBufferCondition* condition, ApiBufferCondition* wait = NULL);
        WMX3APIFUNC FlowElseIf(ApiBufferCondition* condition);
        WMX3APIFUNC FlowElse();
        WMX3APIFUNC FlowEndIf();

    };


}

#endif

