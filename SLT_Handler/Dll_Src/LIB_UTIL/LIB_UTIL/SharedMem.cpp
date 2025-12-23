// SharedMem.cpp: implementation of the CSharedMem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SharedMem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CSharedMem::CSharedMem()
{
	m_hFile		= INVALID_HANDLE_VALUE ;		// 실제 File Access를 위한 Handle
	m_hMap		= NULL ;							
	m_nMapSize	= 0 ;
	m_Pointer	= NULL ;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CSharedMem::~CSharedMem()
{
	Close();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSharedMem::Create(LPCTSTR strFileName, LPCTSTR strMapName, int nMapSize, int nAccessMode)
{
	// 지정한 FileName의 File에 대한 Handle을 생성한다.
	m_hFile = CreateFile(strFileName,							// LPCTSTR lpFileName					
		GENERIC_READ | GENERIC_WRITE,			// DWORD dwDesiredAccess
		FILE_SHARE_READ | FILE_SHARE_WRITE,	// DWORD dwSharedMode
		0,									    // LPSECURITY_ATTRIBUTES lpSecurityAttributes
		OPEN_ALWAYS,							// DWORD dwCreationDisposition
		FILE_ATTRIBUTE_NORMAL,					// DWORD dwFlagsAndAttributes
		NULL);									// HANDLE hTemplateFile

	if(m_hFile == INVALID_HANDLE_VALUE) return FALSE ;

	m_strFileName = strFileName;
	// File과 Memory간의 Mapping을 시킨다.
	m_hMap = CreateFileMapping(m_hFile,						// HANDLE hFile
		NULL,							// LPSECURITY_ATTRIBUTES lpFileMappingAttributes
		PAGE_READWRITE,					// DWORD flProtect
		0,								// DWORD dwMaximumSizeHigh
		nMapSize,						// DWORD dwMaximumSizeLow
		strMapName);					// LPCTSTR lpName
	if(m_hMap == NULL ) return FALSE ;
	m_strMapName = strMapName;
	int nWriteMode = (nAccessMode) ? FILE_MAP_WRITE:0;
	m_Pointer = MapViewOfFile(m_hMap,						// HANDLE hFileMappingObject
		FILE_MAP_READ | nWriteMode,	// DWORD dwDesiredAccess
		0,								// DWORD dwFileOffsetHigh
		0,								// DWORD dwFileOffsetLow
		0);								// DWORD dwNumberofByteToMap
		
	if(m_Pointer == NULL ) return FALSE ;

	m_nMapSize = nMapSize ;
	return TRUE ;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSharedMem::Open(LPCTSTR strMapName, int nAccessMode)
{
	HANDLE hMem = NULL ;
	m_hMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,	// DWORD dwDesiredAccess
		TRUE,								// BOOL bInheritHandle
		strMapName);						// LPCTSTR lpName
	if(m_hMap == NULL ) return FALSE ;

	m_strMapName = strMapName;
	int nWriteMode = (nAccessMode) ? FILE_MAP_WRITE:0;
	m_Pointer = MapViewOfFile(m_hMap,						// HANDLE hFileMappingObject
		FILE_MAP_READ | nWriteMode,	// DWORD dwDesiredAccess
		0,								// DWORD dwFileOffsetHigh
		0,								// DWORD dwFileOffsetLow
		0);								// DWORD dwNumberofByteToMap
	if(m_Pointer == NULL ) return FALSE ;
	return TRUE ;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CSharedMem::Close()
{
	if(m_Pointer)						UnmapViewOfFile(m_Pointer);
	if(m_hMap)							CloseHandle(m_hMap);
	if(m_hFile != INVALID_HANDLE_VALUE) CloseHandle(m_hFile);
}

