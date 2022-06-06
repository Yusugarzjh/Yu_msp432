/*
 * variable.h
 *
 *  Created on: 2022��5��23��
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
#include "opt3001.h"
BMI160_RETURN_FUNCTION_TYPE returnValue;
// BMI160结构体变量定义
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;
//传感器数据变量
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
// OPT3001
uint16_t rawData;
float     convertedLux;
#define  MAPX_MAX 4 //地图大小4x7
#define  MAPY_MAX 7
int i_axle,j_axle;
int GameRecord=0;//记录游戏关卡
int mymap[MAPX_MAX][MAPY_MAX]=   //初始化地图数组0空地、1墙体、3病毒、4疫苗胶囊、5医护人员、7病毒加疫苗胶囊、8人加病毒
{
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  };
int PersonX,PersonY,stick_x=68,stick_y=32;
int BarX=114,BarY=64;//进度条位置初始化
int TimerInterval=1;//进度条控制相关变量
int GameState=0;//记录游戏结束时的状态
void Progress_Bar()//游戏进度条
{
            //获取当前光照强度
            sensorOpt3001Read(&rawData);
            sensorOpt3001Convert(rawData, &convertedLux);
            int BarSpeed;     //控制进度条速度变量
            snprintf(test.txString, 60,
                    "{\"light\":{\"lux\":%d}}\r\n",
                    abs(convertedLux));
            sendTextPc(); //发送光照强度至串口
            sendTextEsp();//发送光照强度至云端
            if(abs(convertedLux)<300)//光照强度低速度较慢
                BarSpeed=10;
            else if(abs(convertedLux)<1000)
                BarSpeed=5;
            else                    //光照强度高速度较快
                BarSpeed=1;
            TimerInterval++;
            if(TimerInterval%BarSpeed==0)
            {
                BarY-=2;
                LCD_draw_line(BarX,BarY,BarX+16,BarY,GREEN);//绘制进度条
            }
}
void paint()             //绘制游戏地图
{
 for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
   { for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
       {
           switch(mymap[i_axle][j_axle])
           {

             case 1:LCD_P16X16Str(j_axle,i_axle, "墙", BLUE, PINK);break;
             case 3:LCD_P16X16Str(j_axle,i_axle, "病", RED, PINK);break;
             case 4:LCD_P16X16Str(j_axle,i_axle, "囊", CYAN, PINK);break;
             case 5:LCD_P16X16Str(j_axle,i_axle, "人", WHITE, PINK);break;
             case 7:LCD_P16X16Str(j_axle,i_axle, "囊", GREEN, PINK);break;
             case 8:LCD_P16X16Str(j_axle,i_axle, "人", WHITE, PINK);break;

             }
         }
     }
 };
void ClsCell(i_axle_rm,j_axle_rm)//抹除一个单元格
{
        switch(mymap[i_axle_rm][j_axle_rm])
    {


        case 1:LCD_P16X16Str(j_axle_rm,i_axle_rm, "墙", PINK, PINK);break;
        case 3:LCD_P16X16Str(j_axle_rm,i_axle_rm, "病", PINK, PINK);break;
        case 4:LCD_P16X16Str(j_axle_rm,i_axle_rm, "囊", PINK, PINK);break;
        case 5:LCD_P16X16Str(j_axle_rm,i_axle_rm, "人",PINK, PINK);break;
        case 7:LCD_P16X16Str(j_axle_rm,i_axle_rm, "囊", PINK, PINK);break;
        case 8:LCD_P16X16Str(j_axle_rm,i_axle_rm, "人", PINK, PINK);break;

      }
}
void PrfCell(i_axle_Pf,j_axle_Pf)//绘制一个单元格
{
        switch(mymap[i_axle_Pf][j_axle_Pf])
    {

        case 1:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "墙", BLUE, PINK);break;
        case 3:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "病", RED, PINK);break;
        case 4:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "囊", CYAN, PINK);break;
        case 5:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "人", WHITE, PINK);break;
        case 7:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "囊", GREEN, PINK);break;
        case 8:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "人", WHITE, PINK);break;

      }
}
void Guidance()//游戏说明函数
{
    LCD_CLS();
    LCD_P16X16Str(2,0, "右侧进度条加", CYAN, SLIVER);
    LCD_P16X16Str(0,1, "载完时，游戏结束", CYAN, SLIVER);
    LCD_P16X16Str(2,2, "亮度高的环境", CYAN, SLIVER);
    LCD_P16X16Str(0,3, "将难以完成挑战", CYAN, SLIVER);
}
void play1()//关卡二
{
    loop:
    delay(1000);
    BarY=64;//初始化进度条
    //change map
    int mymap_new[MAPX_MAX][MAPY_MAX]=
    {
     {1,0,0,3,0,1,1},     
     {1,0,4,5,0,1,1},    
     {1,3,1,4,0,1,1},       
     {1,0,4,0,3,1,1},       
    };
    for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
        {
          for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
            {
                mymap[i_axle][j_axle]= mymap_new[i_axle][j_axle];
            }
        }
    //change map
    LCD_display_full(PINK);
    paint();
    PersonX=1;//初始化医护人员初始位置
    PersonY=3;
    GameState=1;
        while(mymap[2][1]!=7||mymap[0][3]!=7||mymap[3][4]!=7)
        {
            if(BarY<=0)
            {
                GameState=0;
                break;
            }
            delay(500);
            Progress_Bar();
            returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//获取加速度传感器数值
            MAP_ADC14_toggleConversionTrigger();//主动开启ADC
            //数模转换获取摇杆状态
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)//右
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
                }
            else if(curADCResultX<1000)//左
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
            else if(curADCResultY>15000)//下
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
            }
          else if (curADCResultY<1000)//上
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
              }
            }
          else if(s_accelXYZ.z<-15000)
          {
             goto loop;
          }
        }
}
void play2()//关卡三
{
    loop:
    delay(1000);
    BarY=64;
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
//    LCD_CLS();
    LCD_display_full(PINK);
    paint();
    PersonX=3;
    PersonY=5;//��ʼλ��
    GameState=1;
        while(mymap[0][0]!=7||mymap[1][0]!=7||mymap[2][0]!=7||mymap[2][1]!=7||mymap[3][1]!=7)
        {
            if(BarY<=0)
            {
                GameState=0;
                break;
            }
            Progress_Bar();
            returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//获取加速度传感器数值
            MAP_ADC14_toggleConversionTrigger();//主动开启ADC
            //数模转换获取摇杆状态
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)//右
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
                }
            else if(curADCResultX<1000)//左
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
              }
            }
          else if(s_accelXYZ.z<-15000)
          {
             goto loop;
          }
        }
}
void play()//关卡一
{
    Guidance();//游戏说明
loop:
    BarY=64;//初始化进度条
    delay(1000);
    //change map
    int mymap_new[MAPX_MAX][MAPY_MAX]=
    {
     {0,1,0,0,0,1,1},
     {1,1,0,3,0,1,1},
     {1,0,4,4,1,1,1},
     {1,0,0,5,3,1,1},    
    };
        for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
            {for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
                {
                    mymap[i_axle][j_axle]= mymap_new[i_axle][j_axle];
                }
              }
    //change map
    LCD_display_full(PINK);
    paint();
    //初始化医护人员位置
    PersonX=3;
    PersonY=3;
    GameState=1;
        while(mymap[1][3]!=7||mymap[3][4]!=7)
        {
            if(BarY<=0)
            {
                GameState=0;
                break;
            }
            delay(500);
            Progress_Bar();
            Progress_Bar();
            returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//获取加速度传感器数值
            MAP_ADC14_toggleConversionTrigger();//主动开启ADC
            //数模转换获取摇杆状态
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)//右
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
                }
            else if(curADCResultX<1000)//左
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
            else if(curADCResultY>15000)//下
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
            }
          else if (curADCResultY<1000)//上
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
              }
            }
          else if(s_accelXYZ.z<-15000)
          {
             goto loop;
          }
        }
}
void StickAccel()
{
    LCD_draw_dot(stick_x,stick_y,RED);
    LCD_draw_dot(stick_x,stick_y+1,RED);
    LCD_draw_dot(stick_x,stick_y-1,RED);
    LCD_draw_dot(stick_x+1,stick_y,RED);
    LCD_draw_dot(stick_x-1,stick_y,RED);
    while(1)
    {
           delay(200);
           returnValue = bmi160_read_accel_xyz(&s_accelXYZ);
           snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d\r\n",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
           sendTextPc();
           sendTextEsp();
            if(s_accelXYZ.x<-15000)
              {
                    stick_x++;
                    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
                    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                    LCD_draw_dot(stick_x,stick_y,RED);
                    LCD_draw_dot(stick_x,stick_y+1,RED);
                    LCD_draw_dot(stick_x,stick_y-1,RED);
                    LCD_draw_dot(stick_x+1,stick_y,RED);
                    LCD_draw_dot(stick_x-1,stick_y,RED);

                }
             else if(s_accelXYZ.x>5000)
             {
                 stick_x--;
                 MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
                 MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                 MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                 LCD_draw_dot(stick_x,stick_y,RED);
                 LCD_draw_dot(stick_x,stick_y+1,RED);
                 LCD_draw_dot(stick_x,stick_y-1,RED);
                 LCD_draw_dot(stick_x+1,stick_y,RED);
                 LCD_draw_dot(stick_x-1,stick_y,RED);
              }
            else if(s_accelXYZ.y>15000)
            {
                stick_y++;
                MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
                MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
                LCD_draw_dot(stick_x,stick_y,RED);
                LCD_draw_dot(stick_x,stick_y+1,RED);
                LCD_draw_dot(stick_x,stick_y-1,RED);
                LCD_draw_dot(stick_x+1,stick_y,RED);
                LCD_draw_dot(stick_x-1,stick_y,RED);
              }
          else if (s_accelXYZ.y<0)
            {
              stick_y--;
              MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
              MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
              MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
              LCD_draw_dot(stick_x,stick_y,RED);
              LCD_draw_dot(stick_x,stick_y+1,RED);
              LCD_draw_dot(stick_x,stick_y-1,RED);
              LCD_draw_dot(stick_x+1,stick_y,RED);
              LCD_draw_dot(stick_x-1,stick_y,RED);
              }
          else if(s_accelXYZ.z<0)
          {
              MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
              MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
              break;
          }
          else
          {
              MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
              MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
          }
    }
}
void Stick()//OLED与摇杆测试功能函数，成果中未使用
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
            MAP_ADC14_toggleConversionTrigger();
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
