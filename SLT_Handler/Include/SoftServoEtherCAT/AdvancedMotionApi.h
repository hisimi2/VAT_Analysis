/**********************************************************************************************************************
*
* AdvancedMotionApi.h
*
* This file contains the declarations of the AdvancedMotion module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the AdvancedMotion module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_ADVANCED_MOTION_API_H
#define WMX3_ADVANCED_MOTION_API_H
#include <windows.h>
#include "WMX3Api.h"
#include "CoreMotionApi.h"

namespace wmx3Api{

    namespace constants {
        static const int maxSplineDimensions = 8;
        static const int maxSplineChannel = 128;
        static const int maxPvtAppendPoints = 4096;
        static const int maxPvtInterpolateAppendPoints = 2048;
        static const int maxPvtInterpolateAxes = 8;
        static const int maxPathInterpolateAppendPoints = 512;
        static const int maxPathInterpolateOutputs = 512;
        static const int maxPathInterpolateDimensions = 2;
        static const int max3DPathInterpolateDimensions = 3;
        static const int maxPathIntplWithRotationAppendPoints = 1024;
        static const int maxPathIntplWithRotationChannel = 128;
        static const int maxPathIntplLookaheadDimensions = 6;
        static const int maxPathIntplLookaheadAppendPoints = 2000;
        static const int maxPathIntplLookaheadChannel = 128;
        static const int maxPathIntplLookaheadOutputPerSegment = 128;
        static const int maxPathIntplLookaheadFollowAxes = 4;
        static const int maxEcamPoints = 4096;
        static const int maxEcamChannel = 8;
    }

    class AdvancedMotionErrorCode : public CoreMotionErrorCode{
    public:
        enum {
            PositionNotOnPath = 0x00017000,
            SplineModeNotSupported,
            SplineStartingPositionNotCurrentPosition,
            PathInterpolationChannelNotCleared,
            PathInterpolationConfigurationNotSet,
            PathInterpolationAxesHaveBeenMoved,
            PathInterpolationAbnormalStop,
            NotConstLinearVelocitySlave,
            ConstLinearVelocityMinRTooSmall,
            ConstLinearVelocityPositiveMaxRTooSmall,
            ConstLinearVelocityNegativeMaxRTooSmall,
            InvalidCurrentVelocity,
            MasterPositionNotAscendingOrder,
            PositionNotWithinSingleTurnEncoderCount,
            TimeBetweenPointsTooClose,
            TotalTimeTooSmall,
            DimensionOutOfRange,
            PointCountBelowMinimum,
            L1ArgumentOutOfRange,
            L2ArgumentOutOfRange,
            LdirArgumentOutOfRange,
            RzeroArgumentOutOfRange,
            RspanArgumentOutOfRange,
            L1ArgumentGreaterThanL2,
            SlaveCurrentPositionNotInRange,
            OutputCountOutOfRange,
            OutputPointNotFound,
            ConfigurationNotSet,
            SegmentTypeNotSupported,
            AngleCorrectionProfileArgumentOutOfRange,
            AxisNotExecutingPathIntpl,
            AxisNotExecutingPathIntpl3D,
            VelocityMultiplierOutOfRange,
            NoCommandsInBuffer,
            DancerControlDerivativeSamplesOutOfRange,
            DancerControlIntegralTimeOutOfRange,
            DancerControlInputMinMaxDifferenceOutOfRange,
            FirstPointTimeNotZero,
            PointTimeOutOfRange,
            PointTimeNotIncreasing,
            SmoothRatioOutOfRange,
            TwoLinkMotionTypeOutOfRange,
            OutputIOAddressOutOfRange,
            OutputPointOutOfRange,
            TotalDistanceBelowMinimum,
            DistanceBetweenPointsTooClose
        };
    };

    class AdvancedMotion;

    class AdvSync {
    public:
        AdvancedMotion *amApi;
        AdvSync(AdvancedMotion *f) : amApi(f) {}

        bool IsDeviceValid();

        class ECAMType{
        public:
            enum T {
                Normal,
                Periodic,
                Repeat
            };
        };

        class ECAMSourceType {
        public:
            enum T {
                MasterCommandPos,
                MasterFeedbackPos,
                EncoderInput
            };
        };

        class ECAMSourceOptions {
        public:
            ECAMSourceOptions();
            ECAMSourceType::T type;
            unsigned int encoderByteAddress;
            unsigned int encoderSize;
        };

        class ECAMClutchType{
        public:
            enum T {
                None,
                SimpleCatchUp
            };
        };

        class ECAMClutchOptions{
        public:
            ECAMClutchOptions();
            ECAMClutchType::T type;
            double simpleCatchUpVelocity;
            double simpleCatchUpAcc;
        };

        class ECAMOptions{
        public:
            ECAMOptions();
            ECAMType::T type;
            ECAMSourceOptions source;
            ECAMClutchOptions clutch;
        };

        class ECAMData{
        public:
            ECAMData();
            int slaveAxis;
            int masterAxis;
            unsigned int numPoints;
            ECAMOptions options;
            double slavePos[constants::maxEcamPoints];
            double masterPos[constants::maxEcamPoints];
        };

        class DancerControlOptions {
        public:
            DancerControlOptions();
            double proportionalGain;
            double integralGain;
            double derivativeGain;
            double gainFactor;
            int derivativeSamples;
            int integralTimeCycles;
            double maxIntegral;
            unsigned int inputByteAddress;
            unsigned int inputSize;
            double inputZero;
            double inputMax;
            double inputMin;
            unsigned char useOutput;
            double zeroClampMinInput;
            double zeroClampMinVel;
        };

        class DancerControlStatus {
        public:
            DancerControlStatus();
            double P;
            double I;
            double D;
            double inputValue;
            int iSamples;
            int dSamples;
        };

        WMX3APIFUNC StartECAM(int channel, ECAMData *pECAMData);
        WMX3APIFUNC GetECAM(int channel, ECAMData *pECAMData);
        WMX3APIFUNC StopECAM(int channel);
        WMX3APIFUNC StartDancerControl(int axis, DancerControlOptions *pDancerControlOptions);
        WMX3APIFUNC StopDancerControl(int axis);
        WMX3APIFUNC GetDancerControlStatus(int axis, DancerControlStatus *pDancerControlStatus);
    };


    class AdvVelocity {
    public:
        AdvancedMotion *amApi;
        AdvVelocity(AdvancedMotion *f) : amApi(f) {}

        bool IsDeviceValid();

        WMX3APIFUNC SetConstantLinearVel(int masterAxis, int slaveAxis,  double centerPos, double minR,
            double positiveMaxR, double negativeMaxR, double constLinVel, double initVelVariance);
        WMX3APIFUNC ResolveConstantLinearVel(int axis);
    };


    class AdvMotion{
    public:
        AdvancedMotion *amApi;
        AdvMotion(AdvancedMotion *f) : amApi(f){}

        bool IsDeviceValid();

        class SplinePoint{
        public:
            SplinePoint();
            double pos[constants::maxSplineDimensions];
        };

        class PointTimeSplineCommand{
        public:
            PointTimeSplineCommand();
            unsigned int dimensionCount;
            int axis[constants::maxSplineDimensions];
        };

        class TotalTimeSplineCommand {
        public:
            TotalTimeSplineCommand();
            unsigned int dimensionCount;
            int axis[constants::maxSplineDimensions];

            double totalTimeMilliseconds;
            unsigned char ignoreDimensionForDistanceCalc[constants::maxSplineDimensions];
        };

        class ProfileSplineCommand{
        public:
            ProfileSplineCommand();
            unsigned int dimensionCount;
            int axis[constants::maxSplineDimensions];

            Profile profile;
            unsigned char ignoreDimensionForDistanceCalc[constants::maxSplineDimensions];
            unsigned int sampleMultiplier;
            unsigned char sampleMultiplierCubicDistribution;
        };

        class VelAccLimitedSplineCommand{
        public:
            VelAccLimitedSplineCommand();
            unsigned int dimensionCount;
            int axis[constants::maxSplineDimensions];

            double velLimit[constants::maxSplineDimensions];
            double accLimit[constants::maxSplineDimensions];
            double compositeVel;
            double compositeAcc;
            unsigned char ignoreDimensionForDistanceCalc[constants::maxSplineDimensions];
            unsigned int sampleMultiplier;
            unsigned char sampleMultiplierCubicDistribution;
        };

        class PVTPoint{
        public:
            PVTPoint();
            double pos;
            double velocity;
            double timeMilliseconds;
        };

        class PVTCommand{
        public:
            PVTCommand();
            int axis;
            unsigned int pointCount;
            PVTPoint points[constants::maxPvtAppendPoints];
        };

        class PVTAdditionalCommand{
        public:
            PVTAdditionalCommand();
            unsigned int pointCount;
            PVTPoint points[constants::maxPvtAppendPoints];
        };

        class PVTIntplCommand{
        public:
            PVTIntplCommand();
            unsigned int axisCount;
            int axis[constants::maxPvtInterpolateAxes];
            unsigned int pointCount[constants::maxPvtInterpolateAxes];
            PVTPoint points[constants::maxPvtInterpolateAxes][constants::maxPvtInterpolateAppendPoints];
        };

        class PVTIntplAdditionalCommand{
        public:
            PVTIntplAdditionalCommand();
            unsigned int pointCount[constants::maxPvtInterpolateAxes];
            PVTPoint points[constants::maxPvtInterpolateAxes][constants::maxPvtInterpolateAppendPoints];
        };

        class PTPoint {
        public:
            PTPoint();
            double pos;
            double timeMilliseconds;
        };

        class PTCommand {
        public:
            PTCommand();
            int axis;
            unsigned int pointCount;
            PTPoint points[constants::maxPvtAppendPoints];
        };

        class PTAdditionalCommand {
        public:
            PTAdditionalCommand();
            unsigned int pointCount;
            PTPoint points[constants::maxPvtAppendPoints];
        };

        class VTPoint {
        public:
            VTPoint();
            double velocity;
            double timeMilliseconds;
        };

        class VTCommand {
        public:
            VTCommand();
            int axis;
            unsigned int pointCount;
            VTPoint points[constants::maxPvtAppendPoints];
        };

        class VTAdditionalCommand {
        public:
            VTAdditionalCommand();
            unsigned int pointCount;
            VTPoint points[constants::maxPvtAppendPoints];
        };

        class ATPoint {
        public:
            ATPoint();
            double acc;
            double timeMilliseconds;
        };

        class ATCommand {
        public:
            ATCommand();
            int axis;
            unsigned int pointCount;
            ATPoint points[constants::maxPvtAppendPoints];
        };

        class ATAdditionalCommand {
        public:
            ATAdditionalCommand();
            unsigned int pointCount;
            ATPoint points[constants::maxPvtAppendPoints];
        };

        class PathIntplSegmentType {
        public:
            enum T {
                Linear,
                Circular,
                Pause
            };
        };

        class PathIntplOutputType {
        public:
            enum T {
                Immediate,
                RemainingTime,
                CompletedTime,
                RemainingDist,
                CompletedDist,
                DistanceRatio
            };
        };

        class PathIntplOutputSource {
        public:
            enum T {
                IOOutput,
                UserMemory
            };
        };

        class PathIntplCoordinateType {
        public:
            enum T {
                Absolute,
                RelativeFromStart,
                RelativeFromEnd
            };
        };

        class PathIntplCommand{
        public:
            PathIntplCommand();

            int axis[constants::maxPathInterpolateDimensions];
            Profile profile[constants::maxPathInterpolateAppendPoints];

            unsigned int numPoints;
            PathIntplSegmentType::T type[constants::maxPathInterpolateAppendPoints];
            char direction[constants::maxPathInterpolateAppendPoints];
            double target[constants::maxPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double centerPos[constants::maxPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double timeMilliseconds[constants::maxPathInterpolateAppendPoints];
            char enableAutoSmooth;
            double autoSmoothRadius[constants::maxPathInterpolateAppendPoints];
            char enableConstProfile;

            unsigned int numOutputs;
            PathIntplOutputType::T outputType[constants::maxPathInterpolateOutputs];
            unsigned int outputPoint[constants::maxPathInterpolateOutputs];
            double outputTriggerValue[constants::maxPathInterpolateOutputs];
            PathIntplOutputSource::T outputSource[constants::maxPathInterpolateOutputs];
            int outputByteAddr[constants::maxPathInterpolateOutputs];
            char outputBitAddr[constants::maxPathInterpolateOutputs];
            char outputValue[constants::maxPathInterpolateOutputs];
        };

        class PathIntplAdditionalCommand{
        public:
            PathIntplAdditionalCommand();

            Profile profile[constants::maxPathInterpolateAppendPoints];

            unsigned int numPoints;
            PathIntplSegmentType::T type[constants::maxPathInterpolateAppendPoints];
            char direction[constants::maxPathInterpolateAppendPoints];
            double target[constants::maxPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double centerPos[constants::maxPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double timeMilliseconds[constants::maxPathInterpolateAppendPoints];
            double autoSmoothRadius[constants::maxPathInterpolateAppendPoints];
        };

        class PathIntpl3DCommand {
        public:
            PathIntpl3DCommand();

            int axis[constants::max3DPathInterpolateDimensions];
            Profile profile[constants::maxPathInterpolateAppendPoints];

            unsigned int numPoints;
            PathIntplSegmentType::T type[constants::maxPathInterpolateAppendPoints];
            double target[constants::max3DPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double circleIntermediateTarget[constants::max3DPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double timeMilliseconds[constants::maxPathInterpolateAppendPoints];
            char enableAutoSmooth;
            double autoSmoothRadius[constants::maxPathInterpolateAppendPoints];
            char enableConstProfile;

            unsigned int numOutputs;
            PathIntplOutputType::T outputType[constants::maxPathInterpolateOutputs];
            unsigned int outputPoint[constants::maxPathInterpolateOutputs];
            double outputTriggerValue[constants::maxPathInterpolateOutputs];
            PathIntplOutputSource::T outputSource[constants::maxPathInterpolateOutputs];
            int outputByteAddr[constants::maxPathInterpolateOutputs];
            char outputBitAddr[constants::maxPathInterpolateOutputs];
            char outputValue[constants::maxPathInterpolateOutputs];
        };

        class PathIntpl3DAdditionalCommand {
        public:
            PathIntpl3DAdditionalCommand();

            Profile profile[constants::maxPathInterpolateAppendPoints];

            unsigned int numPoints;
            PathIntplSegmentType::T type[constants::maxPathInterpolateAppendPoints];
            double target[constants::max3DPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double circleIntermediateTarget[constants::max3DPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double timeMilliseconds[constants::maxPathInterpolateAppendPoints];
            double autoSmoothRadius[constants::maxPathInterpolateAppendPoints];
        };

        class PathIntplWithRotationConfiguration {
        public:
            PathIntplWithRotationConfiguration();

            int axis[constants::maxPathInterpolateDimensions];
            char enableAutoSmooth;
            char axisCoordinateFlip[constants::maxPathInterpolateDimensions];
            int rotationalAxis;
            char rotationalAxisPolarity;
            char disableRotationalAxis;
            char disableXYRotationalMotion;
            double centerOfRotation[constants::maxPathInterpolateDimensions];
            PathIntplCoordinateType::T coordinateType;
            char enableZAxis;
            int zAxis;
            char enableConstProfile;
            char enableLocalCenterOfRotation;

            double angleCorrectionMinimumAngle;
            Profile angleCorrectionProfile;

            unsigned int numOutputs;
            PathIntplOutputType::T outputType[constants::maxPathInterpolateOutputs];
            unsigned int outputPoint[constants::maxPathInterpolateOutputs];
            double outputTriggerValue[constants::maxPathInterpolateOutputs];
            PathIntplOutputSource::T outputSource[constants::maxPathInterpolateOutputs];
            int outputByteAddr[constants::maxPathInterpolateOutputs];
            char outputBitAddr[constants::maxPathInterpolateOutputs];
            char outputValue[constants::maxPathInterpolateOutputs];
        };

        class PathIntplWithRotationCommandPoint {
        public:
            PathIntplWithRotationCommandPoint();

            PathIntplSegmentType::T type;
            Profile profile;
            char direction;
            double target[constants::maxPathInterpolateDimensions];
            double centerPos[constants::maxPathInterpolateDimensions];
            double autoSmoothRadius;
            double zAxisTarget;
            char useLocalCenterOfRotation;
            char localCenterOfRotationDirection;
            double localCenterOfRotation[constants::maxPathInterpolateDimensions];
        };

        class PathIntplWithRotationCommand{
        public:
            PathIntplWithRotationCommand();

            unsigned int numPoints;
            PathIntplWithRotationCommandPoint point[constants::maxPathIntplWithRotationAppendPoints];
        };

        class PathIntplWithRotationState {
        public:
            enum T {
                Idle,
                Executing
            };
        };

        class PathIntplWithRotationStatus{
        public:
            PathIntplWithRotationStatus();

            PathIntplWithRotationState::T state;
            unsigned int freeBuffer;
            unsigned int executedBuffer;
            unsigned int maxBuffer;
            unsigned int totalBuffer;
            unsigned int executedCommandCount;
            unsigned int totalCommandCount;
            double pos;
            double totalDist;
        };

        class PathIntplLookaheadProfileType {
        public:
            enum T {
                Trapezoidal,
                SCurve
            };
        };

        class PathIntplLookaheadSegmentType {
        public:
            enum T {
                Linear,
                CenterAndLengthCircular,
                CenterAndEndCircular,
                ThroughAndEndCircular,
                LengthAndEndCircular,
                RadiusAndEndCircular,
                ThroughAndEnd3DCircular,
                ThroughAndEnd3DCircularWithAuxiliary,
                Sleep,
                SetOutputBit,
                FollowMotion
            };
        };

        class PathIntplLookaheadCoordinateType {
        public:
            enum T {
                Absolute,
                RelativeFromPreviousPoint,
                RelativeFromStart
            };
        };

        class PathIntplLookaheadConfiguration {
        public:
            PathIntplLookaheadConfiguration();

            unsigned int axisCount;
            int axis[constants::maxPathIntplLookaheadDimensions];

            double velocityLimit[constants::maxPathIntplLookaheadDimensions];
            double accLimit[constants::maxPathIntplLookaheadDimensions];
            double compositeVel;
            double compositeAcc;
            double sampleDistance;

            bool setOverrideTime;
            unsigned int overrideTimePointsPerMillisecond;
            bool setAngleTolerance;
            double angleToleranceDegrees;
            PathIntplLookaheadCoordinateType::T coordinateType;
            bool stopOnEmptyBuffer;

            unsigned int followAxisCount;
            int followAxis[constants::maxPathIntplLookaheadFollowAxes];
        };

        class PathIntplLookaheadCommandPoint {
        public:
            PathIntplLookaheadCommandPoint();

            PathIntplLookaheadSegmentType::T type;
            double smoothRadius;
            bool setSegmentCompositeVel;
            double segmentCompositeVel;

            union Data {
                Data();

                struct {
                    unsigned int axisCount;
                    int axis[constants::maxPathIntplLookaheadDimensions];
                    double target[constants::maxPathIntplLookaheadDimensions];
                }linear;
                struct {
                    int axis[2];
                    double centerPos[2];
                    double arcLengthDegree;
                    unsigned char clockwise;
                }centerAndLengthCircular;
                struct {
                    int axis[2];
                    double centerPos[2];
                    double endPos[2];
                    unsigned char clockwise;
                }centerAndEndCircular;
                struct {
                    int axis[2];
                    double throughPos[2];
                    double endPos[2];
                }throughAndEndCircular;
                struct {
                    int axis[2];
                    double endPos[2];
                    double arcLengthDegree;
                    unsigned char clockwise;
                }lengthAndEndCircular;
                struct {
                    int axis[2];
                    double endPos[2];
                    double radius;
                    unsigned char clockwise;
                }radiusAndEndCircular;
                struct {
                    int axis[3];
                    double throughPos[3];
                    double endPos[3];
                }throughAndEnd3DCircular;
                struct {
                    int axis[3];
                    double throughPos[3];
                    double endPos[3];
                    unsigned int auxiliaryAxisCount;
                    int auxiliaryAxis[3];
                    double auxiliaryTarget[3];
                }throughAndEnd3DCircularWithAuxiliary;
                struct {
                    unsigned int milliseconds;
                }sleep;
                struct {
                    PathIntplOutputType::T type;
                    double triggerValue;
                    PathIntplOutputSource::T source;
                    unsigned int byteAddress;
                    unsigned char bitAddress;
                    unsigned char value;
                }setOutputBit;
                struct {
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
                    double accTimeMilliseconds;
                    double decTimeMilliseconds;
                    double startingVelocity;
                    double endVelocity;
                    double secondVelocity;
                    double movingAverageTimeMilliseconds;
                }followMotion;
            }data;
        };

        class PathIntplLookaheadCommand {
        public:
            PathIntplLookaheadCommand();

            unsigned int numPoints;
            PathIntplLookaheadCommandPoint point[constants::maxPathIntplLookaheadAppendPoints];
        };

        class PathIntplLookaheadState {
        public:
            enum T {
                Idle,
                Executing,
                Stopping,
                Stopped
            };
        };

        class PathIntplLookaheadStatus {
        public:
            PathIntplLookaheadStatus();

            PathIntplLookaheadState::T state;
            unsigned int remainBuffer;
            unsigned int freeBuffer;
            unsigned int executedBuffer;
            unsigned int maxBuffer;
            unsigned int remainCommandCount;
            unsigned int executedCommandCount;
            unsigned int totalCommandCount;
            double lastOverrideTimeMilliseconds;
            double lastOverridePointsPerMillisecond;
        };

        class PosCommand {
        public:
            PosCommand();

            int axis;
            double target;
            Profile profile;
        };

        class CoordinatedPosCommand{
        public:
            CoordinatedPosCommand();

            PosCommand posCommand;
            int axis2;
            double axis2Target;
            double axis2SmoothRatio;
        };

        class CoordinatedJerkRatioPosCommand {
        public:
            CoordinatedJerkRatioPosCommand();

            PosCommand posCommand;
            int followerAxisCount;
            int followerAxis[constants::maxAxes];
            double followerAxisTarget[constants::maxAxes];
            double followerAxisAcc[constants::maxAxes];
            double followerAxisJerkAccRatio[constants::maxAxes];
        };

        class TwoLinkLinearCommand {
        public:
            TwoLinkLinearCommand();

            int axis;
            double target;
            Profile masterProfile;
            double L1;
            double L2;
            double Lzero;
            int Lpolarity;
            double Rspan;
            double Rzero;
            unsigned char specifyTargetInRotaryCoordinates;
        };

        class TwoLinkRotaryCommand {
        public:
            TwoLinkRotaryCommand();

            int axis;
            double target;
            Profile masterProfile;
            double L1;
            double L2;
            double Lzero;
            int Lpolarity;
            double Rzero;
            unsigned char specifyTargetInLinearCoordinates;
        };

        class SimulatePathIntplCommand{
        public:
            SimulatePathIntplCommand();

            int axis[constants::maxPathInterpolateDimensions];
            Profile profile[constants::maxPathInterpolateAppendPoints];

            unsigned int numPoints;
            PathIntplSegmentType::T type[constants::maxPathInterpolateAppendPoints];
            char direction[constants::maxPathInterpolateAppendPoints];
            double target[constants::maxPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double centerPos[constants::maxPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double timeMilliseconds[constants::maxPathInterpolateAppendPoints];
            char enableAutoSmooth;
            double autoSmoothRadius[constants::maxPathInterpolateAppendPoints];
            char enableConstProfile;
            char setStartPos;
            double startPos[constants::maxPathInterpolateDimensions];
        };

        class SimulatePathIntpl3DCommand {
        public:
            SimulatePathIntpl3DCommand();

            int axis[constants::max3DPathInterpolateDimensions];
            Profile profile[constants::maxPathInterpolateAppendPoints];

            unsigned int numPoints;
            PathIntplSegmentType::T type[constants::maxPathInterpolateAppendPoints];
            double target[constants::max3DPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double circleIntermediateTarget[constants::max3DPathInterpolateDimensions][constants::maxPathInterpolateAppendPoints];
            double timeMilliseconds[constants::maxPathInterpolateAppendPoints];
            char enableAutoSmooth;
            double autoSmoothRadius[constants::maxPathInterpolateAppendPoints];
            char enableConstProfile;
            char setStartPos;
            double startPos[constants::max3DPathInterpolateDimensions];
        };

        WMX3APIFUNC CreateSplineBuffer(int channel, unsigned int points);
        WMX3APIFUNC FreeSplineBuffer(int channel);
        WMX3APIFUNC GetSplineBufferPoints(int channel, unsigned int *pPoints);
        WMX3APIFUNC GetSplineBytesPerPoint(unsigned int *pBytes);
        WMX3APIFUNC StartCSplinePos(int channel, PointTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint,
            unsigned int *pPointTimeMilliseconds);
        WMX3APIFUNC StartCSplinePos(int channel, TotalTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCSplinePos(int channel, ProfileSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCSplinePos(int channel, VelAccLimitedSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCSplineMov(int channel, PointTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint,
            unsigned int *pPointTimeMilliseconds);
        WMX3APIFUNC StartCSplineMov(int channel, TotalTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCSplineMov(int channel, ProfileSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCSplineMov(int channel, VelAccLimitedSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCBSplinePos(int channel, PointTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint,
            unsigned int *pPointTimeMilliseconds);
        WMX3APIFUNC StartCBSplinePos(int channel, TotalTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCBSplinePos(int channel, ProfileSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCBSplinePos(int channel, VelAccLimitedSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCBSplineMov(int channel, PointTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint,
            unsigned int *pPointTimeMilliseconds);
        WMX3APIFUNC StartCBSplineMov(int channel, TotalTimeSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCBSplineMov(int channel, ProfileSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);
        WMX3APIFUNC StartCBSplineMov(int channel, VelAccLimitedSplineCommand *pSplineCommand, unsigned int numPoints, SplinePoint *pPoint);

        WMX3APIFUNC CreatePVTBuffer(int axis, unsigned int points);
        WMX3APIFUNC FreePVTBuffer(int axis);
        WMX3APIFUNC GetPVTBufferPoints(int axis, unsigned int *pPoints);
        WMX3APIFUNC GetPVTBytesPerPoint(unsigned int *pBytes);
        WMX3APIFUNC StartPVT(PVTCommand *pPVTCommand, unsigned int numAddlCommands = 0, PVTAdditionalCommand *pPVTAddlCommand = NULL);
        WMX3APIFUNC StartPVT(PVTIntplCommand *pPVTCommand, unsigned int numAddlCommands = 0, PVTIntplAdditionalCommand *pPVTAddlCommand = NULL);
        WMX3APIFUNC StartPT(PTCommand *pPTCommand, unsigned int numAddlCommands = 0, PTAdditionalCommand *pPTAddlCommand = NULL);
        WMX3APIFUNC StartVT(VTCommand *pVTCommand, unsigned int numAddlCommands = 0, VTAdditionalCommand *pVTAddlCommand = NULL);
        WMX3APIFUNC StartAT(ATCommand *pATCommand, unsigned int numAddlCommands = 0, ATAdditionalCommand *pATAddlCommand = NULL);

        WMX3APIFUNC CreatePathIntplBuffer(int axis, unsigned int points);
        WMX3APIFUNC FreePathIntplBuffer(int axis);
        WMX3APIFUNC GetPathIntplBufferPoints(int axis, unsigned int *pPoints);
        WMX3APIFUNC GetPathIntplBytesPerPoint(unsigned int *pBytes);
        WMX3APIFUNC StartPathIntplPos(PathIntplCommand *pPathIntplCommand,
            unsigned int numAddlCommands = 0, PathIntplAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntplMov(PathIntplCommand *pPathIntplCommand,
            unsigned int numAddlCommands = 0, PathIntplAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntplPos(PathIntplCommand *pPathIntplCommand, Trigger *pTrigger,
            unsigned int numAddlCommands = 0, PathIntplAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntplMov(PathIntplCommand *pPathIntplCommand, Trigger *pTrigger,
            unsigned int numAddlCommands = 0, PathIntplAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntplPos(PathIntplCommand *pPathIntplCommand, TriggerEvents *pTriggerEvents,
            unsigned int numAddlCommands = 0, PathIntplAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntplMov(PathIntplCommand *pPathIntplCommand, TriggerEvents *pTriggerEvents,
            unsigned int numAddlCommands = 0, PathIntplAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC OverridePathIntplVelocityMultiplier(int axis, double multiplier);
        WMX3APIFUNC StartPathIntpl3DPos(PathIntpl3DCommand *pPathIntplCommand,
            unsigned int numAddlCommands = 0, PathIntpl3DAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntpl3DMov(PathIntpl3DCommand *pPathIntplCommand,
            unsigned int numAddlCommands = 0, PathIntpl3DAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntpl3DPos(PathIntpl3DCommand *pPathIntplCommand, Trigger *pTrigger,
            unsigned int numAddlCommands = 0, PathIntpl3DAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntpl3DMov(PathIntpl3DCommand *pPathIntplCommand, Trigger *pTrigger,
            unsigned int numAddlCommands = 0, PathIntpl3DAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntpl3DPos(PathIntpl3DCommand *pPathIntplCommand, TriggerEvents *pTriggerEvents,
            unsigned int numAddlCommands = 0, PathIntpl3DAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC StartPathIntpl3DMov(PathIntpl3DCommand *pPathIntplCommand, TriggerEvents *pTriggerEvents,
            unsigned int numAddlCommands = 0, PathIntpl3DAdditionalCommand *pPathIntplAddlCommand = NULL);
        WMX3APIFUNC OverridePathIntpl3DVelocityMultiplier(int axis, double multiplier);

        WMX3APIFUNC CreatePathIntplWithRotationBuffer(int channel, unsigned int points);
        WMX3APIFUNC FreePathIntplWithRotationBuffer(int channel);
        WMX3APIFUNC GetPathIntplWithRotationBytesPerPoint(unsigned int *pBytes);
        WMX3APIFUNC SetPathIntplWithRotationConfiguration(int channel, PathIntplWithRotationConfiguration *pConfig);
        WMX3APIFUNC AddPathIntplWithRotationCommand(int channel, PathIntplWithRotationCommand *pCommand);
        WMX3APIFUNC StartPathIntplWithRotation(int channel);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, Trigger *pTrigger);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, double pos);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, double pos, Trigger *pTrigger);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, double pos, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, unsigned int point);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, unsigned int point, Trigger *pTrigger);
        WMX3APIFUNC StartPathIntplWithRotation(int channel, unsigned int point, TriggerEvents *pTriggerEvents);
        WMX3APIFUNC ClearPathIntplWithRotation(int channel);
        WMX3APIFUNC GetPathIntplWithRotationStatus(int channel, PathIntplWithRotationStatus *pStatus);

        WMX3APIFUNC CreatePathIntplLookaheadBuffer(int channel, unsigned int points);
        WMX3APIFUNC FreePathIntplLookaheadBuffer(int channel);
        WMX3APIFUNC GetPathIntplLookaheadBytesPerPoint(unsigned int *pBytes);
        WMX3APIFUNC SetPathIntplLookaheadConfiguration(int channel, PathIntplLookaheadConfiguration *pConfig);
        WMX3APIFUNC AddPathIntplLookaheadCommand(int channel, PathIntplLookaheadCommand *pCommand);
        WMX3APIFUNC StartPathIntplLookahead(int channel);
        WMX3APIFUNC StopPathIntplLookahead(int channel);
        WMX3APIFUNC ClearPathIntplLookahead(int channel);
        WMX3APIFUNC GetPathIntplLookaheadStatus(int channel, PathIntplLookaheadStatus *pStatus);

        WMX3APIFUNC StartCoordinatedPos(CoordinatedPosCommand *pPosCommand);
        WMX3APIFUNC StartCoordinatedPos(unsigned int numCommands, CoordinatedPosCommand *pPosCommand);
        WMX3APIFUNC StartCoordinatedPos(CoordinatedJerkRatioPosCommand *pPosCommand);

        WMX3APIFUNC StartTwoLinkLinearPos(TwoLinkLinearCommand *pTwoLinkCommand);
        WMX3APIFUNC StartTwoLinkLinearMov(TwoLinkLinearCommand *pTwoLinkCommand);
        WMX3APIFUNC StartTwoLinkRotaryPos(TwoLinkRotaryCommand *pTwoLinkCommand);
        WMX3APIFUNC StartTwoLinkRotaryMov(TwoLinkRotaryCommand *pTwoLinkCommand);
        WMX3APIFUNC StartTwoLinkUntetheredLinearPos(TwoLinkLinearCommand *pTwoLinkCommand);
        WMX3APIFUNC StartTwoLinkUntetheredLinearMov(TwoLinkLinearCommand *pTwoLinkCommand);
        WMX3APIFUNC StartTwoLinkUntetheredRotaryPos(TwoLinkRotaryCommand *pTwoLinkCommand);
        WMX3APIFUNC StartTwoLinkUntetheredRotaryMov(TwoLinkRotaryCommand *pTwoLinkCommand);

        WMX3APIFUNC SimulatePosAtTime(SimulatePathIntplCommand *pPathIntplCommand, double timeMilliseconds, double *pPos1,
            double *pPos2, double *pMoveDistance, double *pRemainDistance, double *pTotalDistance);
        WMX3APIFUNC SimulateTimeAtPos(SimulatePathIntplCommand *pPathIntplCommand, double specificPos1, double specificPos2,
            unsigned int minimumSearchIndex, double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds,
            double *pTotalTimeMilliseconds);
        WMX3APIFUNC SimulateTimeAtDist(SimulatePathIntplCommand *pPathIntplCommand, double specificDistance,
            double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds, double *pTotalTimeMilliseconds);
        WMX3APIFUNC SimulatePosAtTime(SimulatePathIntpl3DCommand *pPathIntplCommand, double timeMilliseconds, double *pPos1,
            double *pPos2, double *pPos3, double *pMoveDistance, double *pRemainDistance, double *pTotalDistance);
        WMX3APIFUNC SimulateTimeAtPos(SimulatePathIntpl3DCommand *pPathIntplCommand, double specificPos1, double specificPos2,
            double specificPos3, unsigned int minimumSearchIndex, double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds,
            double *pTotalTimeMilliseconds);
        WMX3APIFUNC SimulateTimeAtDist(SimulatePathIntpl3DCommand *pPathIntplCommand, double specificDistance,
            double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds, double *pTotalTimeMilliseconds);
    };

    class AdvancedMotion{
        friend class AdvMotion;
        friend class AdvVelocity;
        friend class AdvSync;

    private:
        WMX3Api *wmx3Api;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
    public:
        AdvancedMotion(WMX3Api *f);
        AdvancedMotion(const AdvancedMotion& src);
        AdvancedMotion& operator=(const AdvancedMotion& src);
        AdvancedMotion();
        ~AdvancedMotion();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();
        
        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        AdvMotion *advMotion;
        AdvVelocity *advVelocity;
        AdvSync *advSync;
    };
}

#endif
