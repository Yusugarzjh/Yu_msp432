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
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;
s32 returnRslt;
void display_test(void);
void all_init(void);

void bmi_init(void);


int main(void)
{
    all_init();
//    paint();
    LCD_CLS();
    snprintf(test.txString, 60,"gyro");
    sendTextPc();
    sendTextEsp();
    LCD_P6X8Str(2,0,test.txString,CYAN,BLACK);
    snprintf(test.txString, 60,"accel");
    sendTextPc();
    sendTextEsp();
    LCD_P6X8Str(2,2,test.txString,CYAN,BLACK);
        snprintf(test.txString, 60,"magcomp");
        sendTextPc();
        sendTextEsp();
        LCD_P6X8Str(2,4,test.txString,CYAN,BLACK);
     while(1){
         returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);//���᲻��
         returnValue = bmi160_read_accel_xyz(&s_accelXYZ);//������
         returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);//���᲻��
//             play();                //
//         MAP_PCM_gotoLPM0();

         display_test();
              //delay(1);
             }


}


void all_init(void)
{
    uartInit();//
    ESPInit();
    LCD_init();
    initI2C();
//    adcInit();
    //bmi160��ʼ��
    bmi_init();
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
                      //OLED����
    //    LCD_P16X16Str(1,0,"�������ܿƼ�",RED,BLACK);       //��ʾ�����ַ���
    //    LCD_P6X8Str(2,7,"LongQiu.taobao.com",CYAN,BLACK);   //��ʾӢ��6*8�ַ���
    //    LCD_P8X16Str(2,2,"www.LQIST.cn",BLUE,BLACK);         //��ʾӢ��8*16�ַ���
    //    LCD_P16X16Str(1,1,"Һ��У׼����",YELLOW,BLACK);     //��ʾ�����ַ���
    //      snprintf(test.txString, 60,"��Ϸ��ʼ");
    //      LCD_P8X16Str(1,2,test.txString,BLUE,BLACK);
    //      LCD_P8X16Str(2,2,"www.LQIST.cn",BLUE,BLACK);
//    snprintf(test.txString, 60,"magcomp");
//    sendTextPc();
//    sendTextEsp();
//    LCD_P6X8Str(0,2,test.txString,CYAN,BLACK);
    snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d          ",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
    sendTextPc();
    sendTextEsp();
    LCD_P6X8Str(2,5,test.txString,CYAN,BLACK);
    snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d          ",s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z);
    sendTextPc();
    sendTextEsp();
    LCD_P6X8Str(2,1,test.txString,CYAN,BLACK);
    snprintf(test.txString, 60,"x:\%d,y:\%d,z:\%d         ",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
    sendTextPc();
    sendTextEsp();
    LCD_P6X8Str(2,3,test.txString,CYAN,BLACK);
    //          sendTextEsp();
    //          snprintf(test.txString, 60,"��Ϸ��ʼ\r\n",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
    //          sendTextEsp();
    //          snprintf(test.txString, 60,"{\"gyro\":{\"x\":%d,\"y\":%d,\"z\":%d}\r\n",s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z);
    //          sendTextpc();
    //          snprintf(test.txString, 60,"{\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);
    //          sendTextpc();
              //Send UART data JSON string
    //          LCD_P6X8Str(2,7,test.txString,CYAN,BLACK);
    //          snprintf(test.txString, 60,"{\"mag\":{\"x\":%d,\"y\":%d,\"z\":%d}}\r\n",s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);
    //                    sendTextpc();
    delay(500);
}

