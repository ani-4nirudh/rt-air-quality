/**
 * @file error.h 
 * @brief Header file for error code enumerations
 *
 * @author Anirudh Singh
 * @date 8th October 2026
 */

// Type definition to create functions with error codes
typedef int error_t;

// Enumeration to save error codes
typedef enum {
  ERR_OK = 0,
  ERR_FAIL = -1,
  ERR_TIMEOUT = -2,
  ERR_INVALID_PARAM = -3,
  SENSIRION_SHT_PROBE_FAILED = -4,
  SENSIRION_GET_RHT_SIGNAL_FAILED = -5,
  SENSIRION_SGP_PROBE_FAILED = -6,
  SENSIRION_GET_SGP_SIGNAL_FAILED = -7,
  SENSIRION_SET_RHT_SIGNAL_FAILED = -8,
  MODBUS_INVALID_REG_COUNT = -9,
  MODBUS_INVALID_REG_ADDRESS = -10,
  MODBUS_INVALID_END_ADDRESS = -11,
  MODBUS_INVALID_COIL_COUNT = -12,
  MODBUS_INVALID_COIL_ADDRESS = -13,
  MODBUS_INVALID_COIL_VALUE = -14,
  MODBUS_INVALID_DISC_COUNT = -15,
  MODBUS_INVALID_DISC_ADDRESS = -16,
  MODBUS_INVALID_DISC_VALUE = -17,
  MODBUS_MUTEX_NOT_CREATED = -18,
  MODBUS_MUTEX_TIMEOUT = -19,
  MODBUS_MUTEX_UNLOCK_FAIL = -20,
} error_e;
