/**
 * @file error_handler_task.c 
 * @brief Source file for error handler task
 *
 * @author Anirudh Singh
 * @date 8th October 2026
 */

#include "error_handler_task.h"
#include "error.h"
#include "gpio.h"
#include "gpio_defs.h"
#include "queue.h"

/**
 * Function definitons
 */
static void vErrorHandlerTask(void *param);
static void vErrorHandlerLEDBlink(void);

/**
 * Queue to send and receive error messages.
 */
QueueHandle_t xErrorHandlerQueue = NULL;

/**
 * Array to save the error counts for different event IDs
 */
static uint32_t error_count[EVT_TOTAL] = {0U};

// Error drop count if xQueueSend does not work
static uint32_t error_drop_count[EVT_TOTAL] = {0U};

/************************************
 ********* Public Functions *********
 ************************************/

/**
 * 1. Create queue before launching the task
 * 2. Halt if the queue is not created
 * 3. Launch the Error Handler task
 */
void vErrorHandlerTaskStart(void) {
  // Create a queue of event IDs to be stored
  xErrorHandlerQueue = xQueueCreate(QUEUE_LENGTH, sizeof(event_id_e));

  // Don't use the queue if it isn't created
  if (xErrorHandlerQueue == NULL) {
    // Set the LED pin to HIGH
    if (GPIO_read_pin(&USER_LED) != GPIO_PIN_SET) {
      GPIO_set_pin(&USER_LED);
    }
    while (1)
      ; // Halt here indefinitely!
  }

  // Add queue to the registry for easier debugging
  vQueueAddToRegistry(xErrorHandlerQueue, "Error Handler Queue");

  // Start the vErrorHandlerTask
  configASSERT(xTaskCreate(vErrorHandlerTask, "Error Handler Task", ERROR_HANDLER_TASK_STACK_SIZE, NULL, ERROR_HANDLER_TASK_STACK_PRIORITY, NULL) ==
               pdPASS);
}

/**
 * Send the error event ID to the error queue
 * @param event_id_e Event error ID defined inside the file 'error_handler_task.h'
 * @return Error code defined inside the file 'error.h'
 *
 * Note: The following public functions don't have any priority.
 * Their purpose is just to wake up the error handler task from inside another task.
 */
error_t vErrorHandlerSendMsg(event_id_e evt_err_id) {
  if ((evt_err_id >= EVT_TOTAL) || (xErrorHandlerQueue == NULL)) {
    return ERR_FAIL;
  }

  /**
   * Note: Handle to the queue to which the error ID needs to be posted and 
   * wait indefinitely until a place inside the queue is free (hence, blocking the xQueueSend)
   */
  if (xQueueSend(xErrorHandlerQueue, &evt_err_id, portMAX_DELAY) != pdPASS) {
    error_drop_count[evt_err_id]++;
    return ERR_FAIL;
  }

  return ERR_OK;
}

/**
 * Send the error event ID from ISR
 * @param event_id_e Event error ID defined inside the file 'error_handler_task.h'
 * @return Error code defined inside the file 'error.h'
 */
error_t vErrorHandlerSendMsgFromISR(event_id_e evt_err_id) {
  if ((evt_err_id >= EVT_TOTAL) || (xErrorHandlerQueue == NULL)) {
    return ERR_FAIL;
  }

  /**
   * Handle to the queue to which the error ID needs to be posted
   * Needs to be set to pdFALSE so it can later be set to pdTRUE by any of the functions inside the interrupt
   */
  BaseType_t xHigherTaskPriorityTaskWoken = pdFALSE;

  // Post the item to the back of the queue
  if (xQueueSendFromISR(xErrorHandlerQueue, &evt_err_id, &xHigherTaskPriorityTaskWoken) != pdPASS) {
    UBaseType_t uxSavedInterruptStatus;
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    error_drop_count[evt_err_id]++;
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);

    return ERR_FAIL;
  }

  // If the handle is set to pdTRUE, force a context switch to the highest priority task (in our case it is the Error Handler task)
  portYIELD_FROM_ISR(xHigherTaskPriorityTaskWoken);

  return ERR_OK;
}

/************************************
 ******** Private Functions *********
 ************************************/

/**
 * Logic for the Error Handler task:
 *
 * 1. SendMsg and SendMsgFromISR will send the error code to the queue
 * 2. The following task should receive this error queue
 * 3. Flag this error
 * 4. Start the blinking LED task to visually indicate the error
 */
static void vErrorHandlerTask(void *param) {
  (void)param;

  event_id_e received_evt_id;

  while (1) {
    if (xErrorHandlerQueue != NULL) {
      if (xQueueReceive(xErrorHandlerQueue, &received_evt_id, portMAX_DELAY) == pdPASS) {
        if (received_evt_id < EVT_TOTAL) {
          error_count[received_evt_id]++;
        }

        switch (received_evt_id) {
        case EVT_SYS_HEALTH_AWDG_THRESH_EXCEEDED:
        case EVT_SENSOR_READ_FAIL:
        case EVT_FRAM_INIT_FAIL:
        case EVT_MODBUS_MUTEX_NOT_CREATED:
        case EVT_MODBUS_MUTEX_TIMEOUT:
        case EVT_MODBUS_UART_TX_ERROR:
        case EVT_MODBUS_SLAVE_WRITE_HOLDING_REGS_FAIL:
        case EVT_MODBUS_SLAVE_WRITE_COILS_FAIL:
        case EVT_MODBUS_SLAVE_CRC_MISMATCH:
        case EVT_MODBUS_DATA_UPDATE_COILS_FAIL:
        case EVT_MODBUS_DATA_UPDATE_HOLDING_REGS_FAIL:
        case EVT_MODBUS_DATA_UPDATE_INPUT_REGS_FAIL:

        default:
          vErrorHandlerLEDBlink();
          break;
        }
      }
    }
  }
}

/**
 * Launching a LED blink task when there is an error
 */
static void vErrorHandlerLEDBlink(void) {
  for (uint8_t i = 0; i < EVT_BLINK_CYCLES; i++) {
    GPIO_reset_pin(&USER_LED);
    vTaskDelay(EVT_BLINK_DELAY);
    GPIO_set_pin(&USER_LED);
    vTaskDelay(EVT_BLINK_DELAY);
  }
}
