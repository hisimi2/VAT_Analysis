/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Last Modification:

    @version $Id: EtherCATMaster_AP_Public.h 10657 2010-09-20 09:59:15Z Ulli $

   Description:

    -public definition of task

   Changes:

     Date        Author        Description
     ----------------------------------------------------------------------------------
     2007-11-01  UJ            created

**************************************************************************************/

#ifndef __ETHERCAT_MASTER_AP_PUBLIC_H
#define __ETHERCAT_MASTER_AP_PUBLIC_H

/* the following command is used to register at the TLR-Watchdog */
#define  ETHERCAT_MASTER_AP_CMD_HOST_WDG_TIMEOUT_REQ        0x00640000
#define  ETHERCAT_MASTER_AP_CMD_HOST_WDG_TIMEOUT_CNF        0x00640001

/*************************************************************************************/
/** @name Queue Name
 *
 * Use the queue name to identify the process queue of task 'EtherCAT Master_AP'.
 * Example for internal identification:<br>
 * @code
 * TLR_RESULT eRslt;
 * TLR_HANDLE hQue;
 * TLR_UINT   uiInstance = 0;
 *
 * eRslt = TLR_QUE_IDENTIFY_INTERN(
 *                                 ETHERCAT_MASTER_AP_QUEUE_NAME,
 *                                 uiInstance,
 *                                 &hQue
 *                                );
 * @endcode
 * <br>
 * Example for external or normal identification:<br>
 * @code
 * TLR_RESULT      eRslt;
 * TLR_QUE_LINK_T  tQue;
 * TLR_UINT        uiInstance = 0;
 *
 * eRslt = TLR_QUE_IDENTIFY(ETHERCAT_MASTER_AP_QUEUE_NAME, uiInstance, &tQue);
 * @endcode
 * <br>
 * @{
 */
/** queue name */
#define ETHERCAT_MASTER_AP_QUEUE_NAME        "QUE_EC_MA_AP"

/** @} */


/*************************************************************************************/
/* packets */

/* pragma pack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_PACK_1(__ETHERCAT_MASTER_AP_PACKET)
#endif


/* pragma unpack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_UNPACK_1(__ETHERCAT_MASTER_AP_PACKET)
#endif


/*************************************************************************************/
#endif /* #ifndef __ETHERCAT_MASTER_AP_PUBLIC_H */
