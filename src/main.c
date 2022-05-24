#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "uart_driver.h"
#include "variable.h"
int main(void)
{
    uartInit();//各种初始化
    initI2C();
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
    bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);
     while(1){
          returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);//看歪不歪
          returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//陀螺仪
          returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);//看歪不歪

          //contrsuct JSON string for BMM
          snprintf(test.txString, 60,"{\"mag\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
          sendText();
//          snprintf(test.txString, 60,"{\"gyro\":{\"x\":%d,\"y\":%d,\"z\":%d}\r\n",s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z);
//          sendText();
//          snprintf(test.txString, 60,"{\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
//          sendText();
            //Send UART data JSON string

     }


}
