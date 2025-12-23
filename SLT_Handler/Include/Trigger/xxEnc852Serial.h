#pragma once
#include "serialex.h"

typedef struct tagTRGCTRL
{
	UINT Type				: 4;		//0:DIFF, 1:TTL, 2:Virtual Encoder
	UINT Ch					: 4;		//Encoder CH (0~3)
	UINT Multi				: 4;		//Encoder Multi(1:x1, 2:x2, 4:x4)
	UINT CondFactor			: 4;		//0:DI0 ~ 6:DI6, 7:High, 8:Low
	UINT OutputOperator		: 4;		//0:AND, 1:OR, 2:XOR, 3:NAND
	UINT DirectionFactor	: 4;		//0:DISABLE, 1:DI0 ~ 7:DI6, 8:CW, 9:CCW
	UINT PositionDirection	: 4;		//0:DISABLE, 1:Positive, 2:Negative, 3:Bi-Direction
	UINT Temp				: 2;
	UINT Correction			: 1;		//0:DISABLE, 1:ENABLE
	UINT TriggerBase		: 1;		//0:COUNT, 1:POSITION
} TRGCTRL;


enum {
	BOARD_CH_TYPE_852,
	BOARD_CH_TYPE_100D,
	BOARD_CH_TYPE_100A,
	BOARD_CH_TYPE_ROLL
};
//////////////////////////////////////////////////////////////////////////
/*	
Ex)

	xxEnc852Serial m_serial;
	m_serial.Open("COM1");

	if (m_serial.IsValid())
	{
		//트리거보드 연결안됨.
		return;
	}

	//FirmwareVersion (3.00)
	double firwareVersion = m_serial.GetFirmwareVersion();

	//Resolution 80um
	m_serial.SetEncoderResolution(80);

	m_serial.Purge();
	m_serial.Close();

*/

#define MVSOL_STATUS_SUCCESS			0x0000
#define MVSOL_STATUS_NAK				0x00F1
#define MVSOL_STATUS_NOTREADY			0x00F2
#define MVSOL_STATUS_CHECKSUM			0x00F3
#define MVSOL_STATUS_FPGADATAMISSMATCH	0x00F4
#define MVSOL_STATUS_UNKNOWNADDRESS		0x00F5
#define MVSOL_STATUS_FRAMEERROR			0x00F6
#define MVSOL_STATUS_FLASHMISSMATCH		0x00F7
#define MVSOL_STATUS_RANGEERROR			0x00F8
#define MVSOL_STATUS_DEVICETIMEOUT		0x00F9
#define MVSOL_STATUS_NOTOPEN			0xFFF2
#define MVSOL_STATUS_TIMEOUT			0xFFF9
#define MVSOL_STATUS_COUNTZERO			0xFFFA
#define MVSOL_STATUS_UNKNOWNEXCEPTION	0xFFF6

typedef struct  
{
	byte *dataA;
	byte *dataB;
	byte *dataC;
} SIGNALDATA, *PSIGNALDATA;


class AFX_EXT_CLASS xxEnc852Serial :
	public CSerialEx
{
public:
	xxEnc852Serial(void);
	~xxEnc852Serial(void);

	static EPort CheckPort (LPCTSTR lpszDevice);

	virtual LONG Open (LPCTSTR lpszDevice, DWORD dwInQueue = 0, DWORD dwOutQueue = 0, bool fStartListener = false);
	virtual LONG Close();
	virtual LONG Purge();

	virtual void OnEvent( EEvent eEvent, EError eError );

	UINT SendData(char command, USHORT address, UINT data, UINT *out = NULL);
	UINT SendRead(USHORT address, UINT data = 0, UINT *out = NULL);
	UINT SendWrite(USHORT address, UINT data = 1, UINT *out = NULL);

	bool IsValid();
	
	//트리거 설정
	//TRGCTRL 구조체 참고
	UINT SetTriggerControls(int triggerID, int type, int channel, int multi, int conditionFactor, int outputOperator, int directionFactor, int positionDirection, int correction, int triggerBase);
	UINT SetTriggerControls(int triggerID, const TRGCTRL& ctrl);
	UINT SetTriggerControls(int triggerID, UINT ctrl);
	UINT GetTriggerControls(int triggerID, TRGCTRL* ctrl);
	UINT GetTriggerControls(int triggerID, UINT* ctrl);

	UINT SetPositionDirection(int triggerID, int positionDirection);
	UINT GetPositionDirection(int triggerID, int* positionDirection);
	UINT SetConditionFactor(int triggerID, int conditionFactor);
	UINT GetConditionFactor(int triggerID, int* conditionFactor);

	//트리거출력 설정
	//구형버전 사용, 호환성문제로 기존함수 같이 사용
	UINT SetTriggerGenerator(int triggerID, int cycle, int pulseHigh);
	UINT SetTriggerGenerator(int triggerID, UINT generator);
	UINT GetTriggerGenerator(int triggerID, int* cycle, int* pulseHigh);
	UINT GetTriggerGenerator(int triggerID, UINT* generator);

	//ROLL & 32bitCycle VERSION 전용
	UINT SetTriggerGeneratorCycle(int triggerID, UINT cycle);
	UINT GetTriggerGeneratorCycle(int triggerID, UINT* cycle);

	//아날로그 채배율 설정
	UINT SetEncoderMultiForAnalog(int triggerID, int multi);
	UINT GetEncoderMultiForAnalog(int triggerID, int* multi);

	//사용안함.
	UINT SetEncoderInputRange(int triggerID, int range);
	UINT GetEncoderInputRange(int triggerID, int* range);

	//아날로그 엔코더 전용 함수
	//A상과 B상을 바꿈.
	UINT SetEncoderInversion(int triggerID, int inversion);
	UINT GetEncoderInversion(int triggerID, int* inversion);

	//아날로그 엔코더 캘리브레이션
	UINT StartCalibration(int triggerID);
	UINT StopCalibration(int triggerID);

	//트리거 출력 구간 설정
	//count 값으로 설정
	UINT SetTriggerPosition0(int triggerID, int position);
	//um 값으로 설정
	UINT SetTriggerPosition0(int triggerID, int position, int multi, double resolution, int unit = 0);
	UINT GetTriggerPosition0(int triggerID, int* position);
	//count 값으로 설정
	UINT SetTriggerPosition1(int triggerID, int position);
	//um 값으로 설정
	UINT SetTriggerPosition1(int triggerID, int position, int multi, double resolution, int unit = 0);
	UINT GetTriggerPosition1(int triggerID, int* position);

	UINT SetIndexTriggerLastIndex(int triggerID, int lastIndex);
	UINT SetIndexTriggerPosition(int triggerID, int positionIndex, int position);
	UINT GetIndexTriggerLastIndex(int triggerID, UINT* lastIndex);
	UINT GetIndexTriggerPosition(int triggerID, int positionIndex, int* indxTrgPos);

	UINT SetEncoderDirection(int triggerID, int direction);
	UINT GetEncoderDirection(int triggerID, int* direction);

	//트리거 출력 모드 설정
	UINT SetTriggerOutputMode(int triggerID, int mode);
	UINT GetTriggerOutputMode(int triggerID, int* mode);

	UINT SetTriggerPulseWidth(int triggerID, double pulseWidth);
	UINT GetTriggerPulseWidth(int triggerID, double* pulseWidth);

	UINT SetTriggerDelay(int triggerID, double delay);
	UINT GetTriggerDelay(int triggerID, double* delay);

	UINT SetEncoderResetValue(int triggerID, int position);
	UINT GetEncoderResetValue(int triggerID, int* position);

	//가상 엔코더, 주파수 설정
	UINT SetVirtualEncoderFrequency(int virtualEncoderID, int frequency);
	UINT GetVirtualEncoderFrequency(int virtualEncoderID, int* frequency);

	
	//디지털입력 값 가져오기
	UINT GetDI(int* di0, int* di1, int* di2, int* di3, int* di4, int* di5, int* di6);
	UINT GetDIState(int diID, int* value);
	UINT GetDI(UINT* di);
	
	//디지털입력 카운트 값 가져오기
	UINT GetDiCount(int diID, UINT* count);
	//트리거출력 카운트 값 가져오기
	UINT GetTriggerCount(int triggerID, UINT* count);
	//트리거출력 카운트 64Bit용 값 가져오기
	UINT GetTriggerCount64(int triggerID, ULONG64* count);
	//현재위치 가져오기 count
	UINT GetEncoderPosition(int encoderID, int* position);
	//디지털 현재위치 가져오기 um
	int GetEncoderPosition(int encoderID, int multi, double resolution, int unit = 0);
	//100A 현재위치 가져오기 um
	int GetEncoderPositionForAnalog(int encoderID, int multi, double resolution, int unit = 0);
	//에러 카운트 값 가져오기
	UINT GetTriggerErrorCount(int triggerID, UINT* count);
	
	//펌웨어버전과 보드타입 가져오기
	UINT GetFirmwareVersion(double* version, int *boardType, int *boardChType);
	//로직버전 가져오기
	UINT GetLogicVersion(double* version);

	//보드초기화
	UINT LoadDefaultParameters();
	//플래시메모리에 설정값 저장
	UINT SaveFlash();
	//플래시메모리에서 설정값 가져오기
	UINT LoadFlash();
	//모든 카운트 초기화
	UINT ClearAll();
	//디지털입력 카운트 초기화
	UINT ClearDiAll();
	//트리거출력 카운트 초기화
	UINT ClearTriggerAll();
	//현재위치 초기화
	UINT ClearEncoderPositionAll();
	//에러 카운트 초기화
	UINT ClearErrorCountAll();
	//디지털입력 카운트 초기화 개별
	UINT ClearDiCount(int diID);
	//트리거출력 카운트 초기화 개별
	UINT ClearTriggerCount(int triggerID);
	//현재위치 초기화 개별
	UINT ClearEncoderPosition(int encoderID);
	//에러 카운트 초기화 개별
	UINT ClearErrorCount(int id);
	
	//엔코더 분해능 설정
	//resolution (um)
	UINT SetEncoderResolution(double resolution);
	UINT GetEncoderResolution(double* resolution);

	//사용안함.
	UINT SetUnit(int unit);
	UINT GetUnit(int* unit);
};