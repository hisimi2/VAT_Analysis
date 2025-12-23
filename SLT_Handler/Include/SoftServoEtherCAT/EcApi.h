#ifndef EC_API_LIB
#define EC_API_LIB
#include <windows.h>
#include "WMX3Api.h"


#ifdef WIN32
#define ECAPIFUNC long __stdcall
#else
#define ECAPIFUNC HRESULT
#endif

namespace wmx3Api {
    namespace ecApi {

        namespace constants {
            static const int maxMasters = 2;
            static const int maxSlaves = 256;
            static const int maxSlaveAxes = 16;
            static const int maxEniFilePathLen = 256;
            static const int maxFoEFilePathLen = 256;
            static const int maxFoEFileNameLen = 64;
            static const int maxSdoInfoNameLen = 64;
            static const int maxSdoInfoODListSize = 450;
            static const int maxSdoInfoEDListSize = 50;
			static const int maxLogPdoSize = 256;
			static const int maxLogPdoDataSize = 8;
			static const int maxMappedRxPdo = 128;
			static const int maxMappedTxPdo = 128;
        }

        class EcStateMachine {
        public:
            enum T {
                None = 0x00,
                Init = 0x01,
                Preop = 0x02,
                Boot = 0x03,
                Safeop = 0x04,
                Op = 0x08
            };
        };

        class EcSdoType {
        public:
            enum T {
                Expedited = 0x00,
                Normal = 0x01
            };
        };

        class EcObjectDescriptionListType {
        public:
            enum T {
                Len = 0x00,
                AllObjects = 0x01,
                RxPdo = 0x02,
                TxPdo = 0x03,
                DevReplacement = 0x04,
                Startup = 0x05,
            };
        };

        class EcHotconnectState {
        public:
            enum T {
                Idle = 0x00,
                Running,
                Abort,

                Unknown
            };
        };

        class EcHotconnectAbortCode {
        public:
            enum T {
                None = 0x00,
                PortOpenError,
                Timeout,
                ScanNetworkFailed,
                ApplyNewTopologyError,
                InitNewSlavesError,
                RecalculateDcError,
                ClearDLEventError,

                Unknown
            };
        };

        class EcOperationMode {
        public:
            enum T {
                NONE = 0x00,
                PP = 0x01,
                VL = 0x02,
                PV = 0x03,
                TQ = 0x04,
                HM = 0x06,
                IP = 0x07,
                CSP = 0x08,
                CSV = 0x09,
                CST = 0x0a,
                CSTCA = 0x0b,

                Unknown
            };
        };

		class EcMasterMode {
		public:
			enum T {
				CyclicMode,
				PPMode,
				MonitorMode
			};
		};

        class EcErrorCode: public ErrorCode {
        public:
            enum {
                EcError =  0x00002000,
                ScanNetworkFailed,
                HotconnectFailed,
                SharedMemoryNull,
                SlaveStateChangeError,
                SlaveStateBelowPreop,
                SlaveInvalidAddressingMode,
                SlaveInEmergencyState,
                SlaveMailboxInUse,
                SlaveSdoDownloadError,
                SlaveSdoUploadError,
                SlaveSdoInfoGetODListError,
                SlaveSdoInfoGetODError,
                SlaveSdoInfoGetEDError,
                SlaveActualSizeLargerThanBufferSize,
                SlaveRegWriteError,
                SlaveRegReadError,
                SlaveSIIWriteError,
                SlaveSIIWriteFailedToSetChecksum,
                SlaveSIIReadError,
                SlavePdoReadWriteNotAllowed,
                SlavePdoReadWriteTimeout,
                SlavePdoNotMapped,
                SlaveTxPdoWriteNotAllowed,
                SlaveFoENotInBootState,
                SlaveFoEFileOpenError,
                SlaveFoEReadError,
                SlaveFoEWriteError,
                SlaveFoEFilePathTooLong,
                SlaveFoEFileNameTooLong,
                SlaveAoEReadError,
                SlaveAoEWriteError,
                SlaveSoEReadError,
                SlaveSoEWriteError,
                SlaveVoEReadError,
                SlaveVoEWriteError,
                AxisNotServoOn,
                NotInPPMode,
                NotInHMMode,
				NotInPVMode,
				NotInTQMode,
				PdoNotMapped,
                SlaveNotSupportVirtualSerial,
                OpenComFailed,
                CreateComProcessThreadFailed,
				NotInMonitorMode,
				VirtualSlaveNotEnabled,
				OtherOperationIsRunning
            };

            static int ConvertEcErrorCode(int errCode);
        };

		class EcLogInputPdo{
		public:
			enum DataType {
				Signed,
				Unsigned,
				Hexadecimal
			};

			int slaveId;
			DataType type;
			unsigned int index;
			unsigned int subIndex;
		};

		class EcLogInput : public LogInput {
		public:
			EcLogInput();
			unsigned int GetModuleId();
			WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, unsigned int* dataSize);
			WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, unsigned int dataSize);

			int masterId;
			int numOfPdo;
			EcLogInputPdo pdo[constants::maxLogPdoSize];
		};

		class EcLogOutputPdo{
		public:
			EcLogOutputPdo();

			int slaveId;
			unsigned int index;
			unsigned int subIndex;
			int length;
			unsigned char data[constants::maxLogPdoDataSize];
		};

		class EcLogOutputData{
		public:
			EcLogOutputData();

			int masterId;
			int numOfPdo;
			EcLogOutputPdo pdo[constants::maxLogPdoSize];
		};

		class EcLogOutput : public LogOutput {
		public:
			EcLogOutput();
			unsigned int GetModuleId();
			WMX3APIFUNC SetOutputData(int moduleId, unsigned char* cfgData, unsigned int cfgDataSize, unsigned char* data, unsigned int dataSize, unsigned int dataIndex, unsigned int storeIndex);

			EcLogOutputData ecData[wmx3Api::constants::maxLogOutputDataSize];
		};

		class EcAoESender{
		public:
			unsigned char senderNetId[6];
			unsigned short senderPort;
		};

        class EcSlaveSdoInfoObjectDescription {
        public:
            unsigned short index;
            unsigned short dataType;
            unsigned char maxSubIndex;
            unsigned short objectCode;
            char name[constants::maxSdoInfoNameLen];
        };

        class EcSlaveSdoInfoObjectDescriptionList {
        public:
            unsigned short num;
            EcSlaveSdoInfoObjectDescription objects[constants::maxSdoInfoODListSize];
        };

        class EcSlaveSdoInfoEntryDescription {
        public:
            unsigned short index;
            unsigned char subIndex;
            unsigned char valueInfo;
            unsigned short dataType;
            unsigned short bitLen;
            unsigned short objectAccess;

            unsigned short unitType;
            int defaultValue;
            int minimumValue;
            int maximumValue;
            char name[constants::maxSdoInfoNameLen];
        };

        class EcSlaveSdoInfoEntryDescriptionList {
        public:
            unsigned short num;
            EcSlaveSdoInfoEntryDescription entry[constants::maxSdoInfoEDListSize];
        };

        class EcSlavePdo{
        public:
            bool enable;
            unsigned char size;
            unsigned short offset;
        };

        class EcSlavePdoInfo {
        public:
            EcSlavePdo statusWord;
            EcSlavePdo modesOfOperationDisplay;
            EcSlavePdo positionActualValue;
            EcSlavePdo velocityActualValue;
            EcSlavePdo torqueActualValue;
            EcSlavePdo errorCode;
            EcSlavePdo touchProbeStatus;
            EcSlavePdo touchProbePositionPos1;
            EcSlavePdo touchProbePositionPos2;
            EcSlavePdo followingError;
            EcSlavePdo digitalInput;

            EcSlavePdo controlWord;
            EcSlavePdo modesOfOperation;
            EcSlavePdo targetPosition;
            EcSlavePdo targetVelocity;
            EcSlavePdo targetTorque;
            EcSlavePdo maxTorque;
            EcSlavePdo positiveTorqueLimit;
            EcSlavePdo negativeTorqueLimit;
            EcSlavePdo touchProbeFunction;
            EcSlavePdo velocityOffset;
            EcSlavePdo torqueOffset;
            EcSlavePdo maxMotorSpeed;
            EcSlavePdo digitalOutput;
        };

        class EcSlaveAxisInfo {
        public:
            int axisIndex;
            short digitalInputAddr;
            unsigned short digitalInputSize;
            short digitalOutputAddr;
            unsigned short digitalOutputSize;
            EcSlavePdoInfo pdoInfo;
            EcOperationMode::T operationMode;
            unsigned char targetReached;
            unsigned char followingError;
            unsigned char homingError;
            unsigned char homingAttained;
            unsigned char halt;
        };

        class EcSlaveInfo {
        public:
            int id;

            short position;
            short address;

            unsigned int vendorId;
            unsigned int productCode;
            unsigned int revisionNo;
            unsigned int serialNo;
            unsigned int alias;
            unsigned int numOfAxes;
            EcSlaveAxisInfo axisInfo[constants::maxSlaveAxes];

            EcStateMachine::T state;
            unsigned short alStatusCode;

            unsigned char offline;
            unsigned char inaccessible;
            unsigned char newSlave;
            unsigned char reverseSlave;

            unsigned char dcSupport;
            unsigned char dcWidth;          //0: none, 1: 32bit, 2: 64bit
            unsigned char portState[4];     //bit0: physical link, bit1: Loop, bit2: Communication
            int neighbor[4];
            unsigned int propagaDelay;
            unsigned long long systemTime;
            unsigned long long systemTimeOffset;
            unsigned long long startTime;

            unsigned short inputAddr;
            unsigned short inputSize;
            unsigned short outputAddr;
            unsigned short outputSize;

            unsigned int rxLogicAddr;
            unsigned int rxPhyAddr;
            unsigned int rxSize;
            unsigned int txLogicAddr;
            unsigned int txPhyAddr;
            unsigned int txSize;
        };

        class EcMasterStatisticsInfo {
        public:
            long commPeriod;
            long long cycleCounter;
            double interruptIntervalRatio[9];
            double cycleProcessTimeRatio[6];
            double processTimeRatio[6];
            double commandProcessTimeRatio[6];
            double feedbackProcessTimeRatio[6];
            double communicateProcessTimeRatio[6];

            unsigned int txDelay;
            unsigned int minTxDelay;
            unsigned int maxTxDelay;
            unsigned int packetLoss;
            unsigned int packetTimeout;
            unsigned int packetLossMode[4];
            unsigned int packetTimeoutMode[4];

            unsigned long long minRefClockInterval;
            unsigned long long maxRefClockInterval;
            unsigned long long avgRefClockInterval;

            unsigned long long curRefClock;
            unsigned long long nextDcClock;

            int diffFromNextDcClock;
            int minDiffFromNextDcClock;
            int maxDiffFromNextDcClock;

            unsigned int totalAxesNum;

            unsigned int totalRxPdoSize;
            unsigned int totalTxPdoSize;
            unsigned int totalInputSize;
            unsigned int totalOutputSize;

            unsigned int overCycle;

            unsigned int ringNum;

            unsigned int numOfCyclicFrames;

            EcHotconnectState::T hcState;
            EcHotconnectAbortCode::T hcAbortCode;
            long hcErrorCode;
        };

		class EcVirtualSlavePdo{
		public:
			unsigned int index;
			unsigned int subIndex;
			unsigned int size;
			int offset;
		};

		class EcVirtualSlaveDcStatisticsInfo{
		public:
			long long minRefClockIntvl;						//unit: ns
			long long sysIntvlWhenMinRefClockIntvl;			//unit: ns
			long long maxRefClockIntvl;						//unit: ns
			long long sysIntvlWhenMaxRefClockIntvl;			//unit: ns
			long long avgRefClockIntvl;						//unit: ns

			long long minSysClockIntvl;						//unit: ns
			long long maxSysClockIntvl;						//unit: ns
			long long avgSysClockIntvl;						//unit: ns

			unsigned long long count;
		};

		class EcVirtualSlaveStatisticsInfo{
		public:
			EcVirtualSlaveDcStatisticsInfo dc;
		};

		class EcVirtualSlaveInfo{
		public:
			EcStateMachine::T state;
			short address;
			unsigned char enabled;

			unsigned int vendorId;
			unsigned int productCode;
			unsigned int revisionNo;
			unsigned int serialNo;
			unsigned int alias;

			unsigned int commPeriod;

			unsigned int numOfMappedRxPdo;
			EcVirtualSlavePdo mappedRxPdo[constants::maxMappedRxPdo];
			unsigned int numOfMappedTxPdo;
			EcVirtualSlavePdo mappedTxPdo[constants::maxMappedTxPdo];

			EcVirtualSlaveStatisticsInfo statistics;
		};

		class EcMonitorDcStatisticsInfo{
		public:
			long long minRefClockIntvl;						//unit: ns
			long long sysIntvlWhenMinRefClockIntvl;			//unit: ns
			long long maxRefClockIntvl;						//unit: ns
			long long sysIntvlWhenMaxRefClockIntvl;			//unit: ns
			long long avgRefClockIntvl;						//unit: ns

			long long minSysClockIntvl;						//unit: ns
			long long maxSysClockIntvl;						//unit: ns
			long long avgSysClockIntvl;						//unit: ns

			unsigned long long count;
		};

		class EcMonitorStatisticsInfo{
		public:
			EcMonitorDcStatisticsInfo dc;
		};

		class EcMonitorInfo{
		public:
			unsigned char enabled;

			int id;
			unsigned int commPeriod;
			int numOfSlaves;
			int position;

			EcMonitorStatisticsInfo statistics;

			EcVirtualSlaveInfo virtualSlave;
		};

        class EcMasterInfo {
        public:
            int id;
            EcStateMachine::T state;
			EcMasterMode::T mode;
            unsigned int numOfSlaves;
            EcSlaveInfo slaves[constants::maxSlaves];

            EcMasterStatisticsInfo statisticsInfo;

            int GetOnlineSlaveCount();
            int GetOfflineSlaveCount();
            int GetInaccessibleSlaveCount();
        };

        class EcMasterInfoList{
        public:
            unsigned int numOfMasters;
            EcMasterInfo masters[constants::maxMasters];
        };

        typedef int (*EcSdoDownloadCallBack)(int result, int masterid, int slaveid, int index, int subindex, EcSdoType::T sdoType, int len, unsigned char* data, unsigned int errorCode);
        typedef int (*EcSdoUploadCallBack)(int result, int masterid, int slaveid, int index, int subindex, EcSdoType::T sdoType, int len, unsigned char* data, unsigned int errorCode);
        typedef int (*EcRegisterWriteCallBack)(int result, int masterid, int slaveid, int off, int len, unsigned char* data);
        typedef int (*EcRegisterReadCallBack)(int result, int masterid, int slaveid, int off, int len, unsigned char* data);
        typedef int (*EcRegisterBroadcastWriteCallBack)(int result, int masterid, int off, int len, unsigned char* data, int wkc);
        typedef int (*EcRegisterBroadcastReadCallBack)(int result, int masterid, int off, int len, unsigned char* data, int wkc);
        typedef int (*EcSdoInfoGetODListCallBack)(int result, int masterid, int slaveid, EcObjectDescriptionListType::T type, EcSlaveSdoInfoObjectDescriptionList* list);
        typedef int (*EcSdoInfoGetEDListCallBack)(int result, int masterid, int slaveid, int index, EcSlaveSdoInfoEntryDescriptionList* list);
        typedef int (*EcFoEReadCallBack)(int result, int masterid, int slaveId, wchar_t* filePath, char* fileName, unsigned int password, unsigned int errorCode);
        typedef int (*EcFoEWriteCallBack)(int result, int masterid, int slaveId, wchar_t* filePath, char* fileName, unsigned int password, unsigned int errorCode);
        typedef int (*EcSIIWriteCallBack)(int result, int masterid, int slaveid, int addr, int len, unsigned char* data);
        typedef int (*EcSIIReadCallBack)(int result, int masterid, int slaveid, int addr, int len, unsigned char* data);
        typedef int (*EcAoEReadCallBack)(int result, int masterid, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned char senderNetId[6], unsigned short senderPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, unsigned char* aoeBuff, unsigned int errorCode);
        typedef int (*EcAoEWriteCallBack)(int result, int masterid, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned char senderNetId[6], unsigned short senderPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int writeLength, unsigned char* aoeData, unsigned int errorCode);
        typedef int (*EcAoEWriteControlCallBack)(int result, int masterid, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned char senderNetId[6], unsigned short senderPort, unsigned short aoeState, unsigned short deviceState, unsigned int writeLength, unsigned char* aoeData, unsigned int errorCode);
        typedef int (*EcSoEReadCallBack)(int result, int masterid, int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int buffSize, unsigned char* readSoEBuffer, unsigned int errorCode);
        typedef int (*EcSoEWriteCallBack)(int result, int masterid, int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int dataSize, unsigned char* writeSoEData, unsigned int errorCode);
        typedef int (*EcVoEReadCallBack)(int result, int masterid, int slaveId, unsigned int vendorId, unsigned short vendorType, unsigned int readDataSize, unsigned char* readVoEBuffer);
        typedef int (*EcVoEWriteCallBack)(int result, int masterid, int slaveId, unsigned int vendorId, unsigned short vendorType, unsigned int writeDataSize, unsigned char* writeAoEData);

        class Ecat;

        class Ecat {
        private:
            WMX3Api *wmx3Api;
            int statChnlId;
			int monChnlId;
            bool isSelfDev;
            void init(WMX3Api *f);
            void close();
            HANDLE ioHdl;
            void* ioData;

        public:
            Ecat(WMX3Api *f);
            Ecat(const Ecat& src);
            Ecat& operator=(const Ecat& src);
            Ecat();
            Ecat(DeviceType::T type);
            ~Ecat();

            static ECAPIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
            static ECAPIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
			static ECAPIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
			static ECAPIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
            static ECAPIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

            bool IsDeviceValid();

            ECAPIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

			ECAPIFUNC SetMonitorMode(unsigned char enable);

            ECAPIFUNC ScanNetwork();

            ECAPIFUNC ChangeSlaveState(int slaveId, EcStateMachine::T state, int* errorCode);

            ECAPIFUNC StartHotconnect();
            ECAPIFUNC ResetRefClockInfo();
            ECAPIFUNC ResetTransmitStatisticsInfo();
			ECAPIFUNC ResetMonitorStatisticsInfo();

            ECAPIFUNC GetMasterInfo(EcMasterInfo* master);
			ECAPIFUNC GetMasterMonitorInfo(EcMonitorInfo* monitor);

            ECAPIFUNC SetEniFilePath(int slaveId, char* path, unsigned char oneshot = 1);
            ECAPIFUNC SetEniFilePath(int slaveId, wchar_t* path, unsigned char oneshot = 1);

            ECAPIFUNC RegisterWrite(int slaveId, int regAddr, int len, unsigned char* data);
            ECAPIFUNC RegisterRead(int slaveId, int regAddr, int len, unsigned char* buff);
            ECAPIFUNC RegisterWrite(int slaveId, int regAddr, int len, unsigned char* data, EcRegisterWriteCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC RegisterRead(int slaveId, int regAddr, int len, EcRegisterReadCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC RegisterBroadcastWrite(int regAddr, int len, unsigned char* data, unsigned short* wkc);
            ECAPIFUNC RegisterBroadcastRead(int regAddr, int len, unsigned char* buff, unsigned short* wkc);
            ECAPIFUNC RegisterBroadcastWrite(int regAddr, int len, unsigned char* data, EcRegisterBroadcastWriteCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC RegisterBroadcastRead(int regAddr, int len, EcRegisterBroadcastReadCallBack callbackFunc, unsigned int waitTime);

            //Expedited SDO download/upload
            ECAPIFUNC SdoDownload(int slaveId, int index, int subindex, int sdoDataSize, unsigned char* sdoData, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SdoUpload(int slaveId, int index, int subindex, int sdoBuffSize, unsigned char* sdoBuff, unsigned int* actualSize, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SdoDownload(int slaveId, int index, int subindex, int sdoDataSize, unsigned char* sdoData, EcSdoDownloadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC SdoUpload(int slaveId, int index, int subindex, EcSdoUploadCallBack callbackFunc, unsigned int waitTime);
            //SDO download/upload with specified type
            ECAPIFUNC SdoDownload(int slaveId, int index, int subindex, EcSdoType::T sdoType, int sdoDataSize, unsigned char* sdoData, unsigned int* errCode, unsigned int waitTime = 0, BOOL complete = FALSE);
            ECAPIFUNC SdoUpload(int slaveId, int index, int subindex, EcSdoType::T sdoType, int sdoBuffSize, unsigned char* sdoBuff, unsigned int* actualSize, unsigned int* errCode, unsigned int waitTime = 0, BOOL complete = FALSE);
            ECAPIFUNC SdoDownload(int slaveId, int index, int subindex, EcSdoType::T sdoType, int sdoDataSize, unsigned char* sdoData, EcSdoDownloadCallBack callbackFunc, unsigned int waitTime, BOOL complete = FALSE);
            ECAPIFUNC SdoUpload(int slaveId, int index, int subindex, EcSdoType::T sdoType, EcSdoUploadCallBack callbackFunc, unsigned int waitTime, BOOL complete = FALSE);

            ECAPIFUNC GetSdoInfoODList(int slaveId, EcObjectDescriptionListType::T type, EcSlaveSdoInfoObjectDescriptionList* list);
            ECAPIFUNC GetSdoInfoEDList(int slaveId, int index, EcSlaveSdoInfoEntryDescriptionList* list);
            ECAPIFUNC GetSdoInfoODList(int slaveId, EcObjectDescriptionListType::T type, EcSdoInfoGetODListCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC GetSdoInfoEDList(int slaveId, int index, EcSdoInfoGetEDListCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC TxPdoWrite(int slaveId, int index, int subindex, int pdoDataSize, unsigned char* pdoData, unsigned int mask = 0);
            ECAPIFUNC ClearTxPdoWrite(int slaveId, int index, int subindex);
            ECAPIFUNC PdoRead(int slaveId, int index, int subindex, int pdoBuffSize, unsigned char* pdoBuff, unsigned int* actualSize);

            ECAPIFUNC SetOutputBit(int slaveId, int byte, int bit, unsigned char data);
            ECAPIFUNC SetOutputByte(int slaveId, int byte, unsigned char data);
            ECAPIFUNC SetOutputBytes(int slaveId, int byte, int size, unsigned char *pData);
            ECAPIFUNC SetOutputBits(int slaveId, int *pByte, int *pBit, unsigned char *pData, int count);
            ECAPIFUNC GetInputBit(int slaveId, int byte, int bit, unsigned char *pData);
            ECAPIFUNC GetInputByte(int slaveId, int byte, unsigned char *pData);
            ECAPIFUNC GetInputBytes(int slaveId, int byte, int size, unsigned char *pData);
            ECAPIFUNC GetOutputBit(int slaveId, int byte, int bit, unsigned char *pData);
            ECAPIFUNC GetOutputByte(int slaveId, int byte, unsigned char *pData);
            ECAPIFUNC GetOutputBytes(int slaveId, int byte, int size, unsigned char *pData);

            ECAPIFUNC FoERead(int slaveId, wchar_t* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoEWrite(int slaveId, wchar_t* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoERead(int slaveId, wchar_t* filePath, char* fileName, unsigned int password, EcFoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC FoEWrite(int slaveId, wchar_t* filePath, char* fileName, unsigned int password, EcFoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC FoERead(int slaveId, char* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoEWrite(int slaveId, char* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoERead(int slaveId, char* filePath, char* fileName, unsigned int password, EcFoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC FoEWrite(int slaveId, char* filePath, char* fileName, unsigned int password, EcFoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC SIIWrite(int slaveId, int siiAddr, int len, unsigned char* data, bool checkSum = false);
            ECAPIFUNC SIIRead(int slaveId, int siiAddr, int len, unsigned char* buff);
            ECAPIFUNC SIIWrite(int slaveId, int siiAddr, int len, unsigned char* data, EcSIIWriteCallBack callbackFunc, unsigned int waitTime, bool checkSum = false);
            ECAPIFUNC SIIRead(int slaveId, int siiAddr, int len, EcSIIReadCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC AoERead(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, unsigned char* readAoEBuff, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWrite(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int writeLength, unsigned char* writeAoEData, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWriteControl(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned short aoeState, unsigned short deviceState, unsigned int writeLength, unsigned char* writeAoEData, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoEReadWrite(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, unsigned char* readAoEBuff, unsigned int writeLength, unsigned char* writeAoEData, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoERead(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, EcAoEReadCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWrite(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int writeLength, unsigned char* writeAoEData, EcAoEWriteCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWriteControl(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned short aoeState, unsigned short deviceState, unsigned int writeLength, unsigned char* writeAoEData, EcAoEWriteControlCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);
            ECAPIFUNC AoEReadWrite(int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, unsigned int writeLength, unsigned char* writeAoEData, EcAoEReadCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);

            ECAPIFUNC SoERead(int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int buffSize, unsigned char* readSoEBuffer, unsigned int* actualSize, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SoEWrite(int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int dataSize, unsigned char* writeSoEData, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SoERead(int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, EcSoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC SoEWrite(int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int dataSize, unsigned char* writeSoEData, EcSoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC VoERead(int slaveId, unsigned int* vendorId, unsigned short* vendorType, unsigned int buffSize, unsigned char* readVoEBuffer, unsigned int* actualSize, unsigned int waitTime = 0);
            ECAPIFUNC VoEWrite(int slaveId, unsigned int vendorId, unsigned short vendorType, unsigned int dataSize, unsigned char* writeVoEData, unsigned int waitTime = 0);
            ECAPIFUNC VoERead(int slaveId, EcVoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC VoEWrite(int slaveId, unsigned int vendorId, unsigned short vendorType, unsigned int dataSize, unsigned char* writeVoEData, EcVoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC SetAxisMode(int slaveId, int axisIndex, EcOperationMode::T mode);
            ECAPIFUNC StartAxisPP(int slaveId, int axisIndex, int targetPos, unsigned int profileVel, unsigned int profileAcc, unsigned int profileDec, char immediately = -1, char relative = -1);
			ECAPIFUNC StartAxisPV(int slaveId, int axisIndex, int targetVel, unsigned int profileAcc, unsigned int profileDec);
			ECAPIFUNC StartAxisPV(int slaveId, int axisIndex, int targetVel, unsigned int profileAcc, unsigned int profileDec, unsigned short maxTorque);
			ECAPIFUNC StartAxisTQ(int slaveId, int axisIndex, short targetTrq);
			ECAPIFUNC StartAxisTQ(int slaveId, int axisIndex, short targetTrq, unsigned int maxMotorSpeed);
			ECAPIFUNC StartAxisHM(int slaveId, int axisIndex, int method, unsigned int switchSpeed, unsigned int zeroSpeed, unsigned int hmAcc);
			ECAPIFUNC StartAxisHM(int slaveId, int axisIndex, int method, unsigned int switchSpeed, unsigned int zeroSpeed, unsigned int hmAcc, int homeOffset);
            ECAPIFUNC HaltAxis(int slaveId, int axisIndex);
            ECAPIFUNC HaltAxisRelease(int slaveId, int axisIndex);

            ECAPIFUNC OpenSerialChannel(int slaveId, int chnlId, wchar_t* comName, DCB* dcb, COMMTIMEOUTS* timeout, unsigned int* error = NULL);
            ECAPIFUNC OpenSerialChannel(int slaveId, int chnlId, char* comName, DCB* dcb, COMMTIMEOUTS* timeout, unsigned int* error = NULL);
            ECAPIFUNC CloseSerialChannel(int slaveId, int chnlId);

			ECAPIFUNC VirtualSlaveTxPdoWrite(int index, int subindex, int pdoDataSize, unsigned char* pdoData, unsigned int mask = 0);
			ECAPIFUNC VirtualSlaveClearTxPdoWrite(int index, int subindex);
			ECAPIFUNC VirtualSlavePdoRead(int index, int subindex, int pdoBuffSize, unsigned char* pdoBuff, unsigned int* actualSize);


            ECAPIFUNC SetMonitorMode(int masterId, unsigned char enable);
			
			ECAPIFUNC ScanNetwork(int masterId);

            ECAPIFUNC ChangeSlaveState(int masterId, int slaveId, EcStateMachine::T state, int* errorCode);

            ECAPIFUNC StartHotconnect(int masterId);
            ECAPIFUNC ResetRefClockInfo(int masterId);
            ECAPIFUNC ResetTransmitStatisticsInfo(int masterId);
			ECAPIFUNC ResetMonitorStatisticsInfo(int masterId);

            ECAPIFUNC SetUpdatePeriod(unsigned int period);
            ECAPIFUNC GetMasterInfo(int masterId, EcMasterInfo* master);
            ECAPIFUNC GetMasterInfoList(EcMasterInfoList* masters);

			ECAPIFUNC GetMasterMonitorInfo(int masterId, EcMonitorInfo* monitor);

            ECAPIFUNC SetEniFilePath(int masterId, int slaveId, char* path, unsigned char oneshot = 1);
            ECAPIFUNC SetEniFilePath(int masterId, int slaveId, wchar_t* path, unsigned char oneshot = 1);

            ECAPIFUNC RegisterWrite(int masterId, int slaveId, int regAddr, int len, unsigned char* data);
            ECAPIFUNC RegisterRead(int masterId, int slaveId, int regAddr, int len, unsigned char* buff);
            ECAPIFUNC RegisterWrite(int masterId, int slaveId, int regAddr, int len, unsigned char* data, EcRegisterWriteCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC RegisterRead(int masterId, int slaveId, int regAddr, int len, EcRegisterReadCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC RegisterBroadcastWrite(int masterId, int regAddr, int len, unsigned char* data, unsigned short* wkc);
            ECAPIFUNC RegisterBroadcastRead(int masterId, int regAddr, int len, unsigned char* buff, unsigned short* wkc);
            ECAPIFUNC RegisterBroadcastWrite(int masterId, int regAddr, int len, unsigned char* data, EcRegisterBroadcastWriteCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC RegisterBroadcastRead(int masterId, int regAddr, int len, EcRegisterBroadcastReadCallBack callbackFunc, unsigned int waitTime);

            //Expedited SDO download/upload
            ECAPIFUNC SdoDownload(int masterId, int slaveId, int index, int subindex, int sdoDataSize, unsigned char* sdoData, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SdoUpload(int masterId, int slaveId, int index, int subindex, int sdoBuffSize, unsigned char* sdoBuff, unsigned int* actualSize, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SdoDownload(int masterId, int slaveId, int index, int subindex, int sdoDataSize, unsigned char* sdoData, EcSdoDownloadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC SdoUpload(int masterId, int slaveId, int index, int subindex, EcSdoUploadCallBack callbackFunc, unsigned int waitTime);
            //SDO download/upload with specified type
            ECAPIFUNC SdoDownload(int masterId, int slaveId, int index, int subindex, EcSdoType::T sdoType, int sdoDataSize, unsigned char* sdoData, unsigned int* errCode, unsigned int waitTime = 0, BOOL complete = FALSE);
            ECAPIFUNC SdoUpload(int masterId, int slaveId, int index, int subindex, EcSdoType::T sdoType, int sdoBuffSize, unsigned char* sdoBuff, unsigned int* actualSize, unsigned int* errCode, unsigned int waitTime = 0, BOOL complete = FALSE);
            ECAPIFUNC SdoDownload(int masterId, int slaveId, int index, int subindex, EcSdoType::T sdoType, int sdoDataSize, unsigned char* sdoData, EcSdoDownloadCallBack callbackFunc, unsigned int waitTime, BOOL complete = FALSE);
            ECAPIFUNC SdoUpload(int masterId, int slaveId, int index, int subindex, EcSdoType::T sdoType, EcSdoUploadCallBack callbackFunc, unsigned int waitTime, BOOL complete = FALSE);

            ECAPIFUNC GetSdoInfoODList(int masterId, int slaveId, EcObjectDescriptionListType::T type, EcSlaveSdoInfoObjectDescriptionList* list);
            ECAPIFUNC GetSdoInfoEDList(int masterId, int slaveId, int index, EcSlaveSdoInfoEntryDescriptionList* list);
            ECAPIFUNC GetSdoInfoODList(int masterId, int slaveId, EcObjectDescriptionListType::T type, EcSdoInfoGetODListCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC GetSdoInfoEDList(int masterId, int slaveId, int index, EcSdoInfoGetEDListCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC TxPdoWrite(int masterId, int slaveId, int index, int subindex, int pdoDataSize, unsigned char* pdoData, unsigned int mask = 0);
            ECAPIFUNC ClearTxPdoWrite(int masterId, int slaveId, int index, int subindex);
            ECAPIFUNC PdoRead(int masterId, int slaveId, int index, int subindex, int pdoBuffSize, unsigned char* pdoBuff, unsigned int* actualSize);

            ECAPIFUNC SetOutputBit(int masterId, int slaveId, int byte, int bit, unsigned char data);
            ECAPIFUNC SetOutputByte(int masterId, int slaveId, int byte, unsigned char data);
            ECAPIFUNC SetOutputBytes(int masterId, int slaveId, int byte, int size, unsigned char *pData);
            ECAPIFUNC SetOutputBits(int masterId, int slaveId, int *pByte, int *pBit, unsigned char *pData, int count);
            ECAPIFUNC GetInputBit(int masterId, int slaveId, int byte, int bit, unsigned char *pData);
            ECAPIFUNC GetInputByte(int masterId, int slaveId, int byte, unsigned char *pData);
            ECAPIFUNC GetInputBytes(int masterId, int slaveId, int byte, int size, unsigned char *pData);
            ECAPIFUNC GetOutputBit(int masterId, int slaveId, int byte, int bit, unsigned char *pData);
            ECAPIFUNC GetOutputByte(int masterId, int slaveId, int byte, unsigned char *pData);
            ECAPIFUNC GetOutputBytes(int masterId, int slaveId, int byte, int size, unsigned char *pData);

            ECAPIFUNC FoERead(int masterId, int slaveId, wchar_t* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoEWrite(int masterId, int slaveId, wchar_t* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoERead(int masterId, int slaveId, wchar_t* filePath, char* fileName, unsigned int password, EcFoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC FoEWrite(int masterId, int slaveId, wchar_t* filePath, char* fileName, unsigned int password, EcFoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC FoERead(int masterId, int slaveId, char* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoEWrite(int masterId, int slaveId, char* filePath, char* fileName, unsigned int password, unsigned int* errCode);
            ECAPIFUNC FoERead(int masterId, int slaveId, char* filePath, char* fileName, unsigned int password, EcFoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC FoEWrite(int masterId, int slaveId, char* filePath, char* fileName, unsigned int password, EcFoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC SIIWrite(int masterId, int slaveId, int siiAddr, int len, unsigned char* data, bool checkSum = false);
            ECAPIFUNC SIIRead(int masterId, int slaveId, int siiAddr, int len, unsigned char* buff);
            ECAPIFUNC SIIWrite(int masterId, int slaveId, int siiAddr, int len, unsigned char* data, EcSIIWriteCallBack callbackFunc, unsigned int waitTime, bool checkSum = false);
            ECAPIFUNC SIIRead(int masterId, int slaveId, int siiAddr, int len, EcSIIReadCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC AoERead(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, unsigned char* readAoEBuff, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWrite(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int writeLength, unsigned char* writeAoEData, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWriteControl(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned short aoeState, unsigned short deviceState, unsigned int writeLength, unsigned char* writeAoEData, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoEReadWrite(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, unsigned char* readAoEBuff, unsigned int writeLength, unsigned char* writeAoEData, unsigned int* errCode, unsigned int waitTime = 0, EcAoESender* sender = NULL);
            ECAPIFUNC AoERead(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, EcAoEReadCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWrite(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int writeLength, unsigned char* writeAoEData, EcAoEWriteCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);
            ECAPIFUNC AoEWriteControl(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned short aoeState, unsigned short deviceState, unsigned int writeLength, unsigned char* writeAoEData, EcAoEWriteControlCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);
            ECAPIFUNC AoEReadWrite(int masterId, int slaveId, unsigned char targetNetId[6], unsigned short targetPort, unsigned int indexGroup, unsigned int indexOffset, unsigned int readLength, unsigned int writeLength, unsigned char* writeAoEData, EcAoEReadCallBack callbackFunc, unsigned int waitTime, EcAoESender* sender = NULL);

            ECAPIFUNC SoERead(int masterId, int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int buffSize, unsigned char* readSoEBuffer, unsigned int* actualSize, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SoEWrite(int masterId, int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int dataSize, unsigned char* writeSoEData, unsigned int* errCode, unsigned int waitTime = 0);
            ECAPIFUNC SoERead(int masterId, int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, EcSoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC SoEWrite(int masterId, int slaveId, unsigned char driveNo, unsigned char elementFlags, unsigned short idn, unsigned int dataSize, unsigned char* writeSoEData, EcSoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC VoERead(int masterId, int slaveId, unsigned int* vendorId, unsigned short* vendorType, unsigned int buffSize, unsigned char* readVoEBuffer, unsigned int* actualSize, unsigned int waitTime = 0);
            ECAPIFUNC VoEWrite(int masterId, int slaveId, unsigned int vendorId, unsigned short vendorType, unsigned int dataSize, unsigned char* writeVoEData, unsigned int waitTime = 0);
            ECAPIFUNC VoERead(int masterId, int slaveId, EcVoEReadCallBack callbackFunc, unsigned int waitTime);
            ECAPIFUNC VoEWrite(int masterId, int slaveId, unsigned int vendorId, unsigned short vendorType, unsigned int dataSize, unsigned char* writeVoEData, EcVoEWriteCallBack callbackFunc, unsigned int waitTime);

            ECAPIFUNC SetAxisMode(int masterId, int slaveId, int axisIndex, EcOperationMode::T mode);
            ECAPIFUNC StartAxisPP(int masterId, int slaveId, int axisIndex, int targetPos, unsigned int profileVel, unsigned int profileAcc, unsigned int profileDec, char immediately = -1, char relative = -1);
			ECAPIFUNC StartAxisPV(int masterId, int slaveId, int axisIndex, int targetVel, unsigned int profileAcc, unsigned int profileDec);
			ECAPIFUNC StartAxisPV(int masterId, int slaveId, int axisIndex, int targetVel, unsigned int profileAcc, unsigned int profileDec, unsigned short maxTorque);
			ECAPIFUNC StartAxisTQ(int masterId, int slaveId, int axisIndex, short targetTrq);
			ECAPIFUNC StartAxisTQ(int masterId, int slaveId, int axisIndex, short targetTrq, unsigned int maxMotorSpeed);
            ECAPIFUNC StartAxisHM(int masterId, int slaveId, int axisIndex, int method, unsigned int switchSpeed, unsigned int zeroSpeed, unsigned int hmAcc);
			ECAPIFUNC StartAxisHM(int masterId, int slaveId, int axisIndex, int method, unsigned int switchSpeed, unsigned int zeroSpeed, unsigned int hmAcc, int homeOffset);
            ECAPIFUNC HaltAxis(int masterId, int slaveId, int axisIndex);
            ECAPIFUNC HaltAxisRelease(int masterId, int slaveId, int axisIndex);

            ECAPIFUNC OpenSerialChannel(int masterId, int slaveId, int chnlId, wchar_t* comName, DCB* dcb, COMMTIMEOUTS* timeout, unsigned int* error = NULL);
            ECAPIFUNC OpenSerialChannel(int masterId, int slaveId, int chnlId, char* comName, DCB* dcb, COMMTIMEOUTS* timeout, unsigned int* error = NULL);
            ECAPIFUNC CloseSerialChannel(int masterId, int slaveId, int chnlId);

			ECAPIFUNC VirtualSlaveTxPdoWrite(int masterId, int index, int subindex, int pdoDataSize, unsigned char* pdoData, unsigned int mask = 0);
			ECAPIFUNC VirtualSlaveClearTxPdoWrite(int masterId, int index, int subindex);
			ECAPIFUNC VirtualSlavePdoRead(int masterId, int index, int subindex, int pdoBuffSize, unsigned char* pdoBuff, unsigned int* actualSize);
        };
    }

}

#endif
