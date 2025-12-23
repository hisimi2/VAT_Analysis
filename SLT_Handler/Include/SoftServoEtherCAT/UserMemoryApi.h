/**********************************************************************************************************************
*
* UserMemoryApi.h
*
* This file contains the declarations of the UserMemory module API functions for the C++ library.
* This file contains constants, enumerators, and data types that are used by the UserMemory module.
*
* Copyright (c) 2011-2021, Soft Servo Systems, Inc.
*
* All Rights Reserved. Reproduction or modification of this program is not allowed by any other users.
*
**********************************************************************************************************************/

#ifndef WMX3_USER_MEMORY_API_H
#define WMX3_USER_MEMORY_API_H
#include <windows.h>
#include "WMX3Api.h"

namespace wmx3Api{

    namespace constants {
        static const int maxUserMemoryBytes = 1024*2480;
        static const int maxUserMemoryReadWriteBytes = 1024*248;
        static const int maxUserMemoryLogAddressSize = 1500;
        static const int maxUserMemoryLogFormatSize = 100;
		static const int maxLogOutputMDataByteSize = 128;
    }

    class UserMemoryErrorCode : public ErrorCode{
    public:
        enum {
            CurrentlyReallocatingUserMemory = 0x00018000,
            FailedToReallocateUserMemory,
            UserMemoryCountOutOfRange,
            ExceededMaxReadWriteBytesRange,
            SharedMemoryNull
        };
    };

    class UserMemoryEventInput : public EventInput {
    public:
        enum UserMemoryEventInputType {
            MBit,
            NotMBit,
            OrMBit,
            AndMBit,
            XorMBit,
            NandMBit,
            NorMBit,
            XnorMBit,
            DelayMBit,
            EqualMBytes,
            GreaterMBytes,
            LessMBytes
        };

        UserMemoryEventInputType inputFunction;
        union InputFunctionArguments {
            InputFunctionArguments();

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

            struct EqualMBytes {
                unsigned int byteAddress;
                unsigned char size;
                unsigned int value;
                unsigned char invert;
            }equalMBytes;

            struct GreaterMBytes {
                unsigned int byteAddress;
                unsigned char size;
                unsigned char isSigned;
                unsigned int value;
                unsigned char invert;
            }greaterMBytes;

            struct LessMBytes {
                unsigned int byteAddress;
                unsigned char size;
                unsigned char isSigned;
                unsigned int value;
                unsigned char invert;
            }lessMBytes;
        } input;

        UserMemoryEventInput();

        int GetInputModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, int* dataSize, char *initialState);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, int dataSize);
    };

    class UserMemoryEventOutput : public EventOutput {
    public:
        enum UserMemoryEventOutputType {
            SetMBit,
        };

        UserMemoryEventOutputType outputFunction;
        union OutputFunctionArguments {
            OutputFunctionArguments();

            struct SetMBit {
                unsigned int byteAddress;
                unsigned char bitAddress;
                unsigned char invert;

                unsigned char setOffState;
            }setMBit;
        }output;

        UserMemoryEventOutput();

        int GetOutputModuleId();
        WMX3APIFUNC GetOutputData(unsigned char* buff, int buffSize, int* dataSize);
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* data, int dataSize);
    };

    class UserMemoryLogInput : public LogInput {
    public:
        UserMemoryLogInput();
        unsigned int GetModuleId();
        WMX3APIFUNC GetInputData(unsigned char* buff, int buffSize, unsigned int* dataSize);
        WMX3APIFUNC SetInputData(int moduleId, unsigned char* data, unsigned int dataSize);

        MAddress mAddress[constants::maxUserMemoryLogAddressSize];
        unsigned int mAddressSize;

        AIOFormat mLogFormat[constants::maxUserMemoryLogFormatSize];
        unsigned int mFormatCount;
    };

    class UserMemoryLogOutput : public LogOutput {
    public:
        UserMemoryLogOutput();
        unsigned int GetModuleId();
        WMX3APIFUNC SetOutputData(int moduleId, unsigned char* cfgData, unsigned int cfgDataSize, unsigned char* data, unsigned int dataSize, unsigned int dataIndex, unsigned int storeIndex);

        char data[constants::maxLogOutputDataSize][constants::maxLogOutputMDataByteSize];
    };

    class UserMemory {
    private:
        WMX3Api *wmx3Api;
        bool isSelfDev;
        void init(WMX3Api *f);
        void close();
        HANDLE hdl;
        void* umData;

    public:
        UserMemory(WMX3Api *f);
        UserMemory(const UserMemory& src);
        UserMemory& operator=(const UserMemory& src);
        UserMemory();
        ~UserMemory();

        static WMX3APIFUNC ErrorToString(int errCode, char *pString, unsigned int size);
        static WMX3APIFUNC ErrorToString(int errCode, wchar_t *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, char *pString, unsigned int size);
		static WMX3APIFUNC ApiLogToString(unsigned char* pLogData, unsigned int logDataSize, wchar_t *pString, unsigned int size);
        static WMX3APIFUNC GetLibVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        bool IsDeviceValid();

        WMX3APIFUNC GetVersion(int *pMajorVersion, int *pMinorVersion, int *pRevisionVersion, int *pFixVersion);

        WMX3APIFUNC SetMBit(unsigned int addr, unsigned int bit, unsigned char data);
        WMX3APIFUNC SetMByte(unsigned int addr, unsigned char data);
        WMX3APIFUNC SetMBytes(unsigned int addr, unsigned int size, unsigned char *pData);
        WMX3APIFUNC SetMBits(unsigned int *pAddr, unsigned int *pBit, unsigned char *pData, unsigned int count);

        WMX3APIFUNC SetMAnalogDataChar(int addr, char analogData);
        WMX3APIFUNC SetMAnalogDataUChar(int addr, unsigned char analogData);
        WMX3APIFUNC SetMAnalogDataShort(int addr, short analogData);
        WMX3APIFUNC SetMAnalogDataUShort(int addr, unsigned short analogData);
        WMX3APIFUNC SetMAnalogDataInt(int addr, int analogData);
        WMX3APIFUNC SetMAnalogDataUInt(int addr, unsigned int analogData);

        WMX3APIFUNC GetMBit(unsigned int addr, unsigned int bit, unsigned char *pData);
        WMX3APIFUNC GetMByte(unsigned int addr, unsigned char *pData);
        WMX3APIFUNC GetMBytes(unsigned int addr, unsigned int size, unsigned char *pData);

        WMX3APIFUNC GetMAnalogDataChar(unsigned int addr, char *pAnalogData);
        WMX3APIFUNC GetMAnalogDataUChar(unsigned int addr, unsigned char *pAnalogData);
        WMX3APIFUNC GetMAnalogDataShort(unsigned int addr, short *pAnalogData);
        WMX3APIFUNC GetMAnalogDataUShort(unsigned int addr, unsigned short *pAnalogData);
        WMX3APIFUNC GetMAnalogDataInt(unsigned int addr, int *pAnalogData);
        WMX3APIFUNC GetMAnalogDataUInt(unsigned int addr, unsigned int *pAnalogData);

        WMX3APIFUNC SetMBitEx(unsigned int addr, unsigned int bit, unsigned char data);
        WMX3APIFUNC SetMByteEx(unsigned int addr, unsigned char data);
        WMX3APIFUNC SetMBytesEx(unsigned int addr, unsigned int size, unsigned char *pData);
        WMX3APIFUNC SetMBitsEx(unsigned int *pAddr, unsigned int *pBit, unsigned char *pData, unsigned int count);

        WMX3APIFUNC SetMAnalogDataCharEx(int addr, char analogData);
        WMX3APIFUNC SetMAnalogDataUCharEx(int addr, unsigned char analogData);
        WMX3APIFUNC SetMAnalogDataShortEx(int addr, short analogData);
        WMX3APIFUNC SetMAnalogDataUShortEx(int addr, unsigned short analogData);
        WMX3APIFUNC SetMAnalogDataIntEx(int addr, int analogData);
        WMX3APIFUNC SetMAnalogDataUIntEx(int addr, unsigned int analogData);

        WMX3APIFUNC GetMBitEx(unsigned int addr, unsigned int bit, unsigned char *pData);
        WMX3APIFUNC GetMByteEx(unsigned int addr, unsigned char *pData);
        WMX3APIFUNC GetMBytesEx(unsigned int addr, unsigned int size, unsigned char *pData);

        WMX3APIFUNC GetMAnalogDataCharEx(unsigned int addr, char *pAnalogData);
        WMX3APIFUNC GetMAnalogDataUCharEx(unsigned int addr, unsigned char *pAnalogData);
        WMX3APIFUNC GetMAnalogDataShortEx(unsigned int addr, short *pAnalogData);
        WMX3APIFUNC GetMAnalogDataUShortEx(unsigned int addr, unsigned short *pAnalogData);
        WMX3APIFUNC GetMAnalogDataIntEx(unsigned int addr, int *pAnalogData);
        WMX3APIFUNC GetMAnalogDataUIntEx(unsigned int addr, unsigned int *pAnalogData);

        WMX3APIFUNC GetUserMemoryAddress(void** pptr);
    };
}

#endif
