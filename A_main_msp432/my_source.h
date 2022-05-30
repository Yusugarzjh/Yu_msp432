/*
 * variable.h
 *
 *  Created on: 2022年5月23日
 *      Author: Yu-sugar
 */

#ifndef MY_SOURCE_H_
#define MY_SOURCE_H_

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
#define  TEST_GAME
bool BMI_on = true;
int i_axle,j_axle;
int mymap[8][8]=           //定义一个8x8的地图，用数组表示各单元格元素
{
 {1,1,1,1,1,1,1,1},       //0 blank
{1,0,0,0,1,0,0,1},       //1 wall
{1,0,1,0,1,4,3,1},       //3 destination
{1,0,0,0,0,4,3,1},       //4 box
{1,0,1,0,1,4,3,1},       //5 person
{1,0,0,0,1,0,0,1},       //7 box+destination
{1,1,1,1,1,5,0,1},       //8 person+destination
{0,0,0,0,1,1,1,1}

  };
int PersonX=6,PersonY=5;
void paint()             //绘制初始地图
{
 for( i_axle=0;i_axle<8;i_axle++)
   { for( j_axle=0;j_axle<8;j_axle++)
       {
           switch(mymap[i_axle][j_axle])
           {
             #ifdef  TEST_GAME
             case 1:UART_transmitData(EUSCI_A0_BASE, 0xA8),UART_transmitData(EUSCI_A0_BASE,0x80);break;
             case 3:UART_transmitData(EUSCI_A0_BASE, 0xA1),UART_transmitData(EUSCI_A0_BASE,0xEF);break;
             case 4:UART_transmitData(EUSCI_A0_BASE, 0xA1),UART_transmitData(EUSCI_A0_BASE,0xF5);break;
             case 5:UART_transmitData(EUSCI_A0_BASE, 0xA1),UART_transmitData(EUSCI_A0_BASE,0xE2);break;
             case 7:UART_transmitData(EUSCI_A0_BASE, 0xA1),UART_transmitData(EUSCI_A0_BASE,0xF5);break;
             case 8:UART_transmitData(EUSCI_A0_BASE, 0xA1),UART_transmitData(EUSCI_A0_BASE,0xE2);break;
             default:UART_transmitData(EUSCI_A0_BASE, ' '),UART_transmitData(EUSCI_A0_BASE,' ');break;
             #else
             case 1:UART_transmitData(EUSCI_A2_BASE, 0xA8),UART_transmitData(EUSCI_A2_BASE,0x80);break;
             case 3:UART_transmitData(EUSCI_A2_BASE, 0xA1),UART_transmitData(EUSCI_A2_BASE,0xEF);break;
             case 4:UART_transmitData(EUSCI_A2_BASE, 0xA1),UART_transmitData(EUSCI_A2_BASE,0xF5);break;
             case 5:UART_transmitData(EUSCI_A2_BASE, 0xA1),UART_transmitData(EUSCI_A2_BASE,0xE2);break;
             case 7:UART_transmitData(EUSCI_A2_BASE, 0xA1),UART_transmitData(EUSCI_A2_BASE,0xF5);break;
             case 8:UART_transmitData(EUSCI_A2_BASE, 0xA1),UART_transmitData(EUSCI_A2_BASE,0xE2);break;
             default:UART_transmitData(EUSCI_A2_BASE, ' '),UART_transmitData(EUSCI_A2_BASE,' ');break;
             #endif
             }
         }
            #ifdef  TEST_GAME
                  UART_transmitData(EUSCI_A0_BASE, '\r');
                  UART_transmitData(EUSCI_A0_BASE,'\n');
            #else
                  UART_transmitData(EUSCI_A2_BASE, 0x0D);
                  UART_transmitData(EUSCI_A2_BASE,0x0A);
            #endif
     }
 };
void play()
{

            delay(500);
            MAP_ADC14_toggleConversionTrigger();//控制小人移动速度
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)
              {
                if(mymap[PersonX][PersonY+1]==0||mymap[PersonX][PersonY+1]==3)
                  {
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX][PersonY+1]+=5;
                    PersonY++;
                    }
             else if((mymap[PersonX][PersonY+1]==4||mymap[PersonX][PersonY+1]==7)&&(mymap[PersonX][PersonY+2]==0||mymap[PersonX][PersonY+2]==3))
                       {
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX][PersonY+1]+=1;
                      mymap[PersonX][PersonY+2]+=4;
                       PersonY++;}
                paint();
                }
             else if(curADCResultX<1000)
             {
                    if(mymap[PersonX][PersonY-1]==0||mymap[PersonX][PersonY-1]==3)
                  {
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX][PersonY-1]+=5;
                    PersonY--;
                    }
                else if((mymap[PersonX][PersonY-1]==4||mymap[PersonX][PersonY-1]==7)&&(mymap[PersonX][PersonY-2]==0||mymap[PersonX][PersonY-2]==3))
                       {
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX][PersonY-1]+=1;
                      mymap[PersonX][PersonY-2]+=4;
                      PersonY--;
                     }
                    paint();
              }
            else if(curADCResultY>15000)
            {
                if(mymap[PersonX+1][PersonY]==0||mymap[PersonX+1][PersonY]==3)
                  {
                    mymap[PersonX][PersonY]-=5;
                  mymap[PersonX+1][PersonY]+=5;
                    PersonX++;
                    }
                     else if((mymap[PersonX+1][PersonY]==4||mymap[PersonX+1][PersonY]==7)&&(mymap[PersonX+2][PersonY]==0||mymap[PersonX+2][PersonY]==3))
                       {
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX+1][PersonY]+=1;
                      mymap[PersonX+2][PersonY]+=4;
                       PersonX++;
                       }
                paint();
              }
          else if (curADCResultY<1000)
            {
               if(mymap[PersonX-1][PersonY]==0||mymap[PersonX-1][PersonY]==3)
                  {
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX-1][PersonY]+=5;
                    PersonX--;
                    }
               else if((mymap[PersonX-1][PersonY]==4||mymap[PersonX-1][PersonY]==7)&&(mymap[PersonX-2][PersonY]==0||mymap[PersonX-2][PersonY]==3))
                   {
                     mymap[PersonX][PersonY]-=5;
                     mymap[PersonX-1][PersonY]+=1;
                     mymap[PersonX-2][PersonY]+=4;
                     PersonX--;
                    }
               paint();
              }
};

#endif /* MY_SOURCE_H_ */
