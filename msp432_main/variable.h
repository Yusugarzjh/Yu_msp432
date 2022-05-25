/*
 * variable.h
 *
 *  Created on: 2022Äê5ÔÂ23ÈÕ
 *      Author: Yu-sugar
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "uart_driver.h"
// BMI160/BMM150

//Calibration off-sets
int8_t accel_off_x;
int8_t accel_off_y;
int8_t accel_off_z;
s16 gyro_off_x;
s16 gyro_off_y;
s16 gyro_off_z;
uint16_t gyroAbsX, gyroAbsY, gyroAbsZ;
uint16_t deltaAccelX, deltaAccelY, deltaAccelZ;
int16_t prevAccelX = 0;
int16_t prevAccelY = 0;
int16_t prevAccelZ = 0;
int16_t prevGyroX = 0;
int16_t prevGyroY = 0;
int16_t prevGyroZ = 0;
int16_t stillCount = 0;
int32_t gyroAvgX = 0.0;
int32_t gyroAvgY = 0.0;
int32_t gyroAvgZ = 0.0;
int32_t accelAvgX = 0.0;
int32_t accelAvgY = 0.0;
int32_t accelAvgZ = 0.0;

#define LCD_SCL_PORT GPIO_PORT_P4
#define LCD_SCL_PIN  GPIO_PIN1
#define LCD_SDA_PORT GPIO_PORT_P4
#define LCD_SDA_PIN  GPIO_PIN2
#define LCD_RST_PORT GPIO_PORT_P4
#define LCD_RST_PIN  GPIO_PIN3
#define LCD_DC_PORT  GPIO_PORT_P4
#define LCD_DC_PIN   GPIO_PIN4
#define LCD_CS_PORT  GPIO_PORT_P1
#define LCD_CS_PIN   GPIO_PIN5
#define LCD_SCL(x)  ((x)?(GPIO_setOutputHighOnPin(LCD_SCL_PORT,LCD_SCL_PIN)):(GPIO_setOutputLowOnPin(LCD_SCL_PORT,LCD_SCL_PIN)))
#define LCD_SDA(x)  ((x)?(GPIO_setOutputHighOnPin(LCD_SDA_PORT,LCD_SDA_PIN)):(GPIO_setOutputLowOnPin(LCD_SDA_PORT,LCD_SDA_PIN)))
#define LCD_RST(x)  ((x)?(GPIO_setOutputHighOnPin(LCD_RST_PORT,LCD_RST_PIN)):(GPIO_setOutputLowOnPin(LCD_RST_PORT,LCD_RST_PIN)))
#define LCD_DC(x)   ((x)?(GPIO_setOutputHighOnPin(LCD_DC_PORT,LCD_DC_PIN)):(GPIO_setOutputLowOnPin(LCD_DC_PORT,LCD_DC_PIN)))
#define LCD_CS(x)   ((x)?(GPIO_setOutputHighOnPin(LCD_CS_PORT,LCD_CS_PIN)):(GPIO_setOutputLowOnPin(LCD_CS_PORT,LCD_CS_PIN )))

bool BMI_on = true;


#endif /* VARIABLE_H_ */
