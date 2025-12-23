/**********************************************************************************************************************
*
* LogApi.h
*
* This file contains the declarations of the Log module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used the Log module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_LOG_API_H
#define WMX3_LOG_API_H
#include <stdio.h>
#include <windows.h>
#include "WMX3Api.h"

namespace wmx3Api{

    namespace constants {
        static const unsigned int maxLogBufferSize = ((unsigned int)2048 * 1024);
        static const int maxLogBufferSampleSize = 80;
        static const int maxLogChannel = 8;
        static const int maxLogHeaderBytes = 4096;
        static const int maxLogHeaderLines = 2048;
        static const int maxLogDirSize = 260;
        static const int maxLogFileNameSize = 260;
        static const int maxLogDataSize = (32 * 1024);
        static const int maxLogDelimiterSize = 8;
        static const int maxLogPrecision = 20;

        static const int maxMemLogBufferSize = 1024;
        static const int maxMemLogChannel = 8;
        static const int maxMemLogAxesSize = 8;
        static const int maxMemLogDataSize = 100;
        static const int maxMemLogIoInputByteSize = 128;
        static const int maxMemLogIoOutputByteSize = 128;
        static const int maxMemLogMDataByteSize = 128;
        static const int maxMemLogTriggerEventSize = 32;
    }

    class LogErrorCode : public ErrorCode{
    public:
        enum {
            LogTimeIsZero = 0x00011000,
            LogBufferIsNotOpened,
            LogBufferAlreadyOpened,
            LogBufferAlreadyClosed,
            LogBufferOpenFailed,
            LogBufferIsNull,
            LockLogBufferFailed,
            SetLogIsNotFinished,
            LogBufferDataNotFound,
            CurrentlyCollectingLogData,
            CurrentlyNotCollectingLogData,
            LengthOutOfRange,
            FilePathStringTooLarge,
            FileNameNotSpecified,
            FileDirectoryNotSpecified,
            PrecisionOutOfRange,
            InvalidFilePath,
            TriggerEventCountOutOfRange,
            TriggerEventIDOutOfRange,
            SamplingPeriodOutOfRange,
            MaxLogFileCountSetButNotMaxLogFileSize,
            IOFormatCountOutOfRange,
            UserMemoryFormatCountOutOfRange,
            DataSourceModuleNotLoaded,
			NoMoreLogData
        };
    };

    class LogState {
    public:
        enum T {
            Idle,
            Running,
            WriteFail,
            BufferOverflow,
            Finished
        };
    };

    class LogStatus{
    public:
        LogStatus();
        bool bufferOpened;
        LogState::T logState;
        unsigned int samplesToCollect;
        unsigned int samplesCollected;
        unsigned int samplesWritten;
        double millisecondsToCollect;
        double millisecondsCollected;
        double millisecondsWritten;
    };

    class LogOptions{
    public:
        LogOptions();
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
        unsigned char inPosFlag;
        unsigned char inPosFlag2;
        unsigned char inPosFlag3;
        unsigned char inPosFlag4;
        unsigned char inPosFlag5;
        unsigned char commandDistributionEndFlag;
        unsigned char posSetFlag;
        unsigned char delayedPosSetFlag;
        unsigned char opState;
        unsigned char detailOpState;
        unsigned char userOffsetCommandPos;
        unsigned char userOffsetFeedbackPos;
        unsigned char axisCommandMode;
        unsigned char axisCommandModeFeedback;
        unsigned char triggerOnCommandChange;
        unsigned char triggerOnEvent;
        unsigned int triggerEventID;
    };

    //[Deprecated]
    class IOLogFormat{
    public:
        IOLogFormat();
        int byte;
        int bit;
        FormatType::T ioFormatType;
    };

    //[Deprecated]
    class MLogFormat {
    public:
        MLogFormat();
        int byte;
        int bit;
        FormatType::T mFormatType;
    };

    class MemoryLogStatus{
    public:
        MemoryLogStatus();
        bool bufferOpened;
        LogState::T logState;
        unsigned int samplesToCollect;
        unsigned int samplesCollected;
        double usageRate;
        unsigned char overflowFlag;
        double interruptPeriod;
    };

    class MemoryLogAxisData{
    public:
        MemoryLogAxisData();
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
        unsigned char inPosFlag;
        unsigned char inPosFlag2;
        unsigned char inPosFlag3;
        unsigned char inPosFlag4;
        unsigned char inPosFlag5;
        unsigned char commandDistributionEndFlag;
        unsigned char posSetFlag;
        unsigned char delayedPosSetFlag;
        int opState;
        int detailOpState;
        double userOffsetCommandPos;
        double userOffsetFeedbackPos;
        int axisCommandMode;
        int axisCommandModeFeedback;
    };

    class MemoryLogIOData{
    public:
        MemoryLogIOData();
        char input[constants::maxMemLogIoInputByteSize];
        char output[constants::maxMemLogIoOutputByteSize];
    };

    class MemoryLogMData {
    public:
        MemoryLogMData();
        char data[constants::maxMemLogMDataByteSize];
    };

    class MemoryLogOptions{
    public:
        MemoryLogOptions();
        unsigned char triggerEventCount;
        unsigned int triggerEventID[constants::maxMemLogTriggerEventSize];
    };

    class MemoryLogDatas{
    public:
        MemoryLogDatas();
        long long cycleCounter;
        MemoryLogAxisData logAxisData[constants::maxMemLogAxesSize];
        MemoryLogIOData logIOData;
        MemoryLogMData logMData;
    };

    class MemoryLogData{
    public:
        MemoryLogData();
        unsigned int count;
        unsigned char overflowFlag;
        MemoryLogDatas logData[constants::maxMemLogDataSize];
    };

    class LogChannelOptions {
    public:
        LogChannelOptions();
        unsigned int maxLogFileSize;
        unsigned int maxLogFileCount;
        unsigned int samplingTimeMilliseconds;
        bool isRotateFile;
        bool stopLoggingOnBufferOverflow;

        unsigned int samplingPeriodInCycles;
        unsigned int precision;

        bool isDelimInLastCol;
        char delimiter[constants::maxLogDelimiterSize];
        unsigned char triggerOnCondition;
        unsigned char triggerOnEvent;
        unsigned int triggerEventID;
    };

    class LogFilePathA {
    public:
        LogFilePathA();
        char dirPath[constants::maxLogDirSize];
        char fileName[constants::maxLogFileNameSize];
    };

    class LogFilePathW {
    public:
        LogFilePathW();
        wchar_t dirPath[constants::maxLogDirSize];
        wchar_t fileName[constants::maxLogFileNameSize];
    };

    class DetailLogState {
    public:
        enum T {
            Idle,
            WaitingForTrigger,
            Running,
            Pause,
            Stop,
            WriteFail,
            BufferOverflow,
            Finished
        };
    };

    class DetailLogBufferStatus {
    public:
        DetailLogBufferStatus();
        unsigned char opened;
        unsigned int samplesToCollect;
        unsigned int samplesCollected;
        double millisecondsToCollect;
        double millisecondsCollected;
    };

    class DetailLogStatus {
    public:
        DetailLogStatus();
        DetailLogState::T state;
        DetailLogBufferStatus buffer;
        double interruptPeriod;
        unsigned int logFileCount;
        unsigned int samplesWrittenSize;
        unsigned int samplesWritten;
        double millisecondsWritten;
    };

    class DetailLogMemoryStatus {
    public:
        DetailLogMemoryStatus();
        DetailLogState::T state;
        DetailLogBufferStatus buffer;
        double usageRate;
        unsigned char overflowFlag;
    };

    class LogApiLogInput : public LogInput {
    public:
        enum LogInputType {
            CycleCounter,
            Time,
        };
        LogApiLogInput();
        LogApiLogInput(LogInputType type);
        LogInputType type;
        unsigned int GetModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, unsigned int* dataSize);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, unsigned int dataSize);
    };

    class LogApiLogOutput : public LogOutput {
    public:
        unsigned int GetModuleId();
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* cfgData, unsigned int cfgDataSize, unsigned char* data, unsigned int dataSize, unsigned int dataIndex, unsigned int storeIndex);
    };

    class LogType {
    public:
        enum T {
            Log,
            MemoryLog
        };
    };

    class ApiLogType {
	public:
		enum T {
			Command,
			Response
		};
	};

	class ApiLogInfo{
	public:
		ApiLogInfo();
		int majorVer;
		int minorVer;
		int revision;

		int devId;
		int moduleId;
		int mode;
		ApiLogType::T type;
		unsigned int dataLen;
		unsigned short logId;
		long long timestamp;
	};

	class ApiLogOptions{
	public:
		ApiLogOptions();
		unsigned int sizePerFile;
		bool deviceExcluded[constants::maxDevices];
		bool moduleExcluded[constants::moduleLen];
		bool moduleResp[constants::moduleLen];
	};

	class ApiLogState {
	public:
		enum T {
			Stopped,
			Stopping,
			Running,
			Error,
			Unknown
		};
	};

	class ApiLogStatus{
	public:
		ApiLogStatus();
		ApiLogState::T state;
		unsigned int queueSize;
		unsigned int dataSize;
		int errorCode;
	};

    class Log {
    private:
        HANDLE buff;
        WMX3Api *wmx3Api;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
    public:
        Log(WMX3Api *f);
        Log(const Log& src);
        Log& operator=(const Log& src);
        Log();
        ~Log();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC TimestampToString(long long timestamp, char *pString, unsigned int size);
		static WMX3APIFUNC TimestampToString(long long timestamp, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC StartLog(unsigned int channel);
        WMX3APIFUNC StopLog(unsigned int channel);
        WMX3APIFUNC ResetLog(unsigned int channel);
        WMX3APIFUNC SetLogHeader(unsigned int channel, char **ppLine, unsigned int numLines);
        WMX3APIFUNC SetLog(unsigned int channel, LogInput* input);
        WMX3APIFUNC SetLogOption(unsigned int channel, LogChannelOptions *pOption);
        WMX3APIFUNC GetLogOption(unsigned int channel, LogChannelOptions *pOption);
        WMX3APIFUNC SetLogFilePath(unsigned int channel, LogFilePathA *pPath);
        WMX3APIFUNC GetLogFilePath(unsigned int channel, LogFilePathA *pPath);
        WMX3APIFUNC SetLogFilePath(unsigned int channel, LogFilePathW *pPath);
        WMX3APIFUNC GetLogFilePath(unsigned int channel, LogFilePathW *pPath);
        WMX3APIFUNC GetLogStatus(unsigned int channel, LogStatus *pStatus);
        WMX3APIFUNC GetDetailLogStatus(unsigned int channel, DetailLogStatus *pStatus);
        
        //[For Custom Log Developers]
        WMX3APIFUNC SetCustomLog(unsigned int channel, unsigned int moduleId, void *data, unsigned int dataSize, LogType::T type);

        //[Deprecated]
        WMX3APIFUNC SetLog(unsigned int channel, char *pPath, unsigned int milliseconds,
            unsigned int samplePeriodInCycles, AxisSelection *pAxisSelection, LogOptions *pOptions,
            unsigned int mode, unsigned int burstWriteLines = 4, unsigned int scale = 9);
        //[Deprecated]
        WMX3APIFUNC SetLog(unsigned int channel, wchar_t *pPath, unsigned int milliseconds,
            unsigned int samplePeriodInCycles, AxisSelection *pAxisSelection, LogOptions *pOptions,
            unsigned int mode, unsigned int burstWriteLines = 4, unsigned int scale = 9);
        //[Deprecated]
        WMX3APIFUNC SetIOLog(unsigned int channel, IOAddress *pInputIOAddress, unsigned int inputSize,
            IOAddress *pOutputIOAddress, unsigned int outputSize);
        //[Deprecated]
        WMX3APIFUNC SetIOLogFormat(unsigned int channel, IOLogFormat *pInputIOLogFormat, unsigned int inputIOFormatCount,
            IOLogFormat *pOutputIOLogFormat, unsigned int outputIOFormatCount);
        //[Deprecated]
        WMX3APIFUNC SetMLog(unsigned int channel, MAddress *pMAddress, unsigned int size);
        //[Deprecated]
        WMX3APIFUNC SetMLogFormat(unsigned int channel, MLogFormat *pMLogFormat, unsigned int mFormatCount);

        WMX3APIFUNC OpenMemoryLogBuffer(unsigned int channel);
        WMX3APIFUNC CloseMemoryLogBuffer(unsigned int channel);
        WMX3APIFUNC SetMemoryLog(unsigned int channel, LogInput* input);
        WMX3APIFUNC SetMemoryLogOption(unsigned int channel, MemoryLogOptions *pOption);
        WMX3APIFUNC GetMemoryLogOption(unsigned int channel, MemoryLogOptions *pOption);
        WMX3APIFUNC StartMemoryLog(unsigned int channel);
        WMX3APIFUNC StopMemoryLog(unsigned int channel);
        WMX3APIFUNC ResetMemoryLog(unsigned int channel);
        WMX3APIFUNC GetMemoryLogStatus(unsigned int channel, MemoryLogStatus *pStatus);
        WMX3APIFUNC GetMemoryLogData(unsigned int channel, LogOutput *pOutput);
        WMX3APIFUNC GetMemoryLogData(unsigned int channel, LogOutput **ppOutput, unsigned int size);

        //[Deprecated]
        WMX3APIFUNC SetMemoryLog(unsigned int channel, AxisSelection *pAxisSelection, MemoryLogOptions *pOption);
        //[Deprecated]
        WMX3APIFUNC SetMemoryIOLog(unsigned int channel, IOAddress *pInputIOAddress, unsigned int inputSize, IOAddress *pOutputIOAddress, unsigned int outputSize);
        //[Deprecated]
        WMX3APIFUNC SetMemoryMLog(unsigned int channel, MAddress *pMAddress, unsigned int size);
        //[Deprecated]
        WMX3APIFUNC GetMemoryLogData(unsigned int channel, MemoryLogData *pData);

		WMX3APIFUNC SetApiLog(char *pPath, ApiLogOptions* pOptions = NULL, unsigned int buffSize = 16777216);
		WMX3APIFUNC SetApiLog(wchar_t *pPath, ApiLogOptions* pOptions = NULL, unsigned int buffSize = 16777216);
		WMX3APIFUNC StartApiLog();
		WMX3APIFUNC StopApiLog();
		WMX3APIFUNC GetApiLogStatus(ApiLogStatus* pStatus);

		WMX3APIFUNC OpenApiLogFile(char *pPath, FILE** ppFile);
		WMX3APIFUNC OpenApiLogFile(wchar_t *pPath, FILE** ppFile);
		WMX3APIFUNC GetApiLogData(FILE* pFile, unsigned char* pDataBuff, unsigned int buffSize, ApiLogInfo* pApiLogInfo = NULL);
		WMX3APIFUNC CloseApiLogFile(FILE* pFile);
    };
}

#endif
