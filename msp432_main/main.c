#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <LQ12864.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "uart_driver.h"
#include "variable.h"
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;
s32 returnRslt;


int main(void)
{
    uartInit();//各种初始化
    ESPInit();
    LCD_init();
    initI2C();
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
    bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);

    LCD_CLS();                //OLED清屏
//    LCD_P16X16Str(1,0,"龙邱智能科技",RED,BLACK);       //显示汉字字符串
//    LCD_P6X8Str(2,7,"LongQiu.taobao.com",CYAN,BLACK);   //显示英文6*8字符串
//    LCD_P8X16Str(2,2,"www.LQIST.cn",BLUE,BLACK);         //显示英文8*16字符串
//    LCD_P16X16Str(1,1,"液晶校准测试",YELLOW,BLACK);     //显示汉字字符串
//      snprintf(test.txString, 60,"游戏开始");
//      LCD_P8X16Str(1,2,test.txString,BLUE,BLACK);
//      LCD_P8X16Str(2,2,"www.LQIST.cn",BLUE,BLACK);



     while(1){

//                 MAP_PCM_gotoLPM0();

          returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);//看歪不歪
//          returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//陀螺仪
//          returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);//看歪不歪
//
//          //contrsuct JSON string for BMM
          snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
          sendTextPc();
          sendTextEsp();
          LCD_P6X8Str(1,2,test.txString,CYAN,BLACK);
//          sendTextEsp();
//          snprintf(test.txString, 60,"游戏开始\r\n",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
//          sendTextEsp();
//          snprintf(test.txString, 60,"{\"gyro\":{\"x\":%d,\"y\":%d,\"z\":%d}\r\n",s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z);
//          sendTextpc();
//          snprintf(test.txString, 60,"{\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
//          sendTextpc();
          //Send UART data JSON string
//          LCD_P6X8Str(2,7,test.txString,CYAN,BLACK);
//          snprintf(test.txString, 60,"{\"mag\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
//                    sendTextpc();
              //delay(1);

             }


}
