#ifndef SIMU_API_LIB_H
#define SIMU_API_LIB_H
#include <windows.h>
#include "WMX3Api.h"


#ifdef WIN32
#define SIMUAPIFUNC long __stdcall
#else
#define SIMUAPIFUNC HRESULT
#endif

namespace wmx3Api {
    namespace simuApi {

        class SimuAxisState {
        public:
            enum T {
                Offline,
                Normal
            };
        };

        class SimuErrorCode: public ErrorCode {
        public:
            enum {
                SimuError = 0x00001000
            };
            static int ConvertSimuErrorCode(int errCode);
        };

        class SimuAxisSignal {
        public:
            unsigned char servoOnFeedbackFlag;
            unsigned char servoOnFeedback;

            unsigned char ampAlarmFlag;
            unsigned char ampAlarm;

            unsigned char ampWarningFlag;
            unsigned char ampWarning;

            unsigned char ampHsFlg;
            unsigned char ampHs;

            unsigned char ampLSPFlag;
            unsigned char ampLSP;

            unsigned char ampLSNFlag;
            unsigned char ampLSN;

            unsigned char zPulse0DetectedFlag;
            unsigned char zPulse0Detected;

            unsigned char zPulse1DetectedFlag;
            unsigned char zPulse1Detected;

            unsigned char touchProbe0DetectedFlag;
            unsigned char touchProbe0Detected;

            unsigned char touchProbe1DetectedFlag;
            unsigned char touchProbe1Detected;

            long alarmCode;
            long warningCode;
            long latchedZPulse0Feedback;
            long latchedZPulse1Feedback;
            long latchedTouchProbe0Feedback;
            long latchedTouchProbe1Feedback;
        };

        class SimuAxisData {
        public:
            SimuAxisState::T state;
            SimuAxisSignal signals;
        };

        class SimuIoData {
        public:
            unsigned char inputFlag[wmx3Api::constants::maxIoInSize];
            unsigned char input[wmx3Api::constants::maxIoInSize];
        };

        class SimuMasterInfo {
        public:
            SimuAxisData axisData[wmx3Api::constants::maxAxes];
            SimuIoData ioData;
        };

        class Simu;

        class Simu {
        private:
            WMX3Api *wmx3Api;
            int statChnlId;
            bool isSelfDev;
            void init(WMX3Api *f);
            void close();

        public:
            Simu(WMX3Api *f);
            Simu(const Simu& src);
            Simu& operator=(const Simu& src);
            Simu();
            Simu(DeviceType::T type);
            ~Simu();

            static SIMUAPIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
            static SIMUAPIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
            static SIMUAPIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
            static SIMUAPIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
            static SIMUAPIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

            bool IsDeviceValid();

            SIMUAPIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

            SIMUAPIFUNC SetInBit(int byte, int bit, unsigned char data);
            SIMUAPIFUNC SetInByte(int byte, unsigned char data);
            SIMUAPIFUNC SetInBytes(int byte, int size, unsigned char *pData);
            SIMUAPIFUNC SetInBits(int *pByte, int *pBit, unsigned char *pData, int count);

            SIMUAPIFUNC ClearInBit(int byte, int bit);
            SIMUAPIFUNC ClearInByte(int byte);
            SIMUAPIFUNC ClearInBytes(int byte, int size);
            SIMUAPIFUNC ClearInBits(int *pByte, int *pBit, int count);

            SIMUAPIFUNC SetServoOnFeedback(int axis, int newStatus);
            SIMUAPIFUNC SetAlarm(int axis, int alarmCode);
            SIMUAPIFUNC SetWarning(int axis, int warningCode);
            SIMUAPIFUNC SetHomeSwitch(int axis, int newStatus);
            SIMUAPIFUNC SetLimitSwitchNegative(int axis, int newStatus);
            SIMUAPIFUNC SetLimitSwitchPositive(int axis, int newStatus);
            SIMUAPIFUNC SetZPulse0Detected(int axis, int latchedZPulseFeedback);
            SIMUAPIFUNC SetZPulse1Detected(int axis, int latchedZPulseFeedback);
            SIMUAPIFUNC SetTouchProbe0Detected(int axis, int touchProbeDetectedFeedback);
            SIMUAPIFUNC SetTouchProbe1Detected(int axis, int touchProbeDetectedFeedback);

            SIMUAPIFUNC ClearServoOnFeedback(int axis);
            SIMUAPIFUNC ClearAlarm(int axis);
            SIMUAPIFUNC ClearWarning(int axis);
            SIMUAPIFUNC ClearHomeSwitch(int axis);
            SIMUAPIFUNC ClearLimitSwitchNegative(int axis);
            SIMUAPIFUNC ClearLimitSwitchPositive(int axis);
            SIMUAPIFUNC ClearZPulse0Detected(int axis);
            SIMUAPIFUNC ClearZPulse1Detected(int axis);
            SIMUAPIFUNC ClearTouchProbe0Detected(int axis);
            SIMUAPIFUNC ClearTouchProbe1Detected(int axis);

            SIMUAPIFUNC SetUpdatePeriod(unsigned int period);
            SIMUAPIFUNC GetMasterInfo(SimuMasterInfo* masterInfo);

        };
    }
}

#endif
