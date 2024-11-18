/*
 * hardware_global.h
 *
 *  Created on: Nov 15, 2023
 *      Author: lukas
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

#define SW_NUCLEO   B1_Pin

#define LD293D_ENABLE_OUT1_OUT2()	HAL_GPIO_WritePin(EN1_2_GPIO_Port, EN1_2_Pin, 1)
#define LD293D_ENABLE_OUT3_OUT4()	HAL_GPIO_WritePin(EN3_4_GPIO_Port, EN3_4_Pin, 1)
#define LD293D_DISABLE_OUT1_OUT2()	HAL_GPIO_WritePin(EN1_2_GPIO_Port, EN1_2_Pin, 0)
#define LD293D_DISABLE_OUT3_OUT4()	HAL_GPIO_WritePin(EN3_4_GPIO_Port, EN3_4_Pin, 0)

#define LD293D_TURN_ON_OUT1()	HAL_GPIO_WritePin(Out1_GPIO_Port, Out1_Pin, 1)
#define LD293D_TURN_OFF_OUT1()	HAL_GPIO_WritePin(Out1_GPIO_Port, Out1_Pin, 0)
#define LD293D_TURN_ON_OUT2()	HAL_GPIO_WritePin(Out2_GPIO_Port, Out2_Pin, 1)
#define LD293D_TURN_OFF_OUT2()	HAL_GPIO_WritePin(Out2_GPIO_Port, Out2_Pin, 0)
#define LD293D_TURN_ON_OUT3()	HAL_GPIO_WritePin(Out3_GPIO_Port, Out3_Pin, 1)
#define LD293D_TURN_OFF_OUT3()	HAL_GPIO_WritePin(Out3_GPIO_Port, Out3_Pin, 0)
#define LD293D_TURN_ON_OUT4()	HAL_GPIO_WritePin(Out4_GPIO_Port, Out4_Pin, 1)
#define LD293D_TURN_OFF_OUT4()	HAL_GPIO_WritePin(Out4_GPIO_Port, Out4_Pin, 0)

#define LD293D_TOGGLE_OUT1() 	HAL_GPIO_TogglePin(Out1_GPIO_Port, Out1_Pin)
#define LD293D_TOGGLE_OUT2() 	HAL_GPIO_TogglePin(Out2_GPIO_Port, Out2_Pin)
#define LD293D_TOGGLE_OUT3() 	HAL_GPIO_TogglePin(Out3_GPIO_Port, Out3_Pin)
#define LD293D_TOGGLE_OUT4() 	HAL_GPIO_TogglePin(Out4_GPIO_Port, Out4_Pin)

/* *** UART ***************************************************************** */
#define UART_TERMINAL_HANDLER 	            huart2
#define UART_TERMINAL_INSTANCE 	            USART2
#define UART_SEND_TERMINAL(string, size)    HAL_UART_Transmit(&UART_TERMINAL_HANDLER, string, size, -1)

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


#endif /* HARDWARE_GLOBAL_H */
