/**
 * @file error_handler_task.h 
 * @brief Header file to configure function prototypes used by error handling task
 *
 * @author Anirudh Singh
 * @date 8th October 2026
 */

#ifndef INC_ERROR_HANDLER_TASK_H
#define INC_ERROR_HANDLER_TASK_H

#include "FreeRTOSTasks.h"
#include "error.h"

/**
 * Define the number of LED blink cycles for a particular error event
 */
#define EVT_BLINK_CYCLES (2)

/**
 * Define the delay in milliseconds for blinking
 */
#define EVT_BLINK_DELAY (250)

/**
 * Queue length for the Error Handler Task
 */
#define QUEUE_LENGTH 10

typedef enum {
  // System health analog watchdog threshold error
  EVT_SYS_HEALTH_AWDG_THRESH_EXCEEDED = 0,

  // Sensor read error
  EVT_SENSOR_READ_FAIL,

  // FRAM initialisation failure
  EVT_FRAM_INIT_FAIL,

  // MODBUS Mutex errors
  EVT_MODBUS_MUTEX_NOT_CREATED,
  EVT_MODBUS_MUTEX_TIMEOUT,

  // MODBUS middleware UART Tx error
  EVT_MODBUS_UART_TX_ERROR,

  // MODBUS slave error
  EVT_MODBUS_SLAVE_WRITE_HOLDING_REGS_FAIL,
  EVT_MODBUS_SLAVE_WRITE_COILS_FAIL,
  EVT_MODBUS_SLAVE_CRC_MISMATCH,

  // MODBUS internal data update errors
  EVT_MODBUS_DATA_UPDATE_COILS_FAIL,
  EVT_MODBUS_DATA_UPDATE_HOLDING_REGS_FAIL,
  EVT_MODBUS_DATA_UPDATE_INPUT_REGS_FAIL,

  // Total number of error codes
  EVT_TOTAL
} event_id_e;

/**
 * Sends an error message to the Error Handler task by passing the Error ID
 *
 * @param EVT_ERR_ID Event ID for the error message
 */
error_t vErrorHandlerSendMsg(event_id_e evt_err_id);

/**
 * Sends an error message generated from the interrupt routine (ISR) to the Error Handler task by passing the Error ID
 *
 * @param EVT_ERR_ID Event ID for the error message
 */
error_t vErrorHandlerSendMsgFromISR(event_id_e evt_err_id);

/**
 * Start the error handler FreeRTOS task
 */
void vErrorHandlerTaskStart(void);

#endif // !INC_ERROR_HANDLER_TASK_H
