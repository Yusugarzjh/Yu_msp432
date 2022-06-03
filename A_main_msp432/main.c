#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <LQ12864.h>
#include <my_source.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "uart_driver.h"
extern unsigned char lqTFT12864[] ;
extern unsigned char gImage_lqTFT02[] ;
extern unsigned char lqTFT01[] ;
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;
s32 returnRslt;
void display_test(void);
void all_init(void);
void GameMenu();
void bmi_init(void);
//void Guidance();
void ChooseMenu_master();
void ChooseMenu_gaming_win();
void ChooseMenu_gaming_lose();
void LoadHistory();
void LevelSelection();
int NewGame=1,LoadGame=0;
int main(void)
{
    all_init();
    GameMenu();

     while(1){
//         Progress_Bar();
//         returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);//¿´Íá²»Íá
//         returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//ÍÓÂÝÒÇ
//         returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);//¿´Íá²»Íá
                       //ÑÓÊ±
//display_pic();                              //ÏÔÊ¾Í¼Æ¬picture.h
//time_delay_ms(300);
//         UART_transmitData(EUSCI_A0_BASE, 48);
//            Stick();
//         MAP_PCM_gotoLPM0();
//         display_test();

             }
}



void all_init(void)
{
    uartInit();//
//    ESPInit();
    initI2C();
    bmi_init();
    LCD_init();
//    LCD_draw_line(3,3,33,33,RED);
    adcInit();
    //bmi160³õÊ¼»¯
}
void GameMenu()
{
    LCD_CLS();


    delay(200);
    show_pic(4,1,132,64,gImage_lqTFT02);
    delay(200);
    ChooseMenu_master();

}
void LevelSelection()
{
    switch(GameRecord)
    {
    case 0: play();break;
    case 1: play1();break;
    }
        LCD_CLS();
        if( GameState==0)
        {
            LCD_P6X8Str(6, 4, "GAME OVER", RED, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
            ChooseMenu_gaming_lose();
        }
        else
        {
            LCD_P6X8Str(8, 4, "VICTORY", YELLOW, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
            GameRecord++;
            ChooseMenu_gaming_win();
        }
}
void ChooseMenu_master()
{
    //   chose menu
       NewGame=1;
       LoadGame=0;
        LCD_P6X8Str(7, 3, "NEW GAME->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
         LCD_P6X8Str(7, 5, "LOAD GAME->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P6X8Str(7, 3, "NEW GAME->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                LCD_P6X8Str(7, 5, "LOAD GAME->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P6X8Str(7, 3, "NEW GAME->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                LCD_P6X8Str(7, 5, "LOAD GAME->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                NewGame=0;
                LoadGame=1;
            }
            if(curADCResultX>15000&&NewGame==1)
                {
                LevelSelection();
                }
            else if(curADCResultX>15000&&LoadGame==1)
                {
                LevelSelection();
//                LoadHistory();
                break;
                }

        }
}
void ChooseMenu_gaming_lose()
{
    LCD_CLS();
    //   chose menu
       NewGame=1;
       LoadGame=0;
        LCD_P6X8Str(5, 3, "GAME AGAIN->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
         LCD_P6X8Str(5, 5, "RETURN MENU->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P6X8Str(5, 3, "GAME AGAIN->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                 LCD_P6X8Str(5, 5, "RETURN MENU->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P6X8Str(5, 3, "GAME AGAIN->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                 LCD_P6X8Str(5, 5, "RETURN MENU->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
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
void ChooseMenu_gaming_win()
{
    LCD_CLS();
    //   chose menu
       NewGame=1;
       LoadGame=0;
        LCD_P6X8Str(5, 3, "Continue GAME->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
         LCD_P6X8Str(5, 5, "RETURN MENU->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P6X8Str(5, 3, "Continue GAME->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                 LCD_P6X8Str(5, 5, "RETURN MENU->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P6X8Str(5, 3, "Continue GAME->", WHITE, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
                 LCD_P6X8Str(5, 5, "RETURN MENU->", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
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
void LoadHistory()
{
    LCD_CLS();
    LCD_P6X8Str(1, 2, "This module is not ", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
    LCD_P6X8Str(1, 3, "yet complete", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
    LCD_P6X8Str(1, 4, "<-please exit", CYAN, BLACK);//ÏÔÊ¾Ó¢ÎÄ6*8×Ö·û´®
    while(1)
    {
        MAP_ADC14_toggleConversionTrigger();
        curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
        curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
        if(curADCResultX<1000)
        {
            GameMenu();
              break;
        }
    }
}
void bmi_init(void)
{
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
    bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);
}

void display_test(void)
{

    delay(500);
}

