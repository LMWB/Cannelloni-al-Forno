/*
 * hardware_global.h
 *
 *  Created on: Nov 15, 2023
 *      Author: lukas
 *
 *
 *      This file is meant to be the connector between MCU Hardware and Application C-Code
 *
 *      Minimal Setup to get Smart Cube Gateway Light Software Running
 *
 *		Clock	- 72MHz
 *      UART2	- default Nucleo virtual com port (printf Terminal)
 *      		- DMA RX
 *      		- IRQ on
 *      UART1	- Wifi ESP32
 *      		- DMA RX
 *      		- IRQ on
 *
 *      UART4	- RS485 Tranceiver UART
 *
 *      CAN1	- CAN Tranceiver
 *
 *      SPI2	- EEPROM, ext. Flash, SD Card Interface (SD Card will move to SDIO Interface)
 *
 *      I2C3	- CO2-Gas, Temp, Humm Sensor (PA8, PB4)
 *
 *      RTC		- Real Time Clock
 *      TIM6	- simpleOS Heartbeat Timer
 *      		- PSC = 72-1
 *      TIM7	- simpleOS Microseconds Stop Watch
 *      		- PSC = 72-1
 *      		- ARR = TIM7-ARR (user constant=14650-1)
 *
 *      FatFS	- (eventuell brauchts das nicht)
 *
 *      GPIO A5	- default Nucleo User LED
 *      GPIO B1	- default Nucleo User Push Button
 *      GPIO A6 - SmartCube360 User LED
 *      GPIO C7 - SmartCube360 User Push Button
 *
 *      GPIO C6	- SPI2 Chip Select EEPROM
 *      GPIO C5 - SPI2 Chip Select ext Flash
 *
 */

#ifndef HARDWARE_GLOBAL_H
#define HARDWARE_GLOBAL_H

#include "main.h" // -> includes "stm32f4xx_hal.h" and all drivers
#include "gpio.h"
#include "usart.h"
#include "rtc.h"

/* *** Platform delay (polling) ***************************************************************** */
#define MAX_DELAY        HAL_MAX_DELAY
#define DELAY(ms)        HAL_Delay(ms)
#define GET_TICK()       HAL_GetTick()

/* *** Platform return codes ***************************************************************** */
#define DEVICE_STATUS_DEFINITION        HAL_StatusTypeDef
#define DEVICE_OK                       HAL_OK
#define DEVICE_ERROR                    HAL_ERROR
#define DEVICE_BUSY                     HAL_BUSY
#define DEVICE_TIMEOUT                  HAL_TIMEOUT

/* *** GPIO ***************************************************************** */
#define NUCLEO_LED1_ON()       HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET)
#define NUCLEO_LED1_OFF()      HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET)
#define NUCLEO_LED1_TOGGLE()   HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin)

#define LED1_ON() 			HAL_GPIO_WritePin(	USER_LED1_GPIO_Port, USER_LED1_Pin, GPIO_PIN_SET)
#define LED1_OFF() 			HAL_GPIO_WritePin(	USER_LED1_GPIO_Port, USER_LED1_Pin, GPIO_PIN_RESET)
#define LED1_TOGGLE()		HAL_GPIO_TogglePin(	USER_LED1_GPIO_Port, USER_LED1_Pin)

#define LED2_ON() 			HAL_GPIO_WritePin(	USER_LED2_GPIO_Port, USER_LED2_Pin, GPIO_PIN_SET)
#define LED2_OFF() 			HAL_GPIO_WritePin(	USER_LED2_GPIO_Port, USER_LED2_Pin, GPIO_PIN_RESET)
#define LED2_TOGGLE()		HAL_GPIO_TogglePin(	USER_LED2_GPIO_Port, USER_LED2_Pin)

#define LED3_ON() 			HAL_GPIO_WritePin(	USER_LED3_GPIO_Port, USER_LED3_Pin, GPIO_PIN_SET)
#define LED3_OFF() 			HAL_GPIO_WritePin(  USER_LED3_GPIO_Port, USER_LED3_Pin, GPIO_PIN_RESET)
#define LED3_TOGGLE()		HAL_GPIO_TogglePin(	USER_LED3_GPIO_Port, USER_LED3_Pin)

#define LED4_ON() 			HAL_GPIO_WritePin(	USER_LED4_GPIO_Port, USER_LED4_Pin, GPIO_PIN_SET)
#define LED4_OFF() 			HAL_GPIO_WritePin(  USER_LED4_GPIO_Port, USER_LED4_Pin, GPIO_PIN_RESET)
#define LED4_TOGGLE()		HAL_GPIO_TogglePin( USER_LED4_GPIO_Port, USER_LED4_Pin)

#define SW1         SW1_Pin
#define SW2         SW2_Pin
#define SW3         SW1_Pin
#define SW_NUCLEO   B1_Pin

#define PB1_READ()  HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)
#define PB2_READ()  HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin)
#define SW1_READ()  HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)

/* work around from gateway light */
#define BUTTON1         	SW1_Pin
#define BUTTON2         	SW2_Pin
#define BUTTON3         	SW1_Pin
#define BLUE_BUTTON_NUCLEO	SW1_Pin

/* *** UART ***************************************************************** */
/* Internet Module AT-Command Stuff */
#define UART_INTERNET_HANDLER 		        huart1
#define UART_INTERNET_INSTANCE		        USART1
#define UART_SEND_INTERNET(string, size)    HAL_UART_Transmit(&UART_INTERNET_HANDLER, string, size, -1)

#define CELLULAR_POWER_KEY_HIGH()           HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)
#define CELLULAR_POWER_KEY_LOW()            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET)
#define CELLULAR_STATUS()                   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)

#define WIFI_POWER_KEY_HIGH()               HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)
#define WIFI_POWER_KEY_LOW()                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET)
#define WIFI_STATUS()                       HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)

#define UART_TERMINAL_HANDLER 	            huart2
#define UART_TERMINAL_INSTANCE 	            USART2
#define UART_SEND_TERMINAL(string, size)    HAL_UART_Transmit(&UART_TERMINAL_HANDLER, string, size, -1)

#define UART_MODBUS_HANDLER                 huart4
#define USART_MODBUS_INSTANCE               UART4
#define MODBUS_TX_ENABLE()                  HAL_GPIO_WritePin(ModbusTXEnable_GPIO_Port, ModbusTXEnable_Pin, GPIO_PIN_SET);
#define MODBUS_TX_DISABLE()                 HAL_GPIO_WritePin(ModbusTXEnable_GPIO_Port, ModbusTXEnable_Pin, GPIO_PIN_RESET);

/* Generic UART Stuff */
#define UART_ACTIVATE_RX_INTERRUPT(uart_handler, pData, length)  HAL_UART_Receive_IT(&uart_handler, pData, length)
/* override __weak function prototype from stm32f4xx_hal_can.c */
#define UART_RX_INTERRUPT(huart)                                HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

/* *** I2C ********************************************************************/


/* *** SPI ********************************************************************/


/* *** Hardware Timer ********************************************************************/

/* *** Watch Dog ********************************************************************/

/* *** Real Time Clock ********************************************************************/
#define RTC_HANDLER             hrtc
#define RTC_INSTANCE            RTC    
#define SET_DEVICE_TIME(sTime)  HAL_RTC_SetTime( &RTC_HANDLER, sTime, RTC_FORMAT_BIN)
#define SET_DEVICE_DATE(sDate)  HAL_RTC_SetDate( &RTC_HANDLER, sDate, RTC_FORMAT_BIN)
#define GET_DEVICE_TIME(sTime)  HAL_RTC_GetTime( &RTC_HANDLER, sTime, RTC_FORMAT_BIN)
#define GET_DEVICE_DATE(sDate)  HAL_RTC_GetDate( &RTC_HANDLER, sDate, RTC_FORMAT_BIN)

/* *** CAN ********************************************************************/
#define CAN1_INSTANCE 				CAN1    // hardware instance designed by STM32 (Registers inside the microcontroller)
#define CAN1_HANDLER 				hcan1   // handler defined by STM32 HAL (code structure / object, referencing the hardware instance)

#define CAN1_START()				HAL_CAN_Start(&CAN1_HANDLER)
#define CAN1_STOP()					HAL_CAN_Stop(&CAN1_HANDLER)

#define CAN_ACTIVATE_INTERRUPTS(type) 								HAL_CAN_ActivateNotification( &CAN1_HANDLER, type)
#define CAN_GET_MESSAGE(rx_header, rx_data)     					HAL_CAN_GetRxMessage( &CAN1_HANDLER, CAN_RX_FIFO0, rx_header, rx_data)
#define CAN_CONFIGURE_MESSAGE_FILTER(filter_config) 				HAL_CAN_ConfigFilter( &CAN1_HANDLER, filter_config )
#define CAN_ADD_MESSAGE_TO_TRANSMIT(tx_header, tx_data, tx_mailbox)	HAL_CAN_AddTxMessage( &CAN1_HANDLER, tx_header, tx_data, tx_mailbox );

/* override __weak function prototype from stm32f4xx_hal_can.c */
#define CAN_RECEIVE_MESSAGE_INTERRUPT(hcan)			HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) 
#define CAN_FIFO_FULL_INTERRUPT(hcan)   			HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
#define CAN_ERROR_INTERRUPT(hcan)       			HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)

/* *** Internal FLASH ********************************************************************/
#define UNLOCK_FLASH                HAL_FLASH_Unlock
#define LOCK_FLASH                  HAL_FLASH_Lock
#define CLEAR_FLASH_FLAG            __HAL_FLASH_CLEAR_FLAG
#define FLASH_ERASE                  HAL_FLASHEx_Erase
#define FLASH_PROGRAM                HAL_FLASH_Program

/* *** Else ********************************************************************/
#define GET_RCC_FLAG                    __HAL_RCC_GET_FLAG

#endif /* HARDWARE_GLOBAL_H */
