#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "uart_driver.h"
// BMI160/BMM150
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;
s32 returnRslt;
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
bool BMI_on = true;
int main(void)
{
    // Stop WDT and disabling master interrupts
    MAP_WDT_A_holdTimer();
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    uartInit();
    initI2C();
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
    bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);
     while(1){
          returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);//ø¥Õ·≤ªÕ·
          returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//Õ”¬›“«
          returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);//ø¥Õ·≤ªÕ·

          //contrsuct JSON string for BMM
//          snprintf(test.txString, 60,"{\"mag\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
//          sendText();
//          snprintf(test.txString, 60,"{\"gyro\":{\"x\":%d,\"y\":%d,\"z\":%d}\r\n",s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z);
//          sendText();
          snprintf(test.txString, 60,"{\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
          sendText();
            //Send UART data JSON string

     }


}
