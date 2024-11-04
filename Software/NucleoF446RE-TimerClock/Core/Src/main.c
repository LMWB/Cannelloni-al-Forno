/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
 *
 *
 *
 *
 *
 *
 *
 *
 * Timerclock with STM32 native RTC and very basic scheduler library
 *
 * Set RTC with python based command line tool 'timerclock.py'
 * 'python timerclock.py /dev/ttyACM0'
 *
 *
 *
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <simpleScheduler.h>
#include "main.h"
#include "dma.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "utils.h"
#include "timeProcessing.h"
#include "recirculationTimerClock.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define UART_DMA_BUFFER_SIZE 128
uint8_t uart_dma_buffer[UART_DMA_BUFFER_SIZE];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

	set_recirculation_number_of_timeslots_active(3);

	/* from 5:00 until 9:00 */
	set_recirculation_start(TIMER_SLOTS_1, 5*60);
	set_recirculation_end(	TIMER_SLOTS_1, 9*60);

	/* from 14:26 until 15:00 */
	set_recirculation_start(TIMER_SLOTS_2, 14*60 + 26);
	set_recirculation_end(	TIMER_SLOTS_2, 15*60);

	/* from 16:00 until 23:00 */
	set_recirculation_start(TIMER_SLOTS_3, 16*60);
	set_recirculation_end(	TIMER_SLOTS_3, 23*60);

//	char timestampe_string[26];
//	struct tm timedate = { 0 };
//	convert_compiler_timestamp_to_asctime(__TIME__, __DATE__, timestampe_string);
//	cvt_asctime(timestampe_string, &timedate);
//	myprintf("Start Demo Timer Clock\n");
//	myprintf("Current Date-Time: %s", timestampe_string);

	/* set RTC on target MCU */
//	(void) change_controller_time(&timedate);
//	myprintf("RTC set!\n");

	HAL_Delay(200);
	struct tm *curren_Date_Time = get_gmtime_stm32();
	print_current_time(curren_Date_Time);


	/* activate UART DMA */
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart_dma_buffer, UART_DMA_BUFFER_SIZE);

	void test_callback(void){
		myprintf("Hello World Task 1\n");
	}

	void test_callback2(void){
		myprintf("\tHello World Task 2\n");
	}

	void test_callback3(void){
		run_recirculation_timer_clock();
	}

	simple_task_t test_task = {.delay = eDELAY_1s, .task_callback = test_callback};
	add_task_to_scheduler(&test_task);
	simple_task_t test_task2 = {.delay = eDELAY_5s, .task_callback = test_callback2};
	add_task_to_scheduler(&test_task2);
	simple_task_t test_task3 = {.delay = eDELAY_10s, .task_callback = test_callback3};
	add_task_to_scheduler(&test_task3);
	run_schedular();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == B1_Pin) {
		NUCLEO_LED1_TOGGLE();
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if (huart->Instance == USART2){

		char timestampe_string[26];
		struct tm timedate = { 0 };
		char *time = (char*) &uart_dma_buffer[0];
		char *date = (char*) &uart_dma_buffer[8];
		convert_compiler_timestamp_to_asctime(time, date, timestampe_string);
		cvt_asctime(timestampe_string, &timedate);
		(void) change_controller_time(&timedate);

		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart_dma_buffer, UART_DMA_BUFFER_SIZE);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
