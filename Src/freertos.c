/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "main.h"
#include "stm32f3xx_hal.h"
#include "cmsis_os.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId readTaskHandle;
osThreadId servoTaskHandle;
osMessageQId QueueTHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartReadTask(void const * argument);
void StartServoTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

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

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of readTask */
  osThreadDef(readTask, StartReadTask, osPriorityNormal, 0, 256);
  readTaskHandle = osThreadCreate(osThread(readTask), NULL);

  /* definition and creation of servoTask */
  osThreadDef(servoTask, StartServoTask, osPriorityNormal, 0, 256);
  servoTaskHandle = osThreadCreate(osThread(servoTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of QueueT */
  osMessageQDef(QueueT, 64, uint32_t);
  QueueTHandle = osMessageCreate(osMessageQ(QueueT), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	uint32_t ostatni = 0;
	ADC_InitTypeDef adc;
	  /* Infinite loop */
	uint32_t licz=0 ;
	uint32_t l=0;
	for(;;){
		licz++;

		HAL_ADC_Start(&hadc1);
		        if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK)
		        {
		           l =  HAL_ADC_GetValue(&hadc1);
		                       // sprintf(str, "%d", ADCValue);

		        }
		        HAL_ADC_Stop(&hadc1);
		//uint32_t val = HAL_ADC_GetValue(&hadc1);
		//uint32_t val4=	HAL_ADC_GetValue(&ADC_CHANNEL_1);
		//uint32_t val2 = ADC_Get();
		// uint16_t adc = adc_read(ADC_CHANNEL_1);
		//uint16_t adc =	ADC_GetConversionValue(ADC1);
		//char str[30]={0};
		//char str2[30]={0};
		//  snprintf(str,26,"Licz=%8lu\n   ",val);
				//  HAL_UART_Transmit(&huart2,(uint8_t*)str,14,100);
				 // snprintf(str2,26,"Licz=%8lu\n   ",val2);
				 			//	  HAL_UART_Transmit(&huart2,(uint8_t*)str2,14,100);
		if(licz != ostatni){
		  osMessagePut(QueueTHandle, l, 500);
		}
	    osDelay(20);
	    ostatni = licz;
	  }
  /* USER CODE END StartDefaultTask */
}

/* StartReadTask function */
void StartReadTask(void const * argument)
{
   //USER CODE BEGIN StartReadTask
	GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
		gpio.Pin = GPIO_PIN_5; // konfigurujemy pin 5
		gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
		gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
		gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
		HAL_GPIO_Init(GPIOA, &gpio); // inicjalizacja modu³u GPIOA
  /* Infinite loop */
  for(;;)
  {
	// HAL_UART_Transmit(&huart2, 'e', 1, 1000);
	  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // zapalenie diody
	  		HAL_Delay(100);
	  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // zgaszenie diody
	  		HAL_Delay(100);
  }
  /* USER CODE END StartReadTask */
}

/* StartServoTask function */
void StartServoTask(void const * argument)
{
  /* USER CODE BEGIN StartServoTask */
  /* Infinite loop */
	  char str[30]={0};
	  osEvent event;
	  /* Infinite loop */
	  for(;;){
		event = osMessageGet(QueueTHandle, 1000);
		//HAL_UART_Transmit(&huart1,(uint8_t*)"OK!\n",4,100);
		if(event.status != osEventTimeout){
		  snprintf(str,26,"Licz=%8lu\n   ",event.value.v);
		  HAL_UART_Transmit(&huart2,(uint8_t*)str,14,100);
		}
	    osDelay(10);
	  }
  /* USER CODE END StartServoTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
