#include "StdAfx.h"
#include "TestHandWorkArea.h"


CTestHandWorkArea::CTestHandWorkArea(void)
{
	m_nTestIdx = -1;
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		m_bWorking[i] = false;
		m_reserved_task[i] = nullptr;
	}
	m_veLoadTableIdx.clear();
	
	::InitializeCriticalSection( m_cs );
}


CTestHandWorkArea::~CTestHandWorkArea(void)
{
	::DeleteCriticalSection( m_cs );
}

void CTestHandWorkArea::SetTestPpIdx(int nIdx)
{
	m_nTestIdx = nIdx;
}

void CTestHandWorkArea::SetLoadTableIdx(int nIdx)
{
	if (nIdx == eTestPP_1)
	{
		m_veLoadTableIdx.push_back(eLoadTbl_1);
		m_veLoadTableIdx.push_back(eLoadTbl_2);
	}
	else
	{
		m_veLoadTableIdx.push_back(eLoadTbl_3);
		m_veLoadTableIdx.push_back(eLoadTbl_4);
	}
	
}

bool CTestHandWorkArea::is_work_enable_state(int nPressIdx, CBaseTaskManager* _this)
{
	int nAreaIdx = g_TaskPressUnit[nPressIdx].CovertPressAreaIdx(nPressIdx);
	if (_this == &g_TaskAutoTestPp[eTestPP_1] || _this == &g_TaskAutoTestPp[eTestPP_2])
	{
		if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			return false;

		int nChkMotSafety = g_TaskPressUnit[ nPressIdx ].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisZ);
		if (nChkMotSafety != ERR_NO_ERROR)
			return false;
		//double dTeachPosZ = 0.0;
		//g_TaskPressUnit[nPressIdx].GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY, &dTeachPosZ);
		//double dCurPos = g_TaskPressUnit[nPressIdx].GetAxis(CTaskPressUnit::eAxisZ)->GetCurrentPos();
		//if (dCurPos > dTeachPosZ + MOTOR_INPOSITION)
		//{
		//	return false;
		//}
	}
	else
	{
		int nTestIdx = g_TaskPressUnit[nPressIdx].CovertAutoTestSiteIdx(nPressIdx);
		std::vector<int> vLocList;
		std::vector<int> vUnLocList;
		g_TaskTestPP[nTestIdx].WhereIsMotorPos(eTestPPLoadType,vLocList, CTaskTestPP::eAxisAll);
		g_TaskTestPP[nTestIdx].WhereIsMotorPos(eTestPPUnLoadType, vUnLocList, CTaskTestPP::eAxisAll);

		//TODO KSJ :: X축 Sensor 점검하여 Press 이동을 할 수 있다.
		int nSafetyX1Sen = g_TaskTestPP[nTestIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
		int nSafetyX2Sen = g_TaskTestPP[nTestIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
		if (nSafetyX1Sen != ERR_NO_ERROR || nSafetyX2Sen != ERR_NO_ERROR)
		{
			switch (nAreaIdx) // Y의 위치를 확인하여야 한다.
			{
				case eATS_DN_STATION_L1:
				case eATS_DN_STATION_L2:
				case eATS_DN_STATION_L3:
				case eATS_DN_STATION_L4:
				{
//					if (vLocList[CTaskTestPP::eAxisY] == (CTaskTestPP::ePosY_StationL1 + (nAreaIdx - eATS_DN_STATION_L1)))
// 						&& vLocList[CTaskTestPP::eAxisZ1] <= CTaskTestPP::ePosZ_DownSafetyToPosiLimit
// 						&& vLocList[CTaskTestPP::eAxisZ2] <= CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
					{
//						return false;
					}
				}break;
			}
		}

		if (g_TaskTestPP[nTestIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + nTestIdx);
			if(nAreaIdx == nJobAtsIdx)
				return false;
		}
	}

	if (m_bWorking[nPressIdx] == true && m_reserved_task[nPressIdx] != _this) {
		return false;
	}	

	// 작업가능
	if(m_bWorking[nPressIdx] == false && m_reserved_task[nPressIdx] == nullptr) {
		int	nCnt = 0;
		for (auto nLoadTblIdx : m_veLoadTableIdx)
		{
			int nSupplyEnd = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
			int nStationinDvc = g_TaskPressUnit[nPressIdx].GetSiteInDvc();
			int nLoadTrayppDvcCnt = g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt();
			int nLoadingTblDeviceCnt = g_TaskLoadTbl[nLoadTblIdx].GetLoadDeviceCnt();
			int nTestPPDeviceCnt = g_TaskTestPP[m_nTestIdx].GetLoadPickerDeviceCnt();
			int nCleaningMode = g_TaskAutoTestPp[m_nTestIdx].ChkCleaningMode(nAreaIdx);

			int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
			int nReservedDvcCnt = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + m_nTestIdx);

			if (g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == TRUE
				&& g_TaskPressUnit[nPressIdx].GetSiteInDvc() == 0 && g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() == 0
				&& g_TaskTestPP[m_nTestIdx].GetBufferTableCnt() == 0 && g_TaskTestPP[m_nTestIdx].GetLoadPickerDeviceCnt() == 0
				&& g_TaskLoadTbl[nLoadTblIdx].GetLoadDeviceCnt() == 0 && g_TaskTestPP[m_nTestIdx].GetUnLoadHandDeviceCnt() == 0
				&& g_TaskAutoTestPp[m_nTestIdx].ChkCleaningMode(nAreaIdx) != TRUE)
			{		
				nCnt++;
			}				
		}
		if(nCnt != m_veLoadTableIdx.size())
			return true;
		else
			return false;
	}

	if (m_bWorking[nPressIdx] == true && m_reserved_task[nPressIdx] == _this) {
		return true;
	}

	// 작업 불가능.
	return false;
}

void CTestHandWorkArea::release_work_area(int nPressIdx, CBaseTaskManager* _this)
{
	m_cs.Lock();
	if (m_reserved_task[nPressIdx] == _this)
	{
		m_bWorking[nPressIdx] = false;
		m_reserved_task[nPressIdx] = nullptr;
	}	
	m_cs.Unlock();
}

bool CTestHandWorkArea::reserve_work_area(int nPressIdx, CBaseTaskManager* _this)
{
	m_cs.Lock();

	bool bRet = false;

	if( is_work_enable_state(nPressIdx, _this) == true ) {
		m_bWorking[nPressIdx] = true;
		m_reserved_task[nPressIdx] = _this;
		bRet = true;
	}
	else if (m_bWorking[nPressIdx] == true && m_reserved_task[nPressIdx] == _this) {
		bRet = true;
	}

	m_cs.Unlock();

	return bRet;
}

bool CTestHandWorkArea::get_reserved_task(int nPressIdx , CBaseTaskManager* _this)
{
	m_cs.Lock();
	bool bRet = false;

	if (m_reserved_task[nPressIdx] == _this) {
		bRet = true;
	}
	m_cs.Unlock();
	return bRet;
}