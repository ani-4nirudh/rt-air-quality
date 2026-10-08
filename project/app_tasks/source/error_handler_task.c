/**
 * @file error_handler_task.c 
 * @brief Source file for error handler task
 *
 * @author Anirudh Singh
 * @date 8th October 2026
 */

#include "error_handler_task.h"
#include "queue.h"

#define QUEUE_LENGTH 20

/**
 * Function definitons
 */
static void vErrorHandlerTaskStart(void);
static void vErrorHandlerTask(void *param);

/**
 * Queue to send and receive error messages.
 */
QueueHandle_t xErrorHandlerQueue = NULL;

void vErrorHandlerSendMsg(event_id_e EVT_ERR_ID) {}

void vErrorHandlerSendMsgFromISR(event_id_e EVT_ERR_ID);

static void vErrorHandlerTaskStart(void) {
  // Create a queue of event IDs to be stored
  xErrorHandlerQueue = xQueueCreate(QUEUE_LENGTH, sizeof(event_id_e));

  // Don't use the queue if it isn't created
  if (xErrorHandlerQueue == NULL) {
    while (1) // Halt here!
      ;
  }

  // Add queue to the registry for easier debugging
  vQueueAddToRegistry(xErrorHandlerQueue, "Error Handler Queue");

  // Start the vErrorHandlerTask
  configASSERT(pdPASS ==
               xTaskCreate(vErrorHandlerTask, "Error Handler Task", ERROR_HANDLER_TASK_STACK_SIZE, NULL, ERROR_HANDLER_TASK_STACK_PRIORITY, NULL));
}

static void vErrorHandlerTask(void *param) {}
