#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <LQ12864.h>
#include <my_source.h>//自定义.h文件，用于存放函数及变量
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "uart_driver.h"
extern unsigned char gImage_lqTFT02[] ;//引入外部字符数组，显示图片
s32 returnRslt;
void all_init();//对所有模块进行初始化
void GameMenu();//进入游戏初始界面
void ChooseMenu_master();//游戏菜单主界面
void ChooseMenu_gaming_win();//游戏胜利选择菜单
void ChooseMenu_gaming_lose();//游戏失败选择菜单
void LevelSelection();//关卡选择封装函数
int NewGame=1,LoadGame=0;//定义游戏界面选择变量并初始化
int main(void)//主函数运行
{
    all_init();//总初始化
    StickAccel();//屏幕画线
    GameMenu();//游戏
}
void all_init()//对所有模块进行初始化
{
    uartInit();//串口初始化
    ESPInit();//8266模块初始化
    initI2C();//I2C数据传输初始化
    //BIM160传感器模块初始化
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
    bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);
    //Opt3001光敏模块初始化
    sensorOpt3001Init();
    sensorOpt3001Enable(true);
    LCD_init();//OLED屏幕初始化
    adcInit();//模数转换初始化
}
void GameMenu()//进入游戏初始界面
{
    LCD_CLS();
    delay(200);
    show_pic(4,1,132,64,gImage_lqTFT02);
    delay(200);
    ChooseMenu_master();
}
void LevelSelection()//关卡选择封装函数
{
    switch(GameRecord)//通过游戏记录变量选择游戏关卡
    {
    case 0: play();break;
    case 1: play1();break;
    case 2: play2();break;
    }
    LCD_CLS();         //清屏
    if( GameState==0)  //判断游戏失败后执行
    {
        LCD_P16X16Str(3,1, "很遗憾", RED, SLIVER);
        LCD_P16X16Str(2,2, "你失败了", RED, SLIVER);
        ChooseMenu_gaming_lose();
    }
    else                //游戏胜利执行
    {
        LCD_P16X16Str(3,1, "恭喜你", ORANGE, SLIVER);
        LCD_P16X16Str(2,2, "成功通过", ORANGE, SLIVER);
        GameRecord++;
        ChooseMenu_gaming_win();
    }
}
void ChooseMenu_master()//游戏菜单主界面
{
    //   chose menu
       NewGame=1;
       LoadGame=0;
       LCD_P16X16Str(2,1, "开始游戏", CYAN, SLIVER);
       LCD_P16X16Str(2,2, "继续游戏", WHITE, SLIVER);
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
            break;
            }
    }
}
void ChooseMenu_gaming_lose()//游戏失败选择菜单
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
void ChooseMenu_gaming_win()//游戏胜利选择菜单
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