  /*  diagnostic data of EtherCAT_Master-Task */
#ifndef __STRUCT_ETHERCAT_MASTER_ERROR_T__
#define __STRUCT_ETHERCAT_MASTER_ERROR_T__
typedef __PACKED_PRE struct ETHERCAT_MASTER_ERROR_Ttag {
  /*  Acontis Error Counter */
  UINT32 ulCnt_CYCCMD_WKC_ERROR;
  UINT32 ulCnt_MASTER_INITCMD_WKC_ERROR;
  UINT32 ulCnt_SLAVE_INITCMD_WKC_ERROR;
  UINT32 ulCnt_EOE_MBXRCV_WKC_ERROR;
  UINT32 ulCnt_COE_MBXRCV_WKC_ERROR;
  UINT32 ulCnt_FOE_MBXRCV_WKC_ERROR;
  UINT32 ulCnt_EOE_MBXSND_WKC_ERROR;
  UINT32 ulCnt_COE_MBXSND_WKC_ERROR;
  UINT32 ulCnt_FOE_MBXSND_WKC_ERROR;
  UINT32 ulCnt_FRAME_RESPONSE_ERROR;
  UINT32 ulCnt_SLAVE_INITCMD_RESPONSE_ERROR;
  UINT32 ulCnt_MASTER_INITCMD_RESPONSE_ERROR;
  UINT32 ulCnt_CMD_MISSING;
  UINT32 ulCnt_MBSLAVE_INITCMD_TIMEOUT;
  UINT32 ulCnt_NOT_ALL_DEVICES_OPERATIONAL;
  UINT32 ulCnt_ETH_LINK_NOT_CONNECTED;
  UINT32 ulCnt_CYCCMD_TIMEOUT;
  UINT32 ulCnt_RED_LINEBRK;
  UINT32 ulCnt_STATUS_SLAVE_ERROR;
  UINT32 ulCnt_SLAVE_ERROR_STATUS_INFO;
  UINT32 ulCnt_SLAVE_NOT_ADDRESSABLE;
  /* packet-command counter */
} __PACKED_POST ETHERCAT_MASTER_ERROR_T;
#endif
#ifndef __STRUCT_ETHERCAT_MASTER_CMD_T__
#define __STRUCT_ETHERCAT_MASTER_CMD_T__
typedef __PACKED_PRE struct ETHERCAT_MASTER_CMD_Ttag {
  UINT32 ulEthercatMasterCmdRegisterAtStackReq;
  UINT32 ulEthercatMasterCmdSetBusparamReq;
  UINT32 ulEthercatMasterCmdBusOnReq;
  UINT32 ulEthercatMasterCmdBusOffReq;
  UINT32 ulEthercatMasterCmdHostWdgTimeoutReq;
  UINT32 ulRcxGetSlaveHandleReq;
  UINT32 ulRcxGetSlaveConnInfoReq;
  UINT32 ulConfigurationReloadReq;
  UINT32 ulEthercatMasterCmdUpdateCommunicationStateRes;
  UINT32 ulEthercatMasterCmdUpdateGlobalSlaveInfoRes;
  UINT32 ulUnknownCommandReq;
  UINT32 ulEthercatMasterCmdSdoUploadReq;
  UINT32 ulEthercatMasterCmdSdoDownloadReq;
  UINT32 ulEthercatMasterCmdGetOdListReq;
  UINT32 ulEthercatMasterCmdGetObjectDescReq;
  UINT32 ulEthercatMasterCmdGetEntryDescReq;
  UINT32 ulEthercatMasterCmdReadEmergencyReq;
  UINT32 ulEthercatMasterCmdGetDcDeviationReq;
  UINT32 ulEthercatMasterCmdStartBusScanReq;
  UINT32 ulEthercatMasterCmdBusScanInfoReq;
} __PACKED_POST ETHERCAT_MASTER_CMD_T;
#endif
#ifndef __STRUCT_ETHERCAT_MASTER_INTERNALSTATE_T__
#define __STRUCT_ETHERCAT_MASTER_INTERNALSTATE_T__
typedef __PACKED_PRE struct ETHERCAT_MASTER_INTERNALSTATE_Ttag {
  UINT32 eInternalMasterState;
  UINT32 eErrorDuringReset;
  /* change type from UINT32 to BOOLEAN-something, currently does not work */
  UINT32 fLinkIsUp;
  UINT32 fHardResetRequired;
} __PACKED_POST ETHERCAT_MASTER_INTERNALSTATE_T;
#endif
#ifndef __STRUCT_ETHERCAT_MASTER_DIAG_GET_SLAVE_DIAG_T__
#define __STRUCT_ETHERCAT_MASTER_DIAG_GET_SLAVE_DIAG_T__
typedef __PACKED_PRE struct ETHERCAT_MASTER_DIAG_GET_SLAVE_DIAG_Ttag {
  UINT32 ulStationAddress;
  UINT32 ulAutoIncAddress;
  UINT32 ulCurrentState;
  UINT32 ulLastError;
  //STRING szSlaveName[80];
  CString szSlaveName[80];
  UINT32 fEmergencyReported;
} __PACKED_POST ETHERCAT_MASTER_DIAG_GET_SLAVE_DIAG_T;
#endif
#ifndef __STRUCT_ETHERCAT_MASTER_BUS_SCAN_INFO_T__
#define __STRUCT_ETHERCAT_MASTER_BUS_SCAN_INFO_T__
typedef __PACKED_PRE struct ETHERCAT_MASTER_BUS_SCAN_INFO_Ttag {
  UINT32 ulVendorId;
  UINT32 ulProductCode;
  UINT32 ulRevisionNumber;
  UINT32 ulSerialNumber;
  UINT32 ulPortState;
} __PACKED_POST ETHERCAT_MASTER_BUS_SCAN_INFO_T;
#endif
