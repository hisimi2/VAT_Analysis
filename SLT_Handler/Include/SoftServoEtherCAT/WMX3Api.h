/**********************************************************************************************************************
*
* WMX3Api.h
*
* This file contains the declarations of the base WMX3 API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by any module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_API_LIB_H
#define WMX3_API_LIB_H
#include <windows.h>
#include <tchar.h>

#ifdef WIN32
#define WMX3APIFUNC long __stdcall
#else
#define WMX3APIFUNC HRESULT
#endif

namespace wmx3Api{

    namespace constants {
        static const int maxAxes = 128;
        static const int maxIoInSize = 8000;
        static const int maxIoOutSize = 8000;
        static const int moduleFileNameLen = 32;
        static const int moduleNameLen = 16;
        static const int moduleDescLen = 512;
        static const int moduleLen = 64;
        static const int maxDeviceName = 64;
        static const int maxDevices = 256;
        static const int maxInterrupts = 2;
        static const int maxEventInterpolationAxes = 8;
        static const int maxLogOutputDataSize = 100;
    }

    class ErrorCode{
    public:
        enum {
            None = 0x0,

            StartProcessFailed = 0x00000100,
            StartProcessNull,
            StartProcessMaxInstance,
            StartProcessLockError,
            StartProcessWaitEventFailed,
            StartProcessWaitTimeout,
            MemoryAllocFailed,
            IMLibIsNotRunning,
            ControlChannelLockFailed,
            ControlChannelEventFailed,
            ControlChannelWaitEventFailed,
            ControlChannelSharedMemoryFailed,
            ControlChannelLockWaitTimeOut,
            APIChannelError,
            APIChannelTimeOut,
            StatusChannelError,

            RtssFileNotFoundError,
            RtssStartError,
            RequestChannelTimeOut,
            FreeChannelTimeOut,
            CloseDeviceTimeOut,

            RequestChannelFailed,
            ReachMaxChannelNumber,
            FreeChannelFailed,
            InvalidChannelId,
            CloseDeviceFailed,

            StatusChannelNull,
            StatusChannelIdError,
            StatusSharedMemoryChannelIndexError,
            StatusChannelGettingInitialData,

            RequestQueueFailed,
            InvalidQueueId,
            InvalidQueueSize,
            RequestQueueTimeout,
            InvalidQueue,
            FreeQueueTimeout,
            InvalidQueueControlCommand,
            ControlQueueTimeout,
            SetQueueOptionsTimeout,
            FreeQueueFailed,
            RequestQueueIDAlreadyUsed,

            CreateDeviceLockError,
            CloseDeviceLockError,

            SetDeviceNameTimeout,
            GetActiveDeviceTimeout,

            DeviceIsNull,
            InvalidModuleId,
            InvalidDeterMode,
            NoSupportedFunction,
            CommandInvalidQueueId,
            CommandQueueAddFailed,
            CommandQueueIsFull,
            CommandQueueAddTimeOut,

            IMVersionMismatch,
            ModuleVersionMismatch,

            CommandIdLockError,
            CommandIdGetError,

            CallbackMallocFailed,
            CallbackThreadNull,
            CallbackThreadError,
            CallbackThreadTimeOut,

            IMLibNullParam,
            IMLibInvalidParam,

            ConflictingErrorCode,
            UtilDataChnlSizeTooSmall,
            UtilBuffAndDataSizeDiff,
            UtilNoEnoughDataChnlSize,
            UtilReachMaxDataChnlSize,
            UtilIndexOutOfRange,
            UtilInitFailed,
            UtilLockFailed,
            UtilQueueFull,
            UtilQueueEmpty,
            UtilBufferSizeNotEnough,

            WaitForDeviceEventTimeout,
            DeviceEventProcessFailed,
            ImCommandNotMatched,

			ApiLogCreateQueueFailed,
			ApiLogIsNotSet,
			ApiLogIsRunning,
			ApiLogIsNotRunning,
			ApiLogQueueError,
			ApiLogOpenFileFailed,
			ApiLogWriteFileFailed,
			ApiLogReadFileFailed,
			ApiLogFolderNotExist,
			ApiLogGetStatusError,

            SystemError = 0x00000500,
            SystemIsNoRunning,
            SystemInitializationNotDone,
            EngineNotStartedInSpecifiedTime,
            EngineNotStoppedInSpecifiedTime,
            CommNotStartedInSpecifiedTime,
            CommNotStoppedInSpecifiedTime,
            ModuleIsNotLoaded,
            StartCommunicationError,
            DeviceIsNotInUse,
            SetDeviceEventFailed,
            ResetDeviceEventFailed,
            WaitForDeviceEventFailed,

            Reserved = 0x00000600,
            CommNotStarted,
            CommNotStopped,
            VersionCheckError,
            CallbackFuncError,
            CallbackFuncTimeOut,
            CallbackFuncInvalidState,
            CallbackFuncInvalidMode,
            MissingRequiredRtdll,

            BusyReceivingAnotherAPICommand,
            NotIdleState,
            AxisOutOfRange,
            AxisCountOutOfRange,
            DuplicateAxis,
            IOAddressOutOfRange,
            IOSizeOutOfRange,
            ChannelOutOfRange,
            ChannelUninitialized,
            ChannelInUse,
            ArgumentOutOfRange,
            ArgumentIsNull,
            PrevSettingsBeingApplied,
            ParamOutOfRange,
            IDOutOfRange,
            IDNotDefined,
            ResourceInUse,
            FileOperationFailed,
            StartingPreviousCommand,
            StringConversionError,
            NoMotion,
            ServoOffline,
            ServoOff,
            AxisAmpAlarm,
            AxisSoftLimit,
            AxisNearLimit,
            AxisExternalLimit,
            AxisHardLimit,
            UserMemoryAddressOutOfRange,
            UserMemorySizeOutOfRange,
            InvalidCommandProcessingMode,
            InterruptMismatch,
            ErrorCodeNotDefined,
            BufferTooSmall,
            RequestedBufferTooLarge,
            RequestedBufferTooSmall,
            ModuleIDOutOfRange,
            UpdatesListUninitialized,
            UpdatesListDoesNotMatchInstalledUpdates,
            UpdatesListTimeout,

            UnknownError
        };

        static int convertErrorCode(int errCode);
    };

    class DeviceType{
    public:
        enum T{
            DeviceTypeNormal = 0,
            DeviceTypeLowPriority = 0x1,
            DeviceTypeExitWOCnt = 0x02,
            DeviceTypeLowpriorityExitWOCnt = 0x03
        };
    };

    class EngineState {
    public:
        enum T {
            Idle,
            Running,
            Communicating,
            Shutdown,

            Unknown
        };
    };

    class ModuleType {
    public:
        enum T {
            Motion,
            Platform,

            Unknown
        };
    };

    class ModuleId{
    public:
        enum T{
            WMX3Engine     = 0,
            SimuPlatform   = 1,
            EcPlatform     = 2,
            RtexPlatform   = 3,
            MIIIPlatform   = 4,
            CCLinkPlatform = 5,
			M4Platform	   = 6,

            CoreMotion     = 10,
            Log            = 11,
            APIBuffer      = 12,
            CyclicBuffer   = 13,
            IO             = 14,
            Compensation   = 15,
            Event          = 16,
            AdvancedMotion = 17,
            UserMemory     = 18,
            PMMotion       = 19,

            Coordinate     = 26,
            CNC            = 27,
            Kinematics     = 28,
            PLC            = 29,

            UserRtdll0     = 32,
            UserRtdll1     = 33,
            UserRtdll2     = 34,

            ModuleSize     = 64
        };
    };

    class OperationState {
    public:
        enum T {
            Idle,
            Pos,
            Jog,
            Home,
            Sync,
            GantryHome,
            Stop,
            Intpl,
            Velocity,
            ConstLinearVelocity,
            Trq,
            DirectControl,
            PVT,
            ECAM,
            SyncCatchUp,
            DancerControl
        };
    };

    class DetailOperationState {
    public:
        enum T {
            Idle = 0,
            Pos = 200,
            Pos_OverrideSetup,
            Pos_WaitingForTrigger,
            Jog = 300,
            Jog_OverrideSetup,
            Home = 400,
            Sync = 500,
            Sync_PhaseShift,
            Sync_GearShift,
            GantryHome = 600,
            Stop = 700,
            Stop_QStop,
            Stop_EStop,
            Intpl = 800,
            Intpl_Linear,
            Intpl_Circular,
            Intpl_Helical,
            Intpl_Spline,
            Intpl_Path,
            Intpl_PathWithRotation,
            Intpl_PathLookahead,
            Intpl_OverrideSetup,
            Intpl_OverrideSmoothing,
            Velocity = 900,
            Velocity_OverrideSetup,
            ConstLinearVelocity = 1100,
            Trq = 1200,
            Trq_OverrideSetup,
            DirectControl = 1300,
            PVT = 1400,
            ECAM = 1500,
            SyncCatchUp = 1600,
            DancerControl = 1700
        };
    };

    class AxisCommandMode {
    public:
        enum T {
            Position,
            Velocity,
            Torque
        };
    };

    class AxisSelection{
    public:
        AxisSelection();
        AxisSelection(int axisCount, const int * axis);

        int axisCount;
        int axis[constants::maxAxes];
    };

    class ProfileType {
    public:
        enum T {
            Trapezoidal,
            SCurve,
            JerkRatio,
            Parabolic,
            Sin,
            AdvancedS,
            TrapezoidalMAT,
            JerkLimited,
            JerkLimitedSCurve,
            JerkLimitedAdvancedS,
            TwoVelocityTrapezoidal,
            TwoVelocitySCurve,
            TwoVelocityJerkRatio,
            TimeAccTrapezoidal,
            TimeAccSCurve,
            TimeAccJerkRatio,
            TimeAccParabolic,
            TimeAccSin,
            TimeAccAdvancedS,
            ConstantDec,
            JerkRatioFixedVelocityT,
            JerkRatioFixedVelocityS,
            JerkLimitedFixedVelocityT,
            JerkLimitedFixedVelocityS,
            ParabolicVelocity
        };
    };

    class Profile {
    public:
        Profile();

        ProfileType::T type;
        double velocity;
        double acc;
        double dec;
        double jerkAcc;
        double jerkDec;
        double jerkAccRatio;
        double jerkDecRatio;
        double accTimeMilliseconds;
        double decTimeMilliseconds;
        double startingVelocity;
        double endVelocity;
        double secondVelocity;
        double movingAverageTimeMilliseconds;

        static Profile __stdcall SetupTrapezoidal(
            double velocity,
            double acc,
            double dec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupSCurve(
            double velocity,
            double acc,
            double dec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupJerkRatio(
            double velocity,
            double acc,
            double dec,
            double jerkAccRatio,
            double jerkDecRatio,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupParabolic(
            double velocity,
            double acc,
            double dec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupSin(
            double velocity,
            double acc,
            double dec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupAdvancedS(
            double velocity,
            double acc,
            double dec,
            double jerkAccRatio,
            double jerkDecRatio,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTrapezoidalMAT(
            double velocity,
            double acc,
            double dec,
            double movingAverageTimeMilliseconds,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupJerkLimited(
            double velocity,
            double acc,
            double dec,
            double jerkAcc,
            double jerkDec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupJerkLimitedSCurve(
            double velocity,
            double acc,
            double dec,
            double jerkAcc,
            double jerkDec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupJerkLimitedAdvancedS(
            double velocity,
            double acc,
            double dec,
            double jerkAcc,
            double jerkDec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTwoVelocityTrapezoidal(
            double velocity,
            double acc,
            double dec,
            double secondVelocity,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTwoVelocitySCurve(
            double velocity,
            double acc,
            double dec,
            double secondVelocity,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTwoVelocityJerkRatio(
            double velocity,
            double acc,
            double dec,
            double jerkAccRatio,
            double jerkDecRatio,
            double secondVelocity,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTimeAccTrapezoidal(
            double velocity,
            double accTimeMilliseconds,
            double decTimeMilliseconds,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTimeAccSCurve(
            double velocity,
            double accTimeMilliseconds,
            double decTimeMilliseconds,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTimeAccJerkRatio(
            double velocity,
            double accTimeMilliseconds,
            double decTimeMilliseconds,
            double jerkAccRatio,
            double jerkDecRatio,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTimeAccParabolic(
            double velocity,
            double accTimeMilliseconds,
            double decTimeMilliseconds,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTimeAccSin(
            double velocity,
            double accTimeMilliseconds,
            double decTimeMilliseconds,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupTimeAccAdvancedS(
            double velocity,
            double accTimeMilliseconds,
            double decTimeMilliseconds,
            double jerkAccRatio,
            double jerkDecRatio,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupConstantDec(
            double startingVelocity,
            double endVelocity);
        static Profile __stdcall SetupJerkRatioFixedVelocityT(
            double velocity,
            double acc,
            double dec,
            double jerkAccRatio,
            double jerkDecRatio,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupJerkRatioFixedVelocityS(
            double velocity,
            double acc,
            double dec,
            double jerkAccRatio,
            double jerkDecRatio,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupJerkLimitedFixedVelocityT(
            double velocity,
            double acc,
            double dec,
            double jerkAcc,
            double jerkDec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupJerkLimitedFixedVelocityS(
            double velocity,
            double acc,
            double dec,
            double jerkAcc,
            double jerkDec,
            double startingVelocity = 0,
            double endVelocity = 0);
        static Profile __stdcall SetupParabolicVelocity(
            double jerkAccRatio,
            double jerkDecRatio,
            double accTimeMilliseconds,
            double decTimeMilliseconds,
            double startingVelocity = 0,
            double endVelocity = 0);
    };

    class EStopLevel {
    public:
        enum T {
            Final,
            Level1
        };
    };

    class FormatType {
    public:
        enum T {
            Char,
            Uchar,
            Short,
            Ushort,
            Int,
            Uint,
            Long,
            Ulong,
            Float,
            Double
        };
    };

    class AIOFormat {
    public:
        AIOFormat();
        int byte;
        int bit;
        FormatType::T aioFormatType;
    };

    class IOAddress{
    public:
        IOAddress();
        IOAddress(int byte, int bit, int size);

        int byte;
        int bit;
        int size;
    };

    class IOSourceType {
    public:
        enum T {
            Input,
            Output
        };
    };

    class MAddress {
    public:
        MAddress();
        MAddress(int byte, int bit, int size);

        int byte;
        int bit;
        int size;
    };

    class SizeUnit {
    public:
        enum T {
            Byte,
            Kilobyte,
            Megabyte
        };
    };

    class ModuleInfoA{
    public:
        ModuleInfoA();

        unsigned char suspend;
        ModuleType::T type;
        int id;
        int slot;
        char fileName[constants::moduleFileNameLen];
        char moduleName[constants::moduleNameLen];
        char moduleDesc[constants::moduleDescLen];

        int majorVersion;
        int minorVersion;
        int revisionVersion;
        int fixVersion;

        int memory;
    };

    class ModuleInfoW{
    public:
        ModuleInfoW();

        unsigned char suspend;
        ModuleType::T type;
        int id;
        int slot;
        wchar_t fileName[constants::moduleFileNameLen];
        wchar_t moduleName[constants::moduleNameLen];
        wchar_t moduleDesc[constants::moduleDescLen];

        int majorVersion;
        int minorVersion;
        int revisionVersion;
        int fixVersion;

        int memory;
    };

    class EngineInfo{
    public:
        EngineInfo();

        int majorVersion;
        int minorVersion;
        int revisionVersion;
        int fixVersion;

        int imlibVersion;
        int imlibRevision;

        int numLicensedAxes;
        unsigned char options[64];
    };

    class ModulesInfoA{
    public:
        ModulesInfoA();

        EngineInfo engine;

        int numOfModule;
        ModuleInfoA modules[constants::moduleLen];
    };

    class ModulesInfoW{
    public:
        ModulesInfoW();

        EngineInfo engine;

        int numOfModule;
        ModuleInfoW modules[constants::moduleLen];
    };

    class DeviceInfoA {
    public:
        DeviceInfoA();

        unsigned int id;
        DeviceType::T type;
        unsigned int watchdog;
        unsigned int watchdogCount;
        char name[constants::maxDeviceName];
    };

    class DeviceInfoW {
    public:
        DeviceInfoW();

        unsigned int id;
        DeviceType::T type;
        unsigned int watchdog;
        unsigned int watchdogCount;
        wchar_t name[constants::maxDeviceName];
    };

    class DevicesInfoA {
    public:
        DevicesInfoA();

        unsigned int count;
        DeviceInfoA devices[constants::maxDevices];
    };

    class DevicesInfoW {
    public:
        DevicesInfoW();

        unsigned int count;
        DeviceInfoW devices[constants::maxDevices];
    };

	class InterruptData{
	public:
		InterruptData();

		int numOfAxes;
		int cycleTimeMicroseconds;
		long long cycleCounter;
	};

    class EngineStatus{
    public:
        EngineStatus();

        EngineState::T state;
        unsigned int error;

		int numOfInterrupts;
		InterruptData interrupts[constants::maxInterrupts];
    };

    class DeviceStatistics{
    public:
        DeviceStatistics();

        unsigned int sysFailedCount;
        unsigned int apiSuccessCount;
        unsigned int apiFailedCount;

        unsigned int curApiTime;
        unsigned int avgApiTime;
        unsigned int minApiTime;
        unsigned int maxApiTime;
        LARGE_INTEGER sumApiTime;

        unsigned int rtCount;
        unsigned int curRtTime;
        unsigned int avgRtTime;
        unsigned int minRtTime;
        unsigned int maxRtTime;
        LARGE_INTEGER sumRtTime;

        unsigned int curFuncProcTime;
        unsigned int avgFuncProcTime;
        unsigned int minFuncProcTime;
        unsigned int maxFuncProcTime;
        LARGE_INTEGER sumFuncProcTime;
    };

    class ModuleSuspend{
    public:
        ModuleSuspend();

        unsigned char module[constants::moduleLen];
    };

    class EventInput{
    public:

        virtual int GetInputModuleId() = 0;
        virtual WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, int* dataSize, char *initialState) = 0;
        virtual WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, int dataSize) = 0;
    };

    class EventOutput{
    public:
        virtual int GetOutputModuleId() = 0;
        virtual WMX3APIFUNC GetOutputData(unsigned char* buff, int buffSize, int* dataSize) = 0;
        virtual WMX3APIFUNC SetOutputData(int moduleId, unsigned char* data, int dataSize) = 0;
    };

    class LogInput {
    public:
        virtual unsigned int GetModuleId() = 0;
        virtual WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, unsigned int* dataSize) = 0;
        virtual WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, unsigned int dataSize) = 0;
    };

    class LogOutput {
    public:
        LogOutput();

        unsigned int count;
        unsigned char overflowFlag;
        long long cycleCounter[constants::maxLogOutputDataSize];

        virtual unsigned int GetModuleId() = 0;
        virtual WMX3APIFUNC SetOutputData(int moduleId, unsigned char* cfgData, unsigned int cfgDataSize, unsigned char* data, unsigned int dataSize, unsigned int dataIndex, unsigned int storeIndex) = 0;
    };

    class WMX3Api;

    class WMX3Api{

    public:
        HANDLE dev;

    public:
        WMX3Api();
        ~WMX3Api();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);

		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);

        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);
        static WMX3APIFUNC GetIMDllVersion(int *pVersion, int *pRevision);

        static WMX3APIFUNC PrintToFile(const char* filePath, const char * format, ...);
        static WMX3APIFUNC PrintToFile(const wchar_t* filePath, const wchar_t * format, ...);
        static WMX3APIFUNC PrintToFile(const char* filePath, int charsToPrint, const char* buff);
        static WMX3APIFUNC PrintToFile(const wchar_t* filePath, int charsToPrint, const wchar_t* buff);
        static WMX3APIFUNC PrintToFileIfExist(const char* filePath, const char * format, ...);
        static WMX3APIFUNC PrintToFileIfExist(const wchar_t* filePath, const wchar_t * format, ...);
        static WMX3APIFUNC PrintToFileIfExist(const char* filePath, int charsToPrint, const char* buff);
        static WMX3APIFUNC PrintToFileIfExist(const wchar_t* filePath, int charsToPrint, const wchar_t* buff);

        static WMX3APIFUNC GetStdOutStr(char* buff, int buffSize, unsigned char* isWideChar, int* strlen);
        static WMX3APIFUNC GetStdOutDataSize();
        static WMX3APIFUNC ClearStdOut();
        static WMX3APIFUNC ReleaseStdOut();

        bool IsDeviceValid();

        WMX3APIFUNC StartEngine(char* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
        WMX3APIFUNC StartEngine(wchar_t* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
        WMX3APIFUNC RestartEngine(char* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
        WMX3APIFUNC RestartEngine(wchar_t* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
        WMX3APIFUNC StopEngine(unsigned int waitTimeMilliseconds = 0);

        WMX3APIFUNC CreateDevice(char* path, DeviceType::T type = DeviceType::T::DeviceTypeNormal, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
        WMX3APIFUNC CreateDevice(wchar_t* path, DeviceType::T type = DeviceType::T::DeviceTypeNormal, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
        WMX3APIFUNC CloseDevice();
        WMX3APIFUNC GetDeviceID(int *id);

        WMX3APIFUNC AutoQuitWithoutActiveDev(bool quit);
        WMX3APIFUNC SetWatchdog(unsigned int watchdog);
		WMX3APIFUNC SetWatchdog(int deviceId, unsigned int watchdog);
        WMX3APIFUNC GetWatchdog(unsigned int* watchdog, unsigned int* watchdogCount = NULL);
        WMX3APIFUNC ResetWatchdogTimer();

        WMX3APIFUNC SetStatistic(bool enable);
        WMX3APIFUNC GetStatistic(DeviceStatistics* statistics);

        WMX3APIFUNC SetInterruptId(unsigned int interruptId);
        WMX3APIFUNC GetInterruptId(unsigned int* interruptId);

        WMX3APIFUNC StartCommunication(unsigned int waitTimeMilliseconds = 0);
        WMX3APIFUNC StopCommunication(unsigned int waitTimeMilliseconds = 0);
        WMX3APIFUNC GetModulesInfo(ModulesInfoA* modules);
        WMX3APIFUNC GetModulesInfo(ModulesInfoW* modules);
        WMX3APIFUNC GetModuleInfo(int moduleId, ModuleInfoA* module);
        WMX3APIFUNC GetModuleInfo(int moduleId, ModuleInfoW* module);
        WMX3APIFUNC GetEngineStatus(EngineStatus* status);
        WMX3APIFUNC GetAllDevices(DevicesInfoA* devices);
        WMX3APIFUNC GetAllDevices(DevicesInfoW* devices);
        WMX3APIFUNC SetDeviceName(char* name);
        WMX3APIFUNC SetDeviceName(wchar_t* name);
        WMX3APIFUNC GetDeviceName(char* nameBuf, unsigned int bufSize);
        WMX3APIFUNC GetDeviceName(wchar_t* nameBuf, unsigned int bufSize);

        WMX3APIFUNC SetDeviceWaitEvent(int errCode);
        WMX3APIFUNC ResetDeviceWaitEvent();
        WMX3APIFUNC WaitForDeviceWaitEvent(unsigned int waitTime, int* errCode);

        WMX3APIFUNC SetModuleSuspend(ModuleSuspend* suspend);
        WMX3APIFUNC GetModuleSuspend(ModuleSuspend* suspend);
        WMX3APIFUNC AbortModuleSuspend();

        WMX3APIFUNC SleepAtEngine(unsigned long microSecond);

        WMX3APIFUNC PrintToServerConsole(const char * format, ...);
        WMX3APIFUNC PrintToServerConsole(const wchar_t * format, ...);

        WMX3APIFUNC RecordWindowsUpdates(unsigned int waitTimeMilliseconds = 0);
        WMX3APIFUNC CompareWindowsUpdates(unsigned int waitTimeMilliseconds = 0);

    };
}

#endif
