#ifndef __ETHERCATMASTERAPTASK_ERROR_H
#define __ETHERCATMASTERAPTASK_ERROR_H

/////////////////////////////////////////////////////////////////////////////////////
// EtherCAT Master AP Packet Status codes (EtherCAT Master AP Task)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_COMMAND_INVALID
//
// MessageText:
//
//  Invalid command received.
//
#define TLR_E_ETHERCAT_MASTER_AP_COMMAND_INVALID ((TLR_RESULT)0xC0640001L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_DPM_WATCHDOG_TIMEOUT_EXPIRED
//
// MessageText:
//
//  The watchdog expired.
//
#define TLR_E_ETHERCAT_MASTER_AP_DPM_WATCHDOG_TIMEOUT_EXPIRED ((TLR_RESULT)0xC0640002L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_TIME_TOO_SMALL
//
// MessageText:
//
//  The requested Watchdog time is too small.
//
#define TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_TIME_TOO_SMALL ((TLR_RESULT)0xC0640003L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_TIME_TOO_LARGE
//
// MessageText:
//
//  The requested Watchdog time is too large.
//
#define TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_TIME_TOO_LARGE ((TLR_RESULT)0xC0640004L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_RESET_ERROR
//
// MessageText:
//
//  Error during Reset (resetting watchdog).
//
#define TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_RESET_ERROR ((TLR_RESULT)0xC0640005L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_CLEANUP_ERROR
//
// MessageText:
//
//  Error during Reset (cleanup the dynamic resources).
//
#define TLR_E_ETHERCAT_MASTER_AP_CLEANUP_ERROR ((TLR_RESULT)0xC0640006L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_CRITIAL_ERROR_STATE
//
// MessageText:
//
//  Master is in critical error state, reset required.
//
#define TLR_E_ETHERCAT_MASTER_AP_CRITIAL_ERROR_STATE ((TLR_RESULT)0xC0640007L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_ACTIVATE_ERROR
//
// MessageText:
//
//  Error activating the watchdog.
//
#define TLR_E_ETHERCAT_MASTER_AP_WATCHDOG_ACTIVATE_ERROR ((TLR_RESULT)0xC0640008L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_INPUT_DATA_TOO_LARGE
//
// MessageText:
//
//  Size of configured input data is larger as cyclic DPM input data size.
//
#define TLR_E_ETHERCAT_MASTER_AP_INPUT_DATA_TOO_LARGE ((TLR_RESULT)0xC0640009L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_OUTPUT_DATA_TOO_LARGE
//
// MessageText:
//
//  Size of configured output data is larger as cyclic DPM output data size.
//
#define TLR_E_ETHERCAT_MASTER_AP_OUTPUT_DATA_TOO_LARGE ((TLR_RESULT)0xC064000AL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_ENABLE_BUS_SYNC_FAILED
//
// MessageText:
//
//  Bus Synchronus could not be activated.
//
#define TLR_E_ETHERCAT_MASTER_AP_ENABLE_BUS_SYNC_FAILED ((TLR_RESULT)0xC064000BL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_AP_TASK_CREATION_FAILED
//
// MessageText:
//
//  Task could not be created during runtime.
//
#define TLR_E_ETHERCAT_MASTER_AP_TASK_CREATION_FAILED ((TLR_RESULT)0xC064000CL)




#endif  //__ETHERCATMASTERAPTASK_ERROR_H

