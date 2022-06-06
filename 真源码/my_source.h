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
// BMI160/BMM150
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;
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
bool BME_on = true;
#define  OLED_GAME
//#define  NORMAL_MAP
//#define  TEST_MAP
// OPT3001
uint16_t rawData;
float     convertedLux;
#define  MAPX_MAX 4
#define  MAPY_MAX 7
bool BMI_on = true;
int i_axle,j_axle;
int GameRecord=0;//�������Ϸ��¼
int mymap[MAPX_MAX][MAPY_MAX]=           //����һ��8x8�ĵ�ͼ���������ʾ����Ԫ��Ԫ��
{
  {5,4,3,0,0,0,1},//5 person
  {0,0,0,0,0,0,1},//7virus+vaccine
  {0,0,0,0,0,0,1},//8person+vaccine
  {0,0,0,0,0,0,1},

  };
int PersonX,PersonY,stick_x=68,stick_y=32;
int BarX=114,BarY=64,GameState=0,TimerInterval=1;//���������
void Progress_Bar()
{
            sensorOpt3001Read(&rawData);
            sensorOpt3001Convert(rawData, &convertedLux);
            int BarSpeed;
            snprintf(test.txString, 60,
                    "{\"light\":{\"lux\":%d}}\r\n",
                    abs(convertedLux));
            sendTextPc();
            sendTextEsp();
            if(abs(convertedLux)<300)
                BarSpeed=10;
            else if(abs(convertedLux)<1000)
                BarSpeed=5;
            else
                BarSpeed=1;
            TimerInterval++;
            if(TimerInterval%BarSpeed==0)
            {
//                UART_transmitData(EUSCI_A0_BASE, BarY--);
                BarY-=2;
                LCD_draw_line(BarX,BarY,BarX+16,BarY,GREEN);
            }
}
void paint()             //���Ƴ�ʼ��ͼ
{
 for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
   { for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
       {
           switch(mymap[i_axle][j_axle])
           {
             #ifdef   OLED_GAME
//             case 0:LCD_P16X16Str(j_axle,i_axle, "ǽ", HUI, PINK);break;
             case 1:LCD_P16X16Str(j_axle,i_axle, "ǽ", BLUE, PINK);break;
             case 3:LCD_P16X16Str(j_axle,i_axle, "��", RED, PINK);break;
             case 4:LCD_P16X16Str(j_axle,i_axle, "��", CYAN, PINK);break;
             case 5:LCD_P16X16Str(j_axle,i_axle, "��", WHITE, PINK);break;
             case 7:LCD_P16X16Str(j_axle,i_axle, "��", GREEN, PINK);break;
             case 8:LCD_P16X16Str(j_axle,i_axle, "��", WHITE, PINK);break;
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
//            case 0:LCD_P16X16Str(j_axle,i_axle, "ǽ", HUI, PINK);break;
            case 1:LCD_P16X16Str(j_axle,i_axle, "ǽ", BLUE, PINK);break;
            case 3:LCD_P16X16Str(j_axle,i_axle, "��", CYAN, PINK);break;
            case 4:LCD_P16X16Str(j_axle,i_axle, "��", CYAN, PINK);break;
            case 5:LCD_P16X16Str(j_axle,i_axle, "��", WHITE, PINK);break;
            case 7:LCD_P16X16Str(j_axle,i_axle, "��", GREEN, PINK);break;
            case 8:LCD_P16X16Str(j_axle,i_axle, "��", WHITE, PINK);break;
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
//        case 0:LCD_P16X16Str(j_axle,i_axle, "ǽ", HUI, PINK);break;
        case 1:LCD_P16X16Str(j_axle_rm,i_axle_rm, "ǽ", PINK, PINK);break;
        case 3:LCD_P16X16Str(j_axle_rm,i_axle_rm, "��", PINK, PINK);break;
        case 4:LCD_P16X16Str(j_axle_rm,i_axle_rm, "��", PINK, PINK);break;
        case 5:LCD_P16X16Str(j_axle_rm,i_axle_rm, "��",PINK, PINK);break;
        case 7:LCD_P16X16Str(j_axle_rm,i_axle_rm, "��", PINK, PINK);break;
        case 8:LCD_P16X16Str(j_axle_rm,i_axle_rm, "��", PINK, PINK);break;
      #endif
      }
}
void PrfCell(i_axle_Pf,j_axle_Pf)
{
        switch(mymap[i_axle_Pf][j_axle_Pf])
    {
      #ifdef   OLED_GAME
        case 1:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "ǽ", BLUE, PINK);break;
        case 3:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "��", RED, PINK);break;
        case 4:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "��", CYAN, PINK);break;
        case 5:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "��", WHITE, PINK);break;
        case 7:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "��", GREEN, PINK);break;
        case 8:LCD_P16X16Str(j_axle_Pf,i_axle_Pf, "��", WHITE, PINK);break;
      #endif
      }
}
void Guidance()
{
    LCD_CLS();
//    LCD_P6X8Str(1, 0, "Press s2 to freeze ", CYAN, BLACK);//��ʾӢ��6*8�ַ���
//    LCD_P6X8Str(1, 1, "full screen", CYAN, BLACK);//��ʾӢ��6*8�ַ���
    LCD_P16X16Str(2,0, "�Ҳ��������", CYAN, SLIVER);
    LCD_P16X16Str(0,1, "����ʱ����Ϸ����", CYAN, SLIVER);
    LCD_P16X16Str(2,2, "���ȸߵĻ���", CYAN, SLIVER);
    LCD_P16X16Str(0,3, "�����������ս", CYAN, SLIVER);

//    LCD_P16X16Str(3,1, "���ź�", RED, SLIVER);
//    LCD_P16X16Str(3,1, "���ź�", RED, SLIVER);

}
void play()
{  Guidance();//��Ϸ˵����
    loop:
//    Guidance();//��Ϸ˵����
    delay(1000);
    BarY=64;//��ʼ��������
    //change map
    int mymap_new[MAPX_MAX][MAPY_MAX]=
    {
     {1,0,0,3,0,1,1},       //0 blank
     {1,0,4,5,0,1,1},       // 1 wall
     {1,3,1,4,0,1,1},       // 3 virus
     {1,0,4,0,3,1,1},       //4vaccine
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
    PersonX=1;
    PersonY=3;//��ʼλ��
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
            returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//������
            MAP_ADC14_toggleConversionTrigger();//����С���ƶ��ٶ�
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
          else if(s_accelXYZ.z<-15000)
          {
             goto loop;
          }
        }
}
void play2()
{
    loop:
//    Guidance();//��Ϸ˵����
    delay(1000);
    BarY=64;//��ʼ��������
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
            delay(500);
            Progress_Bar();
            returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//������
            MAP_ADC14_toggleConversionTrigger();//����С���ƶ��ٶ�
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
          else if(s_accelXYZ.z<-15000)
          {
             goto loop;
          }
        }
}
void play1()
{

loop:
    BarY=64;//��ʼ��������
    delay(1000);
//    LCD_CLS();
    //change map
    int mymap_new[MAPX_MAX][MAPY_MAX]=
    {
     {0,1,0,0,0,1,1},//5 person
     {1,1,0,3,0,1,1},//7virus+vaccine
     {1,0,4,4,1,1,1},//8person+vaccine
     {1,0,0,5,3,1,1},       //4vaccine
      };
        for( i_axle=0;i_axle<MAPX_MAX;i_axle++)
            {for( j_axle=0;j_axle<MAPY_MAX;j_axle++)
                {
                    mymap[i_axle][j_axle]= mymap_new[i_axle][j_axle];
                }
              }
    //change map10
    LCD_display_full(PINK);
    paint();
    PersonX=3;
    PersonY=3;//��ʼλ��
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
            returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//������
            MAP_ADC14_toggleConversionTrigger();//
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultX>15000)//right||s_accelXYZ.x<1
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
             else if(curADCResultX<1000)//left||s_accelXYZ.x>10000
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

            else if(curADCResultY>15000)//down||s_accelXYZ.y>6000
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
          else if (curADCResultY<1000)//up||s_accelXYZ.y<-6000
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
          else if(s_accelXYZ.z<-15000)
          {
            goto loop;
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
            MAP_ADC14_toggleConversionTrigger();//�ֶ�����ADCת��
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
void StickAccel()
{
        LCD_draw_dot(stick_x,stick_y,RED);
        LCD_draw_dot(stick_x,stick_y+1,RED);
        LCD_draw_dot(stick_x,stick_y-1,RED);
        LCD_draw_dot(stick_x+1,stick_y,RED);
        LCD_draw_dot(stick_x-1,stick_y,RED);
            LCD_draw_dot(64,32,RED);
    while(1)
    {
            delay(200);
            returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//������
           snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d\r\n",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
           sendTextPc();
           sendTextEsp();
            if(s_accelXYZ.x<-15000)
              {
                    stick_x++;
                    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
                    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
//                    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
//                    LCD_draw_dot(stick_x-1,stick_y,BLACK);
//                    LCD_draw_dot(stick_x-1+1,stick_y,BLACK);
//                    LCD_draw_dot(stick_x-1-1,stick_y,BLACK);
//                    LCD_draw_dot(stick_x-1,stick_y-1,BLACK);
//                    LCD_draw_dot(stick_x-1,stick_y+1,BLACK);
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
//                 MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                 MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
//                 LCD_draw_dot(stick_x+1,stick_y,BLACK);
//                 LCD_draw_dot(stick_x+1,stick_y+1,BLACK);
//                 LCD_draw_dot(stick_x+1,stick_y-1,BLACK);
//                 LCD_draw_dot(stick_x+1+1,stick_y,BLACK);
//                 LCD_draw_dot(stick_x+1-1,stick_y,BLACK);
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
//                LCD_draw_dot(stick_x,stick_y-1,BLACK);
//                LCD_draw_dot(stick_x,stick_y-1+1,BLACK);
//                LCD_draw_dot(stick_x,stick_y-1-1,BLACK);
//                LCD_draw_dot(stick_x+1,stick_y-1,BLACK);
//                LCD_draw_dot(stick_x-1,stick_y-1,BLACK);
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
//              LCD_draw_dot(stick_x,stick_y+1,BLACK);
//              LCD_draw_dot(stick_x,stick_y+1+1,BLACK);
//              LCD_draw_dot(stick_x,stick_y+1-1,BLACK);
//              LCD_draw_dot(stick_x+1,stick_y+1,BLACK);
//              LCD_draw_dot(stick_x-1,stick_y+1,BLACK);
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

#endif /* MY_SOURCE_H_ */
