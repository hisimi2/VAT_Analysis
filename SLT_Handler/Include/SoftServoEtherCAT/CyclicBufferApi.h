/**********************************************************************************************************************
*
* CyclicBufferApi.h
*
* This file contains the declarations of the CyclicBuffer module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the CyclicBuffer module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_CYCLICBUFFER_API_H
#define WMX3_CYCLICBUFFER_API_H
#include <windows.h>
#include "WMX3Api.h"

using namespace wmx3Api;

namespace wmx3Api{

    class CyclicBufferErrorCode : public ErrorCode{
    public:
        enum {
            CreateBufferFailed = 0x00013000,
            BufferIsNull,
            OpenBufferError,
            BufferIsNotOpened,
            BufferSizeIsNotEnough,
            LockBufferFailed,
            BufferAlreadyOpened
        };
    };

    class CyclicBufferCommandType {
    public:
        enum T {
            AbsolutePos,
            RelativePos
        };
    };

    class CyclicBufferState {
    public:
        enum T {
            Stopped,
            ExecutingCommand,
            WaitingForCommand,
            MaxAccError
        };
    };

    class CyclicBufferSingleAxisCommand{
    public:
        CyclicBufferSingleAxisCommand();
        CyclicBufferCommandType::T type;
        double command;
        unsigned int intervalCycles;
    };


    class CyclicBufferMultiAxisCommands {
    public:
        CyclicBufferMultiAxisCommands();
        CyclicBufferSingleAxisCommand cmd[constants::maxAxes];
    };

    class CyclicBufferSingleAxisOption {
    public:
        CyclicBufferSingleAxisOption();
        double maxAcc;
    };

    class CyclicBufferMultiAxisOption {
    public:
        CyclicBufferMultiAxisOption();
        CyclicBufferSingleAxisOption option[constants::maxAxes];
    };

    class CyclicBufferSingleAxisStatus {
    public:
        CyclicBufferSingleAxisStatus();
        CyclicBufferState::T state;
        unsigned int remainCount;
        unsigned int availableCount;
    };

    class CyclicBufferMultiAxisStatus {
    public:
        CyclicBufferMultiAxisStatus();
        CyclicBufferSingleAxisStatus status[constants::maxAxes];
    };

    class CyclicBuffer{
    private:
        HANDLE buff;
        WMX3Api *wmx3Api;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
    public:
        CyclicBuffer(WMX3Api *f);
        CyclicBuffer(const CyclicBuffer& src);
        CyclicBuffer& operator=(const CyclicBuffer& src);
        CyclicBuffer();
        ~CyclicBuffer();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC OpenCyclicBuffer(int axis, unsigned int numOfCycles);
        WMX3APIFUNC OpenCyclicBuffer(AxisSelection *pAxisSelection, unsigned int numOfCycles);
        WMX3APIFUNC CloseCyclicBuffer(int axis);
        WMX3APIFUNC CloseCyclicBuffer(AxisSelection *pAxisSelection);

        WMX3APIFUNC AddCommand(int axis, CyclicBufferSingleAxisCommand *pCmd);
        WMX3APIFUNC AddCommand(int axis, CyclicBufferSingleAxisCommand *pCmd, int count);

        WMX3APIFUNC AddCommand(AxisSelection *pAxisSelection, CyclicBufferMultiAxisCommands *pCmds);
        WMX3APIFUNC AddCommand(AxisSelection *pAxisSelection, CyclicBufferMultiAxisCommands *pCmds, int count);

        WMX3APIFUNC Execute(int axis, CyclicBufferSingleAxisOption *pOption = NULL);
        WMX3APIFUNC Execute(AxisSelection *pAxisSelection, CyclicBufferMultiAxisOption *pOption = NULL);
        WMX3APIFUNC Abort(int axis);
        WMX3APIFUNC Abort (AxisSelection *pAxisSelection);
        WMX3APIFUNC ExecQuickStop(int axis);
        WMX3APIFUNC ExecQuickStop (AxisSelection *pAxisSelection);
        WMX3APIFUNC GetStatus(int axis, CyclicBufferSingleAxisStatus *pStatus);
        WMX3APIFUNC GetStatus(AxisSelection *pAxisSelection, CyclicBufferMultiAxisStatus *pStatus);
    };


}

#endif
