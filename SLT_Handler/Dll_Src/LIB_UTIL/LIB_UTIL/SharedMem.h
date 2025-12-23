// SharedMem.h: interface for the CSharedMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREDMEM_H__F2114606_6251_42A8_A66B_7C428E8537DE__INCLUDED_)
#define AFX_SHAREDMEM_H__F2114606_6251_42A8_A66B_7C428E8537DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//==============================================================================
//
//==============================================================================
#define _MODE_READ_ONLY	0
#define _MODE_READWRITE	1


//==============================================================================
//
//==============================================================================
class AFX_EXT_CLASS CSharedMem  
{
public:
	CSharedMem();
	virtual ~CSharedMem();

	BOOL Create(LPCTSTR strFileName, LPCTSTR strMapName, int nMapSize, int nAccessMode = _MODE_READ_ONLY);
	BOOL Open  (LPCTSTR strMapName, int nAccessMode);

	CString GetFileName()	{ return m_strFileName ; }
	CString GetMapName()	{ return m_strMapName ; }
	int		GetMapSize()	{ return m_nMapSize ; }
	void	*GetPoint()		{ return m_Pointer; }

protected:
	void	Close();

	// Handle ===================================================================
	HANDLE	m_hMap;		// 실제 File과 Mapping 되는 Memory Access를 위한 Handle
	HANDLE	m_hFile;	// 실제 HardDisk 상에 존재하는 File에 대한 Handle
	// ==========================================================================

	// Map File Name ============================================================
	CString m_strFileName;		// File Name
	CString m_strMapName;		// Mapping Memory Name
	// ==========================================================================

	UINT	m_nMapSize;			// Map File로 할당할 Size
	void	*m_Pointer;			// Mapping Memory에 대한 포인터
};

#endif // !defined(AFX_SHAREDMEM_H__F2114606_6251_42A8_A66B_7C428E8537DE__INCLUDED_)
