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
#define  OLED_GAME
//#define  NORMAL_MAP
#define  TEST_MAP
#define  MAPX_MAX 4
#define  MAPY_MAX 7
bool BMI_on = true;
int i_axle,j_axle;
int GameRecord=0;//保存的游戏记录
int mymap[MAPX_MAX][MAPY_MAX]=           //定义一个8x8的地图，用数组表示各单元格元素
{
#ifdef NORMAL_MAP
 {3,0,0,0,0,1,1},       //0 blank
 {3,1,0,4,4,0,1},       // 1 wall
 {3,3,4,4,0,0,1},       // 3 virus
 {0,3,0,0,4,5,1},       //4vaccine
#else
  {5,4,3,0,0,0,1},//5 person
  {0,0,0,0,0,0,1},//7virus+vaccine
  {0,0,0,0,0,0,1},//8person+vaccine
  {0,0,0,0,0,0,1},

#endif
  };
int PersonX,PersonY,stick_x=68,stick_y=32;
int BarX=114,BarY=64,GameState=0,TimerInterval=1;//进度条相关
void Progress_Bar()
{
            TimerInterval++;
            if(TimerInterval%5==0)
            {
                UART_transmitData(EUSCI_A0_BASE, BarY--);
                LCD_draw_line(BarX,BarY,BarX+16,BarY,GREEN);
            }
}
//void SysTick_Handler(void)
//{
//    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
//
//    if(GameState==1)
//    {
//        UART_transmitData(EUSCI_A0_BASE, BarY--);
//        LCD_draw_line(BarX,BarY,BarX+16,BarY,GREEN);
//    }
//
//}
void paint()             //绘制初始地图
{
 for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
   { for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
       {
           switch(mymap[i_axle][j_axle])
           {
             #ifdef   OLED_GAME
             case 1:LCD_P16X16Str(j_axle,i_axle, "墙", BLUE, BLACK);break;
             case 3:LCD_P16X16Str(j_axle,i_axle, "病", RED, BLACK);break;
             case 4:LCD_P16X16Str(j_axle,i_axle, "囊", CYAN, BLACK);break;
             case 5:LCD_P16X16Str(j_axle,i_axle, "人", WHITE, BLACK);break;
             case 7:LCD_P16X16Str(j_axle,i_axle, "囊", GREEN, BLACK);break;
             case 8:LCD_P16X16Str(j_axle,i_axle, "人", WHITE, BLACK);break;
             #endif
             }
         }
     }
 };
void FreezeScreen()
{for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
  { for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
      {
          switch(mymap[i_axle][j_axle])
          {
            #ifdef   OLED_GAME
            case 1:LCD_P16X16Str(j_axle,i_axle, "墙", BLUE, BLACK);break;
            case 3:LCD_P16X16Str(j_axle,i_axle, "病", CYAN, BLACK);break;
            case 4:LCD_P16X16Str(j_axle,i_axle, "囊", CYAN, BLACK);break;
            case 5:LCD_P16X16Str(j_axle,i_axle, "人", WHITE, BLACK);break;
            case 7:LCD_P16X16Str(j_axle,i_axle, "囊", GREEN, BLACK);break;
            case 8:LCD_P16X16Str(j_axle,i_axle, "人", WHITE, BLACK);break;
            #endif
            }
        }
    }
}

void ClsCell(i_axle_rm,j_axle_rm)
{
        switch(mymap[i_axle_rm][j_axle_rm])
    {
      #ifdef   OLED_GAME
        case 1:LCD_P16X16Str(j_axle_rm,i_axle_rm, "墙", BLACK, BLACK);break;
        case 3:LCD_P16X16Str(j_axle_rm,i_axle_rm, "病", BLACK, BLACK);break;
        case 4:LCD_P16X16Str(j_axle_rm,i_axle_rm, "囊", BLACK, BLACK);break;
        case 5:LCD_P16X16Str(j_axle_rm,i_axle_rm, "人", BLACK, BLACK);break;
        case 7:LCD_P16X16Str(j_axle_rm,i_axle_rm, "囊", BLACK, BLACK);break;
        case 8:LCD_P16X16Str(j_axle_rm,i_axle_rm, "人", BLACK, BLACK);break;
      #endif
      }
}
void PrfCell(i_axle_Pf,j_axle_Pf)
{
        switch(mymap[i_axle_Pf][j_axle_Pf])
    {
      #ifdef   OLED_GAME
        case 1:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "墙", BLUE, BLACK);break;
        case 3:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "病", RED, BLACK);break;
        case 4:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "囊", CYAN, BLACK);break;
        case 5:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "人", WHITE, BLACK);break;
        case 7:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "囊", GREEN, BLACK);break;
        case 8:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "人", WHITE, BLACK);break;
      #endif
      }
}
void Guidance()
{
    LCD_CLS();
//    LCD_P6X8Str(1, 0, "Press s2 to freeze ", CYAN, BLACK);//显示英文6*8字符串
//    LCD_P6X8Str(1, 1, "full screen", CYAN, BLACK);//显示英文6*8字符串
    LCD_P6X8Str(1, 2, "The progress bar on ", CYAN, BLACK);//显示英文6*8字符串
    LCD_P6X8Str(1, 3, "the right is over,", CYAN, BLACK);//显示英文6*8字符串
    LCD_P6X8Str(1, 4, "the game is over", CYAN, BLACK);//显示英文6*8字符串
}
void play1()
{
//    Guidance();//游戏说明书
    delay(1000);
    BarY=64;//初始化进度条
    //change map
    int mymap_new[MAPX_MAX][MAPY_MAX]=
    {
     {3,0,0,0,0,1,1},       //0 blank
     {3,1,0,4,4,0,1},       // 1 wall
     {3,3,4,4,0,0,1},       // 3 virus
     {0,3,0,0,4,5,1},       //4vaccine
      };
        for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
            {for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
                {
                    mymap[i_axle][j_axle]= mymap_new[i_axle][j_axle];
                }
              }
    //change map
    LCD_CLS();
    paint();
    PersonX=3;
    PersonY=5;//初始位置
    GameState=1;
        while(mymap[0][0]!=7||mymap[1][0]!=7||mymap[2][0]!=7||mymap[2][1]!=7||mymap[3][1]!=7)
        {
            if(BarY<=0)
            {
                GameState=0;
                break;
            }
            delay(500);
            Progress_Bar();
            MAP_ADC14_toggleConversionTrigger();//控制小人移动速度
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)
              {
                if(mymap[PersonX][PersonY+1]==0||mymap[PersonX][PersonY+1]==3)
                  {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX,PersonY+1);
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX][PersonY+1]+=5;
                    PrfCell(PersonX,PersonY);
                    PrfCell(PersonX,PersonY+1);
                    PersonY++;
                    }
             else if((mymap[PersonX][PersonY+1]==4||mymap[PersonX][PersonY+1]==7)&&(mymap[PersonX][PersonY+2]==0||mymap[PersonX][PersonY+2]==3))
                       {
                     ClsCell(PersonX,PersonY);
                     ClsCell(PersonX,PersonY+1);
                     ClsCell(PersonX,PersonY+2);
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX][PersonY+1]+=1;
                      mymap[PersonX][PersonY+2]+=4;
                      PrfCell(PersonX,PersonY);
                      PrfCell(PersonX,PersonY+1);
                      PrfCell(PersonX,PersonY+2);
                       PersonY++;

                       }
//                paint();
                }
             else if(curADCResultX<1000)
             {
if(PersonY!=0)
                {

                    if(mymap[PersonX][PersonY-1]==0||mymap[PersonX][PersonY-1]==3)
                  {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX,PersonY-1);
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX][PersonY-1]+=5;
                    PrfCell(PersonX,PersonY);
                    PrfCell(PersonX,PersonY-1);
                    PersonY--;
                    }
                else if((mymap[PersonX][PersonY-1]==4||mymap[PersonX][PersonY-1]==7)&&(mymap[PersonX][PersonY-2]==0||mymap[PersonX][PersonY-2]==3))
                       {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX,PersonY-1);
                    ClsCell(PersonX,PersonY-2);
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX][PersonY-1]+=1;
                      mymap[PersonX][PersonY-2]+=4;
                      PrfCell(PersonX,PersonY);
                      PrfCell(PersonX,PersonY-1);
                      PrfCell(PersonX,PersonY-2);
                      PersonY--;
                     }
                }
             }
//                    paint();

            else if(curADCResultY>15000)
            {
                if(mymap[PersonX+1][PersonY]==0||mymap[PersonX+1][PersonY]==3)
                  {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX+1,PersonY);
                    mymap[PersonX][PersonY]-=5;
                  mymap[PersonX+1][PersonY]+=5;
                  PrfCell(PersonX,PersonY);
                  PrfCell(PersonX+1,PersonY);
                    PersonX++;
                    }
                     else if((mymap[PersonX+1][PersonY]==4||mymap[PersonX+1][PersonY]==7)&&(mymap[PersonX+2][PersonY]==0||mymap[PersonX+2][PersonY]==3))
                       {
                         ClsCell(PersonX,PersonY);
                         ClsCell(PersonX+1,PersonY);
                         ClsCell(PersonX+2,PersonY);
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX+1][PersonY]+=1;
                      mymap[PersonX+2][PersonY]+=4;
                      PrfCell(PersonX,PersonY);
                      PrfCell(PersonX+1,PersonY);
                      PrfCell(PersonX+2,PersonY);
                       PersonX++;
                       }
//                paint();
              }
          else if (curADCResultY<1000)
            {
              if(PersonX==2||PersonX==3||(PersonX==1&&(mymap[0][PersonY]==0||mymap[0][PersonY]==3)))
              {
               if(mymap[PersonX-1][PersonY]==0||mymap[PersonX-1][PersonY]==3)
                  {
                   ClsCell(PersonX,PersonY);
                   ClsCell(PersonX-1,PersonY);
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX-1][PersonY]+=5;
                    PrfCell(PersonX,PersonY);
                    PrfCell(PersonX-1,PersonY);
                    PersonX--;
                    }
               else if((mymap[PersonX-1][PersonY]==4||mymap[PersonX-1][PersonY]==7)&&(mymap[PersonX-2][PersonY]==0||mymap[PersonX-2][PersonY]==3))
                   {
                   ClsCell(PersonX,PersonY);
                   ClsCell(PersonX-1,PersonY);
                   ClsCell(PersonX-2,PersonY);
                     mymap[PersonX][PersonY]-=5;
                     mymap[PersonX-1][PersonY]+=1;
                     mymap[PersonX-2][PersonY]+=4;
                     PrfCell(PersonX,PersonY);
                     PrfCell(PersonX-1,PersonY);
                     PrfCell(PersonX-2,PersonY);
                     PersonX--;
                    }
//               paint();
              }
              }
        }
}
void play()
{
    Guidance();//游戏说明书
    BarY=64;//初始化进度条
    delay(1000);
    LCD_CLS();
    paint();
    PersonX=0;
    PersonY=0;//初始位置
    GameState=1;
        while(mymap[0][2]!=7)
        {
            if(BarY<=0)
            {
                GameState=0;
                break;
            }
            delay(500);
            Progress_Bar();
            MAP_ADC14_toggleConversionTrigger();//控制小人移动速度
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)
              {
                if(mymap[PersonX][PersonY+1]==0||mymap[PersonX][PersonY+1]==3)
                  {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX,PersonY+1);
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX][PersonY+1]+=5;
                    PrfCell(PersonX,PersonY);
                    PrfCell(PersonX,PersonY+1);
                    PersonY++;
                    }
             else if((mymap[PersonX][PersonY+1]==4||mymap[PersonX][PersonY+1]==7)&&(mymap[PersonX][PersonY+2]==0||mymap[PersonX][PersonY+2]==3))
                       {
                     ClsCell(PersonX,PersonY);
                     ClsCell(PersonX,PersonY+1);
                     ClsCell(PersonX,PersonY+2);
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX][PersonY+1]+=1;
                      mymap[PersonX][PersonY+2]+=4;
                      PrfCell(PersonX,PersonY);
                      PrfCell(PersonX,PersonY+1);
                      PrfCell(PersonX,PersonY+2);
                       PersonY++;

                       }
//                paint();
                }
             else if(curADCResultX<1000)
             {
if(PersonY!=0)
                {

                    if(mymap[PersonX][PersonY-1]==0||mymap[PersonX][PersonY-1]==3)
                  {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX,PersonY-1);
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX][PersonY-1]+=5;
                    PrfCell(PersonX,PersonY);
                    PrfCell(PersonX,PersonY-1);
                    PersonY--;
                    }
                else if((mymap[PersonX][PersonY-1]==4||mymap[PersonX][PersonY-1]==7)&&(mymap[PersonX][PersonY-2]==0||mymap[PersonX][PersonY-2]==3))
                       {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX,PersonY-1);
                    ClsCell(PersonX,PersonY-2);
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX][PersonY-1]+=1;
                      mymap[PersonX][PersonY-2]+=4;
                      PrfCell(PersonX,PersonY);
                      PrfCell(PersonX,PersonY-1);
                      PrfCell(PersonX,PersonY-2);
                      PersonY--;
                     }
                }
             }
//                    paint();

            else if(curADCResultY>15000)
            {
                if(mymap[PersonX+1][PersonY]==0||mymap[PersonX+1][PersonY]==3)
                  {
                    ClsCell(PersonX,PersonY);
                    ClsCell(PersonX+1,PersonY);
                    mymap[PersonX][PersonY]-=5;
                  mymap[PersonX+1][PersonY]+=5;
                  PrfCell(PersonX,PersonY);
                  PrfCell(PersonX+1,PersonY);
                    PersonX++;
                    }
                     else if((mymap[PersonX+1][PersonY]==4||mymap[PersonX+1][PersonY]==7)&&(mymap[PersonX+2][PersonY]==0||mymap[PersonX+2][PersonY]==3))
                       {
                         ClsCell(PersonX,PersonY);
                         ClsCell(PersonX+1,PersonY);
                         ClsCell(PersonX+2,PersonY);
                      mymap[PersonX][PersonY]-=5;
                      mymap[PersonX+1][PersonY]+=1;
                      mymap[PersonX+2][PersonY]+=4;
                      PrfCell(PersonX,PersonY);
                      PrfCell(PersonX+1,PersonY);
                      PrfCell(PersonX+2,PersonY);
                       PersonX++;
                       }
//                paint();
              }
          else if (curADCResultY<1000)
            {
              if(PersonX==2||PersonX==3||(PersonX==1&&(mymap[0][PersonY]==0||mymap[0][PersonY]==3)))
              {
               if(mymap[PersonX-1][PersonY]==0||mymap[PersonX-1][PersonY]==3)
                  {
                   ClsCell(PersonX,PersonY);
                   ClsCell(PersonX-1,PersonY);
                    mymap[PersonX][PersonY]-=5;
                    mymap[PersonX-1][PersonY]+=5;
                    PrfCell(PersonX,PersonY);
                    PrfCell(PersonX-1,PersonY);
                    PersonX--;
                    }
               else if((mymap[PersonX-1][PersonY]==4||mymap[PersonX-1][PersonY]==7)&&(mymap[PersonX-2][PersonY]==0||mymap[PersonX-2][PersonY]==3))
                   {
                   ClsCell(PersonX,PersonY);
                   ClsCell(PersonX-1,PersonY);
                   ClsCell(PersonX-2,PersonY);
                     mymap[PersonX][PersonY]-=5;
                     mymap[PersonX-1][PersonY]+=1;
                     mymap[PersonX-2][PersonY]+=4;
                     PrfCell(PersonX,PersonY);
                     PrfCell(PersonX-1,PersonY);
                     PrfCell(PersonX-2,PersonY);
                     PersonX--;
                    }
//               paint();
              }
              }
        }
}
void Stick()
{
        LCD_draw_dot(stick_x,stick_y,RED);
        LCD_draw_dot(stick_x,stick_y+1,RED);
        LCD_draw_dot(stick_x,stick_y-1,RED);
        LCD_draw_dot(stick_x+1,stick_y,RED);
        LCD_draw_dot(stick_x-1,stick_y,RED);
            LCD_draw_dot(64,32,RED);
    while(1)
    {
            delay(0);
            MAP_ADC14_toggleConversionTrigger();//控制小人移动速度
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)
              {
                    stick_x++;
                    LCD_draw_dot(stick_x-1,stick_y,BLACK);
                    LCD_draw_dot(stick_x-1+1,stick_y,BLACK);
                    LCD_draw_dot(stick_x-1-1,stick_y,BLACK);
                    LCD_draw_dot(stick_x-1,stick_y-1,BLACK);
                    LCD_draw_dot(stick_x-1,stick_y+1,BLACK);
                    LCD_draw_dot(stick_x,stick_y,RED);
                    LCD_draw_dot(stick_x,stick_y+1,RED);
                    LCD_draw_dot(stick_x,stick_y-1,RED);
                    LCD_draw_dot(stick_x+1,stick_y,RED);
                    LCD_draw_dot(stick_x-1,stick_y,RED);

                }
             else if(curADCResultX<1000)
             {
                 stick_x--;
                 LCD_draw_dot(stick_x+1,stick_y,BLACK);
                 LCD_draw_dot(stick_x+1,stick_y+1,BLACK);
                 LCD_draw_dot(stick_x+1,stick_y-1,BLACK);
                 LCD_draw_dot(stick_x+1+1,stick_y,BLACK);
                 LCD_draw_dot(stick_x+1-1,stick_y,BLACK);
                 LCD_draw_dot(stick_x,stick_y,RED);
                 LCD_draw_dot(stick_x,stick_y+1,RED);
                 LCD_draw_dot(stick_x,stick_y-1,RED);
                 LCD_draw_dot(stick_x+1,stick_y,RED);
                 LCD_draw_dot(stick_x-1,stick_y,RED);
              }
            else if(curADCResultY>15000)
            {
                stick_y++;
                LCD_draw_dot(stick_x,stick_y-1,BLACK);
                LCD_draw_dot(stick_x,stick_y-1+1,BLACK);
                LCD_draw_dot(stick_x,stick_y-1-1,BLACK);
                LCD_draw_dot(stick_x+1,stick_y-1,BLACK);
                LCD_draw_dot(stick_x-1,stick_y-1,BLACK);
                LCD_draw_dot(stick_x,stick_y,RED);
                LCD_draw_dot(stick_x,stick_y+1,RED);
                LCD_draw_dot(stick_x,stick_y-1,RED);
                LCD_draw_dot(stick_x+1,stick_y,RED);
                LCD_draw_dot(stick_x-1,stick_y,RED);
              }
          else if (curADCResultY<1000)
            {
              stick_y--;
              LCD_draw_dot(stick_x,stick_y+1,BLACK);
              LCD_draw_dot(stick_x,stick_y+1+1,BLACK);
              LCD_draw_dot(stick_x,stick_y+1-1,BLACK);
              LCD_draw_dot(stick_x+1,stick_y+1,BLACK);
              LCD_draw_dot(stick_x-1,stick_y+1,BLACK);
              LCD_draw_dot(stick_x,stick_y,RED);
              LCD_draw_dot(stick_x,stick_y+1,RED);
              LCD_draw_dot(stick_x,stick_y-1,RED);
              LCD_draw_dot(stick_x+1,stick_y,RED);
              LCD_draw_dot(stick_x-1,stick_y,RED);
              }
    }
};

#endif /* MY_SOURCE_H_ */
