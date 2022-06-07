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
s32 returnRslt;
void display_test(void);
void all_init();
void GameMenu();
void ChooseMenu_master();
void ChooseMenu_gaming_win();
void ChooseMenu_gaming_lose();
void LoadHistory();
void LevelSelection();
int NewGame=1,LoadGame=0;
int main(void)
{
    all_init();
    StickAccel();
    GameMenu();
     while(1){
              ;
             }
}
void all_init()
{
    uartInit();//
    ESPInit();
    initI2C();
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
    bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);
    sensorOpt3001Init();
    //Enable OPT, and BME sensors
    sensorOpt3001Enable(true);
    LCD_init();
//    LCD_draw_line(3,3,33,33,RED);
    adcInit();
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
    case 2: play2();break;
    }
        LCD_CLS();
        if( GameState==0)
        {
            LCD_P16X16Str(3,1, "很遗憾", RED, SLIVER);
            LCD_P16X16Str(2,2, "你失败了", RED, SLIVER);
            ChooseMenu_gaming_lose();
        }
        else
        {
            LCD_P16X16Str(3,1, "恭喜你", ORANGE, SLIVER);
            LCD_P16X16Str(2,2, "成功通过", ORANGE, SLIVER);
            if(GameRecord==2&&GameState==1)
            {
                GameRecord=0;
                ChooseMenu_master();
            }
            else
            {
                GameRecord++;
                ChooseMenu_gaming_win();
            }
        }
}
void ChooseMenu_master()
{
    //   chose menu
       NewGame=1;
       LoadGame=0;
       LCD_P16X16Str(2,1, "开始游戏", CYAN, SLIVER);
       LCD_P16X16Str(2,2, "继续游戏", WHITE, SLIVER);
//        LCD_P6X8Str(7, 3, "NEW GAME->", CYAN, BLACK);//显示英文6*8字符串
//         LCD_P6X8Str(7, 5, "LOAD GAME->", WHITE, BLACK);//显示英文6*8字符串
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P16X16Str(2,1, "开始游戏", CYAN, SLIVER);
                LCD_P16X16Str(2,2, "继续游戏", WHITE, SLIVER);
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P16X16Str(2,1, "开始游戏", WHITE, SLIVER);
                LCD_P16X16Str(2,2, "继续游戏", CYAN, SLIVER);
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
       LCD_P16X16Str(2,1, "再次挑战", CYAN, SLIVER);
       LCD_P16X16Str(2,2, "回到菜单", WHITE, SLIVER);
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P16X16Str(2,1, "再次挑战", CYAN, SLIVER);
                LCD_P16X16Str(2,2, "回到菜单", WHITE, SLIVER);
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P16X16Str(2,1, "再次挑战", WHITE, SLIVER);
                LCD_P16X16Str(2,2, "回到菜单", CYAN, SLIVER);
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
       LCD_P16X16Str(2,1, "继续挑战", CYAN, SLIVER);
       LCD_P16X16Str(2,2, "回到菜单", WHITE, SLIVER);
        while(1)
        {
            MAP_ADC14_toggleConversionTrigger();
            curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
            curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
            if(curADCResultY<1000)
            {
                LCD_P16X16Str(2,1, "继续挑战", CYAN, SLIVER);
                LCD_P16X16Str(2,2, "回到菜单", WHITE, SLIVER);
                NewGame=1;
                LoadGame=0;
            }
            else if(curADCResultY>15000)
            {
                LCD_P16X16Str(2,1, "继续挑战", WHITE, SLIVER);
                LCD_P16X16Str(2,2, "回到菜单", CYAN, SLIVER);
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
    LCD_P6X8Str(1, 2, "This module is not ", CYAN, BLACK);//显示英文6*8字符串
    LCD_P6X8Str(1, 3, "yet complete", CYAN, BLACK);//显示英文6*8字符串
    LCD_P6X8Str(1, 4, "<-please exit", CYAN, BLACK);//显示英文6*8字符串
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


void display_test(void)
{
    //                 curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
    //                 curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
    //
    //                 adc=curADCResultX;
    //
    //                 MAP_UART_transmitData(EUSCI_A0_BASE, 48+adc/10000);
    //
    //                         MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/1000)%10);
    //                         MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/100)%10);
    //                         MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/10)%10);
    //                         MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc%10));
    //                                 MAP_UART_transmitData(EUSCI_A0_BASE, ' ');
    //
    //
    //                         adc=curADCResultY;
    //                                 MAP_UART_transmitData(EUSCI_A0_BASE, 48+adc/10000);
    //                                 MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/1000)%10);
    //                                 MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/100)%10);
    //                                 MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/10)%10);
    //                                 MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc%10));
    //
    //                         MAP_UART_transmitData(EUSCI_A0_BASE, '\r');
    //                         MAP_UART_transmitData(EUSCI_A0_BASE, '\n');
    //
    //                 MAP_ADC14_toggleConversionTrigger();

    //         returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);//
    //         returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//陀螺仪
    //         returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);//
    //         snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d    ",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
    //         sensorOpt3001Read(&rawData);
    //          sensorOpt3001Convert(rawData, &convertedLux);
    //          //contrsuct JSON string for OPT
    //          snprintf(test.txString, 60,
    //                  "{\"light\":{\"lux\":%5.2f}}\r\n",
    //                  convertedLux);

              //Send UART data JSON string

    //         snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d\r\n",s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z);
    //                  snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d\r\n",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
    //          sendTextPc();


    //         snprintf(test.txString, 120,
    //                  "{\"dominant\":{\"code\":%d},"
    //                  "\"gyro\":{\"x\":%d,\"y\":%d,\"z\":%d},"
    //                  "\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}\n",
    //                  dominant,
    //                  s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z,
    //                  s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
              //Send UART data JSON string
    //          MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    //          delay(100);
    //         MAP_PCM_gotoLPM0();
    //         display_test();
    delay(500);
}

