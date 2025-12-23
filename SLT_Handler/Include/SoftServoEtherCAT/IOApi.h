/**********************************************************************************************************************
*
* IOApi.h
*
* This file contains the declarations of the IO module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the IO module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_IO_API_H
#define WMX3_IO_API_H
#include <windows.h>
#include "WMX3Api.h"

namespace wmx3Api{

    class IOErrorCode : public ErrorCode{
    public:
        enum {
            SharedMemoryNull = 0x00014000,
            IOCountOutOfRange,
        };
    };

    namespace constants {
        static const int maxIoLogAddressSize = 256;
        static const int maxIoLogFormatSize = 100;
        static const int maxLogOutputIoInputByteSize = 128;
        static const int maxLogOutputIoOutputByteSize = 128;
    };

    class IoLogInput : public LogInput {
    public:
        IoLogInput();
        unsigned int GetModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, unsigned int* dataSize);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, unsigned int dataSize);

        IOAddress inputIOAddress[constants::maxIoLogAddressSize];
        unsigned int inputSize;
        IOAddress outputIOAddress[constants::maxIoLogAddressSize];
        unsigned int outputSize;

        AIOFormat inputIOLogFormat[constants::maxIoLogFormatSize];
        unsigned int inputIOFormatCount;
        AIOFormat  outputIOLogFormat[constants::maxIoLogFormatSize];
        unsigned int outputIOFormatCount;
    };

    class IoLogOutput : public LogOutput {
    public:
        IoLogOutput();
        unsigned int GetModuleId();
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* cfgData, unsigned int cfgDataSize, unsigned char* data, unsigned int dataSize, unsigned int dataIndex, unsigned int storeIndex);

        char input[constants::maxLogOutputDataSize][constants::maxLogOutputIoInputByteSize];
        char output[constants::maxLogOutputDataSize][constants::maxLogOutputIoOutputByteSize];
    };

    class IoEventInput : public EventInput {
    public:
        enum IoEventInputType {
            IOBit,
            NotIOBit,
            OrIOBit,
            AndIOBit,
            XorIOBit,
            NandIOBit,
            NorIOBit,
            XnorIOBit,
            DelayIOBit,
            EqualIOBytes,
            GreaterIOBytes,
            LessIOBytes
        };

        IoEventInputType inputFunction;
        union InputFunctionArguments {
            InputFunctionArguments();

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

            struct EqualIOBytes {
                unsigned int byteAddress;
                unsigned char size;
                unsigned int value;
                unsigned char invert;
                IOSourceType::T ioSourceType;
            }equalIOBytes;

            struct GreaterIOBytes {
                unsigned int byteAddress;
                unsigned char size;
                unsigned char isSigned;
                unsigned int value;
                unsigned char invert;
                IOSourceType::T ioSourceType;
            }greaterIOBytes;

            struct LessIOBytes {
                unsigned int byteAddress;
                unsigned char size;
                unsigned char isSigned;
                unsigned int value;
                unsigned char invert;
                IOSourceType::T ioSourceType;
            }lessIOBytes;
        } input;

        IoEventInput();

        int GetInputModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, int* dataSize, char *initialState);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, int dataSize);
    };

    class IoEventOutput : public EventOutput {
    public:
        enum IoEventOutputType {
            SetIOOutBit,
        };

        IoEventOutputType outputFunction;
        union OutputFunctionArguments {
            OutputFunctionArguments();

            struct SetIOOutBit {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;

                unsigned char setOffState;
            }setIOOutBit;
        }output;

        IoEventOutput();

        int GetOutputModuleId();
        WMX3APIFUNC GetOutputData(unsigned char* buff, int buffSize, int* dataSize);
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* data, int dataSize);
    };

    class Io {
    private:
        WMX3Api *wmx3Api;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
        HANDLE hdl;
        void* ioData;

    public:
        Io(WMX3Api *f);
        Io(const Io& src);
        Io& operator=(const Io& src);
        Io();
        ~Io();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC SetOutBit(int addr, int bit, unsigned char data);
        WMX3APIFUNC SetOutByte(int addr, unsigned char data);
        WMX3APIFUNC SetOutBytes(int addr, int size, unsigned char *pData);
        WMX3APIFUNC SetOutBits(int *pAddr, int *pBit, unsigned char *pData, int count);
        WMX3APIFUNC SetOutAnalogDataChar(int addr, char analogData);
        WMX3APIFUNC SetOutAnalogDataUChar(int addr, unsigned char analogData);
        WMX3APIFUNC SetOutAnalogDataShort(int addr, short analogData);
        WMX3APIFUNC SetOutAnalogDataUShort(int addr, unsigned short analogData);
        WMX3APIFUNC SetOutAnalogDataInt(int addr, int analogData);
        WMX3APIFUNC SetOutAnalogDataUInt(int addr, unsigned int analogData);

        WMX3APIFUNC GetInBit(int addr, int bit, unsigned char *pData);
        WMX3APIFUNC GetInByte(int addr, unsigned char *pData);
        WMX3APIFUNC GetInBytes(int addr, int size,unsigned char *pData);
        WMX3APIFUNC GetInAnalogDataChar(int addr, char *pAnalogData);
        WMX3APIFUNC GetInAnalogDataUChar(int addr, unsigned char *pAnalogData);
        WMX3APIFUNC GetInAnalogDataShort(int addr, short *pAnalogData);
        WMX3APIFUNC GetInAnalogDataUShort(int addr, unsigned short *pAnalogData);
        WMX3APIFUNC GetInAnalogDataInt(int addr, int *pAnalogData);
        WMX3APIFUNC GetInAnalogDataUInt(int addr, unsigned int *pAnalogData);

        WMX3APIFUNC GetOutBit(int addr, int bit, unsigned char *pData);
        WMX3APIFUNC GetOutByte(int addr, unsigned char *pData);
        WMX3APIFUNC GetOutBytes(int addr, int size, unsigned char *pData);
        WMX3APIFUNC GetOutAnalogDataChar(int addr, char *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataUChar(int addr, unsigned char *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataShort(int addr, short *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataUShort(int addr, unsigned short *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataInt(int addr, int *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataUInt(int addr, unsigned int *pAnalogData);

        WMX3APIFUNC SetOutBitEx(int addr, int bit, unsigned char data);
        WMX3APIFUNC SetOutByteEx(int addr, unsigned char data);
        WMX3APIFUNC SetOutBytesEx(int addr, int size, unsigned char *pData);
        WMX3APIFUNC SetOutBitsEx(int *pAddr, int *pBit, unsigned char *pData, int count);
        WMX3APIFUNC SetOutAnalogDataCharEx(int addr, char analogData);
        WMX3APIFUNC SetOutAnalogDataUCharEx(int addr, unsigned char analogData);
        WMX3APIFUNC SetOutAnalogDataShortEx(int addr, short analogData);
        WMX3APIFUNC SetOutAnalogDataUShortEx(int addr, unsigned short analogData);
        WMX3APIFUNC SetOutAnalogDataIntEx(int addr, int analogData);
        WMX3APIFUNC SetOutAnalogDataUIntEx(int addr, unsigned int analogData);

        WMX3APIFUNC GetInBitEx(int addr, int bit, unsigned char *pData);
        WMX3APIFUNC GetInByteEx(int addr, unsigned char *pData);
        WMX3APIFUNC GetInBytesEx(int addr, int size,unsigned char *pData);
        WMX3APIFUNC GetInAnalogDataCharEx(int addr, char *pAnalogData);
        WMX3APIFUNC GetInAnalogDataUCharEx(int addr, unsigned char *pAnalogData);
        WMX3APIFUNC GetInAnalogDataShortEx(int addr, short *pAnalogData);
        WMX3APIFUNC GetInAnalogDataUShortEx(int addr, unsigned short *pAnalogData);
        WMX3APIFUNC GetInAnalogDataIntEx(int addr, int *pAnalogData);
        WMX3APIFUNC GetInAnalogDataUIntEx(int addr, unsigned int *pAnalogData);

        WMX3APIFUNC GetOutBitEx(int addr, int bit, unsigned char *pData);
        WMX3APIFUNC GetOutByteEx(int addr, unsigned char *pData);
        WMX3APIFUNC GetOutBytesEx(int addr, int size, unsigned char *pData);
        WMX3APIFUNC GetOutAnalogDataCharEx(int addr, char *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataUCharEx(int addr, unsigned char *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataShortEx(int addr, short *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataUShortEx(int addr, unsigned short *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataIntEx(int addr, int *pAnalogData);
        WMX3APIFUNC GetOutAnalogDataUIntEx(int addr, unsigned int *pAnalogData);

        WMX3APIFUNC SetInitialOutByte(int addr, unsigned char data, unsigned char enable);
        WMX3APIFUNC SetInitialOutBytes(int addr, int size, unsigned char *pData, unsigned char *pEnable);
        WMX3APIFUNC GetInitialOutByte(int addr, unsigned char *pData, unsigned char *pEnable);
        WMX3APIFUNC GetInitialOutBytes(int addr, int size, unsigned char *pData, unsigned char *pEnable);
        WMX3APIFUNC GetInitialOutByteInterruptId(int addr, unsigned char *pInterruptId);
        WMX3APIFUNC GetInitialOutBytesInterruptId(int addr, int size, unsigned char *pInterruptId);
    };
}

#endif
