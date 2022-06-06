#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <LQ12864.h>
#include <my_source.h>//�Զ���.h�ļ������ڴ�ź���������
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "uart_driver.h"
extern unsigned char gImage_lqTFT02[] ;//�����ⲿ�ַ����飬��ʾͼƬ
s32 returnRslt;
void all_init();//������ģ����г�ʼ��
void GameMenu();//������Ϸ��ʼ����
void ChooseMenu_master();//��Ϸ�˵�������
void ChooseMenu_gaming_win();//��Ϸʤ��ѡ��˵�
void ChooseMenu_gaming_lose();//��Ϸʧ��ѡ��˵�
void LevelSelection();//�ؿ�ѡ���װ����
int NewGame=1,LoadGame=0;//������Ϸ����ѡ���������ʼ��
int main(void)//����������
{
    all_init();//�ܳ�ʼ��
    StickAccel();//��Ļ����
    GameMenu();//��Ϸ
}
void all_init()//������ģ����г�ʼ��
{
    uartInit();//���ڳ�ʼ��
    ESPInit();//8266ģ���ʼ��
    initI2C();//I2C���ݴ����ʼ��
    //BIM160������ģ���ʼ��
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
    bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);
    //Opt3001����ģ���ʼ��
    sensorOpt3001Init();
    sensorOpt3001Enable(true);
    LCD_init();//OLED��Ļ��ʼ��
    adcInit();//ģ��ת����ʼ��
}
void GameMenu()//������Ϸ��ʼ����
{
    LCD_CLS();
    delay(200);
    show_pic(4,1,132,64,gImage_lqTFT02);
    delay(200);
    ChooseMenu_master();
}
void LevelSelection()//�ؿ�ѡ���װ����
{
    switch(GameRecord)//ͨ����Ϸ��¼����ѡ����Ϸ�ؿ�
    {
    case 0: play();break;
    case 1: play1();break;
    case 2: play2();break;
    }
    LCD_CLS();         //����
    if( GameState==0)  //�ж���Ϸʧ�ܺ�ִ��
    {
        LCD_P16X16Str(3,1, "���ź�", RED, SLIVER);
        LCD_P16X16Str(2,2, "��ʧ����", RED, SLIVER);
        ChooseMenu_gaming_lose();
    }
    else                //��Ϸʤ��ִ��
    {
        LCD_P16X16Str(3,1, "��ϲ��", ORANGE, SLIVER);
        LCD_P16X16Str(2,2, "�ɹ�ͨ��", ORANGE, SLIVER);
        GameRecord++;
        ChooseMenu_gaming_win();
    }
}
void ChooseMenu_master()//��Ϸ�˵�������
{
    //   chose menu
       NewGame=1;
       LoadGame=0;
       LCD_P16X16Str(2,1, "��ʼ��Ϸ", CYAN, SLIVER);
       LCD_P16X16Str(2,2, "������Ϸ", WHITE, SLIVER);
    while(1)
    {
        MAP_ADC14_toggleConversionTrigger();
        curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
        curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
        if(curADCResultY<1000)
        {
            LCD_P16X16Str(2,1, "��ʼ��Ϸ", CYAN, SLIVER);
            LCD_P16X16Str(2,2, "������Ϸ", WHITE, SLIVER);
            NewGame=1;
            LoadGame=0;
        }
        else if(curADCResultY>15000)
        {
            LCD_P16X16Str(2,1, "��ʼ��Ϸ", WHITE, SLIVER);
            LCD_P16X16Str(2,2, "������Ϸ", CYAN, SLIVER);
            NewGame=0;
            LoadGame=1;
        }
        if(curADCResultX>15000&&NewGame==1)
            {
            GameRecord=0;
            LevelSelection();
            }
        else if(curADCResultX>15000&&LoadGame==1)
            {
            LevelSelection();
            break;
            }
    }
}
void ChooseMenu_gaming_lose()//��Ϸʧ��ѡ��˵�
{
    LCD_CLS();
    //   chose menu
       NewGame=1;
       LoadGame=0;
       LCD_P16X16Str(2,1, "�ٴ���ս", CYAN, SLIVER);
       LCD_P16X16Str(2,2, "�ص��˵�", WHITE, SLIVER);
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P16X16Str(2,1, "�ٴ���ս", CYAN, SLIVER);
                LCD_P16X16Str(2,2, "�ص��˵�", WHITE, SLIVER);
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P16X16Str(2,1, "�ٴ���ս", WHITE, SLIVER);
                LCD_P16X16Str(2,2, "�ص��˵�", CYAN, SLIVER);
                NewGame=0;
                LoadGame=1;
            }
            if(curADCResultX>15000&&NewGame==1)
                {
                LevelSelection();
                }
            else if(curADCResultX>15000&&LoadGame==1)
                {

                GameMenu();
                break;
                }

        }
}
void ChooseMenu_gaming_win()//��Ϸʤ��ѡ��˵�
{
    LCD_CLS();
    //   chose menu
       NewGame=1;
       LoadGame=0;
       LCD_P16X16Str(2,1, "������ս", CYAN, SLIVER);
       LCD_P16X16Str(2,2, "�ص��˵�", WHITE, SLIVER);
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P16X16Str(2,1, "������ս", CYAN, SLIVER);
                LCD_P16X16Str(2,2, "�ص��˵�", WHITE, SLIVER);
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P16X16Str(2,1, "������ս", WHITE, SLIVER);
                LCD_P16X16Str(2,2, "�ص��˵�", CYAN, SLIVER);
                NewGame=0;
                LoadGame=1;
            }
            if(curADCResultX>15000&&NewGame==1)
                {
                LevelSelection();

                }
            else if(curADCResultX>15000&&LoadGame==1)
                {

                GameMenu();
                break;
                }

        }
}