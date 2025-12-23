/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Last Modification:

    @version $Id: EtherCATMaster_Public.h 21732 2012-01-26 14:24:49Z Ulli $

   Description:

    -public definition of task

   Changes:

     Date        Author        Description
     ----------------------------------------------------------------------------------
     2012-01-26  UJ            update header: change __PACKED_* to __TLR_PACKED_*
                               cleanups
     2012-01-24  UJ            added EEPROM access and EC-State access functions
     2011-01-18  UJ            extend API of GET_ODLIST_REQ (new list types)
     2010-12-22  UJ            packet configuration interface added
     2007-11-20  UJ            moved __PACKED_POST behind struct (GCC < 4.0)
     2007-11-07  UJ            added bus scan stuff
     2007-11-01  UJ            created

**************************************************************************************/

#ifndef __ETHERCAT_MASTER_PUBLIC_H
#define __ETHERCAT_MASTER_PUBLIC_H

#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_PACK_1(__ETHERCAT_MASTER_PACKET)
#endif

/** queue name, only needed for code running on netX itself */
#define ETHERCAT_MASTER_QUEUE_NAME        "QUE_EC_MASTER"

/* stack internal packet, not for host */
#define  ETHERCAT_MASTER_CMD_HOST_WDG_TIMEOUT_REQ            0x00650004
#define  ETHERCAT_MASTER_CMD_HOST_WDG_TIMEOUT_CNF            0x00650005

#define  ETHERCAT_MASTER_CMD_SDO_UPLOAD_REQ                  0x00650006
#define  ETHERCAT_MASTER_CMD_SDO_UPLOAD_CNF                  0x00650007

#define  ETHERCAT_MASTER_CMD_SDO_DOWNLOAD_REQ                0x00650008
#define  ETHERCAT_MASTER_CMD_SDO_DOWNLOAD_CNF                0x00650009

#define  ETHERCAT_MASTER_CMD_GET_ODLIST_REQ                  0x0065000A
#define  ETHERCAT_MASTER_CMD_GET_ODLIST_CNF                  0x0065000B

/* stack internal packet, not for host */
#define  ETHERCAT_MASTER_CMD_SET_BUSPARAM_REQ                0x0065000C
#define  ETHERCAT_MASTER_CMD_SET_BUSPARAM_CNF                0x0065000D

/* stack internal packet, not for host */
#define  ETHERCAT_MASTER_CMD_REGISTER_AT_STACK_REQ           0x0065000E
#define  ETHERCAT_MASTER_CMD_REGISTER_AT_STACK_CNF           0x0065000F

/* stack internal packet, not for host */
#define  ETHERCAT_MASTER_CMD_BUS_ON_REQ                      0x00650010
#define  ETHERCAT_MASTER_CMD_BUS_ON_CNF                      0x00650011

/* stack internal packet, not for host */
#define  ETHERCAT_MASTER_CMD_BUS_OFF_REQ                     0x00650012
#define  ETHERCAT_MASTER_CMD_BUS_OFF_CNF                     0x00650013

/* stack internal packet, not for host */
#define  ETHERCAT_MASTER_CMD_UPDATE_COMMUNICATION_STATE_IND  0x00650014
#define  ETHERCAT_MASTER_CMD_UPDATE_COMMUNICATION_STATE_RES  0x00650015

/* stack internal packet, not for host */
#define  ETHERCAT_MASTER_CMD_UPDATE_GLOBAL_SLAVE_INFO_IND    0x00650016
#define  ETHERCAT_MASTER_CMD_UPDATE_GLOBAL_SLAVE_INFO_RES    0x00650017

#define  ETHERCAT_MASTER_CMD_GET_OBJECTDESC_REQ              0x00650018
#define  ETHERCAT_MASTER_CMD_GET_OBJECTDESC_CNF              0x00650019

#define  ETHERCAT_MASTER_CMD_GET_ENTRYDESC_REQ               0x0065001A
#define  ETHERCAT_MASTER_CMD_GET_ENTRYDESC_CNF               0x0065001B

#define  ETHERCAT_MASTER_CMD_READ_EMERGENCY_REQ              0x0065001C
#define  ETHERCAT_MASTER_CMD_READ_EMERGENCY_CNF              0x0065001D

#define  ETHERCAT_MASTER_CMD_GET_DC_DEVIATION_REQ            0x0065001E
#define  ETHERCAT_MASTER_CMD_GET_DC_DEVIATION_CNF            0x0065001F

/* <"old" bus scan, replaced now by generic RCX_BUSSCAN_REQ and RCX_GET_DEVICE_INFO_REQ> */
#define  ETHERCAT_MASTER_CMD_START_BUS_SCAN_REQ              0x00650020
#define  ETHERCAT_MASTER_CMD_START_BUS_SCAN_CNF              0x00650021

#define  ETHERCAT_MASTER_CMD_GET_BUS_SCAN_INFO_REQ           0x00650022
#define  ETHERCAT_MASTER_CMD_GET_BUS_SCAN_INFO_CNF           0x00650023
/* </"old" bus scan, replaced now by generic RCX_BUSSCAN_REQ and RCX_GET_DEVICE_INFO_REQ> */

/* <packet configuration interface> */
#define  ETHERCAT_MASTER_CMD_BEGIN_CONFIGURATION_REQ         0x00650024
#define  ETHERCAT_MASTER_CMD_BEGIN_CONFIGURATION_CNF         0x00650025

#define  ETHERCAT_MASTER_CMD_ADD_SLAVE_REQ                   0x00650026
#define  ETHERCAT_MASTER_CMD_ADD_SLAVE_CNF                   0x00650027

#define  ETHERCAT_MASTER_CMD_ADD_INITCMD_REQ                 0x00650028
#define  ETHERCAT_MASTER_CMD_ADD_INITCMD_CNF                 0x00650029

#define  ETHERCAT_MASTER_CMD_ADD_COE_INITCMD_REQ             0x0065002A
#define  ETHERCAT_MASTER_CMD_ADD_COE_INITCMD_CNF             0x0065002B

#define  ETHERCAT_MASTER_CMD_ADD_CYCLIC_REQ                  0x0065003C
#define  ETHERCAT_MASTER_CMD_ADD_CYCLIC_CNF                  0x0065003D

#define  ETHERCAT_MASTER_CMD_END_CONFIGURATION_REQ           0x0065003E
#define  ETHERCAT_MASTER_CMD_END_CONFIGURATION_CNF           0x0065003F
/* </packet configuration interface> */

#define  ETHERCAT_MASTER_CMD_EEPROM_READ_REQ                 0x00650040
#define  ETHERCAT_MASTER_CMD_EEPROM_READ_CNF                 0x00650041

#define  ETHERCAT_MASTER_CMD_EEPROM_WRITE_REQ                0x00650042
#define  ETHERCAT_MASTER_CMD_EEPROM_WRITE_CNF                0x00650043

#define  ETHERCAT_MASTER_CMD_EEPROM_RELOAD_REQ               0x00650044
#define  ETHERCAT_MASTER_CMD_EEPROM_RELOAD_CNF               0x00650045

#define  ETHERCAT_MASTER_CMD_GET_ECSTATE_REQ                 0x00650046
#define  ETHERCAT_MASTER_CMD_GET_ECSTATE_CNF                 0x00650047

#define  ETHERCAT_MASTER_CMD_SET_ECSTATE_REQ                 0x00650048
#define  ETHERCAT_MASTER_CMD_SET_ECSTATE_CNF                 0x00650049


#define ETHERCAT_MASTER_BUSSTATE_UNKNOWN (0)  /* unknown */
#define ETHERCAT_MASTER_BUSSTATE_INIT    (1)  /* init */
#define ETHERCAT_MASTER_BUSSTATE_PREOP   (2)  /* pre-operational */
#define ETHERCAT_MASTER_BUSSTATE_BOOT    (3)  /* bootstrap mode */
#define ETHERCAT_MASTER_BUSSTATE_SAFEOP  (4)  /* safe operational */
#define ETHERCAT_MASTER_BUSSTATE_OP      (8)  /* operational */


/********************************************************/
/**packets for ETHERCAT_MASTER_CMD_SET_BUSPARAM_REQ/CNF**/
/********************************************************/
/* this define is only valid for SET_BUSPARAM packet! */
#define ETHERCAT_MASTER_AUTO_START (0x00000001)

#define ETHERCAT_MASTER_LEAVE_ALL_BROKEN_SLAVES_DOWN   (0x00000000)
#define ETHERCAT_MASTER_LEAVE_ADDRESS_LESS_SLAVES_DOWN (0x00000001)
#define ETHERCAT_MASTER_LEAVE_NO_SLAVES_DOWN           (0x00000002)


typedef struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SET_BUSPARAM_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_REQ_DATA_Ttag
{
  TLR_UINT32    ulSystemFlags;          /* System Flags */
  TLR_UINT32    ulWdgTime;              /* Watchdog time */
  TLR_UINT32    ulBusCycleTime;         /* Bus Cycle Time in us */
  TLR_UINT32    ulBrokenSlaveBehaviour; /* Handling of slaves with error */
  TLR_BOOLEAN32 fDcActivated;           /* Activate Distributed Clocks */
  TLR_UINT32    ulReserved;
};


typedef struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_REQ_Ttag
      ETHERCAT_MASTER_PACKET_SET_BUSPARAM_REQ_T;

struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_REQ_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
  ETHERCAT_MASTER_PACKET_SET_BUSPARAM_REQ_DATA_T  tData;  /** packet request data. */
};


typedef struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SET_BUSPARAM_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_CNF_DATA_Ttag
{
  TLR_HANDLE     hTrpInputData;        /* handle for the input triple buffer */
  TLR_HANDLE     hTrpOutputData;       /* handle for the output triple buffer */
  TLR_UINT32     ulInputDataSize;      /* size of the input data */
  TLR_UINT32     ulOutputDataSize;     /* size of the output data */
};


typedef struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_CNF_Ttag
      ETHERCAT_MASTER_PACKET_SET_BUSPARAM_CNF_T;

struct ETHERCAT_MASTER_PACKET_SET_BUSPARAM_CNF_Ttag
{
  TLR_PACKET_HEADER_T                        tHead;  /** packet header. */
  /* the data part is only used, if originator of the request-packet is the AP-Task */
  ETHERCAT_MASTER_PACKET_SET_BUSPARAM_CNF_DATA_T  tData;
};


/********************************************************/
/**packets for ETHERCAT_MASTER_CMD_SET_BUSPARAM_REQ/CNF**/
/********************************************************/
typedef struct ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_IND_DATA_Ttag
      ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_IND_DATA_T;

struct ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_IND_DATA_Ttag
{
  TLR_UINT32     ulCommunicationError;        /* last communication error */
  TLR_BOOLEAN32  fCommunicationActive;        /* equal to COM-Bit (at least one slave in cyclic date exchange) */
  TLR_UINT32     ulMasterBusState;            /* current Bus state (init, pre-op, ...) */
};

typedef struct ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_IND_Ttag
      ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_IND_T;

/* packet is send from the stack to the AP-Task to indicate changes in communication state */
struct ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_IND_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
  ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_IND_DATA_T  tData;  /** packet request data. */
};

typedef struct ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_RES_Ttag
      ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_RES_T;

struct ETHERCAT_MASTER_AP_PACKET_UPDATE_COMMUNICATION_STATE_RES_Ttag
{
  TLR_PACKET_HEADER_T             tHead;  /** packet header. */
  /* the update communication state response has no data part */
};


/**********************************************************************/
/**packets for ETHERCAT_MASTER_CMD_UPDATE_COMMUNICATION_STATE_IND/RES**/
/**********************************************************************/
typedef struct ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_IND_DATA_Ttag
      ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_IND_DATA_T;

struct ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_IND_DATA_Ttag
{
  TLR_UINT32 ulSlaveState;         /* Slave status */
  TLR_UINT32 ulNumOfConfigSlaves;  /* Number of configured slaves */
  TLR_UINT32 ulNumOfActiveSlaves;  /* Number of active slaves */
  TLR_UINT32 ulNumOfDiagSlaves;    /* Number of slaves in diag mode */
};


typedef struct ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_IND_Ttag
      ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_IND_T;

/* packet is send from the stack to the AP-Task to indicate changes in slave state */
struct ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_IND_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
  ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_IND_DATA_T  tData;  /** packet request data. */
};


typedef struct ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_RES_Ttag
      ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_RES_T;

struct ETHERCAT_MASTER_AP_PACKET_UPDATE_GLOBAL_SLAVE_INFO_RES_Ttag
{
  TLR_PACKET_HEADER_T             tHead;  /** packet header. */
  /* the update communication state response has no data part */
};


/********************************************************/
/**packets for ETHERCAT_MASTER_CMD_SDO_DOWNLOAD_REQ/CNF**/
/********************************************************/

#define  ETHERCAT_MASTER_COE_MAX_SDO_DOWNLOAD_DATA (RCX_MAX_DATA_SIZE - (sizeof(TLR_UINT32) * 4))

typedef struct ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulIndex;
  TLR_UINT32 ulSubIndex;
  TLR_UINT32 ulDataCnt;
  TLR_UINT8  abSdoData[ETHERCAT_MASTER_COE_MAX_SDO_DOWNLOAD_DATA];
};

typedef struct ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_Ttag
      ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_T;

struct ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_DATA_T  tData;  /** packet request data. */
};


/* the sdo download cnf needs no data part */
typedef struct ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_CNF_Ttag
      ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_CNF_T;

struct ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_CNF_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
};

/******************************************************/
/**packets for ETHERCAT_MASTER_CMD_SDO_UPLOAD_REQ/CNF**/
/******************************************************/

#define  ETHERCAT_MASTER_COE_MAX_SDO_UPLOAD_DATA (RCX_MAX_DATA_SIZE - (sizeof(TLR_UINT32) * 4))

typedef struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulIndex;
  TLR_UINT32 ulSubIndex;
};

typedef struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_Ttag
      ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_T;

struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_Ttag
{
  TLR_PACKET_HEADER_T                           tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_DATA_T  tData;  /** packet request data. */
};

#define ETHERCAT_MASTER_COE_GET_SDO_UPLOAD_CNF_LEN_ON_ERROR (12)

typedef struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulIndex;
  TLR_UINT32 ulSubIndex;
  TLR_UINT32 ulDataCnt;
  TLR_UINT8  abSdoData[ETHERCAT_MASTER_COE_MAX_SDO_UPLOAD_DATA];
};


typedef struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_Ttag
      ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_T;

struct ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
  ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_DATA_T  tData;  /** packet request data. */
};


/******************************************************/
/**packets for ETHERCAT_MASTER_CMD_GET_ODLIST_REQ/CNF**/
/******************************************************/
#define  ETHERCAT_MASTER_COE_GET_ODLIST_DATA ((RCX_MAX_DATA_SIZE - (sizeof(TLR_UINT32) * 3)) / sizeof(TLR_UINT16))

#define ETHERCAT_MASTER_COE_GET_ODLIST_TYPE_ALL      (1)
#define ETHERCAT_MASTER_COE_GET_ODLIST_TYPE_RXPDOMAP (2)
#define ETHERCAT_MASTER_COE_GET_ODLIST_TYPE_TXPDOMAP (3)
#define ETHERCAT_MASTER_COE_GET_ODLIST_TYPE_STORE    (4)
#define ETHERCAT_MASTER_COE_GET_ODLIST_TYPE_STARTUP  (5)


typedef struct ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulListType;
};

typedef struct ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_Ttag
      ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_T;

struct ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_DATA_T  tData;  /** packet request data. */
};

#define ETHERCAT_MASTER_COE_GET_ODLIST_CNF_LEN_ON_ERROR (8)

typedef struct ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulListType;
  TLR_UINT32 ulDataCnt;
  TLR_UINT16 ausObjectList[ETHERCAT_MASTER_COE_GET_ODLIST_DATA];
};

typedef struct ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_Ttag
      ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_T;

struct ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
  ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_DATA_T  tData;  /** packet request data. */
};


/**********************************************************/
/**packets for ETHERCAT_MASTER_CMD_GET_OBJECTDESC_REQ/CNF**/
/**********************************************************/
#define  ETHERCAT_MASTER_COE_GET_OBJECTDESC_NAME_LEN (RCX_MAX_DATA_SIZE - (sizeof(TLR_UINT32) * 7))

typedef struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_REQ_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulIndex;
};

typedef struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_REQ_Ttag
      ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_REQ_T;

struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_REQ_Ttag
{
  TLR_PACKET_HEADER_T                               tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_REQ_DATA_T  tData;  /** packet request data. */
};

#define ETHERCAT_MASTER_COE_GET_OBJECTDESC_CNF_LEN_ON_ERROR (8)

typedef struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_CNF_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulIndex;/* Index in the object dictionary */
  TLR_UINT32 ulDataType; /* Data type of the object */
  TLR_UINT32 ulObjCode; /* Object code */
  TLR_UINT32 ulObjCategory; /* Object category */
  TLR_UINT32 ulMaxNumSubIndex; /* Maximum sub index number */
  TLR_UINT32 ulObNameLen; /* Length of the object name */
  TLR_UINT8  abObjName[ETHERCAT_MASTER_COE_GET_OBJECTDESC_NAME_LEN]; /* Object name (not NULL terminated!) */
};

typedef struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_CNF_Ttag
      ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_CNF_T;

struct ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_CNF_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
  ETHERCAT_MASTER_PACKET_GET_OBJECTDESC_CNF_DATA_T  tData;  /** packet request data. */
};


/**********************************************************/
/**packets for ETHERCAT_MASTER_CMD_GET_ENTRYDESC_REQ/CNF**/
/**********************************************************/
#define  ETHERCAT_MASTER_COE_GET_ENTRYDESC_MAX_DATA (RCX_MAX_DATA_SIZE - (sizeof(TLR_UINT32) * 11))

typedef struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulIndex;
  TLR_UINT32 ulSubIndex;
  TLR_UINT32 ulAccessBitMask;
};

/* EtherCat CoE entry description value information bit definitions */
#define ETHERCAT_MASTER_COE_ENTRY_OBJACCESS              0x01
#define ETHERCAT_MASTER_COE_ENTRY_OBJCATEGORY            0x02
#define ETHERCAT_MASTER_COE_ENTRY_PDOMAPPING             0x04
#define ETHERCAT_MASTER_COE_ENTRY_UNITTYPE               0x08
#define ETHERCAT_MASTER_COE_ENTRY_DEFAULTVALUE           0x10
#define ETHERCAT_MASTER_COE_ENTRY_MINVALUE               0x20
#define ETHERCAT_MASTER_COE_ENTRY_MAXVALUE               0x40

typedef struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_Ttag
      ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_T;

struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_Ttag
{
  TLR_PACKET_HEADER_T                               tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_DATA_T  tData;  /** packet request data. */
};


typedef struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_DATA_Ttag
{
  TLR_UINT32 ulNodeId;
  TLR_UINT32 ulIndex;/* Index in the object dictionary */
  TLR_UINT32 ulSubIndex;
  TLR_UINT32 ulValueInfo; /* Bit mask to define which information is available */
  TLR_UINT32 ulDataType; /* Object data type */
  TLR_UINT32 ulBitLen; /* Object size (number of bits) */
  TLR_UINT32 ulObAccess; /* Access rights */
  TLR_BOOLEAN32 fRxPdoMapping; /* Is the object PDO-mappable? */
  TLR_BOOLEAN32 fTxPdoMapping; /* Can the PDO be changed */
  TLR_UINT32 ulUnitType; /* Unit*/
  TLR_UINT32 ulDataLen; /* Size of the remaining object data */
  TLR_UINT8  abObjData[ETHERCAT_MASTER_COE_GET_ENTRYDESC_MAX_DATA]; /* Remaining object data (see EtherCAT specification) */
};

#define ETHERCAT_MASTER_COE_GET_ENTRYDESC_CNF_LEN_ON_ERROR (12)

typedef struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_Ttag
      ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_T;

struct ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_Ttag
{
  TLR_PACKET_HEADER_T                              tHead;  /** packet header. */
  ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_DATA_T  tData;  /** packet request data. */
};


/**********************************************************/
/**packets for ETHERCAT_MASTER_CMD_READ_EMERGENCY_REQ/CNF**/
/**********************************************************/
#define ETHERCAT_MASTER_COE_NUMBER_OF_EMERGENCY (5)
#define ETHERCAT_MASTER_COE_EMERGENCY_DATA_BYTES (5)

typedef struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_REQ_DATA_Ttag
{
  TLR_UINT32 ulSlaveHandle;
  TLR_BOOLEAN32 fDeleteEmergency;  /* Flag to decide (keep emergeny(s) / clear emergeny(s)) */
};

typedef struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_REQ_Ttag
      ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_REQ_T;

struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_REQ_Ttag
{
  TLR_PACKET_HEADER_T                                     tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_REQ_DATA_T  tData;  /** packet request data. */
};

typedef struct ETHERCAT_MASTER_SLAVE_EMERGENCY_Ttag
      ETHERCAT_MASTER_SLAVE_EMERGENCY_T;

__TLR_PACKED_PRE struct ETHERCAT_MASTER_SLAVE_EMERGENCY_Ttag
{
  TLR_UINT16 usErrorCode;
  TLR_UINT8  bErrorRegister;
  TLR_UINT8  abErrorData[ETHERCAT_MASTER_COE_EMERGENCY_DATA_BYTES];
}__TLR_PACKED_POST ;

typedef struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_CNF_DATA_T;

__TLR_PACKED_PRE struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_CNF_DATA_Ttag
{
  TLR_UINT32 ulSlaveHandle;
  TLR_BOOLEAN32 fDeleteEmergency;  /* value from request */
  TLR_BOOLEAN32 fOverflowOccured;  /* Emergency dropped cause of full buffer */
  ETHERCAT_MASTER_SLAVE_EMERGENCY_T atEmergenyBuffer[ETHERCAT_MASTER_COE_NUMBER_OF_EMERGENCY]; /* up to five emergencies */
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_CNF_Ttag
      ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_CNF_T;

struct ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_CNF_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header. */
  ETHERCAT_MASTER_PACKET_SLAVE_EMERGENCY_INFO_CNF_DATA_T  tData;  /** packet request data. */
};


/************************************************************/
/**packets for ETHERCAT_MASTER_CMD_GET_DC_DEVIATION_REQ/CNF**/
/************************************************************/
#define  ETHERCAT_MASTER_GET_DC_DEVIATION_NUMOFSLAVES ((RCX_MAX_DATA_SIZE - sizeof(TLR_UINT32))/sizeof(TLR_UINT32))

typedef struct ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_REQ_Ttag
      ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_REQ_T;

struct ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_REQ_Ttag
{
  /* the request needs no data part */
  TLR_PACKET_HEADER_T                                 tHead;  /** packet header. */
};


typedef struct ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_DATA_Ttag
{
  TLR_UINT32 ulBroadcastDeviation;
  TLR_UINT32 aulSlaveDeviation[ETHERCAT_MASTER_GET_DC_DEVIATION_NUMOFSLAVES] ;
};

typedef struct ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_Ttag
      ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_T;

struct ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_Ttag
{
  TLR_PACKET_HEADER_T                                 tHead;  /** packet header. */
  ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_DATA_T  tData;  /** packet request data. */
};


/**********************************************************/
/**packets for ETHERCAT_MASTER_CMD_START_BUS_SCAN_REQ/CNF**/
/**********************************************************/
typedef struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_DATA_Ttag
{
  TLR_UINT32 ulTimeout;
};

typedef struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_Ttag
      ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_T;

struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_Ttag
{
  TLR_PACKET_HEADER_T                               tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_DATA_T  tData;  /** packet request data. */
};

typedef struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_DATA_Ttag
{
  TLR_UINT32 ulFoundSlaves;
};

typedef struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_Ttag
      ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_T;

struct ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_Ttag
{
  TLR_PACKET_HEADER_T                               tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_DATA_T  tData;  /** packet confirmation data. */
};


/*************************************************************/
/**packets for ETHERCAT_MASTER_CMD_GET_BUS_SCAN_INFO_REQ/CNF**/
/*************************************************************/
typedef struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_DATA_Ttag
{
  TLR_UINT16 usAutoIncAddr;
};

typedef struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_Ttag
      ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_T;

struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_Ttag
{
  TLR_PACKET_HEADER_T                                  tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_DATA_T  tData;  /** packet request data. */
};

typedef struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_DATA_T;

struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_DATA_Ttag
{
  TLR_UINT32 ulVendorId;
  TLR_UINT32 ulProductCode;
  TLR_UINT32 ulRevisionNumber;
  TLR_UINT32 ulSerialNumber;
  TLR_UINT32 ulPortState;
};

typedef struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_Ttag
      ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_T;

struct ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_Ttag
{
  TLR_PACKET_HEADER_T                                  tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_DATA_T  tData;  /** packet confirmation data. */
};


/* <packet configuration interface> */

/***************************************************************/
/**packets for ETHERCAT_MASTER_CMD_BEGIN_CONFIGURATION_REQ/CNF**/
/***************************************************************/
typedef struct ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_REQ_DATA_Ttag
{
  TLR_UINT8     abEthAddrDestination[6]; /* see <Master><Info><Destination> */
  TLR_UINT8     abEthAddrSource[6];      /* see <Master><Info><Source> */
  TLR_UINT8     abEthType[2];            /* see <Master><Info><EtherType> */
  TLR_UINT32    ulMbxStatesStartAddr;    /* see <Master><MailboxStates><StartAddr> */
  TLR_UINT32    ulMbxStatesCount;        /* see <Master><MailboxStates><Count> */
  TLR_UINT32    ulEoEMaxPorts;           /* see <Master><EoE><MaxPorts>, currently unused, set to 0 */
  TLR_UINT32    ulEoEMaxFrames;          /* see <Master><EoE><MaxFrames>, currently unused, set to 0 */
  TLR_UINT32    ulEoEMaxMACs;            /* see <Master><EoE><MaxMACs>, currently unused, set to 0 */
  TLR_UINT32    ulInputByteSize;         /* see <ProcessImage><Inputs><ByteSize> */
  TLR_UINT32    ulOutputByteSize;        /* see <ProcessImage><Outputs><ByteSize> */
  /* the following variables are all Hilscher specific */
  TLR_UINT32    ulSystemFlags;           /* 0=Auto start; 1 = Bus On Required */
  TLR_UINT32    ulBusCycleTime;          /* Bus Cycle Time in us */
  TLR_UINT32    ulWdgTime;               /* DPM Watchdog timeout value */
  TLR_UINT32    ulLinkUpDelay;           /* start sending Ethernet frames N ms after LinkUp is detected */
  TLR_UINT32    ulBrokenSlaveBehaviour;  /* Handling of slaves with error */
  TLR_BOOLEAN32 fDcActivated;            /* Activate Distributed Clocks in Master */
  TLR_BOOLEAN32 fRedundancyActivated;    /* Use 2 Ethernet Ports for Redundancy; not allowed together with fDcActivated! */
  TLR_UINT8     bTargetState;            /* bus shall be driven into this communication state (usually OPERATIONAL)*/
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_REQ_Ttag
      ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_REQ_T;

struct ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_REQ_Ttag
{
  TLR_PACKET_HEADER_T                                    tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_REQ_DATA_T  tData;  /** packet request data. */
};


typedef struct ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_CNF_Ttag
      ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_CNF_T;

/** confirmation has no data part */
struct ETHERCAT_MASTER_PACKET_BEGIN_CONFIGURATION_CNF_Ttag
{
  TLR_PACKET_HEADER_T                                  tHead;  /** packet header.       */
};


/*****************************************************/
/**packets for ETHERCAT_MASTER_CMD_ADD_SLAVE_REQ/CNF**/
/*****************************************************/
typedef struct ETHERCAT_MASTER_PACKET_ADD_SLAVE_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_ADD_SLAVE_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_ADD_SLAVE_REQ_DATA_Ttag
{
  /* index of the slave (referenced in other packets); First Slave: 0, Second Slave: 1, etc. */
  TLR_UINT16    usSlaveIdx;
  /* <Info> stuff */
  TLR_UINT32    ulPhysAddr;            /* see <Slave><Info><PhysAddr> */
  TLR_UINT32    ulAutoIncAddr;         /* see <Slave><Info><AutoIncAddr> */
  TLR_UINT8     abPhysics[4];          /* see <Slave><Info><Physics>; example "YY\0" */
  TLR_UINT32    ulVendorId;            /* see <Slave><Info><VendorId> */
  TLR_UINT32    ulProductCode;         /* see <Slave><Info><ProductCode> */
  TLR_UINT32    ulRevisionNo;          /* see <Slave><Info><RevisionNo> */
  TLR_UINT32    ulSerialNo;            /* see <Slave><Info><SerialNo> */
  TLR_UINT8     abProductRevision[80]; /* see <Slave><Info><ProductRevision>; currently unused */
  TLR_UINT8     abName[80];            /* see <Slave><Info><Name>; reduced to a len of 80 bytes */

  /* <PreviousPort> stuff */
  TLR_UINT8     bPrevPortSelected; /* see <Slave><PreviousPort><Selected>; set to 1 to make Previous Port settings valid */
  TLR_UINT32    ulPrevDeviceID;    /* see <Slave><PreviousPort><DeviceId>; reserved in spec, set to 0 */
  TLR_UINT8     bPrevPortNr;       /* see <Slave><PreviousPort><Port>; but coded different: 0 is Port0, ... 3 is Port3; 0xFF means ignore this port */
  TLR_UINT16    usPrevPhysAddr;    /* see <Slave><PreviousPort><PhysAddr>; configured station address of the previous device */

  /* <ProcessData> stuff */
  TLR_UINT32    ulSndBitStart; /* see <Slave><ProcessData><Send><BitStart> */
  TLR_UINT32    ulSndBitLen;   /* see <Slave><ProcessData><Send><BitLength> */
  TLR_UINT32    ulRcvBitStart; /* see <Slave><ProcessData><BitStart> */
  TLR_UINT32    ulRcvBitLen;   /* see <Slave><ProcessData><BitLength> */

  /* <Mailbox> stuff */
  TLR_BOOLEAN32 fMbxSupport;            /* Slave supports Mbx? if FALSE, all Mailbox parameter (up to end of packet) are ignored. */
  TLR_BOOLEAN32 fDataLinkLayer;         /* see <Slave><Mailbox><DataLinkLayer> */
  TLR_UINT32    ulProtocol;             /* see <Slave><Mailbox><DataLinkLayer>; but coded as different: describes Mbx protocol support of slave; bitfield, Bit0: CoE, Bit1: SoE, Bit2: AoE, Bit3: EoE, Bit4: FoE, Bit5: VoE */
  TLR_UINT32    ulMbxSendStart;         /* see <Slave><Mailbox><Send><Start> */
  TLR_UINT32    ulMbxSendLen;           /* see <Slave><Mailbox><Send><Length> */
  TLR_UINT32    ulMbxRecvStart;         /* see <Slave><Mailbox><Recv><Start> */
  TLR_UINT32    ulMbxRecvLen;           /* see <Slave><Mailbox><Recv><Length> */
  TLR_BOOLEAN32 fMbxSendShortSend;     /* see <Slave><Mailbox><Send><ShortSend>; spec: reserved for future use; set to 0 */
  TLR_UINT32    ulRecvMbxPollTime;      /* see <Slave><Mailbox><Recv><PollTime>; definition: ulRecvMbxPollTime==0xFFFFFFFF means: no valid Poll time */
  TLR_UINT32    ulRecvMbxStatusBitAddr; /* see <Slave><Mailbox><Recv><StatusBitAddr>; definition: ulRecvMbxStatusBitAddr==0xFFFFFFFF means: no valid status bit addr */

  /* <Mailbox><Bootstrap> stuff */
  TLR_UINT32    ulMbxBootSendStart;         /* see <Slave><Mailbox><Bootstrap><Send><Start> */
  TLR_UINT32    ulMbxBootSendLen;           /* see <Slave><Mailbox><Bootstrap><Send><Length> */
  TLR_UINT32    ulMbxBootRecvStart;         /* see <Slave><Mailbox><Bootstrap><Recv><Start> */
  TLR_UINT32    ulMbxBootRecvLen;           /* see <Slave><Mailbox><Bootstrap><Recv><Length> */
  /* definition: if ulMbxBootSendLen AND ulMbxBootRecvLen are set to 0, the slave has no BOOT mailbox */
  TLR_BOOLEAN32 fMbxBootSendShortSend;     /* see <Slave><Mailbox><Bootstrap><Send><ShortSend>; spec: reserved for future use; set to 0 */
  TLR_UINT32    ulRecvMbxBootPollTime;      /* see <Slave><Mailbox><Bootstrap><Recv><PollTime>; definition: ulRecvMbxBootPollTime==0xFFFFFFFF means: no valid Poll time */
  TLR_UINT32    ulRecvMbxBootStatusBitAddr; /* see <Slave><Mailbox><Bootstrap><Recv><StatusBitAddr>; definition: ulRecvMbxBootStatusBitAddr==0xFFFFFFFF means: no valid status bit addr */
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_ADD_SLAVE_REQ_Ttag
      ETHERCAT_MASTER_PACKET_ADD_SLAVE_REQ_T;

struct ETHERCAT_MASTER_PACKET_ADD_SLAVE_REQ_Ttag
{
  TLR_PACKET_HEADER_T                          tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_ADD_SLAVE_REQ_DATA_T  tData;  /** packet request data. */
};


typedef struct ETHERCAT_MASTER_PACKET_ADD_SLAVE_CNF_Ttag
      ETHERCAT_MASTER_PACKET_ADD_SLAVE_CNF_T;

/** confirmation has no data part */
struct ETHERCAT_MASTER_PACKET_ADD_SLAVE_CNF_Ttag
{
  TLR_PACKET_HEADER_T                          tHead;  /** packet header.       */
};


/* valid bits for usTransition Bitfield (I=Init, P=Pre-Operational, S=Safe-Operational, O=Operational) */
#define ECAT_INITCMD_I_P            0x0001
#define ECAT_INITCMD_P_S            0x0002
#define ECAT_INITCMD_P_I            0x0004
#define ECAT_INITCMD_S_P            0x0008
#define ECAT_INITCMD_S_O            0x0010
#define ECAT_INITCMD_S_I            0x0020
#define ECAT_INITCMD_O_S            0x0040
#define ECAT_INITCMD_O_P            0x0080
#define ECAT_INITCMD_O_I            0x0100
#define ECAT_INITCMD_I_B            0x0200
#define ECAT_INITCMD_B_I            0x0400

/* if set: This command shall be sent before any other init command for this transition (compare <InitCmds><InitCmd><BeforeSlave>) */
#define ECAT_INITCMD_BEFORE         0x8000

#define ECAT_TRANSITION_VALID_BITS (ECAT_INITCMD_I_P | ECAT_INITCMD_P_S | ECAT_INITCMD_P_I | ECAT_INITCMD_S_P | ECAT_INITCMD_S_O | ECAT_INITCMD_S_I | ECAT_INITCMD_O_S | ECAT_INITCMD_O_P | ECAT_INITCMD_O_I | ECAT_INITCMD_BEFORE | ECAT_INITCMD_I_B | ECAT_INITCMD_B_I)


/*******************************************************/
/**packets for ETHERCAT_MASTER_CMD_ADD_INITCMD_REQ/CNF**/
/*******************************************************/
typedef struct ETHERCAT_MASTER_PACKET_ADD_INITCMD_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_ADD_INITCMD_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_ADD_INITCMD_REQ_DATA_Ttag
{
  TLR_UINT16 usDeviceIdx;             /* 0xFFFF for master; otherwise slave instance, start counting with 0 */
  TLR_UINT16 usEcatCmd;               /* see <Master>or<Slave> <InitCmds><InitCmd><Cmd>;  NOP, APRD, APWR, etc. */
  TLR_UINT32 ulAddr;                  /* see <Master>or<Slave> <InitCmds><InitCmd><Addr> + <Ado> + <Adp>; use as 32 Bit value if usEcatCmd == LRD or LWR or LRW
                                         otherwise use lower 16 bit Adp, upper 16 bit Ado */
  TLR_UINT16 usDataLength;            /* see <Master>or<Slave> <InitCmds><InitCmd><DataLength>; use ulInitDataLen XOR usDataLength! */
  TLR_UINT16 usTransition;            /* see <Master>or<Slave> <InitCmds><InitCmd><Transition> and <BeforeSlave>; but codes as bitfield, values see above (ECAT_INITCMD_*)  */
  TLR_UINT16 usExpectedWKC;           /* see <Master>or<Slave> <InitCmds><InitCmd><Cnt>; default: 0xFFFF, means "DONT CHECK" */
  TLR_UINT16 usFlags;                 /* see <Master>or<Slave> <InitCmds><InitCmd><Requires>; reserved for future use, set to 0! */
  TLR_UINT16 usValidateTimeout;       /* see <Master>or<Slave> <InitCmds><InitCmd><Validate><Timeout>, only used if usValidateDataLen != 0 */
  TLR_UINT16 usRetries;               /* see <Master>or<Slave> <InitCmds><InitCmd><Retries> */
  TLR_UINT16 usInitDataLen;           /* describes the used length of abInitData; use ulInitDataLen XOR usDataLength! */
  TLR_UINT8  abInitData[256];         /* see <Master>or<Slave> <InitCmds><InitCmd><Data> */
  TLR_UINT16 usValidateDataLen;       /* describes the used length of abValidateData */
  TLR_UINT8  abValidateData[256];     /* see <Master>or<Slave> <InitCmds><InitCmd><Validate><Data> */
  TLR_UINT16 usValidateDataMaskLen;   /* describes the used length of abValidateDataMask */
  TLR_UINT8  abValidateDataMask[256]; /* see <Master>or<Slave> <InitCmds><InitCmd><Validate><DataMask> */
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_ADD_INITCMD_REQ_Ttag
      ETHERCAT_MASTER_PACKET_ADD_INITCMD_REQ_T;

struct ETHERCAT_MASTER_PACKET_ADD_INITCMD_REQ_Ttag
{
  TLR_PACKET_HEADER_T                            tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_ADD_INITCMD_REQ_DATA_T  tData;  /** packet request data. */
};

typedef struct ETHERCAT_MASTER_PACKET_ADD_INITCMD_CNF_Ttag
      ETHERCAT_MASTER_PACKET_ADD_INITCMD_CNF_T;

/** confirmation has no data part */
struct ETHERCAT_MASTER_PACKET_ADD_INITCMD_CNF_Ttag
{
  TLR_PACKET_HEADER_T                                  tHead;  /** packet header.       */
};


/***********************************************************/
/**packets for ETHERCAT_MASTER_CMD_ADD_COE_INITCMD_REQ/CNF**/
/***********************************************************/
typedef struct ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_DATA_Ttag
{
  TLR_UINT16    usSlaveIdx;      /* slave instance, start counting with 0 */
  TLR_UINT16    usTransition;    /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><Transition>, same coding like in ADD_INITCMD */
  TLR_UINT32    ulTimeout;       /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><Timeout> */
  TLR_UINT32    ulCcs;           /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><Ccs> */
  TLR_UINT32    ulSdoIndex;      /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><Index> */
  TLR_UINT32    ulSdoSubIndex;   /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><SubIndex> */
  TLR_BOOLEAN32 fDisabled;       /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><Disabled> */
  TLR_BOOLEAN32 fFixed;          /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><Fixed> */
  TLR_BOOLEAN32 fCompleteAccess; /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><CompleteAccess> */
  TLR_UINT8     abData[256];     /* see <Slave><Mailbox><CoE><InitCmds><InitCmd><Data> */
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_Ttag
      ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_T;

#define ETHERCAT_MASTER_ADD_COE_INITCMD_EMPTY_SIZE ( sizeof(ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_DATA_T) - 256 * sizeof (TLR_UINT8))

struct ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_Ttag
{
  TLR_PACKET_HEADER_T                                tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_REQ_DATA_T  tData;  /** packet request data. */
};

typedef struct ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_CNF_Ttag
      ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_CNF_T;

/** confirmation has no data part */
struct ETHERCAT_MASTER_PACKET_ADD_COE_INITCMD_CNF_Ttag
{
  TLR_PACKET_HEADER_T                                  tHead;  /** packet header.       */
};


/******************************************************/
/**packets for ETHERCAT_MASTER_CMD_ADD_CYCLIC_REQ/CNF**/
/******************************************************/
typedef struct ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_DATA_Ttag
{
  TLR_UINT16    usCyclicIdx;          /* multiple <Cyclic> entries possible, but usually value is 0 */
  TLR_UINT16    usCycleTime;          /* reserved for later use, set to 0 */
  TLR_UINT16    usPriority;           /* reserved for later use, set to 0 */
  TLR_UINT16    usTaskId;             /* reserved for later use, set to 0 */
  TLR_UINT16    usFrameIdx;           /* located in which cyclic frame */
  TLR_UINT16    usState;              /* compare <Cyclic><Frame><Cmd><State>, but ALL states ORed together into one value */
  TLR_UINT16    usEcatCmd;            /* compare <Cyclic><Frame><Cmd> */
  TLR_UINT32    ulAddr;               /* compare <Cyclic><Frame><Cmd><Addr> + <Ado> + <Adp>; use as 32 Bit value if usEcatCmd == LRD or LWR or LRW
                                         otherwise use lower 16 bit Adp, upper 16 bit Ado */
  TLR_UINT16    usDataLength;         /* compare <Cyclic><Frame><Cmd><DataLength>*/
  TLR_UINT16    usExpectedWKC;        /* compare <Cyclic><Frame><Cmd><Cnt>, 0xFFFF means: "DONT CHECK" */
  TLR_UINT32    ulInputOffs;          /* compare <Cyclic><Frame><Cmd><InputOffs>*/
  TLR_UINT32    ulOutputOffs;         /* compare <Cyclic><Frame><Cmd><OutputOffs>*/
  TLR_UINT16    usNumOfUsedCopyInfos; /* how many of the following CopyInfos are used? */
  /* the following three variables are optional; set usNumOfUsedCopyInfos to 0 if they are not used */
  TLR_UINT16    ausCopyInfoSrcBitOffs[64];
  TLR_UINT16    ausCopyInfoDstBitOffs[64];
  TLR_UINT16    ausCopyInfoBitSize[64];
  TLR_UINT8     abSendData[256];      /* not supported yet: send preconfigured data */
} __TLR_PACKED_POST;


#define ETHERCAT_MASTER_ADD_CYCLIC_MIN_LEN (10 * sizeof(TLR_UINT16) + 3 * sizeof(TLR_UINT32))
/* size without data payload */
#define ETHERCAT_MASTER_ADD_CYCLIC_DATA_LEN (sizeof(ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_DATA_T) - 256 * sizeof (TLR_UINT8))

typedef struct ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_Ttag
      ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_T;

struct ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_Ttag
{
  TLR_PACKET_HEADER_T                           tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_ADD_CYCLIC_REQ_DATA_T  tData;  /** packet request data. */
};

#define ECAT_STATE_INIT                               (0x0001)
#define ECAT_STATE_PREOP                              (0x0002)
#define ECAT_STATE_SAFEOP                             (0x0004)
#define ECAT_STATE_OP                                 (0x0008)

typedef struct ETHERCAT_MASTER_PACKET_ADD_CYCLIC_CNF_Ttag
      ETHERCAT_MASTER_PACKET_ADD_CYCLIC_CNF_T;

/** confirmation has no data part */
struct ETHERCAT_MASTER_PACKET_ADD_CYCLIC_CNF_Ttag
{
  TLR_PACKET_HEADER_T                       tHead;  /** packet header.       */
};


/*************************************************************/
/**packets for ETHERCAT_MASTER_CMD_END_CONFIGURATION_REQ/CNF**/
/*************************************************************/
typedef struct ETHERCAT_MASTER_PACKET_END_CONFIGURATION_REQ_Ttag
      ETHERCAT_MASTER_PACKET_END_CONFIGURATION_REQ_T;

struct ETHERCAT_MASTER_PACKET_END_CONFIGURATION_REQ_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header.       */
};

typedef struct ETHERCAT_MASTER_PACKET_END_CONFIGURATION_CNF_Ttag
      ETHERCAT_MASTER_PACKET_END_CONFIGURATION_CNF_T;

struct ETHERCAT_MASTER_PACKET_END_CONFIGURATION_CNF_Ttag
{
  TLR_PACKET_HEADER_T                                 tHead;  /** packet header.       */
};
/* </packet configuration interface> */


/* <EEPROM access interface> */

/*******************************************************/
/**packets for ETHERCAT_MASTER_CMD_EEPROM_READ_REQ/CNF**/
/*******************************************************/
typedef struct ETHERCAT_MASTER_PACKET_EEPROM_READ_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_READ_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_EEPROM_READ_REQ_DATA_Ttag
{
  TLR_BOOLEAN32    fFixedAddressing; /* TRUE: use fixed addressing (requires configuration), FALSE: use auto increment addressing */
  TLR_UINT16       usSlaveAddress;   /* Slave Address, fixed or auto increment address depending on fFixedAddressing */
  TLR_UINT16       usEEPromStartOffset; /* Address to start EEPRom read from, number of WORDs */
  TLR_UINT16       usReadLen; /* value in bytes, number of WORDs */
  TLR_UINT16       usTimeout; /* time in ms */
} __TLR_PACKED_POST;


typedef struct ETHERCAT_MASTER_PACKET_EEPROM_READ_REQ_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_READ_REQ_T;

struct ETHERCAT_MASTER_PACKET_EEPROM_READ_REQ_Ttag
{
  TLR_PACKET_HEADER_T                           tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_EEPROM_READ_REQ_DATA_T tData;  /** packet request data. */
};

/* packet structure used for ETHERCAT_MASTER_CMD_EEPROM_READ_CNF */
typedef struct ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_DATA_Ttag
{
  TLR_BOOLEAN32    fFixedAddressing;    /* value from request */
  TLR_UINT16       usSlaveAddress;      /* value from request */
  TLR_UINT16       usEEPromStartOffset; /* value from request */
  TLR_UINT16       ausReadData[750];    /* read data, up to 750 WORDs */
} __TLR_PACKED_POST;

#define ETHERCAT_MASTER_EEPROM_READ_EMPTY_SIZE ( sizeof(ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_DATA_T) - 750 * sizeof (TLR_UINT16))


typedef struct ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_T;

struct ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_Ttag
{
  TLR_PACKET_HEADER_T                            tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_EEPROM_READ_CNF_DATA_T  tData;  /** packet request data. */
};


/********************************************************/
/**packets for ETHERCAT_MASTER_CMD_EEPROM_WRITE_REQ/CNF**/
/********************************************************/
typedef struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_DATA_Ttag
{
  TLR_BOOLEAN32    fFixedAddressing;    /* TRUE: use fixed addressing (requires configuration), FALSE: use auto increment addressing */
  TLR_UINT16       usSlaveAddress;      /* Slave Address, fixed or auto increment address depending on fFixedAddressing */
  TLR_UINT16       usEEPromStartOffset; /* Address to start EEPRom write from, number of WORDs */
  TLR_BOOLEAN32    fAssignAccessBack;   /* give slave the EEPROM control back? Set to TRUE to apply new data. Set to FALSE if further fragments follows. */
  TLR_UINT16       usTimeout;           /* time in ms */
  TLR_UINT16       ausWriteData[750];   /* data to write, up to 750 WORDs */
} __TLR_PACKED_POST;

/* packet without payload */
#define ETHERCAT_MASTER_EEPROM_WRITE_EMPTY_SIZE ( sizeof(ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_DATA_T) - 750 * sizeof (TLR_UINT16) )

/* at least one WORD must be written */
#define ETHERCAT_MASTER_EEPROM_WRITE_MIN_SIZE ( ETHERCAT_MASTER_EEPROM_WRITE_EMPTY_SIZE + sizeof (TLR_UINT16) )

typedef struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_T;

struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_EEPROM_WRITE_REQ_DATA_T  tData;  /** packet request data. */
};

/* packet structure used for ETHERCAT_MASTER_CMD_EEPROM_WRITE_CNF */
typedef struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_WRITE_CNF_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_CNF_DATA_Ttag
{
  TLR_BOOLEAN32    fFixedAddressing;     /* value from request */
  TLR_UINT16       usSlaveAddress;       /* value from request */
  TLR_UINT16       usEEPromStartOffset;  /* value from request */
} __TLR_PACKED_POST;


typedef struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_CNF_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_WRITE_CNF_T;

struct ETHERCAT_MASTER_PACKET_EEPROM_WRITE_CNF_Ttag
{
  TLR_PACKET_HEADER_T                             tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_EEPROM_WRITE_CNF_DATA_T  tData;  /** packet request data. */
};


/*********************************************************/
/**packets for ETHERCAT_MASTER_CMD_EEPROM_RELOAD_REQ/CNF**/
/*********************************************************/
typedef struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_REQ_DATA_Ttag
{
  TLR_BOOLEAN32    fFixedAddressing; /* TRUE: use fixed addressing, FALSE: use auto increment addressing */
  TLR_UINT16       usSlaveAddress;   /* Slave Address, fixed or auto increment address depending on fFixedAddressing */
  TLR_UINT16       usTimeout; /* time in ms */
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_REQ_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_REQ_T;

struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_REQ_Ttag
{
  TLR_PACKET_HEADER_T                              tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_REQ_DATA_T  tData;  /** packet request data. */
};

/* packet structure used for ETHERCAT_MASTER_CMD_EEPROM_RELOAD_CNF */
typedef struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_CNF_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_CNF_DATA_Ttag
{
  TLR_BOOLEAN32    fFixedAddressing;     /* value from request */
  TLR_UINT16       usSlaveAddress;       /* value from request */
} __TLR_PACKED_POST;


typedef struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_CNF_Ttag
      ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_CNF_T;

struct ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_CNF_Ttag
{
  TLR_PACKET_HEADER_T                           tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_EEPROM_RELOAD_CNF_DATA_T  tData;  /** packet request data. */
};
/* </EEPROM access interface> */


/* <control EtherCAT state> */
/*******************************************************/
/**packets for ETHERCAT_MASTER_CMD_GET_ECSTATE_REQ/CNF**/
/*******************************************************/
typedef struct ETHERCAT_MASTER_PACKET_GET_ECSTATE_REQ_Ttag
      ETHERCAT_MASTER_PACKET_GET_ECSTATE_REQ_T;

/** request has no data part */
struct ETHERCAT_MASTER_PACKET_GET_ECSTATE_REQ_Ttag
{
  TLR_PACKET_HEADER_T                          tHead;  /** packet header.       */
};

typedef struct ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_DATA_Ttag
      ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_DATA_Ttag
{
  TLR_UINT16    usCurrentEcState;  /* see defines ETHERCAT_MASTER_BUSSTATE_*, following values are reported:
  - ETHERCAT_MASTER_BUSSTATE_UNKNOWN: master not initialized
  - ETHERCAT_MASTER_BUSSTATE_INIT, ETHERCAT_MASTER_BUSSTATE_PREOP, ETHERCAT_MASTER_BUSSTATE_SAFEOP, ETHERCAT_MASTER_BUSSTATE_OP  */
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_Ttag
      ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_T;

struct ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_Ttag
{
  TLR_PACKET_HEADER_T                            tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_DATA_T  tData;  /** packet request data. */
};


/*******************************************************/
/**packets for ETHERCAT_MASTER_CMD_SET_ECSTATE_REQ/CNF**/
/*******************************************************/
typedef struct ETHERCAT_MASTER_PACKET_SET_ECSTATE_REQ_DATA_Ttag
      ETHERCAT_MASTER_PACKET_SET_ECSTATE_REQ_DATA_T;

__TLR_PACKED_PRE
struct ETHERCAT_MASTER_PACKET_SET_ECSTATE_REQ_DATA_Ttag
{
  TLR_UINT16    usNewEcState;  /* see defines ETHERCAT_MASTER_BUSSTATE_*, allowed values are _INIT, _PREOP, _SAFEOP, _OP  */
} __TLR_PACKED_POST;

typedef struct ETHERCAT_MASTER_PACKET_SET_ECSTATE_REQ_Ttag
      ETHERCAT_MASTER_PACKET_SET_ECSTATE_REQ_T;

struct ETHERCAT_MASTER_PACKET_SET_ECSTATE_REQ_Ttag
{
  TLR_PACKET_HEADER_T                            tHead;  /** packet header.       */
  ETHERCAT_MASTER_PACKET_SET_ECSTATE_REQ_DATA_T  tData;  /** packet request data. */
};


typedef struct ETHERCAT_MASTER_PACKET_SET_ECSTATE_CNF_Ttag
      ETHERCAT_MASTER_PACKET_SET_ECSTATE_CNF_T;

/** confirmation has no data part */
struct ETHERCAT_MASTER_PACKET_SET_ECSTATE_CNF_Ttag
{
  TLR_PACKET_HEADER_T                            tHead;  /** packet header.       */
};
/* </control EtherCAT state> */


#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_UNPACK_1(__ETHERCAT_MASTER_PACKET)
#endif

/*************************************************************************************/
#endif /* #ifndef __ETHERCAT_MASTER_PUBLIC_H */
