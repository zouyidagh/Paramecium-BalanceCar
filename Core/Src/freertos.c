/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for chassisControl */
osThreadId_t chassisControlHandle;
uint32_t chassisControlBuffer[ 1024 ];
osStaticThreadDef_t chassisControlControlBlock;
const osThreadAttr_t chassisControl_attributes = {
  .name = "chassisControl",
  .cb_mem = &chassisControlControlBlock,
  .cb_size = sizeof(chassisControlControlBlock),
  .stack_mem = &chassisControlBuffer[0],
  .stack_size = sizeof(chassisControlBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for mpu6050 */
osThreadId_t mpu6050Handle;
uint32_t mpu6050TaskBuffer[ 128 ];
osStaticThreadDef_t mpu6050TaskControlBlock;
const osThreadAttr_t mpu6050_attributes = {
  .name = "mpu6050",
  .cb_mem = &mpu6050TaskControlBlock,
  .cb_size = sizeof(mpu6050TaskControlBlock),
  .stack_mem = &mpu6050TaskBuffer[0],
  .stack_size = sizeof(mpu6050TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for oledDisplay */
osThreadId_t oledDisplayHandle;
uint32_t oledDisplayTaskBuffer[ 128 ];
osStaticThreadDef_t oledDisplayTaskControlBlock;
const osThreadAttr_t oledDisplay_attributes = {
  .name = "oledDisplay",
  .cb_mem = &oledDisplayTaskControlBlock,
  .cb_size = sizeof(oledDisplayTaskControlBlock),
  .stack_mem = &oledDisplayTaskBuffer[0],
  .stack_size = sizeof(oledDisplayTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ps2Controller */
osThreadId_t ps2ControllerHandle;
uint32_t ps2ControllerTaBuffer[ 128 ];
osStaticThreadDef_t ps2ControllerTaControlBlock;
const osThreadAttr_t ps2Controller_attributes = {
  .name = "ps2Controller",
  .cb_mem = &ps2ControllerTaControlBlock,
  .cb_size = sizeof(ps2ControllerTaControlBlock),
  .stack_mem = &ps2ControllerTaBuffer[0],
  .stack_size = sizeof(ps2ControllerTaBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for oledDisplayQueue */
osMessageQueueId_t oledDisplayQueueHandle;
const osMessageQueueAttr_t oledDisplayQueue_attributes = {
  .name = "oledDisplayQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void chassisControlTask(void *argument);
extern void mpu6050Task(void *argument);
extern void oledDisplayTask(void *argument);
extern void ps2ControllerTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of oledDisplayQueue */
  oledDisplayQueueHandle = osMessageQueueNew (8, sizeof(OLED_Message_t), &oledDisplayQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of chassisControl */
  chassisControlHandle = osThreadNew(chassisControlTask, NULL, &chassisControl_attributes);

  /* creation of mpu6050 */
  mpu6050Handle = osThreadNew(mpu6050Task, NULL, &mpu6050_attributes);

  /* creation of oledDisplay */
  oledDisplayHandle = osThreadNew(oledDisplayTask, NULL, &oledDisplay_attributes);

  /* creation of ps2Controller */
  ps2ControllerHandle = osThreadNew(ps2ControllerTask, NULL, &ps2Controller_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_chassisControlTask */
/**
  * @brief  Function implementing the chassisControl thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_chassisControlTask */
__weak void chassisControlTask(void *argument)
{
  /* USER CODE BEGIN chassisControlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END chassisControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

