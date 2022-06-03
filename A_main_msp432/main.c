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

int main(void)
{
    all_init();

//    Guidance();
    GameMenu();
    LCD_CLS();
    LCD_P6X8Str(6, 4, "GAME OVER", RED, BLACK);//显示英文6*8字符串
     while(1){
//         Progress_Bar();
//         returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);//看歪不歪
//         returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//陀螺仪
//         returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);//看歪不歪
                       //延时
//display_pic();                              //显示图片picture.h
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
    //bmi160初始化
}
void GameMenu()
{
    LCD_CLS();
    int NewGame=1,LoadGame=0;

    delay(200);
    show_pic(4,1,132,64,gImage_lqTFT02);
    delay(200);

//    play();
    LCD_P6X8Str(7, 3, "NEW GAME", CYAN, BLACK);//显示英文6*8字符串
     LCD_P6X8Str(7, 5, "LOAD GAME", WHITE, BLACK);//显示英文6*8字符串
    while(1)
    {
        MAP_ADC14_toggleConversionTrigger();//控制小人移动速度
        curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
        curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
        if(curADCResultY<1000)
        {
            LCD_P6X8Str(7, 3, "NEW GAME", CYAN, BLACK);//显示英文6*8字符串
            LCD_P6X8Str(7, 5, "LOAD GAME", WHITE, BLACK);//显示英文6*8字符串
            NewGame=1;
            LoadGame=0;
        }
        else if(curADCResultY>15000)
        {
            LCD_P6X8Str(7, 3, "NEW GAME", WHITE, BLACK);//显示英文6*8字符串
            LCD_P6X8Str(7, 5, "LOAD GAME", CYAN, BLACK);//显示英文6*8字符串
            NewGame=0;
            LoadGame=1;
        }
        if(curADCResultX>15000&&NewGame==1)
            {

            play();
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

