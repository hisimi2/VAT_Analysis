// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// LIB_BBXMNP.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

CBbxmnpDrv* g_pBbxmnpDrv = nullptr;

void FileExceptionFunc(int nCode, char* pcErrorMsg)
{
	CString strErrorMsg = "";
	strErrorMsg.Format("CFile exception error! Error code : %d, Error msg : %s", nCode, pcErrorMsg);
	TWOutputDebugString(strErrorMsg);
}

void	TWOutputDebugString(const char* szfmt, ...)
{
	char szTrace[1024] = { 0, };
	va_list va;
	va_start(va, szfmt);
	_vstprintf_s(szTrace, sizeof(szTrace), szfmt, va);
	va_end(va);
	strcat_s(szTrace, sizeof(szTrace), "\n");
	OutputDebugString(szTrace);
}