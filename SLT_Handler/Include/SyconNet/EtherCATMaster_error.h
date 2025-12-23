#ifndef __ETHERCATMASTER_ERROR_H
#define __ETHERCATMASTER_ERROR_H

/////////////////////////////////////////////////////////////////////////////////////
// EtherCAT Master Packet Status codes (EtherCAT Master Stack)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: TLR_E_ETHERCAT_MASTER_COMMAND_INVALID
//
// MessageText:
//
//  Invalid command received.
//
#define TLR_E_ETHERCAT_MASTER_COMMAND_INVALID ((TLR_RESULT)0xC0650001L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_NO_LINK
//
// MessageText:
//
//  No link exists.
//
#define TLR_E_ETHERCAT_MASTER_NO_LINK    ((TLR_RESULT)0xC0650002L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_ERROR_READING_BUSCONFIG
//
// MessageText:
//
//  Error during reading the bus configuration.
//
#define TLR_E_ETHERCAT_MASTER_ERROR_READING_BUSCONFIG ((TLR_RESULT)0xC0650003L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_ERROR_PARSING_BUSCONFIG
//
// MessageText:
//
//  Error during processing the bus configuration.
//
#define TLR_E_ETHERCAT_MASTER_ERROR_PARSING_BUSCONFIG ((TLR_RESULT)0xC0650004L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_ERROR_BUSSCAN_FAILED
//
// MessageText:
//
//  Existing bus does not match configured bus.
//
#define TLR_E_ETHERCAT_MASTER_ERROR_BUSSCAN_FAILED ((TLR_RESULT)0xC0650005L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_NOT_ALL_SLAVES_AVAIL
//
// MessageText:
//
//  Not all slaves are available.
//
#define TLR_E_ETHERCAT_MASTER_NOT_ALL_SLAVES_AVAIL ((TLR_RESULT)0xC0650006L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_STOPMASTER_ERROR
//
// MessageText:
//
//  Error during Reset (stopping the master).
//
#define TLR_E_ETHERCAT_MASTER_STOPMASTER_ERROR ((TLR_RESULT)0xC0650007L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEINITMASTER_ERROR
//
// MessageText:
//
//  Error during Reset (deinitialize the master).
//
#define TLR_E_ETHERCAT_MASTER_DEINITMASTER_ERROR ((TLR_RESULT)0xC0650008L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_CLEANUP_ERROR
//
// MessageText:
//
//  Error during Reset (cleanup the dynamic resources).
//
#define TLR_E_ETHERCAT_MASTER_CLEANUP_ERROR ((TLR_RESULT)0xC0650009L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_CRITIAL_ERROR_STATE
//
// MessageText:
//
//  Master is in critical error state, reset required.
//
#define TLR_E_ETHERCAT_MASTER_CRITIAL_ERROR_STATE ((TLR_RESULT)0xC065000AL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_INVALID_BUSCYCLETIME
//
// MessageText:
//
//  The requested bus cycle time is invalid.
//
#define TLR_E_ETHERCAT_MASTER_INVALID_BUSCYCLETIME ((TLR_RESULT)0xC065000BL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_INVALID_BROKEN_SLAVE_BEHAVIOUR_PARA
//
// MessageText:
//
//  Invalid parameter for broken slave behaviour.
//
#define TLR_E_ETHERCAT_MASTER_INVALID_BROKEN_SLAVE_BEHAVIOUR_PARA ((TLR_RESULT)0xC065000CL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_WRONG_INTERNAL_STATE
//
// MessageText:
//
//  Master is in wrong internal state.
//
#define TLR_E_ETHERCAT_MASTER_WRONG_INTERNAL_STATE ((TLR_RESULT)0xC065000DL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_WATCHDOG_TIMEOUT_EXPIRED
//
// MessageText:
//
//  The watchdog expired.
//
#define TLR_E_ETHERCAT_MASTER_WATCHDOG_TIMEOUT_EXPIRED ((TLR_RESULT)0xC065000EL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_INVALID_SLAVEID
//
// MessageText:
//
//  Invalid SlaveId was used for CoE.
//
#define TLR_E_ETHERCAT_MASTER_COE_INVALID_SLAVEID ((TLR_RESULT)0xC065000FL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_NO_RESOURCE
//
// MessageText:
//
//  No available resources for CoE transfer.
//
#define TLR_E_ETHERCAT_MASTER_COE_NO_RESOURCE ((TLR_RESULT)0xC0650010L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_INTERNAL_ERROR
//
// MessageText:
//
//  Internal error during CoE usage.
//
#define TLR_E_ETHERCAT_MASTER_COE_INTERNAL_ERROR ((TLR_RESULT)0xC0650011L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_INVALID_INDEX
//
// MessageText:
//
//  Invalid slave index requested.
//
#define TLR_E_ETHERCAT_MASTER_COE_INVALID_INDEX ((TLR_RESULT)0xC0650012L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_INVALID_COMMUNICATION_STATE
//
// MessageText:
//
//  Invalid bus communication state for CoE usage.
//
#define TLR_E_ETHERCAT_MASTER_COE_INVALID_COMMUNICATION_STATE ((TLR_RESULT)0xC0650013L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_FRAME_LOST
//
// MessageText:
//
//  Frame with CoE data is lost.
//
#define TLR_E_ETHERCAT_MASTER_COE_FRAME_LOST ((TLR_RESULT)0xC0650014L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_TIMEOUT
//
// MessageText:
//
//  Timeout during CoE service.
//
#define TLR_E_ETHERCAT_MASTER_COE_TIMEOUT ((TLR_RESULT)0xC0650015L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_SLAVE_NOT_ADDRESSABLE
//
// MessageText:
//
//  Slave is not addressable (not on bus or power down?).
//
#define TLR_E_ETHERCAT_MASTER_COE_SLAVE_NOT_ADDRESSABLE ((TLR_RESULT)0xC0650016L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_INVALID_LIST_TYPE
//
// MessageText:
//
//  Invalid list type requested (during GetOdList).
//
#define TLR_E_ETHERCAT_MASTER_COE_INVALID_LIST_TYPE ((TLR_RESULT)0xC0650017L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_SLAVE_RESPONSE_TOO_BIG
//
// MessageText:
//
//  Data in Slave Response is too big for confirmation packet.
//
#define TLR_E_ETHERCAT_MASTER_COE_SLAVE_RESPONSE_TOO_BIG ((TLR_RESULT)0xC0650018L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_INVALID_ACCESSBITMASK
//
// MessageText:
//
//  Invalid access mask selected (during GetEntryDesc).
//
#define TLR_E_ETHERCAT_MASTER_COE_INVALID_ACCESSBITMASK ((TLR_RESULT)0xC0650019L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_WKC_ERROR
//
// MessageText:
//
//  Slave Working Counter error during CoE service.
//
#define TLR_E_ETHERCAT_MASTER_COE_WKC_ERROR ((TLR_RESULT)0xC065001AL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SERVICE_IN_USE
//
// MessageText:
//
//  The service is already in use.
//
#define TLR_E_ETHERCAT_MASTER_SERVICE_IN_USE ((TLR_RESULT)0xC065001BL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_INVALID_COMMUNICATION_STATE
//
// MessageText:
//
//  Command is not useable in this communication state.
//
#define TLR_E_ETHERCAT_MASTER_INVALID_COMMUNICATION_STATE ((TLR_RESULT)0xC065001CL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DC_NOT_ACTIVATED
//
// MessageText:
//
//  Distributed Clocks must be activated for this command.
//
#define TLR_E_ETHERCAT_MASTER_DC_NOT_ACTIVATED ((TLR_RESULT)0xC065001DL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_BUS_SCAN_CURRENTLY_RUNNING
//
// MessageText:
//
//  Bus Scan is currently running.
//
#define TLR_E_ETHERCAT_MASTER_BUS_SCAN_CURRENTLY_RUNNING ((TLR_RESULT)0xC065001EL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_BUS_SCAN_TIMEOUT
//
// MessageText:
//
//  Bus Scan Timeout. No slave found.
//
#define TLR_E_ETHERCAT_MASTER_BUS_SCAN_TIMEOUT ((TLR_RESULT)0xC065001FL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_BUS_SCAN_NOT_READY_YET
//
// MessageText:
//
//  Bus Scan is not ready yet.
//
#define TLR_E_ETHERCAT_MASTER_BUS_SCAN_NOT_READY_YET ((TLR_RESULT)0xC0650020L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_BUS_SCAN_INVALID_SLAVE
//
// MessageText:
//
//  Invalid slave. No information available.
//
#define TLR_E_ETHERCAT_MASTER_BUS_SCAN_INVALID_SLAVE ((TLR_RESULT)0xC0650021L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_INVALIDACCESS
//
// MessageText:
//
//  Slave does not allow reading or writing (CoE access).
//
#define TLR_E_ETHERCAT_MASTER_COE_INVALIDACCESS ((TLR_RESULT)0xC0650022L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT
//
// MessageText:
//
//  Slave does not support a mailbox.
//
#define TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT ((TLR_RESULT)0xC0650023L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_COE_NO_COE_SUPPORT
//
// MessageText:
//
//  Slave does not support CoE.
//
#define TLR_E_ETHERCAT_MASTER_COE_NO_COE_SUPPORT ((TLR_RESULT)0xC0650024L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_TASK_CREATION_FAILED
//
// MessageText:
//
//  Task could not be created during runtime.
//
#define TLR_E_ETHERCAT_MASTER_TASK_CREATION_FAILED ((TLR_RESULT)0xC0650025L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_INVALID_SLAVE_SM_CONFIGURATION
//
// MessageText:
//
//  The Sync Manager configuration of a slave is invalid.
//
#define TLR_E_ETHERCAT_MASTER_INVALID_SLAVE_SM_CONFIGURATION ((TLR_RESULT)0xC0650026L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TOGGLE
//
// MessageText:
//
//  SDO abort code: Toggle bit not alternated.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TOGGLE ((TLR_RESULT)0xC0650027L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TIMEOUT
//
// MessageText:
//
//  SDO abort code: SDO protocol timed out.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TIMEOUT ((TLR_RESULT)0xC0650028L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_CCS_SCS
//
// MessageText:
//
//  SDO abort code: Client/server command specifier not valid or unknown.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_CCS_SCS ((TLR_RESULT)0xC0650029L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_BLK_SIZE
//
// MessageText:
//
//  SDO abort code: Invalid block size (block mode only).
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_BLK_SIZE ((TLR_RESULT)0xC065002AL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_SEQNO
//
// MessageText:
//
//  SDO abort code: Invalid sequence number (block mode only).
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_SEQNO ((TLR_RESULT)0xC065002BL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_CRC
//
// MessageText:
//
//  SDO abort code: CRC error (block mode only).
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_CRC ((TLR_RESULT)0xC065002CL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_MEMORY
//
// MessageText:
//
//  SDO abort code: Out of memory.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_MEMORY ((TLR_RESULT)0xC065002DL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_ACCESS
//
// MessageText:
//
//  SDO abort code: Unsupported access to an object.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_ACCESS ((TLR_RESULT)0xC065002EL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_WRITEONLY
//
// MessageText:
//
//  SDO abort code:  Attempt to read a write only object.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_WRITEONLY ((TLR_RESULT)0xC065002FL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_READONLY
//
// MessageText:
//
//  SDO abort code: Attempt to write a read only object.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_READONLY ((TLR_RESULT)0xC0650030L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_INDEX
//
// MessageText:
//
//  SDO abort code: Object does not exist in the object dictionary.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_INDEX ((TLR_RESULT)0xC0650031L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_PDO_MAP
//
// MessageText:
//
//  SDO abort code: Object cannot be mapped to the PDO.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_PDO_MAP ((TLR_RESULT)0xC0650032L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_PDO_LEN
//
// MessageText:
//
//  SDO abort code: The number and length of the objects to be mapped would exceed PDO length.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_PDO_LEN ((TLR_RESULT)0xC0650033L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_P_INCOMP
//
// MessageText:
//
//  SDO abort code: General parameter incompatibility reason.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_P_INCOMP ((TLR_RESULT)0xC0650034L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_I_INCOMP
//
// MessageText:
//
//  SDO abort code: General internal incompatibility in the device.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_I_INCOMP ((TLR_RESULT)0xC0650035L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_HARDWARE
//
// MessageText:
//
//  SDO abort code: Access failed due to an hardware error.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_HARDWARE ((TLR_RESULT)0xC0650036L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_SIZE
//
// MessageText:
//
//  SDO abort code: Data type does not match, length of service parameter does not match.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_SIZE ((TLR_RESULT)0xC0650037L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_SIZE1
//
// MessageText:
//
//  SDO abort code: Data type does not match, length of service parameter too high.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_SIZE1 ((TLR_RESULT)0xC0650038L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_SIZE2
//
// MessageText:
//
//  SDO abort code: Data type does not match, length of service parameter too low.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_SIZE2 ((TLR_RESULT)0xC0650039L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_OFFSET
//
// MessageText:
//
//  SDO abort code: Sub-index does not exist.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_OFFSET ((TLR_RESULT)0xC065003AL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_RANGE
//
// MessageText:
//
//  SDO abort code: Value range of parameter exceeded (only for write access).
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_RANGE ((TLR_RESULT)0xC065003BL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_RANGE1
//
// MessageText:
//
//  SDO abort code: Value of parameter written too high.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_RANGE1 ((TLR_RESULT)0xC065003CL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_RANGE2
//
// MessageText:
//
//  SDO abort code: Value of parameter written too low.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DATA_RANGE2 ((TLR_RESULT)0xC065003DL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_MINMAX
//
// MessageText:
//
//  SDO abort code: Maximum value is less than minimum value.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_MINMAX ((TLR_RESULT)0xC065003EL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_GENERAL
//
// MessageText:
//
//  SDO abort code: general error.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_GENERAL ((TLR_RESULT)0xC065003FL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TRANSFER
//
// MessageText:
//
//  SDO abort code: Data cannot be transferred or stored to the application.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TRANSFER ((TLR_RESULT)0xC0650040L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TRANSFER1
//
// MessageText:
//
//  SDO abort code: Data cannot be transferred or stored to the application because of local control.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TRANSFER1 ((TLR_RESULT)0xC0650041L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TRANSFER2
//
// MessageText:
//
//  SDO abort code: Data cannot be transferred or stored to the application because of the present device state.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_TRANSFER2 ((TLR_RESULT)0xC0650042L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DICTIONARY
//
// MessageText:
//
//  SDO abort code: Object dictionary dynamic generation fails or no object dictionary is present (e.g. object dictionary is generated from file and generation fails because of an file error).
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_DICTIONARY ((TLR_RESULT)0xC0650043L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_UNKNOWN
//
// MessageText:
//
//  SDO abort code: unknown code.
//
#define TLR_E_ETHERCAT_MASTER_SDO_ABORTCODE_UNKNOWN ((TLR_RESULT)0xC0650044L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_ERROR
//
// MessageText:
//
//  Slave status code: Unspecified error.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_ERROR ((TLR_RESULT)0xC0650045L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVREQSTATECNG
//
// MessageText:
//
//  Slave status code: Invalid requested state change.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVREQSTATECNG ((TLR_RESULT)0xC0650046L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_UNKREQSTATE
//
// MessageText:
//
//  Slave status code: Unknown requested state.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_UNKREQSTATE ((TLR_RESULT)0xC0650047L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_BOOTSTRAPNSUPP
//
// MessageText:
//
//  Slave status code: Bootstrap not supported.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_BOOTSTRAPNSUPP ((TLR_RESULT)0xC0650048L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_NOVALIDFW
//
// MessageText:
//
//  Slave status code: No valid firmware.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_NOVALIDFW ((TLR_RESULT)0xC0650049L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVALIDMBXCNF1
//
// MessageText:
//
//  Slave status code: Invalid mailbox configuration1.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVALIDMBXCNF1 ((TLR_RESULT)0xC065004AL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVALIDMBXCNF2
//
// MessageText:
//
//  Slave status code: Invalid mailbox configuration2.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVALIDMBXCNF2 ((TLR_RESULT)0xC065004BL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVALIDSMCNF
//
// MessageText:
//
//  Slave status code: Invalid sync manager configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVALIDSMCNF ((TLR_RESULT)0xC065004CL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_NOVALIDIN
//
// MessageText:
//
//  Slave status code: No valid inputs available.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_NOVALIDIN ((TLR_RESULT)0xC065004DL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_NOVALIDOUT
//
// MessageText:
//
//  Slave status code: No valid outputs.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_NOVALIDOUT ((TLR_RESULT)0xC065004EL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SYNCERROR
//
// MessageText:
//
//  Slave status code: Synchronization error.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SYNCERROR ((TLR_RESULT)0xC065004FL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SMWATCHDOG
//
// MessageText:
//
//  Slave status code: Sync manager watchdog.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SMWATCHDOG ((TLR_RESULT)0xC0650050L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVSMTYPES
//
// MessageText:
//
//  Slave status code: Invalid Sync Manager Types.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVSMTYPES ((TLR_RESULT)0xC0650051L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVOUTCONFIG
//
// MessageText:
//
//  Slave status code: Invalid Output Configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVOUTCONFIG ((TLR_RESULT)0xC0650052L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVINCONFIG
//
// MessageText:
//
//  Slave status code: Invalid Input Configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVINCONFIG ((TLR_RESULT)0xC0650053L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVWDCONFIG
//
// MessageText:
//
//  Slave status code: Invalid Watchdog Configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVWDCONFIG ((TLR_RESULT)0xC0650054L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDCOLDRS
//
// MessageText:
//
//  Slave status code: Slave needs cold start.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDCOLDRS ((TLR_RESULT)0xC0650055L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDINIT
//
// MessageText:
//
//  Slave status code: Slave needs INIT.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDINIT ((TLR_RESULT)0xC0650056L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDPREOP
//
// MessageText:
//
//  Slave status code: Slave needs PREOP.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDPREOP ((TLR_RESULT)0xC0650057L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDSAFEOP
//
// MessageText:
//
//  Slave status code: Slave needs SAFEOP.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_SLVNEEDSAFEOP ((TLR_RESULT)0xC0650058L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVOUTFMMUCNFG
//
// MessageText:
//
//  Slave status code: Invalid Output FMMU Configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVOUTFMMUCNFG ((TLR_RESULT)0xC0650059L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVINFMMUCNFG
//
// MessageText:
//
//  Slave status code: Invalid Input FMMU Configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVINFMMUCNFG ((TLR_RESULT)0xC065005AL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCSYNCCNFG
//
// MessageText:
//
//  Slave status code: Invalid DC SYNCH Configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCSYNCCNFG ((TLR_RESULT)0xC065005BL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCLATCHCNFG
//
// MessageText:
//
//  Slave status code: Invalid DC Latch Configuration.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCLATCHCNFG ((TLR_RESULT)0xC065005CL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_PLLERROR
//
// MessageText:
//
//  Slave status code: PLL Error.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_PLLERROR ((TLR_RESULT)0xC065005DL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCIOERROR
//
// MessageText:
//
//  Slave status code: Invalid DC IO Error.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCIOERROR ((TLR_RESULT)0xC065005EL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCTOERROR
//
// MessageText:
//
//  Slave status code: Invalid DC Timeout Error.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_INVDCTOERROR ((TLR_RESULT)0xC065005FL)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_EOE
//
// MessageText:
//
//  Slave status code: MBX_EOE.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_EOE ((TLR_RESULT)0xC0650060L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_COE
//
// MessageText:
//
//  Slave status code: MBX_COE.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_COE ((TLR_RESULT)0xC0650061L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_FOE
//
// MessageText:
//
//  Slave status code: MBX_FOE.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_FOE ((TLR_RESULT)0xC0650062L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_SOE
//
// MessageText:
//
//  Slave status code: MBX_SOE.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_SOE ((TLR_RESULT)0xC0650063L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_VOE
//
// MessageText:
//
//  Slave status code: MBX_VOE.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_MBX_VOE ((TLR_RESULT)0xC0650064L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_OTHER
//
// MessageText:
//
//  Slave status code: vendor specific error code.
//
#define TLR_E_ETHERCAT_MASTER_DEVICE_STATUSCODE_OTHER ((TLR_RESULT)0xC0650065L)

//
// MessageId: TLR_E_ETHERCAT_MASTER_PREVIOUS_PORT_MISSING
//
// MessageText:
//
//  Slave status code: PreviousPort configuration missing in bus configuration file (outdated configurator).
//
#define TLR_E_ETHERCAT_MASTER_PREVIOUS_PORT_MISSING ((TLR_RESULT)0xC0650066L)




#endif  //__ETHERCATMASTER_ERROR_H

