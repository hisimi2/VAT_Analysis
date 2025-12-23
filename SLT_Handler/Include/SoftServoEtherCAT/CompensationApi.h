/**********************************************************************************************************************
*
* CompensationApi.h
*
* This file contains the declarations of the Compensation module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the Compensation module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_COMPENSATION_API_H
#define WMX3_COMPENSATION_API_H
#include <windows.h>
#include "WMX3Api.h"

namespace wmx3Api{

    namespace constants {
        static const int maxPitchErrorCompPoints = 1024;
        static const int max2dPitchErrorCompPoints = 128;
        static const int max2dPitchErrorCompChannel = 32;
        static const int maxSizeSet2dPitchErrorCompValue = 62;
    }

    class CompensationErrorCode : public ErrorCode{
    public:
        enum {
            PitchErrorCompensationIsEnabled = 0x00015000,
            PitchErrorCompensationIsDisabled,
            PitchErrorCompensationDataIsNotSet,
            BacklashCompensationIsEnabled,
            BacklashCompensationIsDisabled,
            PitchCountOutOfRange,
            PitchOriginIndexOutOfRange,
            PitchIntervalOutOfRange,
            EdgeDropoffDistanceOutOfRange,
            CompensationAxisNotPosMode,
            ReferenceAxisNotPosMode
        };
    };

    class PitchErrorCompensationOriginPositionType {
    public:
        enum T {
            Absolute,
            Relative
        };
    };

    class PitchErrorCompensationAlignmentType {
    public:
        enum T {
            AlignBeforeCompensation
        };
    };

    class PitchErrorCompensationOptions {
    public:
        PitchErrorCompensationOptions();
        double catchUpVelocity;
        double catchUpAcc;
        PitchErrorCompensationOriginPositionType::T originPositionType;
        PitchErrorCompensationAlignmentType::T alignmentType;
        bool preventCatchUpMotion;
    };

    class PitchErrorCompensationData{
    public:
        PitchErrorCompensationData();
        int enable;
        double pitchOriginPosition;
        int pitchOriginIndex;
        double pitchInterval;
        char pitchIntervalDirection;
        int pitchCount;
        double pitchCompensationValue[constants::maxPitchErrorCompPoints];
        double edgeDropoffDistance;
        PitchErrorCompensationOptions options;
    };

    class TwoDPitchErrorCompensationData{
    public:
        TwoDPitchErrorCompensationData();
        int enable;
        int axis;
        int referenceAxis[2];
        double pitchOriginPosition[2];
        int pitchOriginIndex[2];
        double pitchInterval[2];
        char pitchIntervalDirection[2];
        int pitchCount[2];
        double pitchCompensationValue[constants::max2dPitchErrorCompPoints][constants::max2dPitchErrorCompPoints];
        double edgeDropoffDistance[2];
        PitchErrorCompensationOptions options;
    };

    class BacklashCompensationData{
    public:
        BacklashCompensationData();
        int enable;
        char offsetDirection;
        double backlashHigh;
        double backlashLow;
        double distanceHigh;
        double distanceLow;
    };

    class Compensation {
    private:
        WMX3Api *wmx3Api;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
    public:
        Compensation(WMX3Api *f);
        Compensation(const Compensation& src);
        Compensation& operator=(const Compensation& src);
        Compensation();
        ~Compensation();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC SetPitchErrorCompensation(int axis, PitchErrorCompensationData *pitchErrCompData);
        WMX3APIFUNC GetPitchErrorCompensation(int axis, PitchErrorCompensationData *pitchErrCompData);
        WMX3APIFUNC EnablePitchErrorCompensation(int axis);
        WMX3APIFUNC DisablePitchErrorCompensation(int axis);
        WMX3APIFUNC GetPitchErrorCompensationAtPosition(int axis, double pos, double *pCompensation);

        WMX3APIFUNC Set2DPitchErrorCompensation(unsigned int channel, TwoDPitchErrorCompensationData *pitchErrCompData2D);
        WMX3APIFUNC Get2DPitchErrorCompensation(unsigned int channel, TwoDPitchErrorCompensationData *pitchErrCompData2D);
        WMX3APIFUNC Enable2DPitchErrorCompensation(unsigned int channel);
        WMX3APIFUNC Disable2DPitchErrorCompensation(unsigned int channel);
        WMX3APIFUNC Get2DPitchErrorCompensationAtPosition(unsigned int channel, double refPos1, double refPos2, double *pCompensation);

        WMX3APIFUNC SetBacklashCompensation(int axis, BacklashCompensationData *pBacklashCompData);
        WMX3APIFUNC GetBacklashCompensation(int axis, BacklashCompensationData *pBacklashCompData);
        WMX3APIFUNC EnableBacklashCompensation(int axis);
        WMX3APIFUNC DisableBacklashCompensation(int axis);
    };


}

#endif
